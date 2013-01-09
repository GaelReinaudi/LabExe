#include "GEoGeneticAlgorithm.h"
#include "GOptimizer.h"
#include "GEoEvalFunctor.h"
#include "GEoGeneticAlgorithmWidget.h"

#include <QtCore>
#include <QDebug>

GEoGeneticAlgorithm::GEoGeneticAlgorithm(GOptimizer* parentOptimizer)
	: GOptimizationAlgorithm(parentOptimizer)
	, m_UintCurrentGeneration(0)
	, m_CurrentGeneration("Current generation", parentOptimizer, GParam::ReadOnly)
	, m_PopulationSize("Population size", parentOptimizer)
	, m_CrossOverProbability("Crossover probability", parentOptimizer)
	, m_MutationProbability("Mutation probability", parentOptimizer)
{
	m_pEoWidget = new GEoGeneticAlgorithmWidget(this, 0);
	SettingsWidget()->layout()->addWidget(m_pEoWidget);

	m_PopulationSize.SetHardLimits(2, 999999);
	m_PopulationSize.SetTypicalStep(10);
	m_CrossOverProbability.SetHardLimits(0.0, 1.0);
	m_CrossOverProbability.SetTypicalStep(0.1);
	m_MutationProbability.SetHardLimits(0.0, 1.0);
	m_MutationProbability.SetTypicalStep(0.1);

	// ini
	m_PopulationSize = 20;
	m_CrossOverProbability = 0.3;
	m_MutationProbability = 0.4;
}

GEoGeneticAlgorithm::~GEoGeneticAlgorithm()
{

}

// Use functions from namespace std
using namespace std;

double GlobalEoEvaluationFunction(const Individual & x, GOptimizationAlgorithm* pAlgo)
{
	GEoGeneticAlgorithm* pGenAlgo = qobject_cast<GEoGeneticAlgorithm*>( pAlgo);

	GVectorDouble NewPosVextor = GVectorDouble::fromStdVector(x);

	if(!pAlgo->m_KeepIterating) {
		return 0.0;
	}

	// test to update the generation display
	pGenAlgo->m_CurrentGeneration = int(pGenAlgo->m_UintCurrentGeneration);

	return pAlgo->EvalNewPosition(NewPosVextor);
}


bool GEoGeneticAlgorithm::Configure()
{
	try {
		// EVAL
		/////////////////////////////
		// Fitness function
		////////////////////////////
		// Evaluation: from a plain C++ fn to an EvalFunc Object
		// you need to give the full description of the function
		m_pEvalFunctor = new GEoEvalFunctor(this, GlobalEoEvaluationFunction);
	}
	catch (QtConcurrent::Exception & e) {
		qWarning() << e.what();
		return false;
	}
	return true;
}

void GEoGeneticAlgorithm::RunOptimization()
{
	m_InitialValues = m_pOptimizer->CurrentValuesVariable();
	if(m_InitialValues.isEmpty()) {
		qWarning() << "No variables to optimize";
		emit FinishedOptimizing(-1);
		return;
	}
	try {
		// initial point
		EvalNewPosition(m_InitialValues);
	}
	catch(exception& e)
	{
		qWarning() << "Exception: " << e.what() << '\n';
	}

	m_UintCurrentGeneration = 1;
	int popSize = m_PopulationSize;

	std::vector<double> CenterValues = m_pOptimizer->CenterValuesVariable().toStdVector();
	std::vector<double> LowerBounds = m_pOptimizer->LowerBoundsVariable().toStdVector();
	std::vector<double> UpperBounds = m_pOptimizer->UpperBoundsVariable().toStdVector();
	std::vector<double> TypicalSteps = m_pOptimizer->TypicalStepsVariable().toStdVector();
	int numVar = m_InitialValues.size();

	eoRealVectorBounds theEoParamBounds(LowerBounds, UpperBounds);
	
	// epsilons and sigmas
	double relativeEpsilon = 0.1;
	double relativeSigma = 0.1;
// 	std::vector<double> epsilons = m_pOptimizer->TypicalStepsVariable().toStdVector();
// 	std::vector<double> sigmas = m_pOptimizer->TypicalStepsVariable().toStdVector();
// 	std::vector<double> probChange(sigmas.size(), 1.0);

// 	const double EPSILON = 40.0;	// range for real uniform mutation
// 	double SIGMA = 40.0;// TODO m_SigmaMutation;//20.3;	    	// std dev. for normal mutation


	// PARAMETRES
	const unsigned int SEED = QDateTime::currentDateTime().toTime_t();	// seed for random number generator
	const unsigned int T_SIZE = 3; // size for tournament selection
	const unsigned int VEC_SIZE = numVar; // Number of object variables in genotypes
	const unsigned int POP_SIZE = popSize;//20; // Size of population

	const unsigned int MAX_GEN = 500; // Maximum number of generation before STOP
	const unsigned int MIN_GEN = 10;  // Minimum number of generation before ...
	const unsigned int STEADY_GEN = 100; // stop after STEADY_GEN gen. without improvement

	const float P_CROSS = m_CrossOverProbability;//0.8;	// Crossover probability
	const float P_MUT = m_MutationProbability;//0.5;	// mutation probability

	// some parameters for choosing among different operators
	const double hypercubeRate = 0.5;     // relative weight for hypercube Xover
	const double segmentRate = 0.5;  // relative weight for segment Xover
	const double uniformMutRate = 0.5;  // relative weight for uniform mutation
	const double detMutRate = 0.5;      // relative weight for det-uniform mutation
	const double normalMutRate = 0.5;   // relative weight for normal mutation


	// GENERAL
	//////////////////////////
	//  Random seed
	//////////////////////////
	//reproducible random seed: if you don't change SEED above,
	// you'll aways get the same result, NOT a random run
	rng.reseed(SEED);


	// INIT
	////////////////////////////////
	// Initialization of population
	////////////////////////////////
	// based on a uniform generator
// 	eoUniformGenerator<double> uGen(-60, 60);
// 	eoInitFixedLength<Individual> random(VEC_SIZE, uGen);
	eoRealInitBounded<Individual> random(theEoParamBounds);
	// Initialization of the population
	m_pPop = new eoPop<Individual>(POP_SIZE, random);

	// and evaluate it in one loop
	apply<Individual>(*m_pEvalFunctor, *m_pPop);	// STL syntax


	// ENGINE
	/////////////////////////////////////
	// selection and replacement
	////////////////////////////////////
	// SELECT
	// The robust tournament selection
	eoDetTournamentSelect<Individual> selectOne(T_SIZE);
	// is now encapsulated in a eoSelectPerc (entage)
	m_pSelector = new eoSelectPerc<Individual>(selectOne);// by default rate==1

	// REPLACE
	// And we now have the full selection/replacement - though with
	// no replacement (== generational replacement) at the moment :-)
 	m_pReplacer = new eoGenerationalReplacement<Individual>;
	m_pReplacer = new eoWeakElitistReplacement<Individual>(*m_pReplacer);

	// OPERATORS
	//////////////////////////////////////
	// The variation operators
	//////////////////////////////////////
	// CROSSOVER
	// uniform chooce on segment made by the parents
	eoSegmentCrossover<Individual> xoverS;
	// uniform choice in hypercube built by the parents
	eoHypercubeCrossover<Individual> xoverA;
	// Combine them with relative weights
	eoPropCombinedQuadOp<Individual> xover(xoverS, segmentRate);
	xover.add(xoverA, hypercubeRate, true);

	// MUTATION
	// offspring(i) uniformly chosen in [parent(i)-epsilon, parent(i)+epsilon]
// 	eoUniformMutation<Individual>  mutationU(EPSILON);
	eoUniformMutation<Individual>  mutationU(theEoParamBounds, relativeEpsilon);
	// k (=1) coordinates of parents are uniformly modified
// 	eoDetUniformMutation<Individual>  mutationD(EPSILON);
	eoDetUniformMutation<Individual>  mutationD(theEoParamBounds, relativeSigma);
	// all coordinates of parents are normally modified (stDev SIGMA)
// 	eoNormalMutation<Individual>  mutationN(SIGMA);
	eoNormalVecMutation<Individual>  mutationN(theEoParamBounds, relativeSigma);
	// Combine them with relative weights
	eoPropCombinedMonOp<Individual> mutation(mutationU, uniformMutRate);
	mutation.add(mutationD, detMutRate);
	mutation.add(mutationN, normalMutRate, true);
	// The operators are  encapsulated into an eoTRansform object
	m_pTransformer = new eoSGATransform<Individual>(xover, P_CROSS, mutation, P_MUT);

	// STOP
	// CHECKPOINT
	//////////////////////////////////////
	// termination conditions: use more than one
	/////////////////////////////////////
	// stop after MAX_GEN generations
	eoGenContinue<Individual> genCont(MAX_GEN);
	// do MIN_GEN gen., then stop after STEADY_GEN gen. without improvement
	eoSteadyFitContinue<Individual> steadyCont(MIN_GEN, STEADY_GEN);
// 	// stop when fitness reaches a target (here VEC_SIZE)
// 	eoFitContinue<Individual> fitCont(0);
	// do stop when one of the above says so
	m_pContinuator = new eoCombinedContinue<Individual>(genCont);
	m_pContinuator->add(steadyCont);
//	m_pContinuator->add(fitCont);

	// CHECKPOINT
	//////////////////////////////////////
	eoCheckPoint<Individual> checkpoint(*m_pContinuator);
	// Create a counter parameter 
//	eoValueParam<unsigned> generationCounter(0, "Gen.");
	// Create an incrementor (sub-class of eoUpdater). Note that the  
	// parameter's value is passed by reference,  
	// so every time the incrementer is updated (every generation), 
	// the data in generationCounter will change. 
 	eoIncrementor<uint> increment(m_UintCurrentGeneration); 
	// Add it to the checkpoint,  
	// so the counter is updated (here, incremented) every generation 
	checkpoint.add(increment); 
	// now some statistics on the population: 
	// Best fitness in population 
	eoBestFitnessStat<Individual> bestStat; 
	// Second moment stats: average and stdev 
	eoSecondMomentStats<Individual> SecondStat; 
	// Add them to the checkpoint to get them called at the appropriate time 
	checkpoint.add(bestStat); 
	checkpoint.add(SecondStat); 

	// GENERATION
	/////////////////////////////////////////
	// the algorithm
	////////////////////////////////////////

	// Easy EA requires
	// selection, transformation, eval, replacement, and stopping criterion
	m_pGenerationalGA = new eoEasyEA<Individual>(checkpoint, *m_pEvalFunctor, *m_pSelector, *m_pTransformer, *m_pReplacer);

	try {
		// Apply algo to pop - that's it!
		(*m_pGenerationalGA)(*m_pPop);
		emit FinishedOptimizing();
	}

	catch (std::runtime_error & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(-1);
	}
	catch (std::invalid_argument & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(-1);
	}
	catch (std::bad_alloc & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(-1);
	}
	catch (QtConcurrent::Exception & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(-1);
	}
	catch (exception & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(-1);
	}

	return;
}

void GEoGeneticAlgorithm::run()
{
	try
	{
		RunOptimization();
	}
	catch(exception& e)
	{
		qWarning() << "Exception: " << e.what() << '\n';
		emit FinishedOptimizing(-1);
	}
}
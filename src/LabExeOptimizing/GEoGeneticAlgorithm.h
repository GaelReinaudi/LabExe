#ifndef GEOGENETICALGORITHM_H
#define GEOGENETICALGORITHM_H

#include "GOptimizationAlgorithm.h"
#include "eo/eo"
#include <es.h>

class GEoGeneticAlgorithm;
class GEoGeneticAlgorithmWidget;

// REPRESENTATION
//-----------------------------------------------------------------------------
// define your individuals
typedef eoReal<double> Individual;
//struct ArgumentsEval
//{
//	const std::vector<double>& x;
//	GEoGeneticAlgorithm* p;
//};

/////////////////////////////////////////////////////////////////////
//! \brief The GEoGeneticAlgorithm class implements the abstract GOptimizationAlgorithm to provide optimization by the EO library. 
/*!
Its constru
*/
class GEoGeneticAlgorithm : public GOptimizationAlgorithm
{
	Q_OBJECT

public:
	GEoGeneticAlgorithm(GOptimizer* parentOptimizer);
	~GEoGeneticAlgorithm();

	//! Re-implemented. Starts the optimization by instantiating a new .......
	void RunOptimization();
	//! Re-implemented. Configures the algorithm parameters. Returns true if configuration went well.
	bool Configure();

protected:
	//! .....
	void run();

private:
//	//! cleans up the m_pGenerationalGA if any, and returns a new one
// 	eoEasyEA<Individual>* MakeNewEoOpt(uint NumVar) {
// 		if(m_pGenerationalGA) 
// 			delete m_pGenerationalGA;
// 		m_pGenerationalGA = new nlopt::opt(theAlgoIdentifier, NumVar);
// 		return m_pImmutableOptimzer;
// 	}

private:
	//! param for the population
	GParamInt m_PopulationSize;
	//! param for the cross over
	GParamDouble m_CrossOverProbability;
	//! param for the Mutation
	GParamDouble m_MutationProbability;
	//! param for the displaying the current generation
	GParamInt m_CurrentGeneration;

private:
	eoCombinedContinue<Individual>* m_pContinuator;
	eoEvalFunc<Individual>* m_pEvalFunctor;
	eoSelect<Individual>* m_pSelector;
	eoTransform<Individual>* m_pTransformer;
	eoReplacement<Individual>* m_pReplacer;
	//! Population used by the algorithm
	eoPop<Individual>* m_pPop;
	//! the algorithm object
	eoAlgo<Individual>* m_pGenerationalGA;

	uint m_UintCurrentGeneration;
	//! pointer to the widget used to set this algorithm settings
	GEoGeneticAlgorithmWidget* m_pEoWidget;

	friend double GlobalEoEvaluationFunction(const Individual & x, GOptimizationAlgorithm* pAlgo);
	friend class GEoGeneticAlgorithmWidget;
};

#endif // GEOGENETICALGORITHM_H

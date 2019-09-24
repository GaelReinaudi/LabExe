#include "GNLOptAlgorithm.h"
#include "GOptimizer.h"
#include <QtConcurrent>


double GlobalNLoptEvaluationFunction(const std::vector<double> &x, std::vector<double> &grad, void* f_data) 
{
    Q_UNUSED(grad);
    GNLOptAlgorithm* pAlgo = (GNLOptAlgorithm*)f_data;
	GVectorDouble NewPosVextor = GVectorDouble::fromStdVector(x);

	if(!pAlgo->m_KeepIterating) {
		pAlgo->m_pImmutableOptimzer->force_stop();
		throw nlopt::forced_stop();
		return 0.0;
	}

	return pAlgo->EvalNewPosition(NewPosVextor);
}

GNLOptAlgorithm::GNLOptAlgorithm(GOptimizer* parentOptimizer, nlopt::algorithm whichAlgorithm)
	: GOptimizationAlgorithm(parentOptimizer)
	, m_pImmutableOptimzer(0)
	, m_AlgoNLoptIdentifier(whichAlgorithm)
{
	quit();
}

GNLOptAlgorithm::~GNLOptAlgorithm()
{
}

bool GNLOptAlgorithm::Configure()
{
	int numVar = m_pOptimizer->CenterValuesVariable().count();
	m_InitialValues = m_pOptimizer->CenterValuesVariable();
	std::vector<double> LowerBounds = m_pOptimizer->LowerBoundsVariable().toStdVector();
	std::vector<double> UpperBounds = m_pOptimizer->UpperBoundsVariable().toStdVector();
	std::vector<double> TypicalSteps;
	// make the steps a bit bigger
	double factorStep = 10.0;
	foreach(double step, m_pOptimizer->TypicalStepsVariable())
		TypicalSteps.push_back(factorStep * step);

	if(!(numVar > 0))
		return false;

	try {
		// let's make a new (immutable) nlopt object.
		nlopt::opt* pTheOpt = MakeNewNLoptOpt(m_AlgoNLoptIdentifier, numVar);
		pTheOpt->set_max_objective(GlobalNLoptEvaluationFunction, this);
//		pTheOpt->set_maxeval(3600);
		pTheOpt->set_lower_bounds(LowerBounds);
 		pTheOpt->set_upper_bounds(UpperBounds);
// 		std::vector<double> dfg = pTheOpt->get_initial_step();
		int popo = pTheOpt->get_population();
		pTheOpt->set_population(qMin(popo, 50));
 		pTheOpt->set_initial_step(TypicalSteps);
		pTheOpt->set_local_optimizer(nlopt::opt(nlopt::LN_NELDERMEAD, numVar));
	}
    catch (QException & e) {
        Q_UNUSED(e)
		return false;
	}
	return true;
}

void GNLOptAlgorithm::RunOptimization()
{
	double result = 0;

	try {
        auto initvec = m_InitialValues.toStdVector();
        nlopt::result theRes = m_pImmutableOptimzer->optimize(initvec, result);
		emit FinishedOptimizing(theRes);
	}
	catch (nlopt::roundoff_limited & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(nlopt::ROUNDOFF_LIMITED);
	}
	catch (nlopt::forced_stop & e) {
		if(m_KeepIterating)
			qWarning() << e.what();
		emit FinishedOptimizing(nlopt::FORCED_STOP);
	}
	catch (std::runtime_error & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(nlopt::FAILURE);
	}
	catch (std::invalid_argument & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(nlopt::INVALID_ARGS);
	}
	catch (std::bad_alloc & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(nlopt::OUT_OF_MEMORY);
	}
    catch (QException & e) {
		qWarning() << e.what();
		emit FinishedOptimizing(nlopt::FAILURE);
	}
	
	return;
}

void GNLOptAlgorithm::run()
{
	RunOptimization();
}

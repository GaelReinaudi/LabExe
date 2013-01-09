#include "GEoEvalFunctor.h"

GEoEvalFunctor::GEoEvalFunctor(GOptimizationAlgorithm* pAlgo, Individual::Fitness (* _eval)( const Individual&, GOptimizationAlgorithm* pAlgo ) )
    : QObject(pAlgo)
	, eoEvalFunc<Individual>()
	, m_pAlgo(pAlgo)
	, m_EvalFunc(_eval)
{

}

GEoEvalFunctor::~GEoEvalFunctor()
{

}

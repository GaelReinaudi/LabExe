#ifndef GEOEVALFUNCTOR_H
#define GEOEVALFUNCTOR_H

#include "GEoGeneticAlgorithm.h"
#include "eo/eo"
#include <es.h>

/////////////////////////////////////////////////////////////////////
//! \brief The GEoEvalFunctor re-implements the eoEvalFuncPtr from EO library
/*!
so that the evaluation function can use the GOptimizationAlgorithm for the actual evaluation. 
*/
class GEoEvalFunctor : public QObject, public eoEvalFunc<Individual>
{
	Q_OBJECT

public:
	GEoEvalFunctor(GOptimizationAlgorithm* pAlgo, Individual::Fitness (* _eval)( const Individual&, GOptimizationAlgorithm* pAlgo ) );
	~GEoEvalFunctor();

	/// Effectively applies the evaluation function to an EO
	virtual void operator() ( Individual & _eo )
	{
		if (_eo.invalid())
			_eo.fitness((*m_EvalFunc)(_eo, m_pAlgo));
	};
	
private:
	GOptimizationAlgorithm* m_pAlgo;
	Individual::Fitness (* m_EvalFunc )( const Individual&, GOptimizationAlgorithm* pAlgo );

};

#endif // GEOEVALFUNCTOR_H

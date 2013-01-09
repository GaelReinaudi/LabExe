#include "GNomadEvaluator.h"
#include "GNomadAlgorithm.h"
#include "GOptimizationAlgorithm.h"

GNomadEvaluator::GNomadEvaluator( const Parameters & p, GOptimizationAlgorithm* pAlgo /*= 0*/ )
	: Evaluator ( p , cout )
	, QObject(pAlgo)
{

}

GNomadEvaluator::~GNomadEvaluator( void )
{

}

bool GNomadEvaluator::eval_x( Eval_Point & x, const NOMAD::Double & h_max, bool & count_eval ) const
{
	int nVariables = x.size();
	double result = 0.0;

	// the values to try
	QList<double>* plistValuesToTry = new QList<double>();
	for(int indVal = 0; indVal < nVariables; indVal++) {
		double val = x[indVal].value();
		plistValuesToTry->append(val);
	}

	emit RequestEvaluation(plistValuesToTry, &result);

	x.set_bb_output ( 0 , -result  ); // objective value
	count_eval = true; // count a black-box evaluation
	return true;       // the evaluation succeeded
}

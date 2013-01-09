#ifndef GNOMADEVALUATOR_H
#define GNOMADEVALUATOR_H

#include "nomad.hpp"
#include <QObject>

class GOptimizationAlgorithm;

/////////////////////////////////////////////////////////////////////
//! \brief The 
/*!
*/
class GNomadEvaluator : public QObject, public Evaluator
{
	Q_OBJECT

public:
	GNomadEvaluator ( const Parameters & p, GOptimizationAlgorithm* pAlgo = 0);
	~GNomadEvaluator ( void );

	bool eval_x ( Eval_Point & x, const NOMAD::Double & h_max, bool & count_eval) const;

signals:
	//! Emitted when the Evaluator needs to evaluate a set of new values for the variables
	void RequestEvaluation(QList<double> * listValuesToTry, double* presult) const;

};

#endif // GNOMADEVALUATOR_H

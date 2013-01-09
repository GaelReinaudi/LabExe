#ifndef GNOMADALGORITHM_H
#define GNOMADALGORITHM_H

#include "GOptimizationAlgorithm.h"
#include "nomad.hpp"
class GNomadEvaluator;

/////////////////////////////////////////////////////////////////////
//! \brief The GNomadAlgorithm is a derivate of GOptimizationAlgorithm and can be used by a GOptimizer.
/*!
It is based on the NOMAD - Nonsmooth Optimization by Mesh Adaptive Direct search - version 3.3.
http://www.gerad.ca/NOMAD/Project/Home.html
user guise pdf http://www.gerad.ca/NOMAD/Downloads/user_guide.pdf

The implementation uses the example "basic-lib".
*/
class GNomadAlgorithm : public GOptimizationAlgorithm
{
	Q_OBJECT

public:
	GNomadAlgorithm(QObject *parent);
	~GNomadAlgorithm();

protected:
	//! reimplemented
	virtual void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget);

protected slots:
	virtual void ConfigureVariables();

protected:
	//! reimplemented from QThread
	virtual void run();

private:
	//! The nomad parameters
	Parameters m_NomadParam;
	//! The nomad evaluator
	GNomadEvaluator* m_ev;
	//! The nomad Mads algorithm
	Mads* m_Mads;
};

#endif // GNOMADALGORITHM_H

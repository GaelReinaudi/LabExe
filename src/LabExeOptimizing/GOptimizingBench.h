#ifndef GOPTIMIZINGBENCH_H
#define GOPTIMIZINGBENCH_H
#include "labexeoptimizing_global.h"

#include "TheLab/GWorkBench.h"

class GOptimizer;

/////////////////////////////////////////////////////////////////////
//! \brief The GOptimizingBench class defines a workbench intended to run optimization algorithms.
/*!
It is a GWorkBench where the central widget has been set to a.... (to be thought about)
*/
class LABEXEOPTIMIZING_EXPORT GOptimizingBench : public GWorkBench
{
	Q_OBJECT

public:
	GOptimizingBench(QWidget *parent);
	~GOptimizingBench();

protected:
	//! Reimplemented.
	virtual void PopulateSettings( QSettings& inQsettings );
	//! Reimplemented.
	virtual void InterpretSettings( QSettings& fromQsettings );
	//! Re-implemented
	virtual void LatterBenchInitialization();

private:
	//! the optimizer device
	GOptimizer* m_pOpt;
	//! creates the optimizer if necessary
	void CreateOptimizer();

};

#endif // GOPTIMIZINGBENCH_H

#ifndef GOPTIMIZATIONALGORITHM_H
#define GOPTIMIZATIONALGORITHM_H

#include <QThread>
#include <QObjectCleanupHandler>
#include "param.h"
#include "ToolBox/GSerializable.h"

class GOptimizer;

/////////////////////////////////////////////////////////////////////
//! \brief The GOptimizationAlgorithm is the abstract base class for an optimization algorithm. 
/*!
It is created with a GOptimizer* parentOptimizer so that the communication between the 2 is facilitated.
If the parent is set to 0, the algorithm should still run, but not communicate with 
an optimizer (this is so that one could still use the optimizing algorithm by itself). 

You have to re-implement it for your algorithm. It is a QThread so that the algo can run concurrently.
- re-implement run(), that is where the algorithm has to be executed. run() will run in a different thread.
- re-implement Configure()

*/
class GOptimizationAlgorithm : public QThread, public GSerializable
{
	Q_OBJECT

public:
	GOptimizationAlgorithm(GOptimizer* parentOptimizer);
	~GOptimizationAlgorithm();

 	//! . . . . . . .
 	virtual void RunOptimization() = 0;
	//! Will be called before the algorithm actually starts to Optimize(). Has to return true if configuration went well.
	virtual bool Configure() = 0;
	//! Returns a pointer to the widget that hold settings of the algo. Base implementation returns the default algorithm's widget which can be populated with a specific widget.
	virtual QWidget* SettingsWidget() {return m_pSettingsWidget;}

	//! Changes the adjustable Variables to the new values and returns the measured objectiveness. 
	virtual double EvalNewPosition( GVectorDouble NewPosVextor );

protected:
	//! Re-implemented
	virtual void PopulateSettings(QSettings& inQsettings) {}
	//! Re-implemented
	virtual void InterpretSettings(QSettings& fromQsettings) {}

protected:
	GVectorDouble m_InitialValues;

signals:
	void StartedOptimizing();
	void FinishedIteration(int numIterFinished);
	void FinishedOptimizing(int withCode = 0); // better would be an enum.


protected:
	GOptimizer* m_pOptimizer;

private:
	//! To destroy the widget if it is not destroyed by a parent
	QObjectCleanupHandler m_CleanUpHandler;
	//! The widget that can hold things for the settings of the algorithm.
	QWidget* m_pSettingsWidget;

public:
	//! boolean to stop the algorithm if necessary
	volatile bool m_KeepIterating;

};

#endif // GOPTIMIZATIONALGORITHM_H

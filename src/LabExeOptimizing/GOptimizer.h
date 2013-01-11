#ifndef GOPTIMIZER_H
#define GOPTIMIZER_H
#include "labexeoptimizing_global.h"

#include "Device/GProgDevice.h"
#include "Param/GSingleParamBucket.h"
#include "Param/GParamBucket.h"
#include "Param/GParamDouble.h"
#include "Param/GParamBool.h"

class GOptimizingBench;
class GOptimizationAlgorithm;

/////////////////////////////////////////////////////////////////////
//! \brief The GOptimizer class is a GProgDevice that can optimize a a parameter using an algorithm.
/*!
The optimizer holds the variables (in a bucket), the parameter to optimize (in an other bucket), and the algorithm. 
The optimizer is in charge of listening for requests from the algorithm, like updating the 
variables values, perform the measurements and return it to the algorithm. 
The algorithm is specified at run time.
*/
class LABEXEOPTIMIZING_EXPORT GOptimizer : public GProgDevice
{
	Q_OBJECT

public:
	GOptimizer(QObject* pParent, QString uniqueIdentifierName = "");
	~GOptimizer();

	//! Reimplemented to provide a GOptimizingWidget
	GDeviceWidget* ProvideNewDeviceGroupBox(QWidget* inWhichWidget, QBoxLayout::Direction orientation = QBoxLayout::LeftToRight);

	//! Select the algorithm to be used
	void SetAlgorithm(GOptimizationAlgorithm* val);
	//! Returns the list of current values for each variable to be optimized.
	GVectorDouble const CurrentValuesVariable() const;
	//! Returns the list of values that correspond to the center of the range to be explored for each variable to be optimized.
	GVectorDouble const CenterValuesVariable() const;
	//! Returns the list of the lower bounds for the parameters.
	GVectorDouble const LowerBoundsVariable() const;
	//! Returns the list of the upper bounds for the parameters.
	GVectorDouble const UpperBoundsVariable() const;
	//! Returns the list of the typical steps for the parameters.
	GVectorDouble const TypicalStepsVariable() const;
	
	//! Adds a new algorithm to the list of possible algorithms that will appear in the combo box of the widget
	void AddNewAlgorithm( GOptimizationAlgorithm* pAlgo, QString name, QString description = "No description available" );

protected:
	//! Re-implemented
	void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented
	void InterpretSettings(QSettings& fromQsettings);
	//! Re-implemented
	void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget) {Q_UNUSED(theDeviceWidget)}
	//! updates the Fitness value with the content of the bucket (plus the sign for maximizing or minimizing)
	double UpdateFitnessValue();

protected slots:
	void StartStopAlgorithm(bool doStart);
	//! Sets new values to the variables
	void SetNewValuesVariables(GVectorDouble newValues, double & returnValue);
	//! Sets new algorithm corresponding to this index indexAlgo, based on the one we inserted using AddNewAlgorithm()
	void SetAlgorithm(int indexAlgoList);
	//! event called by the signal AlgoStopped()
	void EventAlgoStopped();
	//! Called when a variable was added so that we can connect and initialize some things
	void Event_VariableAdded( GParam* theParam );

signals:
	//! emitted when the algo has stopped
	void AlgoStopped(bool someBool = false);
	// NOT DOCUMENTED emitted when the algo has finished with a default false value to switch off a checkable button
	void AlgoFinished(int withCode);
	//! Emitted to inform the gui that a result (and the variables that led to this result) has to be recorded in the history.
	void ResultToRecord(GVectorDouble newValues, double returnValue);
	//! emitted when an algorithm was added
	void NewAlgorithmAdded(GOptimizationAlgorithm* pAlgo);
	//! emitted when an algorithm was selected
	void AlgorithmSelected(GOptimizationAlgorithm* pAlgo);

private:
	GSingleParamBucket m_BucketParamOptim;
	GParamBucket m_BucketVariables;

	GOptimizationAlgorithm* m_pCurrentAlgo;

	//! The fitness value of the last function iteration, set in SetNewValuesVariables()
	GParamDouble m_LastFitnessValue;
	//! The highest value obtained for the fitness function
	GParamDouble m_MaxFitnessValue;
	//! The time to wait after updating the parameters in order to consider the fitting value
	GParamDouble m_WaitTimeIteration;
	//! The bool that says that we maximize or minimize the value to extremize
	GParamBool m_MaximizeOrMinimize;
	//! Boolean mask to access the variables when not all of them are being optimized
	QVector<bool> m_AccessMask;

	//! list of the algorithms
	QList<GOptimizationAlgorithm*> m_ListAlgo;
	//! short names of an algorithms that will appear in the combo box
	QMap<GOptimizationAlgorithm*, QString> m_AlgoNames;
	//! descriptions of the algorithms
	QMap<GOptimizationAlgorithm*, QString> m_AlgoDescritpions;
	//! widgets corresponding to the algo
	QMap<GOptimizationAlgorithm*, QWidget*> m_AlgoWidgets;
	friend class GOptimizingWidget;
	friend class GOptimizationAlgorithm;
};


#endif // GOPTIMIZER_H

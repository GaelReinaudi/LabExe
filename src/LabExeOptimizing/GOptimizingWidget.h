#ifndef GOPTIMIZINGWIDGET_H
#define GOPTIMIZINGWIDGET_H
#include "labexeoptimizing_global.h"

#include <QWidget>
#include "ui_GOptimizingWidget.h"
#include "Device/GDeviceWidget.h"
#include "GOptimizer.h"

class GOptimizer;

/////////////////////////////////////////////////////////////////////
//! \brief The GOptimizingWidget class is a widget that represent the GUI of an optimizer.
/*!
It contains a history widget, which displays the results (and associated variables) used by the algorithm. 
This makes it easy to sort the results and to come back to a configuration.
*/
class LABEXEOPTIMIZING_EXPORT GOptimizingWidget : public GDeviceWidget, public Ui::GOptimizingWidget
{
	Q_OBJECT

public:
	GOptimizingWidget(GOptimizer* pTheOpt, QWidget* parent = 0);
	~GOptimizingWidget();

protected slots:
	//! Records the given result and variables to the history widget
	void AppendToHistory(GVectorDouble newValues, double returnValue);
	//! triggered when the history has been double clicked so that we may set the corresponding values for the variables
	void HistoryDblClicked(QTreeWidgetItem* whichItem, int wichCol);
	//! adds an algorithm in the combobox and a widget in the QStackWidget.
	void AddAlgorithmInCombo(GOptimizationAlgorithm* pAlgo);
	//! called when an algorithm was selected 
	void EventAlgoSelected(GOptimizationAlgorithm* pAlgo);
	
private:
	GOptimizer* m_pOptimizer;
};

#endif // GOPTIMIZINGWIDGET_H

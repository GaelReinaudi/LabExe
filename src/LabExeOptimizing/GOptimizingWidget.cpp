#include "GOptimizingWidget.h"
#include "param.h"
#include "GOptimizationAlgorithm.h"

GOptimizingWidget::GOptimizingWidget(GOptimizer* pTheOpt, QWidget* parent /*= 0*/)
	: GDeviceWidget(pTheOpt, parent)
	, m_pOptimizer(pTheOpt)
{
	setupUi(this);

	if(!m_pOptimizer)
		return;

	QVBoxLayout *layParOpt = new QVBoxLayout;
	pBoxParamOptimize->setLayout(layParOpt);
	layParOpt->setContentsMargins(1, 1, 1, 1);
	// a param bucket for the param to extremize
	GParamBucket* pBucketOptim = &m_pOptimizer->m_BucketParamOptim;
 	layParOpt->addWidget(m_pOptimizer->m_MaximizeOrMinimize.ProvideNewParamRadioButtons(this, "Max", "Min"));
	layParOpt->addWidget(pBucketOptim->ProvideNewParamWidget(this));
	layParOpt->addWidget(m_pOptimizer->m_LastFitnessValue.ProvideNewLabel(this));
	layParOpt->addWidget(m_pOptimizer->m_LastFitnessValue.ProvideNewParamSpinBox(this));
	layParOpt->addWidget(m_pOptimizer->m_MaxFitnessValue.ProvideNewLabel(this));
	layParOpt->addWidget(m_pOptimizer->m_MaxFitnessValue.ProvideNewParamSpinBox(this));

	pWaitingTimeBox->layout()->addWidget(m_pOptimizer->m_WaitTimeIteration.ProvideNewParamSpinBox(this));

	QVBoxLayout *layVar = new QVBoxLayout;
	pBoxVariables->setLayout(layVar);
	layVar->setContentsMargins(1, 1, 1, 1);
	// the bucket for the variables
	GParamBucket* pBucketVariables = &m_pOptimizer->m_BucketVariables;
	layVar->addWidget(pBucketVariables->ProvideNewParamWidget(this));

	layParOpt->addStretch(1);
// 	layVar->addStretch(1);

	// connections to the optimizer
	connect(pButtonRun, SIGNAL(clicked(bool)), m_pOptimizer, SLOT(StartStopAlgorithm(bool)), Qt::QueuedConnection);
	connect(m_pOptimizer, SIGNAL(AlgoStopped(bool)), pButtonRun, SLOT(setChecked(bool)), Qt::QueuedConnection);

	connect(m_pOptimizer, SIGNAL(ResultToRecord(GVectorDouble, double)), this, SLOT(AppendToHistory(GVectorDouble, double)), Qt::QueuedConnection);

	// table history double clicked to set the corresponding values
	connect(pTableHistory, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(HistoryDblClicked(QTreeWidgetItem*, int)), Qt::QueuedConnection);

	// set the algo in the combo and tell the optimizer which is selected
	foreach(GOptimizationAlgorithm* pAlgo, m_pOptimizer->m_ListAlgo)
		AddAlgorithmInCombo(pAlgo);
	EventAlgoSelected(m_pOptimizer->m_pCurrentAlgo);
	connect(m_pOptimizer, SIGNAL(NewAlgorithmAdded(GOptimizationAlgorithm*)), this, SLOT(AddAlgorithmInCombo(GOptimizationAlgorithm*)));
	connect(pAlgoComboBox, SIGNAL(activated(int)), m_pOptimizer, SLOT(SetAlgorithm(int)), Qt::QueuedConnection);
	connect(m_pOptimizer, SIGNAL(AlgorithmSelected(GOptimizationAlgorithm*)), this, SLOT(EventAlgoSelected(GOptimizationAlgorithm*)));

	// deactivate the algo combobox when running
	connect(pButtonRun, SIGNAL(toggled(bool)), pAlgoComboBox, SLOT(setDisabled(bool)), Qt::QueuedConnection);
}

GOptimizingWidget::~GOptimizingWidget()
{

}

int NumColumnPreVariables = 2;
void GOptimizingWidget::AppendToHistory( GVectorDouble newValues, double returnValue )
{
	int NextIndex = pTableHistory->topLevelItemCount();
	int oldNumberColumns = pTableHistory->columnCount();
    int numberNewValues = newValues.count();
	// sets the number of columns to grow if necessary
	int newColCount = qMax(numberNewValues + NumColumnPreVariables, oldNumberColumns);
	bool colCountChanged = newColCount != oldNumberColumns;

	pTableHistory->setColumnCount(newColCount);
	// naming the columns
	int colToName = NumColumnPreVariables;
	foreach(GParam* pPar, m_pOptimizer->m_BucketVariables.ParamNums()) {
		pTableHistory->headerItem()->setText(colToName, pPar->Name());
		colToName++;
	}

	// the new item to be added to the list
	QTreeWidgetItem* pItem = new QTreeWidgetItem(pTableHistory);
	pItem->setData(0, Qt::DisplayRole, NextIndex);
	pItem->setData(1, Qt::DisplayRole, returnValue);
	int indCol = NumColumnPreVariables;
	foreach(double var, newValues) {
		pItem->setData(indCol, Qt::DisplayRole, var);
		indCol++;
	}

	// we resize every column if the colCountChanged
	if(colCountChanged)
		for(int i = 0; i < pTableHistory->columnCount(); i++)
			pTableHistory->resizeColumnToContents(i);
}

void GOptimizingWidget::HistoryDblClicked( QTreeWidgetItem* whichItem, int wichCol )
{
    Q_UNUSED(wichCol);
	int nCol = whichItem->columnCount();
	GVectorDouble theValuesToSet;
	for(int i = NumColumnPreVariables; i < nCol; i++) {
		theValuesToSet.append(whichItem->data(i, Qt::DisplayRole).toDouble());
	}
	m_pOptimizer->m_BucketVariables.SetValues(theValuesToSet, m_pOptimizer->m_AccessMask);
}

void GOptimizingWidget::AddAlgorithmInCombo( GOptimizationAlgorithm* pAlgo)
{
	if(!pAlgo)
		return;
	QObject* pObj = pAlgo;
	pAlgoComboBox->addItem(m_pOptimizer->m_AlgoNames.value(pAlgo), QVariant::fromValue(pObj));
	pAlgoStackSettings->addWidget(m_pOptimizer->m_AlgoWidgets.value(pAlgo));
}

void GOptimizingWidget::EventAlgoSelected( GOptimizationAlgorithm* pAlgo)
{
	if(!pAlgo) {
		pBoxAlgo->setTitle("No algorithm");
		return;
	}
	// the name 
	QString aName = m_pOptimizer->m_AlgoNames.value(pAlgo);
	pBoxAlgo->setTitle(aName);
	// update the combo if necessary:
	int indexcombo = pAlgoComboBox->findText(aName);
	if(indexcombo != -1) {
		pAlgoComboBox->setCurrentIndex(indexcombo);
		pAlgoStackSettings->setCurrentIndex(indexcombo);
	}
	// descritpion
	QString aDescr = m_pOptimizer->m_AlgoDescritpions.value(pAlgo);
	pAlgoDescriptionEdit->setPlainText(aDescr);
}

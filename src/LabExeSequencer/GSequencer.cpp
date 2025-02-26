#include "GSequencer.h"
#include "GSequencerWidget.h"
#include "Sequence/GSequence.h"
#include "Model/GEventGraphicsView.h"

G_REGISTER_PROG_DEVICE_CLASS(GSequencer)

GSequencer::GSequencer(QObject* pParent, QString uniqueIdentifierName /*= ""*/)
	: GProgDevice(pParent, uniqueIdentifierName)
	, m_pSequence(0)
	, m_pModel(0)
{
	if(!IsShelvedInstance()) {
		m_pSequence = new GSequence(0);
	}
	if(m_pSequence) {
		m_pModel = new GSeqModel(m_pSequence, this);
	}

	m_pScene = new QGraphicsScene(this);
	if(!IsShelvedInstance()) {
		GSequenceGraphicsItem* pSeqGraph = new GSequenceGraphicsItem(m_pSequence, 0);
		m_pScene->addItem(pSeqGraph);
	}
}

GSequencer::~GSequencer()
{

}

GDeviceWidget* GSequencer::ProvideNewDeviceGroupBox( QWidget* inWhichWidget, QBoxLayout::Direction orientation /*= QBoxLayout::LeftToRight*/ )
{
	GSequencerWidget* pWid = new GSequencerWidget(this, inWhichWidget);

	QTreeView* treeview = new QTreeView(pWid);
// 	treeview->setAcceptDrops(true);
// 	treeview->setDragEnabled(true);
// 	treeview->setDropIndicatorShown(true);
// 	treeview->setDragDropMode(QAbstractItemView::DragDrop);
	treeview->setDragDropMode(QAbstractItemView::InternalMove);
	treeview->setModel(m_pModel);
	QTimer::singleShot(50, treeview, SLOT(expandAll()));
	pWid->pTreeViewLayout->addWidget(treeview);
	
	connect(treeview, SIGNAL(clicked(QModelIndex)), this, SLOT(DebugIndex(QModelIndex)));

	if(m_pSequence)
		pWid->pSeqViewLayout->addWidget(m_pSequence->m_Length.ProvideNewParamSpinBox(inWhichWidget));

	QGraphicsView* m_pView = new QGraphicsView(inWhichWidget);
	m_pView->setScene(m_pScene);
	pWid->pSeqViewLayout->addWidget(m_pView);

	return pWid;
}

void GSequencer::PopulateSettings( QSettings& inQsettings )
{
	GProgDevice::PopulateSettings(inQsettings);
	m_pSequence->PopulateSettings(inQsettings);
}

void GSequencer::InterpretSettings( QSettings& fromQsettings )
{
	GProgDevice::InterpretSettings(fromQsettings);
	m_pSequence->InterpretSettings(fromQsettings);
}

void GSequencer::DebugIndex( const QModelIndex & index )
{
	qDebug() << index;
}

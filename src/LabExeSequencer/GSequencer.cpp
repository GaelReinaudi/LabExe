#include "GSequencer.h"
#include "GSequencerWidget.h"
#include "Sequence/GSequence.h"
#include "Model/GEventGraphicsView.h"
#include "../../MapExe/src/SceneView/GMapGraphicsView.h"

G_REGISTER_PROG_DEVICE_CLASS(GSequencer)

GSequencer::GSequencer(QObject* pParent, QString uniqueIdentifierName /*= ""*/)
	: GProgDevice(pParent, uniqueIdentifierName)
	, m_pSequence(0)
	, m_pModel(0)
	, m_pSeq(0)
{
	if(!IsShelvedInstance()) {
		m_pSequence = new GSequence(0);
// 		m_pSeq = new GSequence(this);
	}
	if(m_pSequence) {
		m_pModel = new GSeqModel(m_pSequence, this);
	}

	// TRY BASALT
	m_pScene = MakeGraphicsMap(this);
	if(!IsShelvedInstance()) {
		m_pSequence->ProvideNewAgentWrappingItem(m_pScene);
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
	
// 	GEventTreeView* treeview2 = new GEventTreeView(pWid);
// 	if(m_pSequence)
// 		treeview2->setModel(&m_pSequence->m_Seq);
// // 	treeview2->setModel(m_pSeq);
// 	treeview2->setDragDropMode(QAbstractItemView::InternalMove);
// 	pWid->pTreeViewLayout->addWidget(treeview2);
// 
// 	GEventGraphicsView* pView2 = new GEventGraphicsView(pWid);
// 	if(m_pSequence)
// 		pView2->setModel(&m_pSequence->m_Seq);
// 	pWid->pSeqViewLayout->addWidget(pView2);

	// debugs
// 	connect(treeview2, SIGNAL(clicked(QModelIndex)), this, SLOT(DebugIndex(QModelIndex)));
	connect(treeview, SIGNAL(clicked(QModelIndex)), this, SLOT(DebugIndex(QModelIndex)));

	if(m_pSequence)
		pWid->pSeqViewLayout->addWidget(m_pSequence->m_Length.ProvideNewParamSpinBox(inWhichWidget));

	// TRY BASALT
	GMapGraphicsView* m_pView = qobject_cast<GMapGraphicsView*>(MakeGraphicsView(inWhichWidget, m_pScene));
	pWid->pSeqViewLayout->addWidget(m_pView);
	m_pView->SetZoom(1);

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
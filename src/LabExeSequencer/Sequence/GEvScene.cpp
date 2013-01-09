#include "GEvScene.h"
#include "GSequence.h"
#include "GnewChannel.h"
#include "GSynchEvent.h"

GEvScene::GEvScene(GSequence *parentSeq)
	: QGraphicsScene(parentSeq)
	, m_pSeq(parentSeq)
{
	// to have the scene match the sequence length
	connect(m_pSeq, SIGNAL(LengthChanged(double)), this, SLOT(UpdateLength(double)), Qt::QueuedConnection);

	// background color
	setBackgroundBrush(QColor(230, 255, 230));

	// this is a QGraphicsWidget to use a layout in the scene
	QGraphicsWidget* pGraWid = new QGraphicsWidget();
	addItem(pGraWid);
	m_pChannelLayout = new QGraphicsLinearLayout(Qt::Vertical);
	pGraWid->setLayout(m_pChannelLayout);
	// margins from the view border
	m_pChannelLayout->setContentsMargins(0.0, 0.0, 0.0, 0.0);
	// spacing between channels
	m_pChannelLayout->setSpacing(1.0);

	// makes the root item, parent of all event in the sequence
	m_pRootEvent = new GSynchEvent(0);
	m_pRootEvent->setParent(this);
	m_pRootEvent->setFlag(QGraphicsItem::ItemIsMovable, false);;
	addItem(m_pRootEvent);
}

GEvScene::~GEvScene()
{

}

void GEvScene::InsertChannelItem( GnewChannel* pNewChan )
{
	if(!pNewChan)
		return qWarning("GnewChannel was NULL! 2368233");

	int indexNewChannel = m_pSeq->m_ChannelList.indexOf(pNewChan);

	addItem(pNewChan->ChannelGraphicsItem());
	m_pChannelLayout->insertItem(indexNewChannel, pNewChan->ChannelGraphicsItem());
}

void GEvScene::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent )
{
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	QPointF hoverPoint = mouseEvent->scenePos();
	emit HoverInScene(hoverPoint);
}

void GEvScene::UpdateLength( double newLength )
{
	setSceneRect(0.0, 0.0, newLength, sceneRect().height()) ;
}

void GEvScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	QGraphicsScene::dragEnterEvent(event);
	QWidget* sourceWid = event->source();
	QPointF posEnter = event->scenePos();
	const QMimeData* pData = event->mimeData();
	if(pData->hasFormat("LabExe/parameter.single")) {
 		event->accept();
	}
}

void GEvScene::dragMoveEvent( QGraphicsSceneDragDropEvent *event )
{
// Prevents From Accepting !	QGraphicsScene::dragMoveEvent(event);
 	event->accept();
}

void GEvScene::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
	QGraphicsScene::dragLeaveEvent(event);
}

void GEvScene::dropEvent( QGraphicsSceneDragDropEvent * event )
{
	QGraphicsScene::dropEvent(event);
	m_pSeq->CreateNewChannel();
}


















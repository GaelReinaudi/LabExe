#include "GSequenceGraphicsScene.h"
#include "GSequence.h"
#include "GChannel.h"

GSequenceGraphicsScene::GSequenceGraphicsScene(GSequence *parentSeq)
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
	m_pChanLay = new QGraphicsLinearLayout(Qt::Vertical);
	pGraWid->setLayout(m_pChanLay);
	// margins from the view border
	m_pChanLay->setContentsMargins(0.0, 0.0, 0.0, 0.0);
	// spacing between channels
	m_pChanLay->setSpacing(1.0);
}

GSequenceGraphicsScene::~GSequenceGraphicsScene()
{

}

void GSequenceGraphicsScene::InsertChannelItem( GChannel* pNewChan )
{
	if(!pNewChan)
		return qWarning("GChannel was NULL! 2368233");

	int indexNewChannel = m_pSeq->m_ChannelList.indexOf(pNewChan);

	addItem(pNewChan->ChannelGraphicsItem());
	m_pChanLay->insertItem(indexNewChannel, pNewChan->ChannelGraphicsItem());
}

void GSequenceGraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent )
{
	QPointF hoverPoint = mouseEvent->scenePos();
	emit HoverInScene(hoverPoint);
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GSequenceGraphicsScene::UpdateLength( double newLength )
{
	setSceneRect(0.0, 0.0, newLength, sceneRect().height()) ;
}

void GSequenceGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QGraphicsScene::contextMenuEvent(event);
	if(!m_pSeq)
		return;
	
	QList<QGraphicsItem*> listItems = items(event->scenePos());
	foreach(QGraphicsItem* pItem, listItems) {
		dynamic_cast<QString*>(pItem);
	}
}

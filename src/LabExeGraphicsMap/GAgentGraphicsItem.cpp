#include "GAgentGraphicsItem.h"
#include "GAgent.h"
#include <QtGui>
#include <QGraphicsTextItem>

//! generates a random number between 0 and 255
int rand255() { return qrand() % 256; }
//! returns a random QColor with fixed transparency 177
QColor randRGBA() { return QColor(rand255(),rand255(),rand255(),177); }
QColor randRGB() { return QColor(rand255(),rand255(),rand255()); }


QSvgRenderer GAgentGraphicsItem::m_StaticRenderer(QString("../svg-sample/rect.svg"));

double SurfaceChangeDimensionForEmittingSignal = 10.0;

GAgentGraphicsItem::GAgentGraphicsItem(GAgent* pAgent, QGraphicsItem *parentItem /*= 0*/)
	: QGraphicsSvgItem(parentItem)
	, m_pAgent(pAgent)
	, m_ItemsToBeAdded(0)
	, m_Surface(0.0)
	, m_ApproxPercentOfScreenOnScale1(0.0)
{
	// try optimization
	setCacheMode(QGraphicsItem::NoCache);

	m_Brush = randRGB();

	if(!m_pAgent)
		return;
	// informs this graphics item that a child agent was added to this item's agent
	// queued connection because the agent is not totally formed at that point and the virtual function table is not finished....
	// and the agent could not provide the graphics item.
	connect(pAgent, SIGNAL(ChildAgentAdded(GAgent*)), this, SLOT(AgentEvent_ChildAgentAdded(GAgent*)), Qt::QueuedConnection);
	// On the contrary, this is a Direct connection that will increment a counter representing the number of items that are to be added.
	// This is to have an information on the number of items that are added at the same time, before the actual item are created.
	// This can help for e.g. the placement of groups of item
	connect(pAgent, SIGNAL(ChildAgentAdded(GAgent*)), this, SLOT(AgentEvent_IncrementAgentCounterForBatchAdding()), Qt::DirectConnection);
    //informs the graphics item that its corresponding agent has unparented itself
    connect(pAgent, SIGNAL(UnParented()), this, SLOT(AgentEvent_UnParent()));
	
	// we also should probably add the graphics items of the agents that are already children now.
	foreach(GAgent* pChildAgent, m_pAgent->ChildAgents()) {
		pChildAgent->ProvideNewAgentGraphicsItem(this);
	}

	// connect to adjust the item so that it encompasses its children when a child has been added
    connect(this, SIGNAL(ChildAgentGraphicsItemAdded(GAgentGraphicsItem*)), this, SLOT(EncompassChildren()), Qt::QueuedConnection);
}

GAgentGraphicsItem::~GAgentGraphicsItem()
{

}

void GAgentGraphicsItem::AgentEvent_ChildAgentAdded( GAgent* pAgent )
{
	if(!pAgent)
		return;
	GAgentGraphicsItem* pAgIt = pAgent->ProvideNewAgentGraphicsItem(this);
	// removing from the count of items to be added
	m_ItemsToBeAdded--;
	// then emits the signal
	emit ChildAgentGraphicsItemAdded(pAgIt, m_ItemsToBeAdded);
}

void GAgentGraphicsItem::AgentEvent_IncrementAgentCounterForBatchAdding()
{
	m_ItemsToBeAdded++;
}

void GAgentGraphicsItem::AgentEvent_UnParent()
{
	QPointF posScene = scenePos();
    setParentItem(0);
	setPos(posScene);
}

void GAgentGraphicsItem::EncompassChildren()
{
	SetRect(/*boundingRect() | */childrenBoundingRect());
	// call it on the parent
	GAgentGraphicsItem* pParAgentItem = ParentAgentGraphicsItem();
	if(pParAgentItem)
        pParAgentItem->EncompassChildren();
}

GAgentGraphicsItem* GAgentGraphicsItem::ParentAgentGraphicsItem() const
{
	return qobject_cast<GAgentGraphicsItem*>(static_cast<GAgentGraphicsItem*>(parentItem()));
}

QList<GAgentGraphicsItem*> GAgentGraphicsItem::ChildAgentGraphicsItems() const
{
	QList<GAgentGraphicsItem*> listAgentChildren;
	foreach(QGraphicsItem* pItem, childItems()) {
		GAgentGraphicsItem* pAgIt = qobject_cast<GAgentGraphicsItem*>(static_cast<GAgentGraphicsItem*>(pItem));
		if(pAgIt)
			listAgentChildren.append(pAgIt);
	}
	return listAgentChildren;
}

void GAgentGraphicsItem::SetRect(const QRectF & rectangle)
{
	m_Rect = rectangle;
	double oldSurface = m_Surface;
	m_Surface = m_Rect.width() * m_Rect.height();
	// portion of the screen used by the actual representation of the rectangle
	static double screenSurface = 800.0 * 450.0;
	m_ApproxPercentOfScreenOnScale1 = m_Surface / screenSurface;
	// we might emit a signal telling that the surface changed by more than a certain amount since the last time this signal was emitted
	if(m_Rect.width() > m_LastEmittedRectChange.width() + SurfaceChangeDimensionForEmittingSignal
		|| m_Rect.height() > m_LastEmittedRectChange.height() + SurfaceChangeDimensionForEmittingSignal) {
		m_LastEmittedRectChange = m_Rect;
		emit RectGrewNoticably(m_LastEmittedRectChange);
	}
}

void GAgentGraphicsItem::SetRect( const QSizeF & size )
{
	m_Rect.setSize(size);
	SetRect(m_Rect);
}

void GAgentGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget * widget /*= 0*/)
{
	const double lod = option->levelOfDetailFromTransform(painter->worldTransform());
	//	// size of the viewport that displays the scene in pixel unit
	//	qDebug() << painter->viewport().size();
	double approxPercentOfScreen = m_ApproxPercentOfScreenOnScale1 * lod * lod;

	if(approxPercentOfScreen < 1e-5)
		return;

	painter->setBrush(m_Brush);
	painter->drawRect(m_Rect);
}

void GAgentGraphicsItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
//	QGraphicsSvgItem::mouseDoubleClickEvent(event);
	if(!m_pAgent)
		return;
	m_pAgent->Event_DoubleClicked(this);
}

QPropertyAnimation* GAgentGraphicsItem::AnimateMoveTo( QPointF newPos, int msecs )
{
	QPropertyAnimation* animation = new QPropertyAnimation(this, "pos", this);
	animation->setDuration(msecs);
	animation->setStartValue(pos());
	animation->setEndValue(newPos);
	animation->start();
	connect(animation, SIGNAL(finished()), this, SIGNAL(FinishedAnimation()), Qt::QueuedConnection);
	// destroys the animation when finished
	connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()), Qt::QueuedConnection);

	GAgentGraphicsItem* pParentItem = ParentAgentGraphicsItem();
	if(pParentItem)
		connect(animation, SIGNAL(valueChanged(QVariant)), pParentItem, SLOT(EncompassChildren()), Qt::QueuedConnection);

	return animation;
}

QPropertyAnimation* GAgentGraphicsItem::AnimateMoveBy( QPointF moveBy, int msecs )
{
	return AnimateMoveTo(pos() + moveBy, msecs);
}

void GAgentGraphicsItem::MoveTo( QPointF newPos )
{
	setPos(newPos);
	ParentAgentGraphicsItem()->EncompassChildren();
}

QSet<GAgentGraphicsItem*> GAgentGraphicsItem::SiblingAgentGraphicsItems()
{
	GAgentGraphicsItem* pParentItem = ParentAgentGraphicsItem();
	if(!pParentItem)
		return QSet<GAgentGraphicsItem*>();
	QSet<GAgentGraphicsItem*> sib = pParentItem->ChildAgentGraphicsItems().toSet();
	sib.remove(this);
	return sib;
}

void GAgentGraphicsItem::ChangeBrushOpacity( double opacity )
{
	QColor colOpacified = m_Brush.color();
	colOpacified.setAlphaF(opacity);
	m_Brush.setColor(colOpacified);
}
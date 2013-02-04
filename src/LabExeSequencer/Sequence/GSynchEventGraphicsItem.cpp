#include "GSynchEventGraphicsItem.h"
#include "GSynchEvent.h"
#include <QPen>
#include <QGraphicsScene>

GSynchEventGraphicsItem::GSynchEventGraphicsItem(GSynchEvent* pEvent, QGraphicsItem* pParentNode)
	: //GEventNode(pParentNode)
	 QObject(0)
	, QGraphicsLineItem(pParentNode)
	, m_pEvent(pEvent)
{
	setToolTip("m_pEvent->objectName()");
	QPen ChannelPen(Qt::green, 3);

	setFlag(QGraphicsItem::ItemIsMovable, false);

	UpdateXPosition();
}

GSynchEventGraphicsItem::~GSynchEventGraphicsItem()
{
}

void GSynchEventGraphicsItem::UpdateXPosition()
{
	double parentPos = 0.0;
	double delay = 0.0;
	if(m_pEvent) {
		delay = m_pEvent->DelayFromParent();
		// if there is a parent, we place this relatively to the parent-item
		GSynchEvent* pParentEvent = qobject_cast<GSynchEvent*>(m_pEvent->parent());
		if(pParentEvent)
			parentPos = pParentEvent->GlobalXPosition();
	}
	setX(parentPos + delay);
}

QVariant GSynchEventGraphicsItem::itemChange( QGraphicsItem::GraphicsItemChange change, const QVariant &value )
{
	// to prevent the selection of the channel synchevent item in the sequence view, but select the one in the event tree view instead
	if(change == ItemSelectedChange) {
		if(value.toBool()) {
//2012-12-12 			Event()->GraphicsEventTreeItem()->scene()->clearSelection();
//2012-12-12 			Event()->GraphicsEventTreeItem()->setSelected(true);
		}
		return false;
	}
	return QGraphicsLineItem::itemChange(change, value);
}

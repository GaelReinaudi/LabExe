#include "GEventTreeLineItem.h"

GEventTreeLineItem::GEventTreeLineItem(QGraphicsItem* pParentItem)
	: QGraphicsLineItem(pParentItem)
{
	QPen ChannelPen(Qt::red, 2);
	// Next line ensures the width of the line is always the same on the screen, no matter what scale or zooming you use.
	ChannelPen.setCosmetic(true);
	setPen(ChannelPen);

	setFlag(QGraphicsItem::ItemIsSelectable);
	// to make the item movable 
	setFlag(QGraphicsItem::ItemIsMovable);
	// to inform of position change relative to parent. See GSynchEventGraphicsItem::itemChange().
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);

	UpdateVerticalPosition();
}

GEventTreeLineItem::~GEventTreeLineItem()
{

}

void GEventTreeLineItem::UpdateVerticalPosition()
{
	// if their is a parent item, we draw the line within the pParentItem->boundingRect(). 
	double heightLine = 11.4159;
	if(parentItem())
		heightLine = parentItem()->boundingRect().height();
	setLine(0.0, 0.0, 0.0, heightLine);
}

QVariant GEventTreeLineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	// corrects the position about to be changed so that the event can't be moved vertically.
	if(change == ItemPositionChange) {
		// value is the new position.
		QPointF newPos = value.toPointF();
		// resets the vertical position to what it was before: it prevents vertical motion.
		newPos.setY(pos().y());
		// prevents going out of the sequence at negative X
		newPos.setX(qMax(0.0, newPos.x()));
		return newPos;
	}
	return QGraphicsItem::itemChange(change, value);
}

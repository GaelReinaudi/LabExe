#include "GPlacerGraphicsItem.h"

GPlacerGraphicsItem::GPlacerGraphicsItem(GPlacerAgent* pAgent, GAgentGraphicsItem *parentItem /*= 0*/)
	: GAgentGraphicsItem(pAgent, parentItem)
{
	SetRect(QRectF(0, 0, 1, 1));
	SetBrush(QBrush(Qt::blue));
	if(!parentItem || !pAgent) {
		qWarning() << "GPlacerGraphicsItem" << "no parent provided to place things within. 514656";
		return;
	}
	connect(parentItem, SIGNAL(ChildAgentGraphicsItemAdded(GAgentGraphicsItem*, int)), this, SLOT(Event_SiblingAgentItemAdded(GAgentGraphicsItem*, int)));
}

GPlacerGraphicsItem::~GPlacerGraphicsItem()
{

}

/////////////////////////////////////////////////////////////////////
/*!
The default implementation calls the placer agent's AddItemToPlace().
When remainingItemsToBeAdded == 0, the signal SiblingItemsAdded() is emitted.
\param:  GAgentGraphicsItem * pAddedSiblingAgentItem : the item that was added to the as a sibling item.
\param:  int remainingItemsToBeAdded : estimation of items that are still to be added when many are added at once.
*////////////////////////////////////////////////////////////////////
void GPlacerGraphicsItem::Event_SiblingAgentItemAdded( GAgentGraphicsItem* pAddedSiblingAgentItem, int remainingItemsToBeAdded /*= 0*/ )
{
	PlacerAgent()->AddItemToPlace(pAddedSiblingAgentItem, remainingItemsToBeAdded);
	if(!remainingItemsToBeAdded) {
 		emit SiblingItemsAdded();
	}
}

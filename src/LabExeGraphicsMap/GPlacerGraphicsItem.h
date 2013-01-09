#ifndef GPLACERGRAPHICSITEM_H
#define GPLACERGRAPHICSITEM_H

#include "GAgentGraphicsItem.h"
#include "GPlacerAgent.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GPlacerGraphicsItem class defines a GAgentGraphicsItem that will help the placer to hook into signals that trigger the placing.
/*!
Technically, it gathers information about its siblings, i.e. the children of its parent graphics item (except itself). 
*/
class GPlacerGraphicsItem : public GAgentGraphicsItem
{
	Q_OBJECT

public:
	GPlacerGraphicsItem(GPlacerAgent* pAgent, GAgentGraphicsItem *parentItem = 0);
	~GPlacerGraphicsItem();

	//! returns the actual placer agent of this item. 0 if something goes wrong.
	GPlacerAgent* PlacerAgent() {return qobject_cast<GPlacerAgent*>(Agent());}

protected slots:
	//! Called when a sibling agent graphics item has been added. 
	virtual void Event_SiblingAgentItemAdded(GAgentGraphicsItem* pAddedSiblingAgentItem, int remainingItemsToBeAdded = 0);

signals:
	//! Emitted when remainingItemsToBeAdded == 0 in Event_SiblingAgentItemAdded().
	void SiblingItemsAdded();//(const QList<GAgentGraphicsItem*> & siblings);

private:
	
};

#endif // GPLACERGRAPHICSITEM_H

#ifndef GSynchEventGRAPHICSITEM_H
#define GSynchEventGRAPHICSITEM_H

#include "GEventTreeLineItem.h"

class GSynchEvent;

/////////////////////////////////////////////////////////////////////
//! \brief The GSynchEventGraphicsItem class represents a line graphics item that is drawn in the GSequenceGraphicsScene.
/*
It is meant to visualize the event as a vertical line, and to forward some 
user interaction to the actual GSynchEvent it represents.

TO UPDATE ????

The parentItem() of the GSynchEventGraphicsItem object does not necessarily corresponds 
to the Event()->parent(). This is because the graphics object might 
be better represented by having a GChannelGraphicsItem in the sequence scene as parent. It is then 
easy to draw the event in the GChannelGraphicsItem, but some update mechanism has to 
take care of updating the horizontal position of the event relatively to its Event()->parent().

In other word, let's consider this situation:
- an event "A" in channel "1" has a graphicsline drawn in the channel "1" graphicsitem.
- one of its child event (meaning synchronized with event "A"), called event "B" is assigned to channel "2"
- the graphicsline of event "B" is in the channel "2" graphicsitem.
- some mechanism updates the position of "B" when "A" moves.
PS: we could also consider the case where event "B" would be assigned to the whole 
sequence (no specific channel assigned) it could then be drawn with the sequence scene as parent (to be clarified actually). 
*/
class GSynchEventGraphicsItem : public GEventTreeLineItem
{

public:
	//! constructor that gives the parent event and the parent item for drawing, i.e. the item in wich the event will be drawn vertically-wise.
	GSynchEventGraphicsItem(GSynchEvent* pEvent, QGraphicsItem* pParentItem);
	~GSynchEventGraphicsItem();

	void UpdateXPosition();

	// Returns the GSynchEvent of this item.
	GSynchEvent* Event() const { return m_pEvent; }

protected:
	//! reimplemented from GSynchEvent to inform of changes in the item properties.
	virtual	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	//! pointer to the GSynchEvent represented by this GraphicsLineItem.
	GSynchEvent* m_pEvent;
};

#endif // GSynchEventGRAPHICSITEM_H

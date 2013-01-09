#ifndef G_old_TIMEEVENT_H
#define G_old_TIMEEVENT_H

#include "GTreeItem.h"
#include "Sequencer/GSynchEventGraphicsItem.h"

class GChannel;

class G_old_TimeEvent : public GTreeItem
{
	Q_OBJECT

public:
	//! construct an event that will be linked to the GChannel (i.e. drawn in the corresponding GChannelGraphicsItem).
	G_old_TimeEvent(GChannel* pOwningChannel);
	G_old_TimeEvent(G_old_TimeEvent* parentEventsynchro = 0);
	~G_old_TimeEvent();

	//! Sets the parent of this event
	void SetParent( G_old_TimeEvent* newparent );
	G_old_TimeEvent* ParentEvent() const;
	//! Returns a QList of all parent events, starting with the higher.
	QList<G_old_TimeEvent*> AllParentEvent() const;
	G_old_TimeEvent* ChildEvent( int row ) const;

	//! Tells the event to take place with this other event.
	void SynchronizeWith(G_old_TimeEvent* pParentEvent);
	//! Tells the event to be delayed by that amount relatively to the parent event (defined with SynchronizeWith()).
	void DelayFromParent(double RelativeDelay);
	double DelayFromParent() {return m_RelativeDelayFromParentEvent;}
	QGraphicsLineItem* EventGraphicsItem() const { return const_cast<GSynchEventGraphicsItem *>(m_pEventGraphicsItem); }
	//! Gives the absolute time of this event. The reference point t=0 is the highest parent event in the tree hierarchy.
	double AbsoluteTime() const;

signals:
	void AbsoluteTimeChanged();

protected slots:
	//! Updates the position of the event item in its parent coordinates. The abscissa is put at AbsoluteTime()
	void UpdateEventItemPosition();

protected:
	//! For painting itself in a QGraphicsView, each event contains an item (that can contain sub-items...)
	GSynchEventGraphicsItem* m_pEventGraphicsItem;

private:
	//! This is the time delay after which this event is positioned, relatively to his parent event
	double m_RelativeDelayFromParentEvent;

	friend class GChannelGraphicsItem;
};

#endif // G_old_TIMEEVENT_H

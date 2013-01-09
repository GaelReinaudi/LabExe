#include "G_old_TimeEvent.h"
#include "Sequencer/GChannel.h"
#include <QPen>

G_old_TimeEvent::G_old_TimeEvent(G_old_TimeEvent* parentEventsynchro /*= 0*/)
	: GTreeItem(parentEventsynchro)
	, m_RelativeDelayFromParentEvent(0.0)// first, cause GSynchEventGraphicsItem may use the DelayFromParentEvent()
{
	m_pEventGraphicsItem = new GSynchEventGraphicsItem(0/*this*/, 0);
}

G_old_TimeEvent::G_old_TimeEvent( GChannel* pOwningChannel )
	: GTreeItem(0/*!pOwningChannel ? 0 : pOwningChannel->SequenceRootTimeEvent()*/)
	, m_RelativeDelayFromParentEvent(0.0)// first this, cause GSynchEventGraphicsItem may use the DelayFromParentEvent()
{
	if(!pOwningChannel){
		qCritical() << "The GChannel for the G_old_TimeEvent can't be a zero pointer! 47932154";
		return;
	}

	// vertically-wise, the GSynchEventGraphicsItem item will be drawn in the pOwningChannel->m_pChanItem
	m_pEventGraphicsItem = new GSynchEventGraphicsItem(0/*this*/, 0/*pOwningChannel->m_pChanItem*/);
}

G_old_TimeEvent::~G_old_TimeEvent()
{
	if(m_pEventGraphicsItem)
		delete m_pEventGraphicsItem;
}

void G_old_TimeEvent::SynchronizeWith( G_old_TimeEvent* pParentEvent )
{
	if(pParentEvent != this)
		SetParent(pParentEvent);
}

G_old_TimeEvent* G_old_TimeEvent::ParentEvent() const
{
	if(ParentTreeItem() && ParentTreeItem()->inherits("G_old_TimeEvent"))
		return static_cast<G_old_TimeEvent*>(ParentTreeItem());
	return 0;
}

QList<G_old_TimeEvent*> G_old_TimeEvent::AllParentEvent() const
{
	QList<G_old_TimeEvent*> ListAllParentEvents;
	G_old_TimeEvent* pParentEvent = ParentEvent();
	if(pParentEvent) {
		ListAllParentEvents.append(pParentEvent->AllParentEvent());
		ListAllParentEvents.append(pParentEvent);
	}
	return ListAllParentEvents;
}

G_old_TimeEvent* G_old_TimeEvent::ChildEvent( int row ) const
{
	GTreeItem* pItem = ChildrenItems().value(row);
	if(!pItem)
		qDebug("no child event: G_old_TimeEvent::ChildEvent( int row )");
	return qobject_cast<G_old_TimeEvent*>(pItem);
}

void G_old_TimeEvent::DelayFromParent( double RelativeDelay )
{
	m_RelativeDelayFromParentEvent = RelativeDelay;
	UpdateEventItemPosition();
	emit AbsoluteTimeChanged();
}

double G_old_TimeEvent::AbsoluteTime() const
{
	G_old_TimeEvent* pParentEvent = ParentEvent();
	if(pParentEvent)
		return m_RelativeDelayFromParentEvent + pParentEvent->AbsoluteTime();
	else
		return m_RelativeDelayFromParentEvent;
}

//************************************
// Method:    SetParent
// FullName:  G_old_TimeEvent::SetParent
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: GTreeItem* newparent
/*
It is a overridden function from GTreeItem since we want to add 
some signal-slot connections. It starts by calling GTreeItem::SetParent().
*/
//************************************
void G_old_TimeEvent::SetParent( G_old_TimeEvent* newparent )
{
	disconnect(this, 0, ParentEvent(), 0);
	GTreeItem::SetParent(newparent);
	connect(ParentEvent(), SIGNAL(AbsoluteTimeChanged()), this, SLOT(UpdateEventItemPosition()));
	connect(ParentEvent(), SIGNAL(AbsoluteTimeChanged()), this, SIGNAL(AbsoluteTimeChanged()));
}

void G_old_TimeEvent::UpdateEventItemPosition()
{
	m_pEventGraphicsItem->setPos(AbsoluteTime(), 0);
}
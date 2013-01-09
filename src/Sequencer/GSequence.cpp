#include "GSequence.h"
#include "GChannel.h"
#include "GSynchEvent.h"
#include "GSynchEventGraphicsItem.h"
#include "GChannelSynchEvent.h"
#include "GInstruction.h"

GSequence::GSequence(QObject *parent)
	: QObject(parent)
	, m_pEventTreeScene(new GSequenceGraphicsScene(this))
	, m_pChannelScene(new GSequenceGraphicsScene(this))
{
	m_pRootTimeEvent = new GSynchEvent(this);
	createActions();

	emit LengthChanged(Length());
}

GSequence::~GSequence()
{
}

void GSequence::createActions()
{
	m_pActionNewChannel = new QAction(QIcon(":/images/save.png"), tr("New &channel"), this);
	m_pActionNewChannel->setStatusTip(tr("Creates a new channel in the sequence"));
	connect(m_pActionNewChannel, SIGNAL(triggered()), this, SLOT(CreateNewChannel()));

	m_pActionNewEvent = new QAction(QIcon(":/images/save.png"), tr("New &event"), this);
	m_pActionNewEvent->setStatusTip(tr("Creates a new event in the sequence"));
	connect(m_pActionNewEvent, SIGNAL(triggered()), this, SLOT(CreateNewEvent()));
}

GChannel* GSequence::CreateNewChannel( ChannelType theType /*= ChannelType::AskType*/ )
{
	GChannel* pChan = new GChannel(this);
//not necessary 	AddChannel(pChan);

	return pChan;
}

void GSequence::PopulateSettings( QSettings& inQsettings )
{
	// channels		///////////////////////////////////////////////////////////////////////////////////////////
	GSerializable::SaveListAndItems<GChannel>(inQsettings, "Channels/List-Channel-ID", m_ChannelList);
	// events		///////////////////////////////////////////////////////////////////////////////////////////
	QList<GSynchEvent*> listEvents = m_pRootTimeEvent->ChildrenEvents();
	GSerializable::SaveListAndItems<GSynchEvent>(inQsettings, "RootEvent/Children", listEvents);
}

void GSequence::ReadFromSettings( QSettings& fromQsettings )
{
	GSerializable::RestoreListOfObjects(fromQsettings, "Channels/List-Channel-ID", this);
	GSerializable::RestoreListOfObjects(fromQsettings, "RootEvent/Children", m_pRootTimeEvent);
}

// TOGO for context menu in sequenceview and channel items
GSynchEvent* GSequence::CreateNewEvent( GSynchEvent* pParentEvent /*= 0*/, GChannel* pAssignedChannelForGraphics /*= 0*/ )
{
	if(pParentEvent && pAssignedChannelForGraphics) 
		return new GChannelSynchEvent(pParentEvent, pAssignedChannelForGraphics);
	
	if(pParentEvent && !pAssignedChannelForGraphics)
		return new GSynchEvent(pParentEvent);

	// if an event and a channel is selected in the GSequenceScene, let's use those to make a new event assigned to the channel
	QList<QGraphicsItem*> listGraphicsItemSelected = ChannelScene()->selectedItems() + EventTreeScene()->selectedItems();
	QList<GSynchEvent*> listEventItem = GSequence::GetEvents(listGraphicsItemSelected);
	QList<GChannel*> listChannelItem = GSequence::GetChannels(listGraphicsItemSelected);

	// if no event and no channel defined
	if(!pParentEvent && !pAssignedChannelForGraphics) {
		// if there are exactly 1 event and 1 channel selected, we use those for parenting the event 
		if(listEventItem.count() == 1 && listChannelItem.count() == 1)
			return CreateNewEvent(listEventItem.at(0), listChannelItem.at(0));
		// if there are exactly 1 event we use it 
		else if(listEventItem.count() == 1)
			return CreateNewEvent(listEventItem.at(0));
	}

	// if no event but a channel defined
	if(!pParentEvent && pAssignedChannelForGraphics) {
		// if there are exactly 1 event and 1 channel selected, we use those for parenting the event 
		if(listEventItem.count() == 1 && listChannelItem.count() == 1 && listChannelItem.at(0) == pAssignedChannelForGraphics)
			return CreateNewEvent(listEventItem.at(0), pAssignedChannelForGraphics);
		// if there are exactly 1 event we use it 
		else if(listEventItem.count() == 1 && listChannelItem.count() == 0)
			return CreateNewEvent(listEventItem.at(0), pAssignedChannelForGraphics);
	}

	// otherwise, create an event child of the m_pRootTimeEvent
	return CreateNewEvent(m_pRootTimeEvent, pAssignedChannelForGraphics);
}

QList<GChannel*> GSequence::GetChannels( QList<QGraphicsItem*> listItems )
{
	QList<GChannel*> listChannelsToReturn;
	foreach(QGraphicsItem* pItem, listItems) {
		GChannelGraphicsItem* pChanItem = dynamic_cast<GChannelGraphicsItem*>(pItem);
		if(pChanItem && pChanItem->Channel())
			listChannelsToReturn.append(pChanItem->Channel());
	}
	return listChannelsToReturn;
}

QList<GSynchEvent*> GSequence::GetEvents( QList<QGraphicsItem*> listItems)
{
	QList<GSynchEvent*> listEventsToReturn;
	foreach(QGraphicsItem* pItem, listItems) {
		// if it is a GSynchEventGraphicsItem, it should have a pointer to its event
		GSynchEventGraphicsItem* pSyncItem = dynamic_cast<GSynchEventGraphicsItem*>(pItem);
		// else if this is a GSynchEvent
		GSynchEvent* pSyncEvent = dynamic_cast<GSynchEvent*>(static_cast<GSynchEvent*>(dynamic_cast<GEventTreeLineItem*>(pItem)));
		if(pSyncItem && pSyncItem->Event())
			listEventsToReturn.append(pSyncItem->Event());
		else if(pSyncEvent) // it could also be a GSynchEvnet directly (it inherits GEventTreeLineItem)
			listEventsToReturn.append(pSyncEvent);
	}
	return listEventsToReturn;
}

void GSequence::AddChannel( GChannel* pChan )
{
	m_ChannelList.append(pChan);
	ChannelScene()->InsertChannelItem(pChan);
}

GSynchEvent* GSequence::GetTheSelectedEvent()
{
	GSynchEvent* pEventToReturn = 0;
	QList<GSynchEvent*> listEv = GetEvents(EventTreeScene()->selectedItems());
	if(listEv.count() == 1)
		pEventToReturn = listEv.at(0);
	return pEventToReturn;
}
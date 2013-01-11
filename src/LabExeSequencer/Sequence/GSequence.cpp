#include "GSequence.h"
#include "GnewChannel.h"
#include "GSynchEvent.h"
#include "GnewChannelSynchEvent.h"
#include "GnewInstruction.h"
#include "GnewSynchEventGraphicsItem.h"
//#include "GSequenceEventItem.h"

GSequence::GSequence(QObject *parent)
	: GContentAgent<GSequence, GSequenceGraphicsItem>(parent)
	, m_pEventTreeScene(new GEvScene(this))
	, m_pChannelScene(new GEvScene(this))
{
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

GnewChannel* GSequence::CreateNewChannel( ChannelType theType /*= ChannelType::AskType*/ )
{
	GnewChannel* pChan = new GnewChannel(this);
//not necessary 	AddChannel(pChan);

	return pChan;
}

void GSequence::PopulateSettings( QSettings& inQsettings )
{
	GContentAgent<GSequence, GSequenceGraphicsItem>::PopulateSettings(inQsettings);
	// channels		///////////////////////////////////////////////////////////////////////////////////////////
	GSerializable::SaveListAndItems<GnewChannel>(inQsettings, "Channels/List-Channel-ID", m_ChannelList);
	// events		///////////////////////////////////////////////////////////////////////////////////////////
	QList<GSynchEvent*> listEvents = RootTimeEvent()->ChildrenEvents();
	GSerializable::SaveListAndItems<GSynchEvent>(inQsettings, "RootEvent/Children", listEvents);
}

void GSequence::InterpretSettings( QSettings& fromQsettings )
{
	GSerializable::RestoreListOfObjects(fromQsettings, "Channels/List-Channel-ID", this);
	GSerializable::RestoreListOfObjects(fromQsettings, "RootEvent/Children", RootTimeEvent());
}

// TOGO for context menu in sequenceview and channel items
GSynchEvent* GSequence::CreateNewEvent( GSynchEvent* pParentEvent /*= 0*/, GnewChannel* pAssignedChannelForGraphics /*= 0*/ )
{
	if(pParentEvent && pAssignedChannelForGraphics)  {
		return new GnewChannelSynchEvent(pParentEvent, pAssignedChannelForGraphics);
	}
	
	if(pParentEvent && !pAssignedChannelForGraphics) {
//
// 		GSequenceEventItem* pGE = new GSequenceEventItem();
// 		static int i = 0;
// 		pGE->setText(QString("Yep!%1").arg(i++));
// 		m_Seq.invisibleRootItem()->appendRow(pGE);
//
		GSynchEvent* pEv = new GSynchEvent(pParentEvent);
		return pEv;
	}

	// if an event and a channel is selected in the GEvScene, let's use those to make a new event assigned to the channel
	QList<QGraphicsItem*> listGraphicsItemSelected = ChannelScene()->selectedItems() + EventTreeScene()->selectedItems();
	QList<GSynchEvent*> listEventItem = GSequence::GetEvents(listGraphicsItemSelected);
	QList<GnewChannel*> listChannelItem = GSequence::GetChannels(listGraphicsItemSelected);

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

	// otherwise, create an event child of the RootTimeEvent()
	return CreateNewEvent(RootTimeEvent(), pAssignedChannelForGraphics);
}

QList<GnewChannel*> GSequence::GetChannels( QList<QGraphicsItem*> listItems )
{
	QList<GnewChannel*> listChannelsToReturn;
	foreach(QGraphicsItem* pItem, listItems) {
		GnewChannelGraphicsItem* pChanItem = dynamic_cast<GnewChannelGraphicsItem*>(pItem);
		if(pChanItem && pChanItem->Channel())
			listChannelsToReturn.append(pChanItem->Channel());
	}
	return listChannelsToReturn;
}

QList<GSynchEvent*> GSequence::GetEvents( QList<QGraphicsItem*> listItems)
{
	QList<GSynchEvent*> listEventsToReturn;
	foreach(QGraphicsItem* pItem, listItems) {
		// if it is a GnewSynchEventGraphicsItem, it should have a pointer to its event
		GnewSynchEventGraphicsItem* pSyncItem = dynamic_cast<GnewSynchEventGraphicsItem*>(pItem);
		// else if this is a GSynchEvent
		GSynchEvent* pSyncEvent = dynamic_cast<GSynchEvent*>(static_cast<GSynchEvent*>(dynamic_cast<GEventNode*>(pItem)));
		if(pSyncItem && pSyncItem->Event())
			listEventsToReturn.append(pSyncItem->Event());
		else if(pSyncEvent) // it could also be a GSynchEvnet directly (it inherits GEventNode)
			listEventsToReturn.append(pSyncEvent);
	}
	return listEventsToReturn;
}

void GSequence::AddChannel( GnewChannel* pChan )
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
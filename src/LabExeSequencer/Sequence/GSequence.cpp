#include "GSequence.h"
#include "GChannel.h"
#include "GSynchEvent.h"
#include "GSequenceEventItem.h"

#include <QtDeclarative>

GSequence::GSequence(QObject *parent)
	: QObject(parent)
	, m_pEventTreeScene(new GEvScene(this))
	, m_Length("lenght", this)
{
	connect(&m_Length, SIGNAL(ValueUpdated(double)), this, SIGNAL(LengthChanged(double)));

	qmlRegisterType<GSequence>("GSequence", 1, 0, "Sequence");

	emit LengthChanged(Length());

// 	// test
// 	GChannel* pChan = new GChannel(this);
// 	AddChannel(pChan);
}

GSequence::~GSequence()
{
}

GChannel* GSequence::CreateNewChannel( ChannelType theType /*= ChannelType::AskType*/ )
{
	GChannel* pChan = new GChannel(this);
 	AddChannel(pChan);

	return pChan;
}

void GSequence::PopulateSettings( QSettings& inQsettings )
{
	// channels		///////////////////////////////////////////////////////////////////////////////////////////
	GSerializable::SaveListAndItems<GChannel>(inQsettings, "Channels/List-Channel-ID", m_ChannelList);
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
GSynchEvent* GSequence::CreateNewEvent( GSynchEvent* pParentEvent /*= 0*/, GChannel* pAssignedChannelForGraphics /*= 0*/ )
{
// 	if(pParentEvent && pAssignedChannelForGraphics)  {
// 		return new GChannelSynchEvent(pParentEvent, pAssignedChannelForGraphics);
// 	}
	
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
	GSynchEvent* pEv = new GSynchEvent(RootTimeEvent());
	return pEv;
}

void GSequence::AddChannel( GChannel* pChan )
{
	m_ChannelList.append(pChan);

	emit NumChannelsChanged(NumChannels());
}

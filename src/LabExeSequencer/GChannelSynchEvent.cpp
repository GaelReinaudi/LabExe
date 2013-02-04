#include "GChannelSynchEvent.h"
#include "Sequence/GSynchEventGraphicsItem.h"
#include "Sequence/GSequence.h"
#include "GChannel.h"
#include "../ToolBox/GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GChannelSynchEvent, GSynchEvent)


GChannelSynchEvent::GChannelSynchEvent(GSynchEvent* parentEventSynchro, GChannel* pAssignedChannel /*= 0*/)
	: GSynchEvent(parentEventSynchro)
	, m_pChannel(pAssignedChannel)
	, m_pEventGraphicsItem(new GSynchEventGraphicsItem(this, 0))
{
	setFlag(ItemSendsScenePositionChanges);
	connect(this, SIGNAL(AbsoluteTimeChanged(double)), this, SLOT(UpdatePositionChannelEventItem(double)));

	if(pAssignedChannel) {
		AssignChannel(pAssignedChannel);
	}
}

GChannelSynchEvent::~GChannelSynchEvent()
{

}

void GChannelSynchEvent::PopulateSettings( QSettings& inQsettings )
{
	if(m_pChannel)
		inQsettings.setValue("AssignedChannel", m_pChannel->UniqueSystemID());
	GSynchEvent::PopulateSettings(inQsettings);
}

void GChannelSynchEvent::InterpretSettings( QSettings& fromQsettings )
{
	QString strChanID = fromQsettings.value("AssignedChannel").toString();
	GChannel* pChan = qobject_cast<GChannel*>(GSerializable::RestoredObject(strChanID));
	AssignChannel(pChan);
	GSynchEvent::InterpretSettings(fromQsettings);
}

void GChannelSynchEvent::AssignChannel( GChannel* pChan )
{
	m_pChannel = pChan;
	if(m_pChannel)
		m_pEventGraphicsItem->setParentItem(pChan->ChannelGraphicsItem());
}

void GChannelSynchEvent::UpdatePositionChannelEventItem( double newAbsoluteTime )
{
	m_pEventGraphicsItem->setX(newAbsoluteTime);
}
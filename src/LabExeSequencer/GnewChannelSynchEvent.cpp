#include "GnewChannelSynchEvent.h"
#include "Sequence/GnewSynchEventGraphicsItem.h"
#include "Sequence/GSequence.h"
#include "GnewChannel.h"
#include "../ToolBox/GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GnewChannelSynchEvent, GSynchEvent)


GnewChannelSynchEvent::GnewChannelSynchEvent(GSynchEvent* parentEventSynchro, GnewChannel* pAssignedChannel /*= 0*/)
	: GSynchEvent(parentEventSynchro)
	, m_pChannel(pAssignedChannel)
	, m_pEventGraphicsItem(new GnewSynchEventGraphicsItem(this, 0))
{
	setFlag(ItemSendsScenePositionChanges);
	connect(this, SIGNAL(AbsoluteTimeChanged(double)), this, SLOT(UpdatePositionChannelEventItem(double)));

	if(pAssignedChannel) {
		AssignChannel(pAssignedChannel);
	}
}

GnewChannelSynchEvent::~GnewChannelSynchEvent()
{

}

void GnewChannelSynchEvent::PopulateSettings( QSettings& inQsettings )
{
	if(m_pChannel)
		inQsettings.setValue("AssignedChannel", m_pChannel->UniqueSystemID());
	GSynchEvent::PopulateSettings(inQsettings);
}

void GnewChannelSynchEvent::InterpretSettings( QSettings& fromQsettings )
{
	QString strChanID = fromQsettings.value("AssignedChannel").toString();
	GnewChannel* pChan = qobject_cast<GnewChannel*>(GSerializable::RestoredObject(strChanID));
	AssignChannel(pChan);
	GSynchEvent::InterpretSettings(fromQsettings);
}

void GnewChannelSynchEvent::AssignChannel( GnewChannel* pChan )
{
	m_pChannel = pChan;
	if(m_pChannel)
		m_pEventGraphicsItem->setParentItem(pChan->ChannelGraphicsItem());
}

void GnewChannelSynchEvent::UpdatePositionChannelEventItem( double newAbsoluteTime )
{
	m_pEventGraphicsItem->setX(newAbsoluteTime);
}
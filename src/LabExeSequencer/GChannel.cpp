#include "GChannel.h"
#include "Sequence/GSequence.h"
#include "Sequence/GSynchEvent.h"
#include "GInstruction.h"
#include "../ToolBox/GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GChannel, GSequence)

GChannel::GChannel(GSequence* parentSequence)
	: QObject(parentSequence)
	, m_pChannelItem(new GChannelGraphicsItem(this))
{
	if(!parentSequence)
		qCritical() << "The GSequence for the GChannel can't be a zero pointer! 1561746";
	else 
		parentSequence->AddChannel(this);
}

GChannel::~GChannel()
{

}

GSequence* GChannel::Sequence()
{
	return qobject_cast<GSequence*>(parent());
}

void GChannel::PopulateSettings( QSettings& inQsettings )
{
}

void GChannel::InterpretSettings( QSettings& fromQsettings )
{
}

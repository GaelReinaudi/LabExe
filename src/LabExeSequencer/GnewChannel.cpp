#include "GnewChannel.h"
#include "Sequence/GSequence.h"
#include "Sequence/GSynchEvent.h"
#include "GnewInstruction.h"
#include "../ToolBox/GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GnewChannel, GSequence)

GnewChannel::GnewChannel(GSequence* parentSequence)
	: QObject(parentSequence)
	, m_pChannelItem(new GnewChannelGraphicsItem(this))
{
	if(!parentSequence)
		qCritical() << "The GSequence for the GnewChannel can't be a zero pointer! 1561746";
	else 
		parentSequence->AddChannel(this);

	createActions();
}

GnewChannel::~GnewChannel()
{

}

GSequence* GnewChannel::Sequence()
{
	return qobject_cast<GSequence*>(parent());
}

void GnewChannel::createActions()
{
	m_pActionNewEvent = new QAction(QIcon(":/images/save.png"), tr("New &Event"), this);
	m_pActionNewEvent->setStatusTip(tr("Creates a new event synchronized with the root event of the sequence."));
	connect(m_pActionNewEvent, SIGNAL(triggered()), this, SLOT(CreateNewSynchEvent()));

	m_pActionNewInstruction = new QAction(QIcon(":/images/save.png"), tr("New &dummy instruction"), this);
	m_pActionNewInstruction->setStatusTip(tr("Creates a new instruction synchronized with the selected event."));
	connect(m_pActionNewInstruction, SIGNAL(triggered()), this, SLOT(CreateNewInstruction()));
}

void GnewChannel::PopulateSettings( QSettings& inQsettings )
{
}

void GnewChannel::InterpretSettings( QSettings& fromQsettings )
{
}

GnewInstruction* GnewChannel::CreateNewInstruction( QString strRegisteredType /*= ""*/, GSynchEvent* pParentEvent /*= 0*/ )
{
	GnewInstruction* pInsToReturn = 0;
	// this is supposed (for now) to be called by a action from a menu parented by a GnewChannel
	// we get the registered type of the instruction that has to be made
	QAction* pSenderAction = qobject_cast<QAction*>(sender());
	if(pSenderAction && strRegisteredType == "")
		strRegisteredType = pSenderAction->data().toString();

	// now we get the event to parent the instruction
	if(!pParentEvent)
		pParentEvent = Sequence()->GetTheSelectedEvent();
	// if still no event, use the root event
	if(!pParentEvent)
		pParentEvent = Sequence()->RootTimeEvent();

	if(pParentEvent && strRegisteredType != "") {
		pInsToReturn = qobject_cast<GnewInstruction*>(ObjectFactoryInstance()->CreateNewObject(strRegisteredType, pParentEvent));
		pInsToReturn->AssignChannel(this);
	}
	if(!pInsToReturn)
		qWarning("the Instruction could not be created. 284532");
	return pInsToReturn;
}

GSynchEvent* GnewChannel::CreateNewSynchEvent( GSynchEvent* pParentEvent /*= 0*/ )
{
	GSynchEvent* pEventToReturn = 0;
	// now we get the event to parent the instruction
	if(!pParentEvent)
		pParentEvent = Sequence()->GetTheSelectedEvent();
	// if still no event, use the root event
	if(!pParentEvent)
		pParentEvent = Sequence()->RootTimeEvent();

	if(pParentEvent) {
		pEventToReturn = new GnewChannelSynchEvent(pParentEvent, this);
	}
	if(!pEventToReturn)
		qWarning("the event could not be created. 284532");
	return pEventToReturn;
}

#include "GChannel.h"
#include "GSequence.h"
#include "GSynchEvent.h"
#include "GInstruction.h"
#include "GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GChannel, GSequence)

GChannel::GChannel(GSequence* parentSequence)
	: QObject(parentSequence)
	, m_pChannelItem(new GChannelGraphicsItem(this))
{
	if(!parentSequence)
		qCritical() << "The GSequence for the GChannel can't be a zero pointer! 1561746";
	else 
		parentSequence->AddChannel(this);

	createActions();
}

GChannel::~GChannel()
{

}

GSequence* GChannel::Sequence()
{
	return qobject_cast<GSequence*>(parent());
}

void GChannel::createActions()
{
	m_pActionNewEvent = new QAction(QIcon(":/images/save.png"), tr("New &Event"), this);
	m_pActionNewEvent->setStatusTip(tr("Creates a new event synchronized with the root event of the sequence."));
	connect(m_pActionNewEvent, SIGNAL(triggered()), this, SLOT(CreateNewSynchEvent()));

	m_pActionNewInstruction = new QAction(QIcon(":/images/save.png"), tr("New &dummy instruction"), this);
	m_pActionNewInstruction->setStatusTip(tr("Creates a new instruction synchronized with the selected event."));
	connect(m_pActionNewInstruction, SIGNAL(triggered()), this, SLOT(CreateNewInstruction()));
}

void GChannel::PopulateContextMenu( QMenu* pMenu )
{
	pMenu->addAction(m_pActionNewEvent);
	pMenu->addAction(m_pActionNewInstruction);
	// test the insertion of an action and providing data through the action setData. data will be retrieved by sender()->data()
	m_Menu.clear();
	m_Menu.addSeparator();
	// sets the title
	m_Menu.setTitle(QString("Channel: %1").arg(objectName()));
	// sets the new channel-synch-event action
	QAction* pNewEventAction = m_Menu.addAction("New synch-event", this, SLOT(CreateNewSynchEvent()));

	// sets the new instruction action
	QAction* pNewInsAction = m_Menu.addAction("New digital instruction", this, SLOT(CreateNewInstruction()));
	pNewInsAction->setData("GDigitalInstruction");

//	pMenu->addMenu(&m_Menu);
	pMenu->addActions(m_Menu.actions());
}

void GChannel::PopulateSettings( QSettings& inQsettings )
{
}

void GChannel::ReadFromSettings( QSettings& fromQsettings )
{
}

GInstruction* GChannel::CreateNewInstruction( QString strRegisteredType /*= ""*/, GSynchEvent* pParentEvent /*= 0*/ )
{
	GInstruction* pInsToReturn = 0;
	// this is supposed (for now) to be called by a action from a menu parented by a GChannel
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
		pInsToReturn = qobject_cast<GInstruction*>(ObjectFactoryInstance()->CreateNewObject(strRegisteredType, pParentEvent));
		pInsToReturn->AssignChannel(this);
	}
	if(!pInsToReturn)
		qWarning("the Instruction could not be created. 284532");
	return pInsToReturn;
}

GSynchEvent* GChannel::CreateNewSynchEvent( GSynchEvent* pParentEvent /*= 0*/ )
{
	GSynchEvent* pEventToReturn = 0;
	// now we get the event to parent the instruction
	if(!pParentEvent)
		pParentEvent = Sequence()->GetTheSelectedEvent();
	// if still no event, use the root event
	if(!pParentEvent)
		pParentEvent = Sequence()->RootTimeEvent();

	if(pParentEvent) {
		pEventToReturn = new GChannelSynchEvent(pParentEvent, this);
	}
	if(!pEventToReturn)
		qWarning("the event could not be created. 284532");
	return pEventToReturn;
}

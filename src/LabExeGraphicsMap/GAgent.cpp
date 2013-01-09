#include "GAgent.h"
#include "GAgentGraphicsItem.h"
#include <QChildEvent>

#include "Agent/GAgentManager.h"												

GAgentManager* AgentManagerInstance() { return &(GAgentManagerSing::Instance()); }

bool RegisterProgAgentInManager( const QString & className, GAgent*(deviceCreator)(QString, QObject*) )
{
	bool isOk = AgentManagerInstance()->Register(className, deviceCreator);
	if(isOk)
		AgentManagerInstance()->RegisterProgAgent(className);
	return isOk;
}

GAgent::GAgent(QObject *parent, QString uniqueIdentifierName /*= ""*/)
	: QThread(parent)
	, GSerializable(uniqueIdentifierName)
{
	GAgent* pParentAgent = ParentAgent();
	if(pParentAgent)
		emit pParentAgent->ChildAgentAdded(this);
}

GAgent::~GAgent()
{

}

/////////////////////////////////////////////////////////////////////
/*!
NOTE: most of the time, this thing will not emit. Because the created object that is becoming a child is not finished constructed.
it will only work when setParent is called on the child or other mechanism adds the fully constructed child to this agent.
is the added child a GAgent
*////////////////////////////////////////////////////////////////////
void GAgent::childEvent( QChildEvent* pChildEvent )
{
	GAgent* pChildAgent = qobject_cast<GAgent*>(pChildEvent->child());
	if(pChildEvent->added() && pChildAgent)
		emit ChildAgentAdded(pChildAgent);
}

QList<GAgent*> GAgent::ChildAgents() const
{
	QList<GAgent*> listAgentChildren;
	foreach(QObject* pObj, QObject::children()) {
		GAgent* pAg = qobject_cast<GAgent*>(pObj);
		if(pAg)
			listAgentChildren.append(pAg);
	}
	return listAgentChildren;
}

GAgent* GAgent::ParentAgent() const
{
	return qobject_cast<GAgent*>(QObject::parent());
}

QList<GAgent*> GAgent::SiblingAgents() const
{
	QList<GAgent*> listSibAg;
	GAgent* pParAgent = ParentAgent();
	if(pParAgent) {
		listSibAg = pParAgent->ChildAgents();
		listSibAg.removeOne(const_cast<GAgent *const>(this));
	}
	return listSibAg;
}

// When this function is called needs to be tweaked. Currently, the UnParenting takes place too early, so that as sibling folders get fired,
// they have no knowledge of where previously fired, and now un-parented, siblings have been placed. This causes lots of overlap.
void GAgent::UnParent()
{
// 	GAgent* pParentAgent = ParentAgent();
// 	if(pParentAgent)
// 		emit pParentAgent->ChildAgentRemoved(this);

	setParent(0);
	emit UnParented();
}

void GAgent::Event_UniqueSystemIDChanged( QString newSystemID )
{

}
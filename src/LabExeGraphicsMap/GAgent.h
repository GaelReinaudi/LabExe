#ifndef GAGENT_H
#define GAGENT_H
#include "labexegraphicsmap_global.h"


#include "ToolBox/GSerializable.h"
#include "GAgentGraphicsItem.h"
#include <QThread>

class GAgentManager;
//! Returns the Agent manager
LABEXEGRAPHICSMAP_EXPORT GAgentManager* AgentManagerInstance(); 

/////////////////////////////////////////////////////////////////////
//! \brief The GAgent class is the base abstract class of any agent in the scene bench and that can provide a graphics item.
/*!
The user interface will be provided by calling the ProvideNewAgentGraphicsItem() function. The provided GraphicsItem 
should be in charge of the interaction with the agent (e.g. updates from the agent, or click events to the agent). 

- Agents can parent other agents and for a tree structure. 
- Agents can react at a child being added by re-implementing the QObject::childEvent() method.
- By default a signal ChildAgentAdded() is emitted when an agent becomes a child of this GAgent object.
*/
class LABEXEGRAPHICSMAP_EXPORT GAgent : public QThread, public GSerializable
{
	Q_OBJECT

public:
	GAgent(QObject *parent, QString uniqueIdentifierName = "");
	virtual ~GAgent();

	//! Returns the direct children agent of this agent.
	QList<GAgent*> ChildAgents() const;
	//! Returns the siblings agent of this agent, i.e. the child agent of its parent, except itself.
	QList<GAgent*> SiblingAgents() const;
	//! Returns the Parent agent of this agent. 0 if none.
	GAgent* ParentAgent() const;
    //! Removes the parent from an agent. This is for removing folders from parent folders to disengage the hierarchy so that folders don't drag child folders.
    void UnParent();

	//! Returns a new graphics item that represents the object. 
	virtual GAgentGraphicsItem* ProvideNewAgentGraphicsItem(GAgentGraphicsItem* pParentItem) = 0;

public:
	//! Re-implemented 
	virtual void PopulateSettings(QSettings& inQsettings) {}
	//! Re-implemented 
	virtual void ReadFromSettings(QSettings& fromQsettings) {}

protected:
	//! Called by a graphics item when it is double-clicked. pOnWhichItem is the graphics item that actually got clicked. Default does nothing.
	virtual void Event_DoubleClicked(GAgentGraphicsItem* pOnWhichItem) {}

protected:
	//! Re-implemented. Checks if the child is a GAgent and send the appropriate signal (e.g. to the graphics items).
	void childEvent(QChildEvent* pChildEvent);
	//! Re-implemented to add the new ID in the AgentManagerInstance(). It doesn't remove the previous ID.
	void Event_UniqueSystemIDChanged(QString newSystemID);

signals:
	//! Emitted when a GAgent child is added to this GAgent object
	void ChildAgentAdded(GAgent* pAgent);
    void UnParented();

private:
// 	//! Makes an agent for the given directory if it doesn't already exists.
// 	void MakeDirectoryAgent();
	
friend class GAgentGraphicsItem;
};

Q_DECLARE_METATYPE(GAgent*)

/////////////////////////////////////////////////////////////////////
//! \brief GHasAgent is a class that defines something that contains a GAgent object.
/*!
This is because some devices should be able to provide agents for a graphics view.
Using this class, we thus can have a GAgent in a GDevice (through multiple inheritance from GHasAgent).
*/
class LABEXEGRAPHICSMAP_EXPORT GHasAgent
{

public:
	GHasAgent(GAgent* pAgent) {m_pAgent = pAgent;}
	virtual ~GHasAgent() {}

	//! Returns the agent that is provided in the constructor.
	GAgent* Agent() const { return m_pAgent; }

protected:
	//! The agent that is provided in the constructor.
	GAgent* m_pAgent;

};

//! Registers the prog device into the device manager.
LABEXEGRAPHICSMAP_EXPORT bool RegisterProgAgentInManager(const QString & className, GAgent*(deviceCreator)(QString, QObject*)); 

/*! \def G_REGISTER_PROG_AGENT_CLASS(ClassName)
\brief This is the Macro that makes your derived class from GProgAgent available to the Agent factory. 
You must put it in your derived class cpp file.If your Agent is defined in a plugin, 
you should use the G_REGISTER_PLUGIN_PROG_AGENT instead (in the .h file).

It is this that allows you to de-serialize derived Agent using the name of the corresponding class. 
Take a look here: 
- "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
*/
#define G_REGISTER_PROG_AGENT_CLASS(ClassName)														\
	namespace																						\
{ 																									\
	int	nProgAgCreated = 0;																		\
	/*! Definition of a function that returns a new instance of a Agent !*/						\
	GAgent* CreateAgent(QString uniqueIdentifierName, QObject *parent)							\
	{																								\
		nProgAgCreated++;																			\
		GAgent* pNAg = 0;																			\
		pNAg = new ClassName(parent);						\
		pNAg->ChangeUniqueSystemID(uniqueIdentifierName);	\
		QString strName(pNAg->metaObject()->className());											\
		strName += QString("-%1").arg(nProgAgCreated);												\
		pNAg->setObjectName(strName);																		\
		return pNAg;																				\
	}																								\
	const bool regPrDe = RegisterProgAgentInManager( # ClassName , CreateAgent);					\
}


#endif // GAGENT_H

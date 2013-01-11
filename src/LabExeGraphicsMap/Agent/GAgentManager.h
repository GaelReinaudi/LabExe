#ifndef GAgentManager_H
#define GAgentManager_H
#include "labexegraphicsmap_global.h"

// to prevent static error linking when defining agents in a library (eg phidget, pxi). see loki's doc.
#define LOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT 

#include "loki/Factory.h"
#include "loki/Singleton.h"
#include <QString>
#include <QHash>
#include <QtGlobal>
#include <QObjectCleanupHandler>
class QMimeData;

class QObject;
class GAgent;
class GAgentShelf;
class GAgentPlugin;
class GAgentManager;

// A custom FactoryError that doesn't throw an exception when OnUnknownType() of GAgent
template <typename IdentifierType, class AbstractProduct>
struct MyAgentFactoryError
{
	struct Exception : public std::exception
	{
		const char* what() const throw() { return "Unknown Type"; }
	};

	static AbstractProduct* OnUnknownType(IdentifierType s)
	{
		QString mess("The Agent type \"%1\" has not been registered with the AgentFacory.");
			mess += "\r\n You should use the Macro G_REGISTER_AGENT_CLASS(%1) in the corresponding .cpp file.";
			mess = mess.arg(QString(s));
		qCritical(mess.toUtf8());
		return 0;
	}
};

class _GAgentManager_ToSingletonize_DO_NOT_USE_AS_IS 
	: public Loki::Factory<	  GAgent
							, QString
							, Loki::Typelist<QString, Loki::Typelist< QObject*, Loki::NullType > > 
							, MyAgentFactoryError>
{
public:
	_GAgentManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
	~_GAgentManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
};

/////////////////////////////////////////////////////////////////////
//! \brief The agent manager is an object factory for the abstract class GAgent.
/*!
In order for a Agent to be instantiated by the GAgentManager, it must:
- inherit GAgent
- have a constructor taking as argument: (QString uniqueID, QObject pParentObject).
- have the macro G_REGISTER_AGENT_CLASS at the top of its corresponding .cpp file

If you need to learn what is and what is the use to an object factory, you should look for "factory pattern" on Google,
or better, read "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
It is also a singleton: there is only one instance of agent manager in the program.

In order to use this singleton agent manager you have to use the (typical) syntax:
ex:\code
AgentManagerInstance()->SomeMemberFunction();
\endcode

The Agent manager also store all the pointer to all the created GAgent's (created by himself or not).
It is easy to obtain any GAgent* by using its uniqueIdentifierName:
ex:\code
GAgent* pag = GetAgentFromAgentManager(uniqueIdentifierName);
\endcode
*/
class LABEXEGRAPHICSMAP_EXPORT GAgentManager
	: public QObject 
	, public _GAgentManager_ToSingletonize_DO_NOT_USE_AS_IS
	, private QHash<QString, GAgent*>
{
	Q_OBJECT

public:
	GAgentManager();
	~GAgentManager();

	//! Creates a new Agent from the provided class name, with a provided unique uniqueIdentifierName
	GAgent* CreateNewAgent(QString className, QString uniqueIdentifierName, QObject *parent) const;
	//! Creates a new GProgAgent using the class name provided and parent. It checks that the name is in the RegisteredProgAgents() to prevent using this function on an hardware Agent.
	GAgent* CreateNewProgAgent(QString className, QObject *parent) const;
	bool Contains(QString uniqueIdentifierName) const {return contains(uniqueIdentifierName);}
	//! Adds the pair <uniqueId, pointer to agent> to the hash of agents.
	void Add(GAgent* pAgent);
	int Remove(QString uniqueIdentifierName) {return remove(uniqueIdentifierName);}
	//! Returns the Agent whose uniqueIdentifierName matches, 0 otherwise.
	GAgent* GetAgent(QString uniqueIdentifierName) const {return value(uniqueIdentifierName, 0);}
	//! Overload provided for convenience. It takes a const QMimeData* from e.g. a DropEvent. 0 if no match.
	GAgent* GetAgent( const QMimeData* theMimeData ) const;
	//! Returns a list of all the type registered in the factory.
	QList<QString> RegisteredAgentTypes() const;
	QList<GAgent*> AllAgents() const {return values();}

	//! Loads the GAgentPlugin's from the directory AgentPlugins in the application directory. Those will later be used to add Agents in the agent shelf.
	void LoadAgentPlugins();

// 	//! Adds a Agent to the Agent shelf. Will create the shelf object if it doesn't exist
// 	void AddAgentToShelf(GAgent* pAgent);
// 	//! Returns the pointer to the hardware shelf.
// 	GAgentShelf* HardwareAgentShelf() const { MakeTheAgentShelf(); return m_pHardAgentShelf; }

private:
	//! keeps track of the GProgAgent`s class name registered with G_REGISTER_PROG_AGENT_CLASS.
	QHash<QString, QString> m_RegisteredProgAgent;
	//! The main Agent shelf. Can contain other shelves if the Agents returned by AgentsToShelf() have a shelf to.
	mutable GAgentShelf* m_pHardAgentShelf;
// 	//! Creates the Agent shelf. Cannot be done in the constructor because it is being called before the app actually starts.
// 	void MakeTheAgentShelf() const;

public:
	//! GProgAgent`s class name that have been registered with the macro G_REGISTER_PROG_AGENT_CLASS.
	QList<QString> RegisteredProgAgents() const { return m_RegisteredProgAgent.keys(); }
	//! Registers the GProgAgent`s class name. Called by the macro G_REGISTER_PROG_AGENT_CLASS.
	bool RegisterProgAgent(QString ProgAgentsClassName) { m_RegisteredProgAgent.insert(ProgAgentsClassName, "not-used-yet"); return true;}
	//! intended to help cleanup when creating Agents with parent = 0
	mutable QObjectCleanupHandler m_CleanUpAgents;
};

typedef Loki::SingletonHolder<GAgentManager> GAgentManagerSing;

#endif // GAgentManager_H


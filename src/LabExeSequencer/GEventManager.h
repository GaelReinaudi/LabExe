#ifndef GEventManager_H
#define GEventManager_H
#include "labexe_global.h"

// to prevent static error linking when defining events in a library (eg phidget, pxi). see loky's doc.
#define LOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT 

#include "loki/Factory.h"
#include "loki/Singleton.h"
#include <QString>
#include <QHash>
#include <QMessageBox>
#include <QtGlobal>
#include <QObjectCleanupHandler>
class QMimeData;

class QObject;
class GEventNode;
class GEventShelf;
class GEventPlugin;
class GEventManager;

// A custom FactoryError that doesn't throw an exception when OnUnknownType() of GEventNode
template <typename IdentifierType, class AbstractProduct>
struct MyEventFactoryError
{
	struct Exception : public std::exception
	{
		const char* what() const throw() { return "Unknown Type"; }
	};

	static AbstractProduct* OnUnknownType(IdentifierType s)
	{
		QString mess("The event type \"%1\" has not been registered with the EventFacory.");
			mess += "\r\n You should use the Macro G_REGISTER_HARD_DEVICE_CLASS(%1) in the corresponding .cpp file.";
			mess = mess.arg(QString(s));
		qCritical(mess.toUtf8());
		return 0;
	}
};

class _GEventManager_ToSingletonize_DO_NOT_USE_AS_IS 
	: public Loki::Factory<  GEventNode
							, QString
							, Loki::Typelist<QString, Loki::Typelist< QObject*, Loki::NullType > > 
							, MyEventFactoryError>
{
public:
	_GEventManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
	~_GEventManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
};

/////////////////////////////////////////////////////////////////////
//! \brief The event manager is an object factory for the abstract class GEventNode.
/*!
In order for a event to be instantiated by the GEventManager, it must:
- inherit GEventNode
- have a constructor taking as argument: (QString uniqueID, QObject pParentObject).
- have the macro G_REGISTER_HARD_DEVICE_CLASS at the top of its corresponding .cpp file

If you need to learn what is and what is the use to an object factory, you should look for "factory pattern" on google,
or better, read "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
It is also a singleton: there is only one instance of event manager in the program.

In order to use this singleton event manager you have to use the (typical) syntax:
ex:\code
EventManagerInstance()->SomeMemberFunction();
\endcode

The event manager also store all the pointer to all the created GEventNode's (created by himself or not).
It is easy to obtain any GEventNode* by using its uniqueIdentifierName:
ex:\code
GEventNode* pdev = GetEventFromEventManager(uniqueIdentifierName);
\endcode
*/
class GEventManager: public QObject, 
	public _GEventManager_ToSingletonize_DO_NOT_USE_AS_IS
	, private QHash<QString, GEventNode*>
{
	Q_OBJECT

public:
	GEventManager();
	~GEventManager();

	//! Creates a new event from the provided class name, with a provided unique uniqueIdentifierName
	GEventNode* CreateNewEvent(QString className, QString uniqueIdentifierName, QObject *parent) const;
	//! Creates a new GProgEvent using the class name provided and parent. It checks that the name is in the RegisteredProgEvents() to prevent using this function on an hardware event.
	GEventNode* CreateNewProgEvent(QString className, QObject *parent) const;
	bool Contains(QString uniqueIdentifierName) const {return contains(uniqueIdentifierName);}
	//! Adds the pair <uniqueId, pointer to event> to the hash of events.
	void Add(GEventNode* pEvent);
	int Remove(QString uniqueIdentifierName) {return remove(uniqueIdentifierName);}
	//! Returns the event whose uniqueIdentifierName matches, 0 otherwise.
	GEventNode* GetEvent(const QString & uniqueIdentifierName) const {return value(uniqueIdentifierName, 0);}
	//! Overload provided for convenience. It takes a const QMimeData* from e.g. a DropEvent. 0 if no match.
	GEventNode* GetEvent( const QMimeData* theMimeData ) const;
	//! Returns a list of all the type registered in the factory.
	QList<QString> RegisteredEventTypes() const;
	QList<GEventNode*> AllEvents() const {return values();}

	//! Loads the GEventPlugin's from the directory EventPlugins in the application directory. Those will later be used to add events in the event shelf.
	void LoadEventPlugins();

	//! Returns the pointer to the hardware shelf.
	GEventShelf* HardwareEventShelf() const { MakeTheEventShelf(); return m_pHardEventShelf; }
	//! Adds a event to the event shelf. Will create the shelf object if it doesn't exist
	void AddEventToShelf(GEventNode* pEvent);

private:
	//! keeps track of the GProgEvent`s class name registered with G_REGISTER_PROG_DEVICE_CLASS.
	QHash<QString, QString> m_RegisteredProgEvent;
	//! The main event shelf.
	mutable GEventShelf* m_pHardEventShelf;
	//! Creates the event shelf. Cannot be done in the constructor because it is being called before the app actually starts.
	void MakeTheEventShelf() const;

public:
	//! GProgEvent`s class name that have been registered with the macro G_REGISTER_PROG_DEVICE_CLASS.
	QList<QString> RegisteredProgEvents() const { return m_RegisteredProgEvent.keys(); }
	//! Registers the GProgEvent`s class name. Called by the macro G_REGISTER_PROG_DEVICE_CLASS.
	bool RegisterProgEvent(QString ProgEventsClassName) { m_RegisteredProgEvent.insert(ProgEventsClassName, "not-used-yet"); return true;}
	//! intended to help cleanup when creating events with parent = 0
	mutable QObjectCleanupHandler m_CleanUpEvents;
};

typedef Loki::SingletonHolder<GEventManager> GEventManagerSing;

#endif // GEventManager_H


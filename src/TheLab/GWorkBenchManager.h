#ifndef GWORKBENCHMANAGER_H
#define GWORKBENCHMANAGER_H

#include "labexe_global.h"

// to prevent static error linking. see loky's doc.
#define LOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT 
#include "loki/Factory.h"
#include "loki/Singleton.h"

#include <QtCore>

class GWorkBenchManager;
class GWorkBench;

LABEXE_EXPORT GWorkBenchManager* WorkBenchManagerInstance(); 

// A custom FactoryError that doesn't throw an exception when OnUnknownType() of object
template <typename IdentifierType, class AbstractProduct>
struct MyWorkBenchManagerError
{
	struct Exception : public std::exception
	{
		const char* what() const throw() { return "Unknown Type"; }
	};

	static AbstractProduct* OnUnknownType(IdentifierType s)
	{
		QString mess("The workbench type \"%1\" has not been registered with the WorkBenchManager.");
			mess += "\r\n You should use the Macro G_REGISTER_WORKBENCH_CLASS(%1) in the corresponding .cpp file.";
			mess = mess.arg(QString(s));
        qCritical(mess.toUtf8());
		return 0;
	}
};

class _GWorkBenchManager_ToSingletonize_DO_NOT_USE_AS_IS 
	: public Loki::Factory<  GWorkBench
							, QString
							, Loki::Typelist< QWidget*, Loki::NullType > 
							, MyWorkBenchManagerError>
{
public:
	_GWorkBenchManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
	~_GWorkBenchManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
};

/////////////////////////////////////////////////////////////////////
//! \brief The GWorkBenchManager is an object factory for GWorkBench.
/*!
It keeps track of the available workbench that the GControlPanel can create.

In order for a class to be instantiated by the GWorkBenchManager, it must:
- inherit GWorkBench,
- use the Q_OBJECT macro,
- have the macro G_REGISTER_WORKBENCH_CLASS at the top of its corresponding .cpp file,

In order to use this singleton GWorkBenchManager you have to use the (typical) syntax:
ex:\code
WorkBenchManagerInstance()->CreateNewObject("GChannelSynchEvent", m_SomeEvent);
\endcode
If you need to learn what is and what is the use to an object factory, you should look for "factory pattern" on google,
or better, read "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
It is also a singleton: there is only one instance of device manager in the program.
*/
class LABEXE_EXPORT GWorkBenchManager: public QObject, 
	public _GWorkBenchManager_ToSingletonize_DO_NOT_USE_AS_IS
{
//	Q_OBJECT

public:
	GWorkBenchManager(void){}
	~GWorkBenchManager(void){}

	//! Creates a new workbench from the provided class name, with the provided parent
	GWorkBench* CreateNewWorkBench(QString className, QWidget *parent);
	//! Creates a new workbench, reading a file that was serialized by GWorkBench::PopulateSettings(), and with the provided parent.
	GWorkBench* CreateNewWorkBenchFromFile(QString fileName, QWidget *parent);
	//! Returns a list of all the type registered in the factory.
	QStringList RegisteredTypes() const;

};

typedef Loki::SingletonHolder<GWorkBenchManager> GWorkBenchManagerSing;

/*! \def G_REGISTER_WORKBENCH_CLASS(ClassName)
\brief This macro makes your derived class of GWorkBench available to the GLabControlPanel. 

arguments:
- ClassName is the name of the derived workbench class that is registered in the factory (i.e. the class that will be instatiable).
ex:\code
G_REGISTER_WORKBENCH_CLASS(GSequencerBench)
\endcode
*/
#define G_REGISTER_WORKBENCH_CLASS(ClassName)			\
namespace																		\
{ 																				\
	/*! Definition of a function that returns a new instance of an object !*/	\
	GWorkBench* CreateObject(QWidget *parent)										\
	{																			\
		return new ClassName(parent);											\
	}																			\
	const bool registered = WorkBenchManagerInstance()->Register( # ClassName , CreateObject);	\
} 																				\



#endif // GWORKBENCHMANAGER_H

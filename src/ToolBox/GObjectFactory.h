#ifndef GObjectFactory_H
#define GObjectFactory_H
#include "../labexe_global.h"

// to prevent static error linking. see loky's doc.
#define LOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT 
#include "loki/Factory.h"
#include "loki/Singleton.h"

#include <QtCore>

// LABEXE_EXPORT GObjectFactory& ObjectFactoryInstance(); 

// A custom FactoryError that doesn't throw an exception when OnUnknownType() of object
template <typename IdentifierType, class AbstractProduct>
struct MyObjectFactoryError
{
	struct Exception : public std::exception
	{
		const char* what() const throw() { return "Unknown Type"; }
	};

	static AbstractProduct* OnUnknownType(IdentifierType s)
	{
		QString mess("The object type \"%1\" has not been registered with the ObjectFactory.");
			mess += "\r\n You should use the Macro G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(%1, ParentType) in the corresponding .cpp file.";
			mess = mess.arg(QString(s));
		qCritical(mess.toAscii());
		return 0;
	}
};

class _GObjectFactory_ToSingletonize_DO_NOT_USE_AS_IS 
	: public Loki::Factory<  QObject
							, QString
							, Loki::Typelist< QObject*, Loki::NullType > 
							, MyObjectFactoryError>
{
public:
	_GObjectFactory_ToSingletonize_DO_NOT_USE_AS_IS(void){}
	~_GObjectFactory_ToSingletonize_DO_NOT_USE_AS_IS(void){}
};

/////////////////////////////////////////////////////////////////////
//! \brief The GObjectFactory is an object factory for QObjects.
/*!
It is used to instantiate objects during e.g. a de-serialization process.

In order for a class to be instantiated by the GObjectFactory, it must:
- inherit QObject,
- use the Q_OBJECT macro,
- have the macro G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE at the top of its corresponding .cpp file,
- have at least a constructor taking one argument as a parent object,
- that parent class has to use the Q_OBJECT macro as well.

In order to use this singleton object factory you have to use the (typical) syntax:
ex:\code
ObjectFactoryInstance()->CreateNewObject("GChannelSynchEvent", m_SomeEvent);
\endcode
If you need to learn what is and what is the use to an object factory, you should look for "factory pattern" on google,
or better, read "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
It is also a singleton: there is only one instance of device manager in the program.
*/
class LABEXE_EXPORT GObjectFactory: public QObject, 
	public _GObjectFactory_ToSingletonize_DO_NOT_USE_AS_IS
{
//	Q_OBJECT

public:
	GObjectFactory(void){}
	~GObjectFactory(void){}

	//! Creates a new object from the provided class name, with a provided parent
	QObject* CreateNewObject(QString className, QObject *parent);
	//! Returns a list of all the type registered in the factory.
	QStringList RegisteredTypes() const;

};

typedef Loki::SingletonHolder<GObjectFactory> GObjectFactorySing;

/*! \def G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(ClassName, ParentType)
\brief This is the Macro that makes your classes available to the object factory. 

It is this that allows you to unserialize derived device using the name of the corresponding class. 

arguments:
- ClassName is the name of the class that is registered in the factory (i.e. the class that will be instatiable)
- ParentType is the class name of the parent of such an object. The one that is neeeded in the constructor.
ex:\code
G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GChannelSynchEvent, GSynchEvent)
\endcode
*/
#define G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(ClassName, ParentType)			\
namespace																		\
{ 																				\
	/*! Definition of a function that returns a new instance of an object !*/	\
	QObject* CreateObject(QObject *parent)										\
	{																			\
		ParentType* pParentObj = qobject_cast<ParentType*>(parent);				\
		if(parent && !pParentObj) {												\
			QString mess("The declared parent type \"%2\" seems not correct for the constructor of \"%1\".");	\
			mess += "\r\n Check that \"%1\" and \"%2\" uses the Q_OBJECT macro.";			\
			mess += "\r\n Check your use of the Macro G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(%1, ParentType) in the corresponding .cpp file.";	\
			mess = mess.arg(# ClassName).arg(# ParentType);						\
            qCritical(mess.toUtf8());											\
		}																		\
		else 																	\
			return new ClassName(pParentObj);									\
	return 0;																	\
	}																			\
	const bool registered = RegisterInObjectFactory( # ClassName , CreateObject);	\
} 																				\


#endif // GObjectFactory_H

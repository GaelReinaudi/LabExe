#ifndef GPARAMMANAGER_H
#define GPARAMMANAGER_H
#include "labexe_global.h"

// to prevent static error linking when defining agents in a library. see loki's doc.
#define LOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT 

#include "loki/Factory.h"
#include "loki/Singleton.h"
#include <QUuid>
#include <QHash>
#include <QSet>
#include "ToolBox/GSerializable.h"
// #include "GParam.h"

class QObject;
class QMenu;
class GParam;
class QMimeData;
class QSettings;

// A custom FactoryError that doesn't throw an exception when OnUnknownType() of GParam
template <typename IdentifierType, class AbstractProduct>
struct MyParamFactoryError
{
	struct Exception : public std::exception
	{
        const char* what() const noexcept { return "Unknown Type"; }
	};

	static AbstractProduct* OnUnknownType(IdentifierType s)
	{
		QString mess("The param type \"%1\" has not been registered with the ParamFacory.");
		mess += "\r\n You should use the Macro G_REGISTER_NEW_PARAM_CLASS(%1) in the corresponding .cpp file.";
		mess = mess.arg(QString(s));
        qCritical() << mess.toUtf8();
		return 0;
	}
};

class _GParamManager_ToSingletonize_DO_NOT_USE_AS_IS 
	: public Loki::Factory<  GParam
							, QString
							, Loki::Typelist<QString, Loki::Typelist< QObject*, Loki::NullType > > 
							, MyParamFactoryError>
{
public:
	_GParamManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
	~_GParamManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
};

/////////////////////////////////////////////////////////////////////
//! \brief The Param manager is an object factory for the abstract class GParam.
/*!
If you need to learn what is and what is the use to an object factory, you should look for "factory pattern" on google,
or better, read "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
It is also a singleton: there is only one instance of Param manager in the program.

In order to use this singleton Param manager you have to use the (typical) syntax:
ex:\code
ParamManagerInstance()->SomeMemberFunction();
\endcode

The Param manager also stores all the pointer to all the created GParam's (created by it or not).
It is easy to obtain any GParam* by using its unique ID:
ex:\code
GParam* pPar = ParamManagerInstance()->GetParam(uniqueParamID);
\endcode
*/
class LABEXE_EXPORT GParamManager
	: public QObject
	, public _GParamManager_ToSingletonize_DO_NOT_USE_AS_IS
	, private QHash<QString, GParam*>
	, public GSerializable
{
	Q_OBJECT

public:
	GParamManager(void);
	~GParamManager(void){}
	//! Creates a new param from the provided class name, with a provided unique uniqueParamID
	GParam* CreateNewParam(QString className, QString uniqueParamID, QObject *parent);
	//! Creates a new param from the provided class name, with a new (random) uniqueParamID
	GParam* CreateNewParam(QString className, QObject *parent);
	bool Contains(QString uniqueParamID) const {return contains(uniqueParamID);}
	void Add(GParam* pParam);
	int Remove(QString uniqueParamID) {return remove(uniqueParamID);}
	//! Returns the param whose uniqueParamID matches, 0 otherwise.
	GParam* GetParam(QString uniqueParamID) const {return value(uniqueParamID, 0);}
	//! Overload provided for convenience. It takes a const QMimeData* from e.g. a DropEvent. 0 if no match.
	GParam* GetParam( const QMimeData* theMimeData ) const;
	//! Gets a pointer from the GParamManagerSing, corresponding to the UniqueIdentifier stored under keyString in the fromQsettings. 0 if no match.
	GParam* GetParam( QString keyString, QSettings* fromQsettings );
	//! returns a list of all the pointers to all the GParam's
    QList<GParam*> ListAllParams() {
        auto container = values();
        std::sort(container.begin(), container.end());
        container.erase(std::unique(container.begin(), container.end()),
                        container.end());
        return container;
    }
	//! Returns a list of all the type registered in the factory.
	QList<QString> RegisteredParamTypes() const;

protected:
	//! Reimplemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	void InterpretSettings(QSettings& fromQsettings);

private:
};


typedef Loki::SingletonHolder<GParamManager> GParamManagerSing;

/*! \def G_REGISTER_NEW_PARAM_CLASS(ClassName)
\brief This is the Macro that makes your derived class available to the Param factory. 

It is this that allows you to unserialize derived Param using the name of the corresponding class. 
Take a look here: 
- "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
*/
#define G_REGISTER_NEW_PARAM_CLASS(ClassName)									\
	namespace																	\
{ 																				\
	/*! Definition of a function that returns a new instance of a Param !*/	\
	GParam* CreateParam(QString uniqueParamID, QObject *parent)		\
	{																				\
		return new ClassName(uniqueParamID, parent);							\
	}																				\
	const bool registered = ParamManagerInstance()->Register( # ClassName , CreateParam);	\
} 																				\

#endif // GPARAMMANAGER_H

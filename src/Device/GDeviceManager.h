#ifndef GDeviceManager_H
#define GDeviceManager_H
#include "labexe_global.h"

// to prevent static error linking when defining devices in a library (eg phidget, pxi). see loky's doc.
#define LOKI_FUNCTOR_IS_NOT_A_SMALLOBJECT 

#include "loki/Factory.h"
#include "loki/Singleton.h"
#include <QString>
#include <QHash>
#include <QMessageBox>
#include <QtGlobal>
#include <QObjectCleanupHandler>
#include <QDebug>
class QMimeData;

class QObject;
class GDevice;
class GDeviceShelf;
class GDevicePlugin;
class GDeviceManager;

// A custom FactoryError that doesn't throw an exception when OnUnknownType() of GDevice
template <typename IdentifierType, class AbstractProduct>
struct MyDeviceFactoryError
{
	struct Exception : public std::exception
	{
        const char* what() const noexcept { return "Unknown Type"; }
	};

	static AbstractProduct* OnUnknownType(IdentifierType s)
	{
		QString mess("The device type \"%1\" has not been registered with the DeviceFacory.");
			mess += "\r\n You should use the Macro G_REGISTER_HARD_DEVICE_CLASS(%1) in the corresponding .cpp file.";
			mess = mess.arg(QString(s));
        qCritical() << mess.toUtf8();
		return 0;
	}
};

class _GDeviceManager_ToSingletonize_DO_NOT_USE_AS_IS 
	: public Loki::Factory<  GDevice
							, QString
							, Loki::Typelist<QString, Loki::Typelist< QObject*, Loki::NullType > > 
							, MyDeviceFactoryError>
{
public:
	_GDeviceManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
	~_GDeviceManager_ToSingletonize_DO_NOT_USE_AS_IS(void){}
};

/////////////////////////////////////////////////////////////////////
//! \brief The device manager is an object factory for the abstract class GDevice.
/*!
In order for a device to be instantiated by the GDeviceManager, it must:
- inherit GDevice
- have a constructor taking as argument: (QString uniqueID, QObject pParentObject).
- have the macro G_REGISTER_HARD_DEVICE_CLASS at the top of its corresponding .cpp file

If you need to learn what is and what is the use to an object factory, you should look for "factory pattern" on google,
or better, read "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
It is also a singleton: there is only one instance of device manager in the program.

In order to use this singleton device manager you have to use the (typical) syntax:
ex:\code
DeviceManagerInstance()->SomeMemberFunction();
\endcode

The device manager also store all the pointer to all the created GDevice's (created by himself or not).
It is easy to obtain any GDevice* by using its uniqueIdentifierName:
ex:\code
GDevice* pdev = GetDeviceFromDeviceManager(uniqueIdentifierName);
\endcode
*/
class LABEXE_EXPORT GDeviceManager: public QObject, 
	public _GDeviceManager_ToSingletonize_DO_NOT_USE_AS_IS
	, private QHash<QString, GDevice*>
{
	Q_OBJECT

public:
	GDeviceManager();
	~GDeviceManager();

	//! Creates a new device from the provided class name, with a provided unique uniqueIdentifierName
	GDevice* CreateNewDevice(QString className, QString uniqueIdentifierName, QObject *parent) const;
	//! Creates a new GProgDevice using the class name provided and parent. It checks that the name is in the RegisteredProgDevices() to prevent using this function on an hardware device.
	GDevice* CreateNewProgDevice(QString className, QObject *parent) const;
	bool Contains(QString uniqueIdentifierName) const {return contains(uniqueIdentifierName);}
	//! Adds the pair <uniqueId, pointer to device> to the hash of devices.
	void Add(GDevice* pDevice);
	int Remove(QString uniqueIdentifierName) {return remove(uniqueIdentifierName);}
	//! Returns the device whose uniqueIdentifierName matches, 0 otherwise.
    GDevice* GetDevice(QString uniqueIdentifierName) const {return value(uniqueIdentifierName, nullptr);}
	//! Overload provided for convenience. It takes a const QMimeData* from e.g. a DropEvent. 0 if no match.
	GDevice* GetDevice( const QMimeData* theMimeData ) const;
	//! Returns a list of all the type registered in the factory.
	QList<QString> RegisteredDeviceTypes() const;
	QList<GDevice*> AllDevices() const {return values();}

	//! Loads the GDevicePlugin's from the directory DevicePlugins in the application directory. Those will later be used to add devices in the device shelf.
	void LoadDevicePlugins();

	//! Returns the pointer to the hardware shelf.
	GDeviceShelf* HardwareDeviceShelf() const { MakeTheDeviceShelf(); return m_pHardDeviceShelf; }
	//! Adds a device to the device shelf. Will create the shelf object if it doesn't exist
	void AddDeviceToShelf(GDevice* pDevice);

private:
	//! keeps track of the GProgDevice`s class name registered with G_REGISTER_PROG_DEVICE_CLASS.
	QHash<QString, QString> m_RegisteredProgDevice;
	//! The main device shelf.
	mutable GDeviceShelf* m_pHardDeviceShelf;
	//! Creates the device shelf. Cannot be done in the constructor because it is being called before the app actually starts.
	void MakeTheDeviceShelf() const;

public:
	//! GProgDevice`s class name that have been registered with the macro G_REGISTER_PROG_DEVICE_CLASS.
	QList<QString> RegisteredProgDevices() const { return m_RegisteredProgDevice.keys(); }
	//! Registers the GProgDevice`s class name. Called by the macro G_REGISTER_PROG_DEVICE_CLASS.
	bool RegisterProgDevice(QString ProgDevicesClassName) { m_RegisteredProgDevice.insert(ProgDevicesClassName, "not-used-yet"); return true;}
	//! intended to help cleanup when creating devices with parent = 0
	mutable QObjectCleanupHandler m_CleanUpDevices;
};

typedef Loki::SingletonHolder<GDeviceManager> GDeviceManagerSing;

#endif // GDeviceManager_H


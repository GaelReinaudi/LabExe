#ifndef GDEVICEPLUGIN_H
#define GDEVICEPLUGIN_H
#include "labexe_global.h"

#include <QObject>
#include <QtPlugin>

class GDevice;

/////////////////////////////////////////////////////////////////////
//! \brief The GDevicePlugin class defines an interface that has to be used when writing a device plugin.
/*!
This is how a user can use his own device into the LabExe.
See the Qt help about writing plugins to extend applications.

A GDevicePlugin is not the device itself. It is actually a factory that will get called by the GDeviceManager 
and that will be able to produced an object (the device) from a class derived from GDevice.
*/
class LABEXE_EXPORT GDevicePlugin : public QObject
{
	Q_OBJECT

public:
	GDevicePlugin(QObject *parent);
	virtual ~GDevicePlugin();

private:
	// This is a function that is defined only through the macro G_REGISTER_PLUGIN_PROG_DEVICE
	// It will register the device class in the device manager.
	virtual void RegisterDeviceClassInDeviceManager() const = 0;

	friend class GDeviceManager;
};

Q_DECLARE_INTERFACE(GDevicePlugin, "Gael.GDevicePlugin/2.0")

/*! \def G_REGISTER_PLUGIN_HARD_DEVICE(ClassName)
\brief This is the Macro that makes your derived class from GDevice available to the device factory. 
You must put it in the GDevicePlugin-derived class header file, in the private section of the class, bellow Q_OBJECT and Q_INTERFACES.

It declares and defines an inline static creator function, CreateDevice(QString uniqueIdentifierName, QObject *parent), 
that will be used to register in the device manager.
*/
#define G_REGISTER_PLUGIN_HARD_DEVICE(ClassName, UniqueIdForTheInstance)											\
G_1_PART_REGISTER_PLUGIN_HARD_DEVICE(ClassName)																		\
void RegisterDeviceClassInDeviceManager() const {																	\
	G_2_PART_REGISTER_PLUGIN_HARD_DEVICE(ClassName, UniqueIdForTheInstance)											\
}																													

#define G_1_PART_REGISTER_PLUGIN_HARD_DEVICE(ClassName)																\
static GDevice* CreatorUsedForTheDeviceManager ## ClassName (QString uniqueIdentifierName, QObject *parent)			\
{																													\
	static int nHardDevCreated ## ClassName = 0;																	\
	nHardDevCreated ## ClassName ++;																				\
	GDevice* pNDev = 0;																								\
	pNDev = new ClassName(uniqueIdentifierName, parent);															\
	return pNDev;																									\
}																													

#define G_2_PART_REGISTER_PLUGIN_HARD_DEVICE(ClassName, UniqueIdForTheInstance)										\
	bool isOkkk = RegisterHardDeviceInManager( # ClassName , CreatorUsedForTheDeviceManager ## ClassName );			\
    AddToDeviceShelf( CreatorUsedForTheDeviceManager ## ClassName (UniqueIdForTheInstance, 0));                     \
    Q_UNUSED(isOkkk);



/*! \def G_REGISTER_PLUGIN_PROG_DEVICE(ClassName)
\brief This is the Macro that makes your derived class from GProgDevice available to the device factory. 
You must put it in the GDevicePlugin-derived class header file, in the private section of the class, bellow Q_OBJECT and Q_INTERFACES.

It declares and defines an inline static creator function, CreateDevice(QString uniqueIdentifierName, QObject *parent), 
that will be used to register in the device manager.

This macro is like G_REGISTER_PLUGIN_HARD_DEVICE, but also register the GProgDevice in the e-device shelf
using RegisterProgDeviceInManager();

The difference of those to macro is also justified by the fact that a GProgDevice 
doesn't take a QString uniqueID argument in the constructor.
*/
#define G_REGISTER_PLUGIN_PROG_DEVICE(ClassName)																	\
G_1_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName)																		\
void RegisterDeviceClassInDeviceManager() const {																	\
G_2_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName)																		\
}																													

/*! \def G_REGISTER_2_PLUGIN_PROG_DEVICE(ClassName, ClassName2)
\brief same as G_REGISTER_PLUGIN_PROG_DEVICE, but for 2 devices
*/
#define G_REGISTER_2_PLUGIN_PROG_DEVICE(ClassName, ClassName2)														\
G_1_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName)																		\
G_1_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName2)																	\
void RegisterDeviceClassInDeviceManager() const {																	\
G_2_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName)																		\
G_2_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName2)																	\
}																													

/*! \def G_REGISTER_3_PLUGIN_PROG_DEVICE(ClassName, ClassName2, ClassName3)
\brief same as G_REGISTER_PLUGIN_PROG_DEVICE, but for 3 devices
*/
#define G_REGISTER_3_PLUGIN_PROG_DEVICE(ClassName, ClassName2, ClassName3)											\
G_1_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName)																		\
G_1_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName2)																	\
G_1_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName3)																	\
void RegisterDeviceClassInDeviceManager() const {																	\
G_2_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName)																		\
G_2_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName2)																	\
G_2_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName3)																	\
}																													

#define G_1_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName)																\
static GDevice* CreatorUsedForTheDeviceManager ## ClassName (QString uniqueIdentifierName, QObject *parent) {		\
	static int nProgDevCreated ## ClassName = 0;																	\
	nProgDevCreated ## ClassName ++;																				\
	GDevice* pNDev = 0;																								\
	pNDev = new ClassName(parent, uniqueIdentifierName);															\
	QString strName(pNDev->metaObject()->className());																\
	strName += QString("-%1").arg(nProgDevCreated ## ClassName);													\
	pNDev->ReName(strName);																							\
	return pNDev;																									\
}																													

#define G_2_PART_REGISTER_PLUGIN_PROG_DEVICE(ClassName)																\
	RegisterProgDeviceInManager( # ClassName , CreatorUsedForTheDeviceManager ## ClassName );					


#endif // GDEVICEPLUGIN_H

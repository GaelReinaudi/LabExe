#include "GProgDevice.h"
#include <QUuid>

/////////////////////////////////////////////////////////////////////
/*!
The uniqueIdentifierName should not be specified by the user. It is normally only used by the 
device manager when a previously saved GProgDevice is recreated and gets its unique ID back. 
\param:  QObject * parent : the parent of the device object
\param:  QString uniqueIdentifierName : Unique ID, only used by the framework when de-serializing a GProgDevice.
*////////////////////////////////////////////////////////////////////
GProgDevice::GProgDevice(QObject *parent, QString uniqueIdentifierName /*= ""*/)
	: GDevice(uniqueIdentifierName == "bbbbbbbbbbbbbb" 
				? QUuid::createUuid().toString() /*this is a temporary unique ID, see below*/ 
				: uniqueIdentifierName, parent) // creation time initialization !
{
	// here we update the unique ID of this device directly in the DeviceManagerInstance() 
	// because it is not good to use AssignNewUniqueID() in the initialization list above. 
	// because the class is not finished being instantiated.
// 	if(uniqueIdentifierName == "")
// 		ChangeUniqueSystemID(GSerializableSettings::AssignNewUniqueID(this));
}

GProgDevice::~GProgDevice()
{

}

/////////////////////////////////////////////////////////////////////
/*!
This method can be useful to prevent or perform some initialization in the shelved version of the device.
\return: bool : true if the parent is the device shelf.
*////////////////////////////////////////////////////////////////////
bool GProgDevice::IsShelvedInstance()
{
	bool isShelfed = false;
	QObject* pDevShelf = qobject_cast<QObject*>(parent());
	if(pDevShelf && pDevShelf->metaObject())
		isShelfed = QString(pDevShelf->metaObject()->className()) == QString("GDeviceShelf");
	return isShelfed;
}


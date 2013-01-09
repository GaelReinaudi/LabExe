#ifndef GProgDevice_H
#define GProgDevice_H
#include "labexe_global.h"

#include "GDevice.h"

// ! This macro simplifies the auto-naming of the uniqueID of a given class. It sets the name to <ClassName> followed by a <{UUID}>.
//#define G_AUTO_ID_FOR_PROGRAM_DEVICE (QString(metaObject()->className()) + QUuid::createUuid().toString())
//#define G_GETCLASS_FROM_AUTO_ID(QstringAutoID) (QString(QstringAutoID).remove(QstringAutoID.indexOf("{"),1000))

/////////////////////////////////////////////////////////////////////
//! \brief The GProgDevice class is a GDevice that represent a software (not hardware) feature (e.g.: a Variator).
/*!
The first main difference from a GDevice is that you dont need to (and shouldn't) give it a unique ID at creation. 
It is because hardware are in a limited number and have to be accessed via a unique identifier that depends 
on the programming interface of the device). 
Software devices, on the other hand, can likely be created in an unlimited number.
A GProgDevice does have a UniqueIdentifier! But it is created automatically by the program.

Since there can be many instances of a class that inherits GProgDevice, it could 
be a good idea to use the macro G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE so that it can 
be re-instantiated during a de-serialization process.
*/
class LABEXE_EXPORT GProgDevice : public GDevice
{
	Q_OBJECT

public:
	//! Constructor. The uniqueIdentifierName defaults to "", in which case the device is named with a unique string (using a QUuid).
	GProgDevice(QObject *parent, QString uniqueIdentifierName = "");
	virtual ~GProgDevice();

	//! Returns true if this instance of the GProgDevice is parented by the shelf.
	bool IsShelvedInstance();

private:
	
};

#endif // GProgDevice_H

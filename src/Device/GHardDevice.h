#ifndef GHARDDEVICE_H
#define GHARDDEVICE_H
#include "labexe_global.h"

#include "GDevice.h"

class LABEXE_EXPORT GHardDevice : public GDevice
{
	Q_OBJECT

public:
	//! Constructor. You have to provide a unique name to identify the physical device in the program
	GHardDevice(QString uniqueIdentifierName, QObject *parent);
	virtual ~GHardDevice();

private:
	
};

#endif // GHARDDEVICE_H

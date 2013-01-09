#ifndef GPARAMCREATOR_H
#define GPARAMCREATOR_H

#include "device.h"
#include "param.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GParamCreator class defines a very simple device that will create some params so that they can be used in devices that have free parameters.
/*!
*/
class GParamCreator : public GProgDevice
{
	Q_OBJECT

public:
	GParamCreator(QObject *parent, QString uniqueIdentifierName = "");
	~GParamCreator();

protected:
	//! Re-implemented
	void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget);

private:
	GParamBucket m_Bucket;
};

#endif // GPARAMCREATOR_H

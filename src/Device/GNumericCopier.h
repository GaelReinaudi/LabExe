#ifndef GNUMERICCOPIER_H
#define GNUMERICCOPIER_H
#include "labexe_global.h"

#include "device.h"
#include "param.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GNumericCopier class defines a very simple device that will copy the value of a numerical param on one or several others.
/*!
*/
class GNumericCopier : public GProgDevice
{
	Q_OBJECT

public:
	GNumericCopier(QObject *parent, QString uniqueIdentifierName = "");
	~GNumericCopier();

protected:
	//! Re-implemented
	void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget);

protected slots:
	//! will assign the newVal to every GParam in the output GParamBucket object
	void UpdateOutputs(double newVal);

private:
	GSingleParamBucket m_BucketIn;
	GParamBucket m_MutliBucketOut;

};

#endif // GNUMERICCOPIER_H

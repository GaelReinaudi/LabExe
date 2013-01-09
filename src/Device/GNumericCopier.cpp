#include "GNumericCopier.h"
#include "Device/GDeviceManager.h"

G_REGISTER_PROG_DEVICE_CLASS(GNumericCopier)

GNumericCopier::GNumericCopier(QObject *parent, QString uniqueIdentifierName /*= ""*/)
	: GProgDevice(parent, uniqueIdentifierName)
	, m_BucketIn("Input", this)
	, m_MutliBucketOut("Output", this)
{
	connect(&m_BucketIn, SIGNAL(ValueDidChange(double)), this, SLOT(UpdateOutputs(double)), Qt::QueuedConnection);
}

GNumericCopier::~GNumericCopier()
{
}

void GNumericCopier::PopulateDeviceWidget(GDeviceWidget* theDeviceWidget)
{
	QBoxLayout* pLay = new QHBoxLayout();
	theDeviceWidget->AddSubLayout(pLay);
	pLay->insertWidget(0, m_BucketIn.ProvideNewParamWidget(theDeviceWidget));
	pLay->insertWidget(1, m_MutliBucketOut.ProvideNewParamWidget(theDeviceWidget));
}

void GNumericCopier::UpdateOutputs( double newVal )
{
	foreach(GParam* pPar, m_MutliBucketOut.Params()) {
		GParamNum* pParNum = qobject_cast<GParamNum*>(pPar);
		if(pParNum)
			pParNum->SetParamValue(newVal);
	}
}
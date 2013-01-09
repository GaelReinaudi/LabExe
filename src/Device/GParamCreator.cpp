#include "GParamCreator.h"

G_REGISTER_PROG_DEVICE_CLASS(GParamCreator)

GParamCreator::GParamCreator(QObject *parent, QString uniqueIdentifierName /*= ""*/)
	: GProgDevice(parent, uniqueIdentifierName)
	, m_Bucket("Params", this)
{
	for(int i = 0; i < 10; i++) {
		QString namePar = "New-";
		namePar += QString("%1").arg(i + 1, 2);
		GParamNum* pPar = new GParamDouble(namePar, this);
		m_Bucket.AddParam(pPar);
	}
	m_Bucket.Freeze();
}

GParamCreator::~GParamCreator()
{
}

void GParamCreator::PopulateDeviceWidget(GDeviceWidget* theDeviceWidget)
{
	QBoxLayout* pLay = new QHBoxLayout();
	theDeviceWidget->AddSubLayout(pLay);
	pLay->insertWidget(0, m_Bucket.ProvideNewParamWidget(theDeviceWidget));
}

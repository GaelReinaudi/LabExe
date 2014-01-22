#include "GImageLineProfiler.h"
#include "device.h"

//G_REGISTER_PROG_DEVICE_CLASS(GImageLineProfiler)

GImageLineProfiler::GImageLineProfiler(QObject *parent, QString uniqueIdentifierName /*= ""*/)
	: GImageProcessor(parent, uniqueIdentifierName)
{

}

GImageLineProfiler::~GImageLineProfiler()
{

}

void GImageLineProfiler::PopulateDeviceWidget( GDeviceWidget* theDeviceWidget )
{
	QVBoxLayout* lay = new QVBoxLayout();
	theDeviceWidget->AddSubLayout(lay);
	lay->addWidget(new QLabel("gdgdgdfgdg"));
}

void GImageLineProfiler::ProcessImageAOIed(const GImageDouble & aoiImage)
{
    Q_UNUSED(aoiImage);
}

#ifndef GIMAGEPROCESSORWIDGET_H
#define GIMAGEPROCESSORWIDGET_H

#include "Device/GDeviceWidget.h"
#include "ui_GImageProcessorWidget.h"

class GImageProcessor;

using namespace Ui;

class GImageProcessorWidget : public GDeviceWidget, public GImageProcessorWidgetClass
{
	Q_OBJECT

public:
	GImageProcessorWidget(GImageProcessor* pImageProcessor, QWidget *parent);
	~GImageProcessorWidget();
};

#endif // GIMAGEPROCESSORWIDGET_H

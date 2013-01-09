#ifndef GPARAMDOUBLESETTINGWIDGET_H
#define GPARAMDOUBLESETTINGWIDGET_H

#include <QWidget>
#include "ui_GParamDoubleSettingWidget.h"
#include "Param/GParamSettingsWidget.h"

class GParamDouble;

class GParamDoubleSettingWidget : public QWidget, public Ui::GParamDoubleSettingWidget
{
	Q_OBJECT

public:
	GParamDoubleSettingWidget(GParamDouble* pTheParam, QWidget *parent = 0);
	~GParamDoubleSettingWidget();

private:
	GParamDouble* m_pParam;
};

#endif // GPARAMDOUBLESETTINGWIDGET_H

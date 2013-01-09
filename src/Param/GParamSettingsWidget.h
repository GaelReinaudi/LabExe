#ifndef GPARAMSETTINGSWIDGET_H
#define GPARAMSETTINGSWIDGET_H

#include <QWidget>
#include "ui_GParamSettingsWidget.h"

class GParam;

class GParamSettingsWidget : public QWidget, public Ui::GParamSettingsWidget
{
	Q_OBJECT

public:
	GParamSettingsWidget(GParam* pTheParam, QWidget *parent = 0);
	~GParamSettingsWidget();

protected:
	void showEvent(QShowEvent* event);

private:
	GParam* m_pParam;
};

#endif // GPARAMSETTINGSWIDGET_H

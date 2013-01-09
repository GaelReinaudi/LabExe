#include "GParamDouble.h"
#include "GParamManager.h"
#include "GParamDoubleSettingWidget.h"
#include "GDoubleSpinBox.h"

G_REGISTER_NEW_PARAM_CLASS(GParamDouble);
 
GParamDouble::GParamDouble(QString theName, QObject* parent, GParam::Properties paramOptions /*= NoOption*/)
	: GParamNum(theName, parent, paramOptions)
{
}

GParamDouble::~GParamDouble()
{

}

GDoubleSpinBox* GParamDouble::ProvideNewParamSpinBox( QWidget* forWhichParent)
{
	return qobject_cast<GDoubleSpinBox*>(ProvideNewParamWidget(forWhichParent/*, someOptions*/));
}

GParamSettingsWidget* GParamDouble::PopupSettingWidget()
{
	GParamSettingsWidget* pWid = GParamNum::PopupSettingWidget();
	GParamDoubleSettingWidget* pWidDouble = new GParamDoubleSettingWidget(this, 0);
	pWid->layout()->addWidget(pWidDouble);
	return pWid;
}
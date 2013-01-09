#include "GParamDoubleSettingWidget.h"
#include "GParamDouble.h"

GParamDoubleSettingWidget::GParamDoubleSettingWidget(GParamDouble* pTheParam, QWidget *parent)
	: QWidget(parent)
	, m_pParam(pTheParam)
{
	setupUi(this);
	setWindowFlags(Qt::Popup);

	if(!m_pParam)
		return;

	m_pMin->setValue(m_pParam->Minimum());
	m_pMax->setValue(m_pParam->Maximum());
	m_pDecimals->setValue(m_pParam->DisplayDecimals());
	m_pStep->setValue(m_pParam->TypicalStep());

	m_pUnits->setText(m_pParam->Units());
	m_pDisplayUnits->setChecked(m_pParam->ShowingUnits());
}

GParamDoubleSettingWidget::~GParamDoubleSettingWidget()
{
	m_pParam->SetHardLimits(m_pMin->value(), m_pMax->value());
	m_pParam->SetDisplayDecimals(m_pDecimals->value());
	m_pParam->SetTypicalStep(m_pStep->value());
	m_pParam->SaveParamSettingsToRegistry();

	QString strUnits = m_pUnits->text();
	m_pParam->SetUnits(strUnits);
	m_pParam->ShowUnits(m_pDisplayUnits->isChecked());
}

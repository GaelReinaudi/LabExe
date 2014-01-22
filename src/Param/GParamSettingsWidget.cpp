#include "GParamSettingsWidget.h"
#include "GParam.h"

GParamSettingsWidget::GParamSettingsWidget(GParam* pTheParam, QWidget *parent)
	: QWidget(parent)
	, m_pParam(pTheParam)
{
	setupUi(this);
	if(!m_pParam)
		return;

	m_pID->setText(m_pParam->UniqueSystemID());
	m_pNameEdit->setText(m_pParam->Name());
}

GParamSettingsWidget::~GParamSettingsWidget()
{
	QString newName = m_pNameEdit->text();
	if(newName != "")
		m_pParam->SetName(newName);
}

void GParamSettingsWidget::showEvent( QShowEvent* event )
{
    Q_UNUSED(event);
    m_pNameEdit->selectAll();
	m_pNameEdit->setFocus();
}

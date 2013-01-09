#include "GCameraSettingsWidget.h"
#include "GCamera.h"
#include "ui_GCameraSettingsWidget.h"
#include "Param/GDoubleSpinBox.h"

GCameraSettingsWidget::GCameraSettingsWidget(GCamera* pCamera, QWidget *parent /*= 0*/)
	: GDeviceWidget(pCamera, parent)
	, m_pCamera(pCamera)
{
	if(!pCamera) {
		qDebug() << "Pointer to the camera is null! 6549124";
		return;
	}
	ui = new Ui::GCameraSettingsWidgetClass();
	ui->setupUi(this);
//	ui->pCamGroupBox->setTitle(m_pCamera->Name());

	// sets the exposure and gain spinboxes in the appropriate layout
	ui->pExpoGainLayout->addRow(m_pCamera->m_Expo_ms.ProvideNewLabel(this), m_pCamera->m_Expo_ms.ProvideNewParamSpinBox(this));
	ui->pExpoGainLayout->addRow(m_pCamera->m_Gain.ProvideNewLabel(this), m_pCamera->m_Gain.ProvideNewParamSpinBox(this));
	// sets the trigger check box in the appropriate layout
	m_pTriggerCheckBox = m_pCamera->m_HardwareTrigger.ProvideNewParamCheckBox(this, "");
	m_pTriggerLabel = m_pCamera->m_HardwareTrigger.ProvideNewLabel(this);
	ui->pExpoGainLayout->addRow(m_pTriggerLabel, m_pTriggerCheckBox);
	// the optical system 
	ui->pLayoutOpticalSystem->addRow(m_pCamera->m_PixelSize.ProvideNewLabel(this), m_pCamera->m_PixelSize.ProvideNewParamSpinBox(this));

	ui->pGrabLayout->addWidget(m_pCamera->m_DoAquireContinuously.ProvideNewParamButton(this));
	// some connections
	connect(ui->pSnapOneButton, SIGNAL(clicked()), m_pCamera, SLOT(SingleShotNow()));
	connect(ui->m_pSelectAoiButton, SIGNAL(clicked()), m_pCamera, SLOT(SelectAOI()));

	// connect for some display updates when capabilities may have changed
	connect(m_pCamera, SIGNAL(CapabilitiesMayHaveChanged()), this, SLOT(UpdateCapabilityDisplay()), Qt::QueuedConnection);

	// connect to update the Frame rate
	connect(m_pCamera, SIGNAL(FrameRateChanged(double)), ui->pFpsSpinBox, SLOT(setValue(double)), Qt::QueuedConnection);
	connect(m_pCamera, SIGNAL(NumFrameSentChanged(int)), ui->pSentFrameBox, SLOT(setValue(int)), Qt::QueuedConnection);
	connect(m_pCamera, SIGNAL(NumFrameFailedChanged(int)), ui->pFailedFrameBox, SLOT(setValue(int)), Qt::QueuedConnection);

	UpdateCapabilityDisplay();
}

GCameraSettingsWidget::~GCameraSettingsWidget()
{
	delete ui;
}

void GCameraSettingsWidget::UpdateCapabilityDisplay()
{
	if(!m_pCamera->IsAble()) {
		Enable(false);
		return;
	}
	Enable(true);

	// support for the hardware trigger
	bool camTriggerAble = m_pCamera->SupportsExternalTrigger();
	if(!camTriggerAble)
		m_pTriggerCheckBox->setChecked(false);
	m_pTriggerCheckBox->setEnabled(camTriggerAble);
	m_pTriggerLabel->setEnabled(camTriggerAble);
}

QWidget* GCameraSettingsWidget::CameraSpecificGroupBox()
{
	return ui->pGroupBoxCameraSpecific;
}
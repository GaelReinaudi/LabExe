#include "GCamera.h"
#include "GImageProcessor.h"
#include "GAoiSelector.h"
#include "GCameraSettingsWidget.h"

#include "ui_GCameraSettingsWidget.h"

GCamera::GCamera(QString uniqueIdentifierName, QObject *parent, GImageProcessor* pImProcessor)
	: GHardDevice(uniqueIdentifierName, parent)
	, GHasImageProcessor(pImProcessor)
	, m_DoAquireContinuously("Continuous shot", nullptr) // 0 means not saved and restore automatically in the device settings
	, m_Expo_ms("Exposure [ms]", this)
	, m_Gain("Gain factor", this)
	, m_HardwareTrigger("Hardware trigger", this)
	, m_PixelSize("Pixel [um]", this)
	, m_NumFrameSent(0)
	, m_NumFrameFailed(0)
	, m_IsOpened(false)
{
	m_Gain.SetDisplayDecimals(2);
	m_Gain.SetTypicalStep(0.5);
	m_PixelSize.SetDisplayDecimals(2);
	m_PixelSize.SetTypicalStep(0.0);
	m_PixelSize.SetDescription("Size in micro-meter represented by a pixel in the image plan of the optical system.");

	connect(&m_DoAquireContinuously, SIGNAL(ValueUpdated(bool)), this, SLOT(ContinuousShot(bool)));
	connect(&m_Expo_ms, SIGNAL(ValueDidChange(double)), this, SLOT(ChangeExposureTime(double)));
	connect(&m_Gain, SIGNAL(ValueDidChange(double)), this, SLOT(ChangeGainFactor(double)));
	connect(&m_HardwareTrigger, SIGNAL(ValueUpdated(bool)), this, SLOT(UseExternalTrigger(bool)), Qt::QueuedConnection);
}

GCamera::~GCamera()
{
}

void GCamera::LatterInitialization()
{
	m_IsOpened = OpenCamera();
	if(!m_IsOpened)
		return;

	if(ImageProcessor()) {
		disconnect(ImageProcessor(), SIGNAL(OutputImageDouble(GImageDouble)), this, SLOT(NotifyImageSent()));
		disconnect(ImageProcessor(), SIGNAL(ProcessorFailed()), this, SLOT(NotifyImageFailed()));
		connect(ImageProcessor(), SIGNAL(OutputImageDouble(GImageDouble)), this, SLOT(NotifyImageSent()), Qt::QueuedConnection);
		connect(ImageProcessor(), SIGNAL(ProcessorFailed()), this, SLOT(NotifyImageFailed()), Qt::QueuedConnection);
	}

	ChangeExposureTime(m_Expo_ms);
	ChangeGainFactor(m_Gain);
	emit CapabilitiesMayHaveChanged();

	GDevice::LatterInitialization();
}

GDeviceWidget* GCamera::ProvideNewDeviceGroupBox( QWidget* inWhichWidget, QBoxLayout::Direction orientation /*= QBoxLayout::LeftToRight*/ )
{
	Q_UNUSED(orientation)
	GCameraSettingsWidget* pWidToReturn = new GCameraSettingsWidget(this, inWhichWidget);
	// a kind of device widget in the GCameraSettingsWidget so that it get populated by classes inheriting GCamera
	GDeviceWidget* pDevSpefWid = new GDeviceWidget(nullptr, pWidToReturn->CameraSpecificGroupBox());
	pWidToReturn->CameraSpecificGroupBox()->layout()->addWidget(pDevSpefWid);
 	PopulateDeviceWidget(pDevSpefWid);
	return pWidToReturn;
}

void GCamera::NotifyImageSent()
{
	m_NumFrameSent++;
	emit NumFrameSentChanged(m_NumFrameSent);
}

void GCamera::NotifyImageFailed()
{
	m_NumFrameFailed++;
	emit NumFrameFailedChanged(m_NumFrameFailed);
}

void GCamera::ChangeExposureTime( double newExpo )
{
	if(!m_IsOpened)
		return;
	double actualUsedExpo = SetExposureTime(newExpo);
	if(!gEqual(actualUsedExpo, m_Expo_ms))
		m_Expo_ms.SetParamValue(actualUsedExpo, false);
}

void GCamera::UpdateExposureTime()
{
	ChangeExposureTime(m_Expo_ms);
}
void GCamera::ChangeGainFactor( double newGain )
{
	if(!m_IsOpened)
		return;
	double actualUsedGain = SetGainFactor(newGain);
	if(!gEqual(actualUsedGain, m_Gain))
		m_Gain.SetParamValue(actualUsedGain, false);
}

void GCamera::PopulateSettings( QSettings& inQsettings )
{
	GDevice::PopulateSettings(inQsettings);
	inQsettings.beginGroup("Acquire");
	m_DoAquireContinuously.SaveDeviceSettings(inQsettings);
	inQsettings.endGroup();
	// processor
	inQsettings.beginGroup("Processor");
	//TOGO: false just for compatibility with previous version
	ImageProcessor()->SaveDeviceSettings(inQsettings, false);
	inQsettings.endGroup();
}

void GCamera::InterpretSettings( QSettings& fromQsettings )
{
	GDevice::InterpretSettings(fromQsettings);
	if(!SupportsExternalTrigger())
		m_HardwareTrigger = false;

	// processor
	fromQsettings.beginGroup("Processor");
	//TOGO: false just for compatibility with previous version
	ImageProcessor()->ReadDeviceSettings(fromQsettings, false);
	// no display of the AOI for the camera
	ImageProcessor()->ShowAOI(false);
	fromQsettings.endGroup();

	fromQsettings.beginGroup("Acquire");
	LatterReadCurrentGroup(fromQsettings);
	fromQsettings.endGroup();

}

void GCamera::DelayedInterpretSettings( QSettings& fromQsettings )
{
	m_DoAquireContinuously.ReadDeviceSettings(fromQsettings);
}

void GCamera::SelectAOI()
{
	if(!m_IsOpened)
		return;
	SelectFullAOI();
	ImageProcessor()->AoiSelector()->open();
}

void GCamera::SelectFullAOI()
{
	if(!m_IsOpened)
		return;
	QRect maxImageRect(QPoint(0, 0), GetMaxImageSize());
	// compare the expected AOI to the one actually used. It should be the same!
	QRect returnedAOI = SetSensorAOI(maxImageRect);
	if(returnedAOI != maxImageRect)
		qWarning() << "GCamera::SelectFullAOI(), the maximal AOI used:" << returnedAOI << "is different from the maximal expected AOI:" << maxImageRect;
}

QRect GCamera::ChangeSensorAOI( QRect AoiRect )
{	
	if(!m_IsOpened)
		return QRect();
	QRect oldAoi = SensorAOI();
	if(oldAoi == AoiRect)
		return oldAoi;

	QRect usedRect = SetSensorAOI(AoiRect);

	if(oldAoi != usedRect)
		ImageProcessor()->SetAoiRect(usedRect);

	return usedRect;
}

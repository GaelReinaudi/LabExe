#ifndef GCAMERA_H
#define GCAMERA_H
#include "labexeimaging_global.h"

#include "device.h"
#include "param.h"
#include "GImageProcessor.h"

// class GImageProcessor;

/////////////////////////////////////////////////////////////////////
//! \brief A GCamera class defines the abstract base class for a camera.
/*!
It contains many pure virtual function that have to be implemented for the specific Camera 
driver your will be using.

One of its main characteristic for the integration in the LabExe framework is that it 
owns a pointer to a GImageProcessor object. The QThread (base class of GImageProcessor) 
can be use to grab images and redistribute them to the rest of the application with the OutputImage() signal.

When you create a new instance of the GImageProcessor in the derivate class, this 
should probably happen after the LatterInitialization() function if the constructor of 
the GImageProcessor object uses some member or handle to the camera.

Note that the user implementation of PopulateDeviceWidget() in the derived class will 
actually populate the widget that holds settings specific to the users camera. This is because the 
GCamera already fills part of the widget with typical camera controls.
*/
class LABEXEIMAGING_EXPORT GCamera : public GHardDevice, public GHasImageProcessor
{
	Q_OBJECT

public:
	GCamera(QString uniqueIdentifierName, QObject *parent, GImageProcessor* pImProcessor);
	virtual ~GCamera();

	//! Reimplemented from GDevice in order to provide a special kind of widget: GCameraSettingsWidget.
	virtual GDeviceWidget* ProvideNewDeviceGroupBox(QWidget* inWhichWidget, QBoxLayout::Direction orientation = QBoxLayout::LeftToRight);

	//! Gets the maximal size of an image. When reimplementing, you can (should) query and store the range in m_MaxImageSize on the first call of the function (since it will not change ever for a given camera). Any other call will return that m_MaxImageSize.
	virtual QSize GetMaxImageSize() const = 0;
	//! Gets the minimal exposure available on the camera and may adjust the exposure accordingly.
	virtual double GetExposureMin() = 0;
	//! Gets the maximal exposure available on the camera and may adjust the exposure accordingly.
	virtual double GetExposureMax() = 0;
	//! Gets the bits per pixel. Grey scale is assumed.
	virtual int BitsPerPixel() const = 0;
	//! Returns the frame-rate
	virtual double ImagePerSecond() = 0;
	//! The current Area Of Interest on the sensor.
	virtual QRect SensorAOI() const = 0;
	//! Return true if the camera supports being triggered by hardware TTL pulse.
	virtual bool SupportsExternalTrigger() const = 0;
	//! Uses the maximal size of the AOI: the whole sensor
	void SelectFullAOI();

protected:
	//! Re-implemented
	void PopulateSettings( QSettings& inQsettings );
	//! Re-implemented
	void InterpretSettings( QSettings& fromQsettings );

public slots:
	//! Grabs a single fram immediately, and fills the image with the data.
	virtual void SingleShotNow() = 0;
	//! Starts a continuous acquisition. A good idea when reimplementing this function is to start a QThread to wait for new images. (see GImageProcessor).
	virtual void ContinuousShot(bool StartOrStop = true) = 0;
	//! If supported, set the camera to wait for external trigger for taking pictures
	virtual void UseExternalTrigger(bool yesORno) = 0;
	//! Changes the exposure time. The default implementation calls SetExposureTime() and updates the m_Expo_ms param if necessary.
	virtual void ChangeExposureTime(double newExpo);
	//! Updates the exposure time in case some camera settings changed and the exposure range changed. It updates the m_Expo_ms param if necessary.
	void UpdateExposureTime();
	//! Changes the gain factor. The default implementation calls SetGainFactor() and updates the m_Gain param.
	virtual void ChangeGainFactor(double newGain);
	//! Opens a window to select the area of interest on the sensor
	void SelectAOI();
	//! Sets a correct AOI on the sensor based on the AoiRect and will return the actual rect used after matching the actual possibilities of the camera. If the camera doesn't support AOI, it will return a rect corresponding to the full sensor.
	QRect ChangeSensorAOI(QRect AoiRect);

protected:
	//! To implement in subclasses. Must call camera code to initialize/open the camera and return true if successful.
	virtual bool OpenCamera() = 0;
	//! To implement in subclasses. Must call camera code to close the camera properly and return true if successful.
	virtual bool CloseCamera() = 0;
	//! Sets the proposedNewExpo time to the new value. Returns the value that the camera actually adopts after the operation (like if the camera only accepts some discrete values).
	virtual double SetExposureTime(double proposedNewExpo) = 0;
	//! Sets the proposedGainFactor to the new value. Returns the value that the camera actually adopts after the operation (like if the camera only accepts some discrete values).
	virtual double SetGainFactor(double proposedGainFactor) = 0;
	//! Sets a correct AOI on the sensor based on the AoiRect and must return the actual rect used after matching the actual possibilities of the camera. If the camera doesn't support AOI, it should return a rect corresponding to the full sensor.
	virtual QRect SetSensorAOI(QRect AoiRect) = 0;
	//! Re-implemented
	virtual void LatterInitialization();
	//! Re-implement
	void DelayedInterpretSettings(QSettings& fromQsettings);

private slots:
	//! is connected to the ImageProcessor().OutputImage signal. This will emit other signals such as NumFrameSentChanged().
	void NotifyImageSent();
	//! is connected to the ImageProcessor().OutputImage signal. This will emit other signals such as NumFrameFailedChanged().
	void NotifyImageFailed();


signals:
	//! Should be emitted (by the serived class) when the camera is initialized or when a setting changed some other parameter range (e.g. pixel clock can change the exposure range).
	void CapabilitiesMayHaveChanged();
	//! Should be emitted (by the serived class) when the camera frame rate may have changed, e.g. just after every frame has been received.
	void FrameRateChanged(double fps);
	//! Is emitted when sending a frame, with the total number of frame sent as argument.
	void NumFrameSentChanged(int numTotalImages);
	//! Is emitted when transferring a frame from the camera failed, with the total number of failed frame.
	void NumFrameFailedChanged(int numTotalImages);
public:

protected:
	//! convenient variable to store the max image size range after GetMaxImageSize was called once.
	mutable QSize m_MaxImageSize;

private:
	//! param for the grabbing of images
	GParamBool m_DoAquireContinuously;
	//! param for the exposure, in ms
	GParamDouble m_Expo_ms;
	//! param for the gain factor
	GParamDouble m_Gain;
	//! param for the hardware triggering
	GParamBool m_HardwareTrigger;
	//! the pixel size in SI unit (m)
	GParamDouble m_PixelSize;

	int m_NumFrameSent;
	int m_NumFrameFailed;
	bool m_IsOpened;
	friend class GCameraSettingsWidget;
};

#endif // GCAMERA_H

#ifndef GIMAGEPROCESSOR_H
#define GIMAGEPROCESSOR_H
#include "labexeimaging_global.h"

#include "Device/GProgDevice.h"
#include "Param/GParamDouble.h"
#include "Param/GParamBool.h"
#include <QThread>

#include "GImageDouble.h"

class GAoiSelector;
class GImagingBench;

/////////////////////////////////////////////////////////////////////
//! \brief A GImageProcessor is a QObject that defines a base class for an object that can process an image.
/*!
It has 3 important features:
- a InputImage() slot to accept incoming images
- a OutputImage() signal to output images
- TO UPDATE
- it is a QThread so that some intensive processing can be performed concurrently (if necessary).
- it is actually a GProgDevice so that it can be dropped in benches and restored during de-serialization.
Examples of class that should own (or be) a derivate from GImageProcessor are:
- GCamera, that would not use the incoming slot,
- GImageFitting, that would not use the outgoing signal (unless the image has to be passed to some other processor object),
- GAbsorptionImagingProcessor that would wait for several incoming images before emitting a signal containing the resulting image.

A processor contains a GAoiSelector that takes care of defining the part of the incoming image that will be truncated out of the incoming images.
*/
class LABEXEIMAGING_EXPORT GImageProcessor : public GProgDevice
{
	Q_OBJECT

public:
	GImageProcessor(QObject *parent = 0, QString uniqueIdentifierName = "");
	virtual ~GImageProcessor();

	//! Returns true if this GImageProcessor has been defined as being an image provider, i.e. actually sending images out of the OutputImage() signal. For this, m_IsImageProvider has to be set to true in the constructor of the derived class.
	bool IsImageProvider() const { return m_IsImageProvider; }
	//! Should stop the process that was launched in the run()
	virtual void StopProcessing() {}
	//! Re-implemented
	GDeviceWidget* ProvideNewDeviceGroupBox(QWidget* inWhichWidget, QBoxLayout::Direction orientation = QBoxLayout::LeftToRight);
	//! Returns the AOI selector of the processor
	GAoiSelector* AoiSelector() const { return m_pAoiSelector; }
	//! Returns the rectangle of the AOI in the absolute frame of the image (not relatively to the GraphicsItem position, but to the scene)
	QRectF AoiRect() const {return m_pAoiItem->rect().translated(m_pAoiItem->scenePos());};
	//! Sets the rectangle of the AOI in the absolute frame of the image (not relatively to the GraphicsItem position,, but to the scene)
	void SetAoiRect(QRectF theRect);
	//! makes the imaging bench to have another color table for the image display. This will queue the request so that it can be called from the constructor if needed and still take effect after the bench is actually connected.
	void ChangeColorTable(QVector<QRgb> newColorTable) { 
		QMetaObject::invokeMethod(this, "RequestColorTable", Q_ARG(QVector<QRgb>, newColorTable));
	}

protected:
	//! Re-implemented
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented
	virtual void InterpretSettings(QSettings& fromQsettings);

public slots:
	//! slot that has to be re-implemented to process the image. Don't forget to call the base implementation that takes care of sending some signals like NumImagesInputChanged().
    virtual void InputImage(QImage);
	//! this slot will first call ProcessImageFull(), then ProcessImageAOIed() with a GImageDouble that is truncated to the AOI.
	void InputImageDouble(GImageDouble imageIn);
	//! tells the processor to show the AOI rectangle.
	void ShowAOI(bool doShow);
	
protected:
	//! Has to be re-implemented to process the full captured image while still specifying what is the Area Of Interest of the image processor. if the AOI is the whole picture, an invalid QRect() will be passed.
	virtual void ProcessImageFull(const GImageDouble & fullImage, const QRect & theAoiInTheFullImage) {
		Q_UNUSED(fullImage);
		Q_UNUSED(theAoiInTheFullImage);
	}
	//! Has to be re-implemented to process the image truncated by the AOI. It is called by InputImageDouble(). The aoiRect is the QRect corresponding to the AOI in the original picture. You have to call IncrementCountProcessed() to update the spin box in the widgets.
	virtual void ProcessImageAOIed(const GImageDouble & aoiImage) {Q_UNUSED(aoiImage);}
	//! Called when the AOI changed. Re-implement to perform custom actions. Default implementation emits the signal AoiChanged().
	virtual void EventAoiChanged(QRectF theAoiRect) {emit AoiChanged(theAoiRect.toRect());}

protected slots:
	//! slot used to tell the main thread to update the GraphicsItem in the display (if any). Indeed GraphicsItam is not thread safe so you HAVE to re-implement this slot if you intend to update the GraphicsItem from a concurrent thread.
	virtual void UpdateGraphicsItem() {}

signals:
	void OutputImage(QImage imageOut);
	void OutputImageDouble(GImageDouble imageOut);
	//! Is emitted when inputing a frame, with the total number of frame input as argument.
	void NumImagesInputChanged(int numTotalImages);
	//! Has to be emitted when the processing actually begins in the derived class.
	void NumImagesProcessedChanged(int numTotalImages);
	//! Can be emitted when the process failed, e.g. for a camera, the transfert could have failed.
	void ProcessorFailed();
	//! Emitted when the the AOI rect has changed
	void AoiChanged(QRect theValidatesRect);
	//! Emitted when requesting the imaging bench to have another color table for the image display
	void RequestColorTable(QVector<QRgb> newColorTable);

protected:
	//! tells if this GImageProcessor is an image provider, i.e. actually sending images out of the OutputImage() signal.
	bool m_IsImageProvider;
	//! Pointer to a graphics item that, if defined in derived class, will be drawn on top of the image of the GImagingBench's scene.
	QGraphicsRectItem* m_pAoiItem;
	//! Increments the count of processed images
	void IncrementCountProcessed(int byN = 1);
	//! tells the processor to treat incoming images or to be inactive, i.e. it wouldn't fire ProcessImageAOIed().
	GParamBool m_DoProcessImages;
	//! tells the processor to display the AOI.
	GParamBool m_DoDisplayAoi;

private:
	//! Number of images that have been input in this image processor
	int m_NumImagesInput;
	//! Number of images that have started being proceed
	int m_NumImagesProcessed;
	GImagingBench* m_pImBench;
	//! To select the AOI
	GAoiSelector* m_pAoiSelector;
	//! Sets the bench that the image processor works with
	void SetBench( GImagingBench* pTheBench );
	//! mutex
	QMutex m_Mutex;

	friend class GImagingBench;
// 	friend class GAoiSelector;
	friend class GImageProcessorWidget;
};

/////////////////////////////////////////////////////////////////////
//! \brief GHasImageProcessor is a class that defines something that contains a GImageProcessor object.
/*!
This is a class in test!

this is because some physical devices (hardware) are also a GImageProcessor, but GImageProcessor 
already inherits from GProgDevice. 
Using this class, we thus can have a GImageProcessor in a GDevice (through multiple inheritance from GHasImageProcessor).
*/
class LABEXEIMAGING_EXPORT GHasImageProcessor
{

public:
	GHasImageProcessor(GImageProcessor* pImProcessor) {m_pImageProcessor = pImProcessor;}
	virtual ~GHasImageProcessor() {}

	//! Returns the image processor that is to be defined in the constructor.
	GImageProcessor* ImageProcessor() const { return m_pImageProcessor; }

protected:
	//! The image processor that has to be defined and instantiated in the constructor.
	GImageProcessor* m_pImageProcessor;

};

#endif // GIMAGEPROCESSOR_H

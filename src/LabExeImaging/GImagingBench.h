#ifndef GIMAGINGBENCH_H
#define GIMAGINGBENCH_H
#include "labexeimaging_global.h"

#include "TheLab/GWorkBench.h"
#include "GImageDouble.h"
class GImageGraphicsScene;
class GImageGraphicsView;
class GImageProcessor;

/////////////////////////////////////////////////////////////////////
//! \brief The GImagingBench class defines a workbench intended to process images. 
/*!
It is a GWorkBench where the central widget has been set to a GImageGraphicsView/Scene.
*/
class LABEXEIMAGING_EXPORT GImagingBench : public GWorkBench
{
	Q_OBJECT

public:
	GImagingBench(QWidget *parent);
	~GImagingBench();

	// returns the scene that displays the image in its background.
	GImageGraphicsScene* ImageScene() const { return m_pScene; }

protected:
	//! Re-implemented.
	void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented.
	void InterpretSettings( QSettings& fromQsettings );

public:
	//! Re-implemented from GWorkBench in order to accept only and connect GCamera (FOR NOW).
	GDeviceWidget* AddDevice(GDevice* pTheDevice, QPoint whereInParent = QPoint(0, 0), bool putOnStack = true);
	//! Re-implemented from GWorkBench in order to disconnect some signals.
	void RemoveDevice(GDevice* pTheDevice, bool putOnStack = true);
	//! Re-implemented from GWorkBench in order to place the dock widget on the left if it IsImageProvider().
	virtual GBenchDockWidget* AddDeviceInNewDock(GDevice* pTheDevice, bool putOnStack = true);

signals:
	//! informs that an image has arrived from the outside (from the MainImageProvider()), e.g. a camera, or a file opener.
	void ImageHasArrived(QImage theImage);
	//! informs that an image has arrived from the outside (from the MainImageProvider()), e.g. a camera, or a file opener.
	void ImageDoubleHasArrived(GImageDouble theImage);

private:
	//! scene for the display
	GImageGraphicsScene* m_pScene;
	//! view for the display
	GImageGraphicsView* m_pView;
	//! the main image provider, i.e. the main (only) source of images that arrives in the bench from the outside, e.g. a camera, or a file opener.
	GImageProcessor* m_MainImageProvider;
	//! Returns the main image provider, i.e. the main (only) source of images that arrives in the bench from the outside, e.g. a camera, or a file opener.
	GImageProcessor* MainImageProvider() const { return m_MainImageProvider; }
	//! Sets the main image provider, i.e. the main (only) source of images that arrives in the bench from the outside, e.g. a camera, or a file opener.
	void SetMainImageProvider(GImageProcessor* pNewMainImageProvider);

};

#endif // GIMAGINGBENCH_H

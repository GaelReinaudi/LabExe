#include "GImagingBench.h"
#include "TheLab/GBenchDockWidget.h"
#include "GImageGraphicsScene.h"
#include "GImageGraphicsView.h"
#include "GCamera.h"
#include "GImageProcessor.h"

G_REGISTER_WORKBENCH_CLASS(GImagingBench)

GImagingBench::GImagingBench(QWidget *parent)
	: GWorkBench(parent)
	, m_MainImageProvider(0)
{
	m_pScene = new GImageGraphicsScene(this);
	m_pView = new GImageGraphicsView(m_pScene, this);
	m_pView->setAcceptDrops(false);
	setCentralWidget(m_pView);

	//connection of incoming images to the display
//	connect(this, SIGNAL(ImageHasArrived(QImage)), ImageScene(), SLOT(SetImage(QImage)));
 	connect(this, SIGNAL(ImageDoubleHasArrived(GImageDouble)), ImageScene(), SLOT(SetImage(GImageDouble)));
}

GImagingBench::~GImagingBench()
{

}

GDeviceWidget* GImagingBench::AddDevice( GDevice* pTheDevice, QPoint whereInParent /*= QPoint(0, 0)*/, bool putOnStack /*= true*/ )
{
	// we allow only GImageProcessors here!
	// is the device a GImageProcessor (through ProgDevice).
	GImageProcessor* pImProcessor = qobject_cast<GImageProcessor*>(pTheDevice);
	// or is it a hardware device (like a camera) the GHasImageProcessor?
	GHasImageProcessor* phasip = dynamic_cast<GHasImageProcessor*>(pTheDevice);

	if(!pImProcessor)
		if(phasip)
			pImProcessor = phasip->ImageProcessor();

	if(!pImProcessor)
		return GWorkBench::AddDevice(pTheDevice, whereInParent, putOnStack);

	// FIRST tell the image processor this is the bench it is part of
	pImProcessor->SetBench(this);
	GDeviceWidget* pDevWid = GWorkBench::AddDevice(pTheDevice, whereInParent, putOnStack);
	// put the graphics item of the processor in the scene
	m_pScene->addItem(pImProcessor->m_pAoiItem);

	if(pImProcessor->IsImageProvider()) {
		SetMainImageProvider(pImProcessor);
		return pDevWid;
	}
	// if not an image provider, it must be a processor of image
// 	connect(this, SIGNAL(ImageHasArrived(QImage)), pImProcessor, SLOT(InputImage(QImage)));
	connect(this, SIGNAL(ImageDoubleHasArrived(GImageDouble)), pImProcessor, SLOT(InputImageDouble(GImageDouble)));

	return pDevWid;
}

void GImagingBench::RemoveDevice( GDevice* pTheDevice, bool putOnStack /*= true*/ )
{
	GWorkBench::RemoveDevice(pTheDevice, putOnStack);
	// is the device a GImageProcessor (through ProgDevice).
	GImageProcessor* pImProcessor = qobject_cast<GImageProcessor*>(pTheDevice);
	// or is it a hardware device (like a camera) the GHasImageProcessor?
	GHasImageProcessor* phasip = dynamic_cast<GHasImageProcessor*>(pTheDevice);
	if(!pImProcessor)
		if(phasip)
			pImProcessor = phasip->ImageProcessor();
	if(!pImProcessor)
		return;
	// so here, we have an image processor pImProcessor
	
	// TEST
//	pImProcessor->deleteLater();
	if(pImProcessor == m_MainImageProvider)
		m_MainImageProvider = 0;

	disconnect(this, SIGNAL(ImageHasArrived(QImage)), pImProcessor, SLOT(InputImage(QImage)));
	disconnect(this, SIGNAL(ImageDoubleHasArrived(GImageDouble)), pImProcessor, SLOT(InputImageDouble(GImageDouble)));
	// removes the graphics item of the processor from the scene
	m_pScene->removeItem(pImProcessor->m_pAoiItem);
}

void GImagingBench::SetMainImageProvider( GImageProcessor* pNewMainImageProvider )
{
	// if there is already a m_MainImageProvider, let's disconnect it before using the new one
	if(m_MainImageProvider) {
//		disconnect(m_MainImageProvider, SIGNAL(OutputImage(QImage)), this, 0);
		disconnect(m_MainImageProvider, SIGNAL(OutputImageDouble(GImageDouble)), this, 0);
	}
	// still, if there already was a m_MainImageProvider, we connect its output to the input of the new one
	// this is so that an image provider like GAbsorptionProcessor can use the image from the camera
	if(m_MainImageProvider && pNewMainImageProvider) {
		connect(m_MainImageProvider, SIGNAL(OutputImageDouble(GImageDouble)), pNewMainImageProvider, SLOT(InputImageDouble(GImageDouble)));
	}

	m_MainImageProvider = pNewMainImageProvider;

	if(m_MainImageProvider) {
//  		connect(m_MainImageProvider, SIGNAL(OutputImage(QImage)), this, SIGNAL(ImageHasArrived(QImage)));
		connect(m_MainImageProvider, SIGNAL(OutputImageDouble(GImageDouble)), this, SIGNAL(ImageDoubleHasArrived(GImageDouble)));
	}
}

GBenchDockWidget* GImagingBench::AddDeviceInNewDock( GDevice* pTheDevice, bool putOnStack /*= true*/ )
{
	Q_UNUSED(putOnStack);
	GDeviceWidget* pDevWid = AddDevice(pTheDevice);
	if(!pDevWid)
		return 0;

	// this is just to get the image processor, and IsImageProvider() ? so that we put the dock on the left.
	// is the device a GImageProcessor (through ProgDevice).
	// or is it a hardware device (like a camera) the GHasImageProcessor?
	GImageProcessor* pImProcessor = qobject_cast<GImageProcessor*>(pTheDevice);
	GHasImageProcessor* phasip = dynamic_cast<GHasImageProcessor*>(pTheDevice);
	if(!pImProcessor)
		if(phasip)
			pImProcessor = phasip->ImageProcessor();
	// if IsImageProvider(), we put it on the left
	GBenchDockWidget* pNewDock = 0;
	if(pImProcessor && pImProcessor->IsImageProvider())
		pNewDock = new GBenchDockWidget(this, Qt::LeftDockWidgetArea);
	else // else the default
		pNewDock = new GBenchDockWidget(this, Qt::AllDockWidgetAreas);

	pNewDock->InsertDeviceWidget(pDevWid);
	return pNewDock;
}

void GImagingBench::PopulateSettings(QSettings& inQsettings)
{
	GWorkBench::PopulateSettings(inQsettings);
	QPointF centerPoint = m_pView->mapToScene(m_pView->viewport()->rect().center());
	inQsettings.setValue("center-point", centerPoint);
}

void GImagingBench::InterpretSettings( QSettings& fromQsettings )
{
	GWorkBench::InterpretSettings(fromQsettings);
	QPointF centerPoint = fromQsettings.value("center-point").toPointF();
	m_pView->centerOn(centerPoint);
}

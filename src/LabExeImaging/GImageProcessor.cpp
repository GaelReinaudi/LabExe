#include "GImageProcessor.h"
#include "GImageProcessorWidget.h"
#include "GAoiSelector.h"
#include "GImagingBench.h"

GImageProcessor::GImageProcessor(QObject *parent /*= 0*/, QString uniqueIdentifierName /*= ""*/)
 	: GProgDevice(parent, uniqueIdentifierName)
	, m_IsImageProvider(false)
	, m_pAoiItem(new QGraphicsRectItem(0.0, 0.0, 0.0, 0.0))
	, m_NumImagesInput(0)
	, m_NumImagesProcessed(0)
	, m_pImBench(0)
	, m_pAoiSelector(new GAoiSelector(this))
	, m_DoProcessImages("Enable", this)
	, m_DoDisplayAoi("Show", this)
{
	m_pAoiItem->setPen(QPen(Color()));
	m_DoProcessImages = false;
	m_DoDisplayAoi = true;
	connect(&m_DoDisplayAoi, SIGNAL(ValueUpdated(bool)), this, SLOT(ShowAOI(bool)));
}

GImageProcessor::~GImageProcessor()
{
// 	if(m_pAoiItem && !m_pAoiItem->scene())
// 		delete m_pAoiItem;
// 	if(m_pAoiSelector)
// 		delete m_pAoiSelector;
}

void GImageProcessor::InputImage(QImage imageIn )
{
//	m_NumImagesInput++;
//	emit NumImagesInputChanged(m_NumImagesInput);
// 	qDebug() << Name() << "received" << m_NumImagesInput << "images";
}

void GImageProcessor::InputImageDouble(GImageDouble imageIn )
{
	m_NumImagesInput++;
	emit NumImagesInputChanged(m_NumImagesInput);

	QSize sizeImage = imageIn.size();
	QRect aoirect = m_pAoiSelector->ValidRect(sizeImage);

	// if we are not suppose to treat the image:
	if(!m_DoProcessImages)
		return;

	// if the AOI is the whole image, we treat it directly
	// else, we treat the extracted part
	if(aoirect.size() == sizeImage) {
		ProcessImageFull(imageIn, QRect());
		ProcessImageAOIed(imageIn);
	}
	else {
		GImageDouble aoiImage = imageIn.ExtractAoi(aoirect);
		ProcessImageFull(imageIn, aoirect);
		ProcessImageAOIed(aoiImage);
	}
}

GDeviceWidget* GImageProcessor::ProvideNewDeviceGroupBox( QWidget* inWhichWidget, QBoxLayout::Direction orientation /*= QBoxLayout::LeftToRight*/ )
{
	if(!m_pImBench) {
// 		qWarning() << "the Imaging bench pointer in the GImageProcessor is 0.   642481";
// 		return 0;
	}

	GImageProcessorWidget* pWidToReturn = new GImageProcessorWidget(this, inWhichWidget);
	PopulateDeviceWidget(pWidToReturn);
	return pWidToReturn;
}

void GImageProcessor::SetBench( GImagingBench* pTheBench )
{
	m_pImBench = pTheBench;
	m_pAoiSelector->m_pView->setScene(m_pImBench->ImageScene());
	m_pAoiSelector->setParent(m_pImBench);
	m_pAoiSelector->setWindowFlags(Qt::Dialog);
	connect(this, SIGNAL(RequestColorTable(QVector<QRgb>)), m_pImBench->ImageScene(), SLOT(QVector<QRgb>));
}

void GImageProcessor::IncrementCountProcessed(int byN /*= 1*/)
{
	m_Mutex.lock();
	m_NumImagesProcessed += byN;
	emit NumImagesProcessedChanged(m_NumImagesProcessed);
	m_Mutex.unlock();
}

void GImageProcessor::PopulateSettings( QSettings& inQsettings )
{
	GProgDevice::PopulateSettings(inQsettings);
	inQsettings.setValue("AoiRect", AoiRect());
}

void GImageProcessor::InterpretSettings( QSettings& fromQsettings )
{
	GProgDevice::InterpretSettings(fromQsettings);
	QRectF theAoi = fromQsettings.value("AoiRect", QRectF()).toRectF();
	if(theAoi.isValid()) {
		SetAoiRect(theAoi);
	}
}

void GImageProcessor::ShowAOI( bool doShow )
{
	m_pAoiItem->setVisible(doShow);
}

void GImageProcessor::SetAoiRect( QRectF theRect )
{
	m_pAoiItem->setPos(theRect.topLeft());
	QRectF aoiRectRelativeToItem(QPointF(0.0, 0.0), theRect.size());
	m_pAoiItem->setRect(aoiRectRelativeToItem);

	EventAoiChanged(AoiRect());
}
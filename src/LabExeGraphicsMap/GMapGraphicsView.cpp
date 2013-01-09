#include "GMapGraphicsView.h"
#include "GAgentGraphicsItem.h"
#include <QGLWidget>
#include <QDir>

GMapGraphicsView::GMapGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	// try optimization
// 	setViewport(new QGLWidget());
	setOptimizationFlags(QGraphicsView::DontSavePainterState);
 	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setCacheMode(QGraphicsView::CacheBackground);

	// tracking fps
	FPSTracker* pFps = new FPSTracker(this);
	viewport()->installEventFilter(pFps);

	// remove scrollbars
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setBackgroundBrush(QBrush::QBrush("#4C4C4C"));
//	// try to load the background image
// 	m_BackgroundImage = QPixmap(":/images/milkyway_galaxy.jpg");
// 	if(m_BackgroundImage.isNull())
// 		qDebug() << "Failed to load background image" << QDir(":/images/milkyway_galaxy.jpg");

	scaleView(0.02);
}

GMapGraphicsView::~GMapGraphicsView()
{

}

void GMapGraphicsView::wheelEvent( QWheelEvent *event )
{
    int deltaMouse = event->delta();
    // if we zoom in, the zoom is centered on the mouse
    if(deltaMouse > 0)
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    else // else, we unzoom from the center of the view
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    // from Qt Elastic Nodes Example
    scaleView(pow(1.2, deltaMouse / 160.0));
}

// from Qt Elastic Nodes Example
void GMapGraphicsView::scaleView(qreal scaleFactor)
{
//     qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    scale(scaleFactor, scaleFactor);
}

void GMapGraphicsView::mouseDoubleClickEvent( QMouseEvent* event )
{
	if(QGraphicsItem *item = itemAt(event->pos())) {
//		fitInView(item, Qt::KeepAspectRatio);
		GAgentGraphicsItem* pAgIt = qobject_cast<GAgentGraphicsItem*>(static_cast<GAgentGraphicsItem*>(item));
		if(pAgIt)
			pAgIt->mouseDoubleClickEvent(0);
	}
}

void GMapGraphicsView::drawBackground( QPainter * painter, const QRectF & rect )
{
	QGraphicsView::drawBackground(painter, rect);
	// try to draw a fixed background centered
	QRect rectViewPort = painter->viewport();
	QRect croppedRectViewPort = rectViewPort;
	croppedRectViewPort.moveCenter(QPoint(m_BackgroundImage.width() / 2, m_BackgroundImage.height() / 2));
	QPixmap cropedImage = m_BackgroundImage.copy(croppedRectViewPort);
//	const QTransform old = painter->transform();
	painter->setTransform(QTransform());
	painter->drawPixmap(rectViewPort, m_BackgroundImage, croppedRectViewPort);
//	painter->setTransform(old);
}

void GMapGraphicsView::UpdateFromSceneRect( QRectF newSceneRect )
{
	double w = newSceneRect.width() / 1.0;
	double h = newSceneRect.height() / 1.0;
	newSceneRect.adjust(-w, -h, w, h);
	setSceneRect(newSceneRect);
}
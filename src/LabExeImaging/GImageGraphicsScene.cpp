#include "GImageGraphicsScene.h"
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

GImageGraphicsScene::GImageGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
// 	, m_pImageGetter(new GImageProcessor(this))
{
	QVector<QRgb> colorTable(256);
	for(int i  = 0; i < 256; i++)
		colorTable[i] = qRgb(i, i, i);
	colorTable[255] = qRgb(255, 0, 0);
	SetColorTable(colorTable);
}

GImageGraphicsScene::~GImageGraphicsScene()
{

}

void GImageGraphicsScene::SetImage(QImage image)
{
	image.setColorTable(m_ColorTable);

	setBackgroundBrush(QBrush::QBrush(image));
	setSceneRect(0, 0, image.width(), image.height());
}

void GImageGraphicsScene::SetImage( GImageDouble image )
{
	SetImage((QImage)image);
	m_CurrentImage = image;
}

void GImageGraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent )
{
	int xPos = mouseEvent->scenePos().x();
	int yPos = mouseEvent->scenePos().y();
	double pixelValue = m_CurrentImage.PixelAt(xPos, yPos);
	qDebug() << xPos << yPos << pixelValue;
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

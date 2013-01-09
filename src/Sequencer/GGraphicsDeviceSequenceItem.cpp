#include "GGraphicsDeviceSequenceItem.h"
// #include "Device/GSignalDevice.h"
#include <QPen>

#define STANDARD_HEIGHT_DEVICE_SEQUENCE (40.0)

GGraphicsDeviceSequenceItem::GGraphicsDeviceSequenceItem(GDevice* pDevice, QGraphicsItem *parentItem)
	: QGraphicsRectItem(parentItem)
{
	m_pDevice = pDevice;
	setRect(0, -STANDARD_HEIGHT_DEVICE_SEQUENCE / 2, 0, STANDARD_HEIGHT_DEVICE_SEQUENCE);
// 	//! If the device has a range, we set the rectangle so that its hight matches the range .
// 	if(m_pDevice->inherits("GSignalDevice")) {
// 		GSignalDevice* sigDevice = static_cast<GSignalDevice*>(pDevice);
// 		setRect(0, -sigDevice->RangeMax(), 0.3, sigDevice->RangeSpan());
// 	}

	ScaleVerticallyToMatchAGivenHeight(STANDARD_HEIGHT_DEVICE_SEQUENCE);

	QPen pen(Qt::blue, 0, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
	setPen(pen);
	setBrush(QBrush(Qt::white));
//	setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	scale(2, 2);
}

GGraphicsDeviceSequenceItem::~GGraphicsDeviceSequenceItem()
{

}

void GGraphicsDeviceSequenceItem::PositionBelow( GGraphicsDeviceSequenceItem* pDeviceGraph )
{
	QRectF previousDeviceRect = pDeviceGraph->sceneBoundingRect();
	QRectF thatDeviceRect = sceneBoundingRect();
	double botOther = previousDeviceRect.bottom();
	double topThis = thatDeviceRect.top();
	if(botOther != topThis)
		moveBy(0, botOther - topThis);
}

void GGraphicsDeviceSequenceItem::ScaleVerticallyToMatchAGivenHeight( double heightToMatch )
{
	double actualheight = rect().height();
	scale(1, heightToMatch / actualheight);
}
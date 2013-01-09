#ifndef QGRAPHICSDEVICESEQUENCEITEM_H
#define QGRAPHICSDEVICESEQUENCEITEM_H

#include <QGraphicsRectItem>
class GDevice;

class GGraphicsDeviceSequenceItem : public QGraphicsRectItem
{
public:
	GGraphicsDeviceSequenceItem(GDevice* pDevice, QGraphicsItem *parentItem);
	~GGraphicsDeviceSequenceItem();

	//! Moves the Item vertically to position it below an other DeviceItem.
	void PositionBelow(GGraphicsDeviceSequenceItem* pDeviceGraph);
	//! Scales the Device sequence item vertically so that it appears with a height of heightToMatch.
	void ScaleVerticallyToMatchAGivenHeight(double heightToMatch);

private:
	//! The device it is attached to
	GDevice* m_pDevice;
};

#endif // QGRAPHICSDEVICESEQUENCEITEM_H

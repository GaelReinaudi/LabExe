#ifndef GSEQUENCE_H
#define GSEQUENCE_H

#include <QHash>
#include "Instruction/G_old_Instruction.h"
class GGraphicsDeviceSequenceItem;

class G_old_Sequence : public G_old_TimeEvent
{
	Q_OBJECT

public:
	G_old_Sequence(QObject *parent);
	~G_old_Sequence();

	void AddDevice(GDevice* pDev);
	void RemoveDevice(GDevice* pDev);

	const G_old_TimeEvent* TimeOrigin() const { return this; }

signals:
	void DeviceJustAdded(GDevice* pDev);

protected:
	//! This will add the new child and all its children to the QGraphicsview, in the QGraphicsItem corresponding to the right GDevice.
	void ChildWasJustAdded(GTreeItem* pItemAdded, int AtGeneration);
	//! This registers the newly added instructions (or event) and all its children.
	//! It is updating the list of devices and the GraphicsItems that represents the device's sequence in the QGraphicsScene.
	void RegisterNewInstructionsDevices( GTreeItem* pItemAdded );
	// This adds the instruction's device to the list of device managed by the sequence (if not already in it).
	void LinkInstruction_ToListOfDevices(G_old_Instruction* pInstruction);
	//! This adds the instruction's Graphics item to the QGraphicsItem corresponding to the right GDevice.
	//! It inserts actually the QGraphicsItem AnchorEventGraphicsItem() that represent the AnchorPoint of the instruction.
	void LinkInstructionGraphics_ToDeviceGraphics(G_old_Instruction* pInstruction);

private:
	//! The list of devices managed by the sequence
	QList<GDevice*> m_Devices;
	//! Each GDevice in the sequence has its own QGraphicsRectItem in which will be drawn the corresponding instructions.
	QHash<GDevice*, GGraphicsDeviceSequenceItem*> m_DevicesGraphicsItem;

	friend class GSequenceModel;
};

#endif // GSEQUENCE_H

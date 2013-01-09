#include "G_old_Sequence.h"
#include "Sequencer/GGraphicsDeviceSequenceItem.h"

G_old_Sequence::G_old_Sequence(QObject *parent)
	: G_old_TimeEvent()
{

}

void G_old_Sequence::AddDevice( GDevice* pDev )
{
	if(!pDev)
		return;
	if(!m_DevicesGraphicsItem.contains(pDev)) {
		GGraphicsDeviceSequenceItem* pNewGraphItem = new GGraphicsDeviceSequenceItem(pDev, EventGraphicsItem());
		m_DevicesGraphicsItem.insert(pDev, pNewGraphItem);
	}
	if(!m_Devices.contains(pDev)) {
		m_Devices.append(pDev);
		emit DeviceJustAdded(pDev);
	}
}

void G_old_Sequence::RemoveDevice( GDevice* pDev )
{

}

G_old_Sequence::~G_old_Sequence()
{

}

void G_old_Sequence::ChildWasJustAdded( GTreeItem* pItemAdded, int AtGeneration )
{
	RegisterNewInstructionsDevices(pItemAdded);
}

void G_old_Sequence::RegisterNewInstructionsDevices( GTreeItem* pItemAdded )
{
	if(!pItemAdded)
		return;
	// let's consider the newly added GTreeItem, 
	// and all its children
	QList<GTreeItem*> ListItems = pItemAdded->AllGenerationsChildrenItems();
	ListItems.prepend(pItemAdded);
	foreach(GTreeItem* pIt, ListItems) {
		// if it is a G_old_Instruction, we add the instruction to the sequence
		if(pItemAdded->inherits("G_old_Instruction")) {
			G_old_Instruction* pIns = static_cast<G_old_Instruction*>(pItemAdded);
			// first we add the device so that...
			LinkInstruction_ToListOfDevices(pIns);
			// ... we can add the GraphicsItem to the sequence
			LinkInstructionGraphics_ToDeviceGraphics(pIns);
		}
	}
}

void G_old_Sequence::LinkInstruction_ToListOfDevices( G_old_Instruction* pInstruction )
{
	if(!pInstruction)
		return;
	// Does the instruction has a device?
	GDevice* pDev = pInstruction->Device();
	if(!pDev)
		return;
	// Is this GDevice in the G_old_Sequence?
	if(m_Devices.contains(pDev))
		return;
	// Let's add then
	AddDevice(pDev);
}

void G_old_Sequence::LinkInstructionGraphics_ToDeviceGraphics( G_old_Instruction* pInstruction )
{
	if(!pInstruction)
		return;
	// Does the instruction has a GDevice?
	GDevice* pDev = pInstruction->Device();
	if(!pDev)
		return;
	// Is this GDevice in the G_old_Sequence Graphics?
	QGraphicsItem* pDeviceGraphicsItem = m_DevicesGraphicsItem.value(pDev, 0);
	if(!pDeviceGraphicsItem)
		return;
	// Let's add then
	QGraphicsLineItem* pAnchorIns = pInstruction->AnchorEventGraphicsItem();
	pAnchorIns->setParentItem(pDeviceGraphicsItem);
}

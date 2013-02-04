#ifndef GInstruction_H
#define GInstruction_H
#include "labexe_global.h"

#include "GChannelSynchEvent.h"
class GInstructionGraphicsItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GInstruction class is a GChannelSynchEvent that defines an instruction.
/*!
It is still displayed in the tree-scene of the sequence as it is a subclass of GChannelSynchEvent.
It holds an other graphics item inheriting the GInstructionGraphicsItem (QGraphicsRectItem).

Event-wise:
- the GSynchEvent inherited from GChannelSynchEvent is defined as the anchor point of the instruction,
- it coincides with the beginning of the instruction, unless if (to be implemented!) the instruction is moved relatively to its anchor point to start before/after the GSynchEvent.
*/
class GInstruction : public GChannelSynchEvent
{
	Q_OBJECT

public:
	GInstruction(GSynchEvent* parentEventSynchro, GChannel* pAssignedChannel = 0);
	~GInstruction();

protected:
	//! For painting itself and interacting in the sequence QGraphicsView. This has to be created with the appropriate subclass of GInstructionGraphicsItem for each subclass of GInstruction.
	GInstructionGraphicsItem* m_pInstructionGraphicsItem;

	friend class GInstructionGraphicsItem;
};

#endif // GInstruction_H

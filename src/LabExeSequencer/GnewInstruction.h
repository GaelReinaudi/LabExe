#ifndef GInstruction_H
#define GInstruction_H
#include "labexe_global.h"

#include "GnewChannelSynchEvent.h"
class GnewInstructionGraphicsItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GnewInstruction class is a GnewChannelSynchEvent that defines an instruction.
/*!
It is still displayed in the tree-scene of the sequence as it is a subclass of GnewChannelSynchEvent.
It holds an other graphics item inheriting the GnewInstructionGraphicsItem (QGraphicsRectItem).

Event-wise:
- the GSynchEvent inherited from GnewChannelSynchEvent is defined as the anchor point of the instruction,
- it coincides with the beginning of the instruction, unless if (to be implemented!) the instruction is moved relatively to its anchor point to start before/after the GSynchEvent.
*/
class GnewInstruction : public GnewChannelSynchEvent
{
	Q_OBJECT

public:
	GnewInstruction(GSynchEvent* parentEventSynchro, GnewChannel* pAssignedChannel = 0);
	~GnewInstruction();

protected:
	//! For painting itself and interacting in the sequence QGraphicsView. This has to be created with the appropriate subclass of GnewInstructionGraphicsItem for each subclass of GnewInstruction.
	GnewInstructionGraphicsItem* m_pInstructionGraphicsItem;

	friend class GnewInstructionGraphicsItem;
};

#endif // GInstruction_H

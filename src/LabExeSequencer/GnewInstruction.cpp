#include "GnewInstruction.h"
#include "GnewChannel.h"
#include "GnewChannelGraphicsItem.h"
#include "GnewInstructionGraphicsItem.h"
#include "../ToolBox/GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GnewInstruction, GSynchEvent)

GnewInstruction::GnewInstruction(GSynchEvent* parentEventSynchro, GnewChannel* pAssignedChannel /*= 0*/)
	: GnewChannelSynchEvent(parentEventSynchro, pAssignedChannel)
	, m_pInstructionGraphicsItem(new GnewInstructionGraphicsItem(this))
{
	// for the event treeLineItem
	setVisible(false);
}

GnewInstruction::~GnewInstruction()
{

}

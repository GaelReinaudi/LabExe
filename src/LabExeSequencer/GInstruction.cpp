#include "GInstruction.h"
#include "GChannel.h"
#include "GChannelGraphicsItem.h"
#include "GInstructionGraphicsItem.h"
#include "../ToolBox/GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GInstruction, GSynchEvent)

GInstruction::GInstruction(GSynchEvent* parentEventSynchro, GChannel* pAssignedChannel /*= 0*/)
	: GChannelSynchEvent(parentEventSynchro, pAssignedChannel)
	, m_pInstructionGraphicsItem(new GInstructionGraphicsItem(this))
{
	// for the event treeLineItem
	setVisible(false);
}

GInstruction::~GInstruction()
{

}

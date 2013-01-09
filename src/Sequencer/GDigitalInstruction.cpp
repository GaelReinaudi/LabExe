#include "GDigitalInstruction.h"
#include "GDigitalInstructionGraphicsItem.h"
#include "GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GDigitalInstruction, GSynchEvent)

GDigitalInstruction::GDigitalInstruction(GSynchEvent* parentEventSynchro, GChannel* pAssignedChannel /*= 0*/)
	: GInstruction(parentEventSynchro, pAssignedChannel)
	, m_Value(false)
{
	m_pInstructionGraphicsItem = new GDigitalInstructionGraphicsItem(this);
}

GDigitalInstruction::~GDigitalInstruction()
{

}

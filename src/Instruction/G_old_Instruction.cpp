#include "G_old_Instruction.h"

G_old_Instruction::G_old_Instruction(G_old_TimeEvent* parent)
	: G_old_TimeEvent(parent)
{
	m_AnchorPointTime = 0;
	// we tell the event item (G_old_Instruction is derived from G_old_TimeEvent) that it is now the parent of the instruction's item.
	m_GraphicsItem.setParentItem(AnchorEventGraphicsItem());
}

G_old_Instruction::~G_old_Instruction()
{

}

//************************************
// Method:    SetAnchorPointTime
// FullName:  G_old_Instruction::SetAnchorPointTime
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: double newAnchorPointTime
/*!
The anchor point specifies the relative point in time that represents the t=0 of the instruction.
If you SetAnchorPointTime(0.23), then the anchor point will be set 230ms after the beginning of the instruction, 
which will make it actually start 230ms before the time you insert the instruction.
*/
//************************************
void G_old_Instruction::SetAnchorPointTime( double newAnchorPointTime )
{
	if(newAnchorPointTime == m_AnchorPointTime)
		return;
	m_AnchorPointTime = newAnchorPointTime;
	m_GraphicsItem.setPos(-m_AnchorPointTime, 0);
	emit AnchorPointChanged(newAnchorPointTime);
}

//void G_old_Instruction::UpdateGraphicsItem()
//{
//	m_GraphicsItem.setRect(BoundingSceneRect());
//}
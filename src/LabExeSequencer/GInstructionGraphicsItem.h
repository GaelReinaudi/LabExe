#ifndef GINSTRUCTIONGRAPHICSITEM_H
#define GINSTRUCTIONGRAPHICSITEM_H

#include <QGraphicsRectItem>
class GInstruction;
class GSynchEventGraphicsItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GInstructionGraphicsItem class represents an instruction in GSequenceGraphicsScene.
/*
It is meant to visualize the event as rectangle, and to forward some 
user interaction to the actual GInstruction it represents.

*/
class GInstructionGraphicsItem : public QObject, public QGraphicsRectItem
{
public:
	GInstructionGraphicsItem(GInstruction* pInstruction);
	~GInstructionGraphicsItem();

	//! Returns the instruction represented by this item
	GInstruction* Instruction() const {return m_pInstruction;}


private:
	//! pointer to the GInstruction represented by this GInstructionGraphicsItem.
	GInstruction* m_pInstruction;
};

#endif // GINSTRUCTIONGRAPHICSITEM_H

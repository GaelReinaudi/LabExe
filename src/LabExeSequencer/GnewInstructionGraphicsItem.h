#ifndef GINSTRUCTIONGRAPHICSITEM_H
#define GINSTRUCTIONGRAPHICSITEM_H

#include <QGraphicsRectItem>
class GnewInstruction;
class GnewSynchEventGraphicsItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GnewInstructionGraphicsItem class represents an instruction in GSequenceGraphicsScene.
/*
It is meant to visualize the event as rectangle, and to forward some 
user interaction to the actual GnewInstruction it represents.

*/
class GnewInstructionGraphicsItem : public QObject, public QGraphicsRectItem
{
public:
	GnewInstructionGraphicsItem(GnewInstruction* pInstruction);
	~GnewInstructionGraphicsItem();

	//! Returns the instruction represented by this item
	GnewInstruction* Instruction() const {return m_pInstruction;}


private:
	//! pointer to the GnewInstruction represented by this GnewInstructionGraphicsItem.
	GnewInstruction* m_pInstruction;
};

#endif // GINSTRUCTIONGRAPHICSITEM_H

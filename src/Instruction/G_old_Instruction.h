#ifndef GINSTRUCTION_H
#define GINSTRUCTION_H

#include "G_old_TimeEvent.h"
#include "GGraphicsInstructionItem.h"
class GDevice;

/////////////////////////////////////////////////////////////////////
//! \brief The G_old_Instruction class is the base abstract class of any instruction that can be used by the G_old_Sequence.
/*!
more
*/
class G_old_Instruction : public G_old_TimeEvent
{
 	Q_OBJECT

public:
	G_old_Instruction(G_old_TimeEvent* parent);
	~G_old_Instruction();

public:
	//! The duration associated with the instruction.
	virtual double Duration() const = 0;
	//! Gets the GDevice associated with the instruction.
	virtual GDevice* Device() const = 0;
	//! Sets the anchor point of the instruction. 
	void SetAnchorPointTime(double newAnchorPointTime);

	//! Returns the GraphicsItem that can represent the instruction in a QGraphicsScene.
	GGraphicsInstructionItem* InstructionGraphicsItem() const { return &m_GraphicsItem; }
	//! Returns the QGraphicsItem that can represent the anchor point event in a QGraphicsScene. 
	//! It is the parent of the InstructionGraphicsItem();
	QGraphicsLineItem* AnchorEventGraphicsItem() /*const*/ { return m_pEventGraphicsItem; }

signals:
	void AnchorPointChanged(double newAnchorPointTime);

protected slots:
	void testSlot(double newval) {DelayFromParent(newval);}
	//! Updates the graphics item accordingly to the wishes of the derived class. 
	//! The default implementation is a rectangle given by BoundingSceneRect()
//	virtual void UpdateGraphicsItem();

protected:
//	//! The relevant QrectF that defines the display bounding rect of a QGraphicsScene in which we are drawing the instruction.
//	virtual QRectF BoundingSceneRect() const = 0;

private:
	double m_AnchorPointTime;
	//! For painting itself in a QGraphicsView, each instruction contains an item (that can contain sub-items...)
	mutable GGraphicsInstructionItem m_GraphicsItem;

	friend class GInstructionView;
};

#endif // GINSTRUCTION_H

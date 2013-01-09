#ifndef GDIGITALINSTRUCTIONGRAPHICSITEM_H
#define GDIGITALINSTRUCTIONGRAPHICSITEM_H

#include "GInstructionGraphicsItem.h"
class GDigitalInstruction;

/////////////////////////////////////////////////////////////////////
//! \brief The GDigitalInstructionGraphicsItem class is derived from GInstructionGraphicsItem to represent a digital value.
/*
*/
class GDigitalInstructionGraphicsItem : public GInstructionGraphicsItem
{
	Q_OBJECT

public:
	GDigitalInstructionGraphicsItem(GDigitalInstruction* pInstruction);
	~GDigitalInstructionGraphicsItem();

protected:
	//! Re-implemented
	void mousePressEvent(QGraphicsSceneMouseEvent * event);

protected slots:
	//! Changes the state (=color) of the graphics item.
	void SwitchState(bool newState);

signals:
	void Clicked();

private:
	
};

#endif // GDIGITALINSTRUCTIONGRAPHICSITEM_H

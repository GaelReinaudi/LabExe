#include "GDigitalInstructionGraphicsItem.h"
#include "GDigitalInstruction.h"

GDigitalInstructionGraphicsItem::GDigitalInstructionGraphicsItem(GDigitalInstruction* pInstruction)
	: GInstructionGraphicsItem(pInstruction)
{
	connect(Instruction(), SIGNAL(ValueChanged(bool)), this, SLOT(SwitchState(bool)));
}

GDigitalInstructionGraphicsItem::~GDigitalInstructionGraphicsItem()
{

}

void GDigitalInstructionGraphicsItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	// this should be a digital instruction
	GDigitalInstruction* pDigInt =  qobject_cast<GDigitalInstruction*>(Instruction());
	// switch the state of the instruction
	if(pDigInt && event->button() == Qt::LeftButton)
		pDigInt->SetValue(!pDigInt->GetValue());
}

void GDigitalInstructionGraphicsItem::SwitchState( bool newState )
{
	if(newState)
		setBrush(QBrush(Qt::red));
	else
		setBrush(QBrush(Qt::white));
}
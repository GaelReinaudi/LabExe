#include "GInstructionGraphicsItem.h"
#include "Sequence/GSynchEventGraphicsItem.h"
#include "GInstruction.h"
#include "GChannel.h"

GInstructionGraphicsItem::GInstructionGraphicsItem(GInstruction* pInstruction)
	: QGraphicsRectItem(pInstruction ? pInstruction->m_pEventGraphicsItem : 0)
	, m_pInstruction(pInstruction)
{
	double length = 2.718;
	double height = 2.718 * 2;
	QRectF theRect(0.0, 0.0, length, height);
	// adapts the height of the instruction to the height of the channel
	if(m_pInstruction && m_pInstruction->Channel()) {
		theRect.setHeight(m_pInstruction->Channel()->ChannelGraphicsItem()->rect().height());
	}
	setRect(theRect);

	setBrush(QBrush(Qt::white));
}

GInstructionGraphicsItem::~GInstructionGraphicsItem()
{

}

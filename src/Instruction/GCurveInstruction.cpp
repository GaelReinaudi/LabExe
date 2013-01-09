#include "GCurveInstruction.h"

GCurveInstruction::GCurveInstruction(G_old_TimeEvent* parent)
	: G_old_Instruction(parent)
{
	m_pDevice = 0;
	m_pCurve = 0;
}

GCurveInstruction::~GCurveInstruction()
{

}

double GCurveInstruction::Duration() const
{
	Q_ASSERT(m_pCurve); 
	return m_pCurve->Duration();
}

void GCurveInstruction::setCurve( GCurve* pCurve )
{
	m_pCurve = pCurve;
	if(m_pCurve) {
		m_pCurve->GetNewItemGroupForDrawing()->setParentItem(InstructionGraphicsItem());
	}
}

void GCurveInstruction::SetSignalDevice( /*GSignal*/GDevice* pDevice )
{
	m_pDevice = pDevice;
//	UpdateGraphicsItem();
}

//QRectF GCurveInstruction::BoundingSceneRect() const
//{
//	if(!m_pDevice || !m_pCurve)
//		return QRect();
//	double min = SignalDevice()->RangeMin();
//	double max = SignalDevice()->RangeMax();
//	QRectF therect;
//	therect.setBottom(-min);
//	therect.setTop(-max);
//	therect.setLeft(0);
//	therect.setRight(Duration());
//	return therect;
//}

//void GCurveInstruction::UpdateGraphicsItem()
//{
//	//G_old_Instruction::UpdateGraphicsItem();
//	//if(m_pCurve) {
//	//	m_pCurve->GetNewItemGroupForDrawing()->setParentItem(InstructionGraphicsItem());
//	//}
//}

#include "GInstructionView.h"
#include "Instruction/G_old_Instruction.h"

GInstructionView::GInstructionView(QWidget *parent)
	: QGraphicsView(parent)
{
	m_pInstruction = 0;
}

GInstructionView::~GInstructionView()
{

}

void GInstructionView::SetInstruction( G_old_Instruction* pInstruction )
{
	m_pInstruction = pInstruction;
	UpdateView();
}

void GInstructionView::resizeEvent( QResizeEvent * event )
{
	QGraphicsView::resizeEvent(event);
	FitInstructionInView();
}

void GInstructionView::UpdateView()
{
	if(!m_pInstruction)
		return;
//	m_pInstruction->UpdateGraphicsItem();
	QGraphicsItem* pItemInstruction = &m_pInstruction->m_GraphicsItem;
	// is the item already displayed in a scene?
	m_pInsScene = pItemInstruction->scene();
	// if so, we display this scene, centered on the item. An item can only be in ONE scene at a time
	// else we make a scene and display the item.
	if(!m_pInsScene) {
		m_pInsScene = new QGraphicsScene(m_pInstruction);
		m_pInsScene->addItem(pItemInstruction);
	}
	setScene(m_pInsScene);
	pItemInstruction->setZValue(0.0);
	FitInstructionInView();
}

void GInstructionView::FitInstructionInView()
{
	if(!m_pInstruction || !m_pInsScene)
		return;
//	setSceneRect(m_pInstruction->BoundingSceneRect());
	fitInView(&m_pInstruction->m_GraphicsItem);

	QRectF SceneRect = sceneRect();
	if(!SceneRect.isValid())
		return;
	// we don't want blank areas at the beginning or at the ending of the G_old_Instruction in the QGraphicsView
	// so here is what we are going to do: we will see what is the viewport coordinate in the scene,...
	// then, we will add 1 or so, and change the scene coordinate so that it corresponds to the beginning/ending of the G_old_Instruction
	fitInView(SceneRect);
	QRect geomView = geometry();
	int Xini = -1;
	int Xfin = geomView.width() - 3;
	if(Xfin <= 2)
		return;
	double Xiniscene = mapToScene(Xini, 0).rx();
	double Xfinscene = mapToScene(Xfin, 0).rx() - m_pInstruction->Duration();
	SceneRect.adjust(- Xiniscene, 0, - Xfinscene, 0);
	setSceneRect(SceneRect);
	fitInView(SceneRect);
}

void GInstructionView::mouseMoveEvent( QMouseEvent * e )
{
return;
}
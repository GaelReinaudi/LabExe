#include "GSequenceGraphicsView.h"
#include "GSequenceGraphicsScene.h"
#include "GSequence.h"

GSequenceGraphicsView::GSequenceGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

GSequenceGraphicsView::~GSequenceGraphicsView()
{

}

void GSequenceGraphicsView::resizeEvent( QResizeEvent * event )
{
	GSequenceGraphicsScene* pScene = qobject_cast<GSequenceGraphicsScene*>(scene());
	if(!pScene)
		return;
	GSequence* pSeq = qobject_cast<GSequence*>(pScene->parent());
	if(!pSeq)
		return;
	QRectF rectToFit = pScene->sceneRect();
	rectToFit.setLeft(0.0);
	rectToFit.setRight(pSeq->Length());
// 	// if this is a GSequenceGraphicsScene, we laterally fit the whole sequence
// 	GSequenceGraphicsScene* pSeqScene = qobject_cast<GSequenceGraphicsScene*>(pScene);
// 	if(pSeqScene)
// 		rectToFit.setRight(pSeqScene->m_pSeq->Length());

	fitInView(rectToFit);
	QGraphicsView::resizeEvent(event);
}
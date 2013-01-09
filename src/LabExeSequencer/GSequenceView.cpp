#include "GSequenceView.h"
#include "Sequence/GEvScene.h"
#include "Sequence/GSequence.h"

GSequenceView::GSequenceView(QWidget *parent)
	: QGraphicsView(parent)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

GSequenceView::~GSequenceView()
{

}

void GSequenceView::resizeEvent( QResizeEvent * event )
{
	QGraphicsView::resizeEvent(event);
	GEvScene* pScene = qobject_cast<GEvScene*>(scene());
	if(!pScene)
		return;
	GSequence* pSeq = qobject_cast<GSequence*>(pScene->parent());
	if(!pSeq)
		return;
	QRectF rectToFit = pScene->sceneRect();
	rectToFit.setLeft(0.0);
	rectToFit.setRight(pSeq->Length());
// 	// if this is a GEvScene, we laterally fit the whole sequence
// 	GEvScene* pSeqScene = qobject_cast<GEvScene*>(pScene);
// 	if(pSeqScene)
// 		rectToFit.setRight(pSeqScene->m_pSeq->Length());

	fitInView(rectToFit);
}

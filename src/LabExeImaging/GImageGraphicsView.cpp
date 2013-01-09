#include "GImageGraphicsView.h"
#include "GImageGraphicsScene.h"

GImageGraphicsView::GImageGraphicsView(GImageGraphicsScene* pScene, QWidget *parent)
	: QGraphicsView(pScene, parent)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

GImageGraphicsView::GImageGraphicsView( QWidget *parent )
	: QGraphicsView(parent)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

GImageGraphicsView::~GImageGraphicsView()
{

}

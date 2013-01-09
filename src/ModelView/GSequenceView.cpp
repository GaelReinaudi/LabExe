#include "GSequenceView.h"
#include <QGraphicsEllipseItem>
#include "GSequenceModel.h"

GSequenceView::GSequenceView(QWidget *parent)
	: QAbstractItemView(parent), m_GraphicsView(this)
{
	m_GraphicsView.resize(size());
	m_GraphicsView.setBackgroundBrush(Qt::yellow);
	m_GraphicsView.setAlignment(Qt::AlignLeft);

	QGraphicsScene* pscccc = new QGraphicsScene(this);
	m_GraphicsView.setScene(pscccc);
	QGraphicsItem* pell = pscccc->addEllipse(200, 100, 60, 40);
	pscccc->addRect(pell->boundingRect());
}

GSequenceView::~GSequenceView()
{

}

void GSequenceView::setModel( GSequenceModel * model )
{
	m_GraphicsView.setScene(model->Scene());
//	QAbstractItemView::setModel(model);
	m_GraphicsView.fitInView(-0.01, -10, 0.1, 20);
}
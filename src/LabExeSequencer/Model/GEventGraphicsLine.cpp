#include "GEventGraphicsLine.h"
#include "GSequenceEventItem.h"

GEventGraphicsLine::GEventGraphicsLine(GSequenceEventItem* pEvent, QGraphicsItem* parent)
	: QGraphicsLineItem(parent)
{
	setLine(0.0, 0.0, 0.0, 100.0);
	QGraphicsProxyWidget* pGrWid = new QGraphicsProxyWidget(this);
	pGrWid->setWidget(pEvent->m_Delay->ProvideNewParamSpinBox(0));

	double relXpos = pEvent->m_Delay->DoubleValue();
	setX(relXpos);

	connect(pEvent->m_Delay, SIGNAL(ValueUpdated(double)), this, SLOT(SetX(double)));
}

GEventGraphicsLine::~GEventGraphicsLine()
{

}

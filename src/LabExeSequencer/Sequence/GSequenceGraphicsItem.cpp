#include "GSequenceGraphicsItem.h"
#include "GSequence.h"
#include "GSynchEvent.h"


GSequenceGraphicsItem::GSequenceGraphicsItem( GSequence* pSeq, QGraphicsItem* pParIt ) 
	: QGraphicsRectItem(0.0, 0.0, 1000.0, 500.0)
	, m_pSeq(pSeq)
{
	connect(pSeq, SIGNAL(ChildAgentAdded(GAgent*)), this, SLOT(Event_ChildAgentAdded(GAgent*)));

	m_pSeq->RootTimeEvent()->ProvideNewAgentWrappingItem(this);
}

GSequenceGraphicsItem::~GSequenceGraphicsItem()
{

}

void GSequenceGraphicsItem::Event_ChildAgentAdded( GAgent* pAgentAdded )
{
	qDebug() << "Event_ChildAgentAdded" << pAgentAdded;
	pAgentAdded->ProvideNewAgentWrappingItem(this);
}
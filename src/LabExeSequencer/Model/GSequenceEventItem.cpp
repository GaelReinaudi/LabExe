#include "GSequenceEventItem.h"
#include "GEventGraphicsLine.h"

#include <QStandardItem>
#include <QGraphicsLineItem>

GSequenceEventItem::GSequenceEventItem()
	: QStandardItem()
	, m_Delay(new GParamDouble("delay", this))
{
	static int i = 0;
	setText(QString("brrrrrr!%1").arg(i++));
}

GSequenceEventItem::~GSequenceEventItem()
{

}

QGraphicsItem* GSequenceEventItem::ProvideGraphicsItem(QGraphicsItem* parent) const
{
	GSequenceEventItem* const pConstEvent = const_cast<GSequenceEventItem* const>(this);
	QGraphicsItem* pLine = new GEventGraphicsLine(pConstEvent, parent);
	pLine->setFlag(QGraphicsItem::ItemIsMovable);
	return pLine;
}

QList<GSequenceEventItem*> GSequenceEventItem::ChildItems(int inCol) const
{
	QList<GSequenceEventItem*> listRet;
	int nR = rowCount();
	int nC = columnCount();
	if(inCol < nC) {
		for(int i = 0; i < nR ; i++) {
			listRet.append(static_cast<GSequenceEventItem*>(child(i, inCol)));
		}
	}
	return listRet;
}

QList<GSequenceEventItem*> GSequenceEventItem::ChildItems() const
{
	QList<GSequenceEventItem*> listRet;
	for(int c = 0; c < columnCount() ; c++) {
		listRet.append(ChildItems(c));
	}
	return listRet;
}
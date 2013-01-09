#include "GEventGraphicsView.h"
#include "../Sequence/GSequence.h"
#include "GSequenceEventItem.h"

#include <QGraphicsLineItem>
#include <QDebug>

GEventGraphicsView::GEventGraphicsView(QWidget *parent)
	: QAbstractItemView(parent)
	, m_pScene(new QGraphicsScene(this))
	, m_pView(new QGraphicsView(m_pScene, this))
	, m_pRootLine(new QGraphicsLineItem(0.0, 0.0, 0.0, 1000.0))
{
	setViewport(m_pView);
	m_pScene->addItem(m_pRootLine);

}

GEventGraphicsView::~GEventGraphicsView()
{

}

void GEventGraphicsView::setModel( QAbstractItemModel * model )
{
	QAbstractItemView::setModel(model);
	m_pSeq = qobject_cast<GSequence*>(model);

	m_Std_Line.insert(static_cast<GSequenceEventItem*>(m_pSeq->invisibleRootItem()), m_pRootLine);
	m_Std_Line.insert(0, m_pRootLine);
}

void GEventGraphicsView::rowsInserted( const QModelIndex & parentIndex, int start, int end )
{
	QAbstractItemView::rowsInserted(parentIndex, start, end);

	// get the parent standard item from the model
	QStandardItem* parentSeqItem = m_pSeq->ItemFromIndex(parentIndex);
	// if not valid index, we use the root item
	if(!parentSeqItem) {
		parentSeqItem = m_pSeq->invisibleRootItem();
	}
// 	qDebug() << "parent std" << parentSeqItem << (pParentLine == m_pRootLine ? "root" : "");

	// for each of those rows, we add the new line.
	// if there is no standard item in the model, it means that dataChanged will be called soon with this missing info
	for(int i = start; i <= end ; i++) {
		// if no child item, bye now
		GSequenceEventItem* childStdItem = static_cast<GSequenceEventItem*>(parentSeqItem->child(i, 0));
 		if(!childStdItem) {
// 			qDebug() << "waiting dataChanged() for" << parentIndex.child(i, 0);
			continue;
 		}
		// else 
		// if the line is not in the hash already, we make it and insert it
		QGraphicsItem* pLine = GetMakeGraphLine(childStdItem);
	}
}

void GEventGraphicsView::dataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
	QAbstractItemView::dataChanged(topLeft, bottomRight);
	if(topLeft != bottomRight) {
		Q_ASSERT(false);
	}
//	// in a drag drop operation, it seems that the item that changed is the one being dropped after it creates the QStandardOtem.
//	qDebug() << "change in parent" << topLeft.parent();
//	qDebug() << "changed" << topLeft;

	QStandardItem* parentSeqItem = m_pSeq->ItemFromIndex(topLeft.parent());
	// if not valid index, we use the root item
	if(!parentSeqItem) {
		parentSeqItem = m_pSeq->invisibleRootItem();
	}

	for(int i = topLeft.row(); i <= bottomRight.row() ; i++) {
		for(int j = topLeft.column(); j <= bottomRight.column() ; j++) {
			QModelIndex & index = topLeft.sibling(i, j);
			GSequenceEventItem* childStdItem = static_cast<GSequenceEventItem*>(parentSeqItem->child(i, j));
			if(childStdItem) {
				// if the line is not in the hash already, we make it and insert it
				QGraphicsItem* pLine = GetMakeGraphLine(childStdItem);
			}
		}
// 		qDebug() << "row" << i << static_cast<QStandardItem*>(childStdItem);
	}
}

void GEventGraphicsView::rowsAboutToBeRemoved( const QModelIndex & parentIndex, int start, int end )
{
	// for each of those rows, we remove the item
	for(int i = start; i <= end ; i++) {
		QModelIndex childIndex = parentIndex.isValid() ? parentIndex.child(start, 0) : m_pSeq->index(start, 0);
		GSequenceEventItem* childStdItem = m_pSeq->ItemFromIndex(childIndex);
		// remove the item and all the child items too
		QGraphicsItem* pLine = TakeItemAndDescendence(childStdItem);
		if(pLine) {
			m_pScene->removeItem(pLine);
 			delete pLine;
		}
	}
// 	qDebug() << "items in hash" << m_Std_Line.count();
}

QGraphicsItem* GEventGraphicsView::GetMakeGraphLine( GSequenceEventItem* seqItem )
{
	QGraphicsItem* pIt = GraphLine(seqItem);
	if(!pIt) {
		// if not, we request a new one from the seqItem and gives it the parent's line which may, recursively, lead to making  all the needed lines.
		QGraphicsItem* parentLine = GetMakeGraphLine(seqItem->ParentItem());
		pIt = seqItem->ProvideGraphicsItem(parentLine);
		m_Std_Line.insert(seqItem, pIt);
		// checks for the children of seqItem to recursively make the graphics line if needed.
		if(true) {
			foreach(GSequenceEventItem* pChIt, seqItem->ChildItems()) {
				GetMakeGraphLine(pChIt);
			}
		}
// 		qDebug() << "items in hash" << m_Std_Line.count();
	}
	return pIt;
}

QGraphicsItem* GEventGraphicsView::TakeItemAndDescendence( GSequenceEventItem* seqItem )
{
	QGraphicsItem* pIt = m_Std_Line.take(seqItem);
	foreach(GSequenceEventItem* pChIt, seqItem->ChildItems()) {
		TakeItemAndDescendence(pChIt);
	}
	return pIt;
}
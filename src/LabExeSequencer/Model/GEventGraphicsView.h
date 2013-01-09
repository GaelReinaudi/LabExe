#ifndef GEVENTGRAPHICSVIEW_H
#define GEVENTGRAPHICSVIEW_H

#include <QAbstractItemView>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPersistentModelIndex>
#include <QStandardItem>

class GSequence;
class GSequenceEventItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GSequenceScene class is a QGraphicsScene made to ease the display of a GnewSequence in  a GSequenceGraphicsView. 
/*!

http://stackoverflow.com/questions/1004701/model-view-controler-in-qt

http://stackoverflow.com/questions/3188584/how-to-use-qt-model-view-framework-with-the-graphics-view-framework

http://invalidmagic.wordpress.com/2010/10/05/qgraphicsscene-used-as-a-qabstractitemview-iii

*/
class GEventGraphicsView : public QAbstractItemView
{
	Q_OBJECT

public:
	GEventGraphicsView(QWidget *parent);
	~GEventGraphicsView();

	//! Gets the graphics item that corresponds to the sequence item. Will recursively request/make the one needed for parenting the line.
	QGraphicsItem* GraphLine(GSequenceEventItem* seqItem) const {
		QGraphicsItem* pIt = m_Std_Line.value(seqItem, 0);
		return pIt;
	}
	//! Requests a new line item from the seqItem. Will recursively request/make the one needed for parenting the line.
	QGraphicsItem* GetMakeGraphLine(GSequenceEventItem* seqItem);

	//! re-implemented to keep a pointer to the Sequence (model)
	void setModel(QAbstractItemModel * model);

protected:
	QRect visualRect(const QModelIndex & index) const {return QRect();}
	void scrollTo(const QModelIndex & index, QAbstractItemView::ScrollHint hint) {}
	QModelIndex indexAt(const QPoint & pt) const {return QModelIndex();}
	QModelIndex moveCursor(QAbstractItemView::CursorAction action, Qt::KeyboardModifiers mod) {return QModelIndex();}
	int horizontalOffset(void) const {return 0;}
	int verticalOffset(void) const {return 0;}
	bool isIndexHidden(const QModelIndex & index) const {return false;}
	void setSelection(const QRect & rect, QItemSelectionModel::SelectionFlags flags) {}
	QRegion visualRegionForSelection(const QItemSelection & sel) const {return QRegion();}

protected:
	void rowsInserted(const QModelIndex & parentIndex, int start, int end);
	void rowsAboutToBeRemoved(const QModelIndex & parentIndex, int start, int end);
	void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
	QGraphicsItem* TakeItemAndDescendence( GSequenceEventItem* seqItem );

private:
	QGraphicsScene* m_pScene;
	QGraphicsItem* m_pRootLine;
	QGraphicsView* m_pView;
	GSequence* m_pSeq;
	QHash<GSequenceEventItem*, QGraphicsItem*> m_Std_Line;
};

#endif // GEVENTGRAPHICSVIEW_H

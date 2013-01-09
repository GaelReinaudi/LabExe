#ifndef GSEQUENCEVIEW_H
#define GSEQUENCEVIEW_H

#include <QAbstractItemView>
#include <QGraphicsView>
class GSequenceModel;

class GSequenceView : public QAbstractItemView
{
//	Q_OBJECT

public:
	GSequenceView(QWidget *parent);
	~GSequenceView();

	void setModel( GSequenceModel * model );

protected:
	int horizontalOffset () const {return 0;}
	QModelIndex indexAt ( const QPoint & point ) const {return QModelIndex();}
	bool isIndexHidden ( const QModelIndex & index ) const {return 0;}
	QModelIndex moveCursor ( CursorAction cursorAction, Qt::KeyboardModifiers modifiers ) {return QModelIndex();}
	void scrollTo ( const QModelIndex & index, ScrollHint hint = EnsureVisible ) {}
	void setSelection ( const QRect & rect, QItemSelectionModel::SelectionFlags flags ) {}
	int verticalOffset () const {return 0;}
	QRect visualRect ( const QModelIndex & index ) const {return QRect();}
	QRegion visualRegionForSelection ( const QItemSelection & selection ) const {return QRegion();}

protected:
	void resizeEvent ( QResizeEvent * event ) {m_GraphicsView.resize(size());}

private:
	QGraphicsView m_GraphicsView;
	
};

#endif // GSEQUENCEVIEW_H

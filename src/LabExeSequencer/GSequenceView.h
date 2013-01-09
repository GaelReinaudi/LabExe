#ifndef GSequenceView_H
#define GSequenceView_H

#include <QGraphicsView>

/////////////////////////////////////////////////////////////////////
//! \brief The GSequenceView class is a QGraphicsView made to ease the display of a GSequenceGraphicsScene. 
/*!
*/
class GSequenceView : public QGraphicsView
{
//	Q_OBJECT

public:
	GSequenceView(QWidget *parent);
	~GSequenceView();

protected:
	//! Reimplemented to fit the sequence (or rather the sceneRect) in the view.
	virtual void resizeEvent( QResizeEvent * event );

private:
	
};

#endif // GSequenceView_H

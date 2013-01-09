#ifndef GSequenceGraphicsView_H
#define GSequenceGraphicsView_H
#include "labexe_global.h"

#include <QGraphicsView>

/////////////////////////////////////////////////////////////////////
//! \brief The GSequenceGraphicsView class is a QGraphicsView made to ease the display of a GSequenceGraphicsScene. 
/*!
*/
class LABEXE_EXPORT GSequenceGraphicsView : public QGraphicsView
{
//	Q_OBJECT

public:
	GSequenceGraphicsView(QWidget *parent);
	~GSequenceGraphicsView();

protected:
	//! Reimplemented to fit the sequence (or rather the sceneRect) in the view.
	virtual void resizeEvent( QResizeEvent * event );

private:
	
};

#endif // GSequenceGraphicsView_H

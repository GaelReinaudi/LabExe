#ifndef GSEQUENCEGRAPHICSSCENE_H
#define GSEQUENCEGRAPHICSSCENE_H
#include "labexe_global.h"

#include <QtGui>

class GSequence;
class GChannel;

/////////////////////////////////////////////////////////////////////
//! \brief The GSequenceGraphicsScene class is a QGraphicsScene made to ease the display of a GSequence in  a GSequenceGraphicsView. 
/*!
*/
class LABEXE_EXPORT GSequenceGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GSequenceGraphicsScene(GSequence *parentSeq);
	~GSequenceGraphicsScene();


protected:
	//! Reimplemented 
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent );
	//! Reimplemented from QGraphicsItem. 
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:
	//! emitted to give the position of the mouse over the scene.
	void HoverInScene(QPointF pointHover);

public slots:
 	//! Set the sceneRect right() position. Used to have the scene always matching the Sequence Length().
 	void UpdateLength(double newLength);

public:
	//! Will insert the Channel RectItem in the scene m_pChanLay.
	void InsertChannelItem(GChannel* pNewChan);

private:
	//! the parent sequence, that is represented in this scene
	GSequence* m_pSeq;
	//! the layout for positioning the channels vertically
	QGraphicsLinearLayout* m_pChanLay;

//	friend class GSequenceGraphicsView;
};

#endif // GSEQUENCEGRAPHICSSCENE_H

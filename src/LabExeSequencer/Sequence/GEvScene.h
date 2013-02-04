#ifndef GSequenceScene_H
#define GSequenceScene_H

#include <QtGui>
#include "../../MapExe/src/SceneView/GMapGraphicsScene.h"

class GSequence;
class GChannel;
class GSynchEvent;

/////////////////////////////////////////////////////////////////////
//! \brief The GEvScene class is a QGraphicsScene made to ease the display of a GSequence in  a GSequenceGraphicsView. 
/*!
*/
class GEvScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GEvScene(GSequence *parentSeq);
	~GEvScene();


protected:
	//! Reimplemented 
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent );
	//! Re-implemented
	void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
	//! Re-implemented
	void dragLeaveEvent(QGraphicsSceneDragDropEvent * event);
	//! Re-implemented
	void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
	//! Re-implemented
	void dropEvent(QGraphicsSceneDragDropEvent * event);

signals:
	//! emitted to give the position of the mouse over the scene.
	void HoverInScene(QPointF pointHover);

public slots:
 	//! Set the sceneRect right() position. Used to have the scene always matching the Sequence Length().
 	void UpdateLength(double newLength);

public:
	//! Will insert the Channel RectItem in the scene m_pChannelLayout.
	void InsertChannelItem(GChannel* pNewChan);

private:
	//! the parent sequence, that is represented in this scene
	GSequence* m_pSeq;
	//! the layout for positioning the channels vertically
	QGraphicsLinearLayout* m_pChannelLayout;
public:
	//! This is the root event of the sequence, the one that defines t = 0. It is at the top of the instruction tree.
	GSynchEvent* m_pRootEvent;

	friend class GSynchEvent;
};

#endif // GSequenceScene_H

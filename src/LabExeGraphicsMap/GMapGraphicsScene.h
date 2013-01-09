#ifndef GMAPGRAPHICSSCENE_H
#define GMAPGRAPHICSSCENE_H

#include <QGraphicsScene>

/////////////////////////////////////////////////////////////////////
//! \brief The GMapGraphicsScene class inherits QGraphicsScene to provide a convenient interaction with the agent based structure.
/*!
*/
class GMapGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GMapGraphicsScene(QObject *parent);
	~GMapGraphicsScene();

protected:
	//! Re-implemented
	void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
	//! Re-implemented
	void dragLeaveEvent(QGraphicsSceneDragDropEvent * event);
	//! Re-implemented
	void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
	//! Re-implemented
	void dropEvent (QGraphicsSceneDragDropEvent * event);
private:
	
};

#endif // GMAPGRAPHICSSCENE_H

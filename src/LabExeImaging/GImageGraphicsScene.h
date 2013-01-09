#ifndef GIMAGEGRAPHICSSCENE_H
#define GIMAGEGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "GImageDouble.h"
// class GImageProcessor;

/////////////////////////////////////////////////////////////////////
//! \brief A GImageGraphicsScene is a QGraphicsScene that is used to display an image and to perform some drawing over it.
/*!
The image typically comes from a camera or a file. the drawing over it could be the result of a fit.

For now, the image is displayed using the Background of the scene.
It is meant to work with the GImageGraphicsView class.
*/
class GImageGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GImageGraphicsScene(QObject *parent);
	~GImageGraphicsScene();

protected:
	//! Re-implemeted
	void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);

protected slots:
	//! Displays the incoming picture
	void SetImage(QImage image);
	//! Displays the incoming picture
	void SetImage(GImageDouble image);

private:
// 	//! The object that listen for incoming picture. The picture will be diplayed in the scene
// 	GImageProcessor* m_pImageGetter;
	//! the image currently displayed
	GImageDouble m_CurrentImage;
	
};

#endif // GIMAGEGRAPHICSSCENE_H

#ifndef GIMAGEGRAPHICSVIEW_H
#define GIMAGEGRAPHICSVIEW_H

#include <QGraphicsView>
class GImageGraphicsScene;

/////////////////////////////////////////////////////////////////////
//! \brief A GImageGraphicsView is a QGraphicsView that is used to display an image and to perform some drawing over it.
/*!
The image typically comes from a camera or a file. the drawing over it could be the result of a fit.

For now, the image is displayed using the Background of the scene.
It is meant to work with the GImageGraphicsScene class.
*/
class GImageGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	GImageGraphicsView(QWidget *parent);
	GImageGraphicsView(GImageGraphicsScene* pScene, QWidget *parent);
	~GImageGraphicsView();

private:
	
};

#endif // GIMAGEGRAPHICSVIEW_H

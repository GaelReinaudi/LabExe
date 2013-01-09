#ifndef GMAPGRAPHICSVIEW_H
#define GMAPGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QTime>
#include <QDebug>
#include <math.h>

/////////////////////////////////////////////////////////////////////
//! \brief The GMapGraphicsScene class inherits QGraphicsView to provide a convenient interaction with the agent based structure.
/*!
*/
class GMapGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	GMapGraphicsView(QWidget *parent);
	~GMapGraphicsView();

protected:
	//! Re-implemented
    void wheelEvent(QWheelEvent *event);
	//! Re-implemented
	void mouseDoubleClickEvent(QMouseEvent* event);
	//! Re-implemented
	void drawBackground (QPainter * painter, const QRectF & rect);

protected slots:
	//! updates the size of the view when the scene rect has changed. We make the view rect bigger so that it is easier to zoom on the edges.
    void UpdateFromSceneRect(QRectF newSceneRect);

private:
    void scaleView(qreal scaleFactor);

private:
	//! background image
	QPixmap m_BackgroundImage;
	
};


/////////////////////////////////////////////////////////////////////
//! \brief The FPSTracker should help tracking the time spent on painting the view for our optimization purposes.
/*!
*/
class FPSTracker : public QObject
{
public:
	FPSTracker(QObject* pParent) : QObject(pParent)
	{
		m_time.start();
		m_frames = 0;
	}

	bool eventFilter(QObject *object, QEvent *e) {
		if(e->type() == QEvent::Paint) {
			++m_frames;
			if(m_time.elapsed() > 2000) {
// 				qDebug() << QString("Average ms pr frame: %1, %2 frames in %3 ms").arg(m_time.elapsed() / (qreal) m_frames).arg(m_frames).arg(m_time.elapsed());
				m_time.start();
				m_frames = 0;
			}
		}
		return QObject::eventFilter(object, e);
	}

private:
	QTime m_time;
	bool m_started;
	int m_frames;
};

#endif // GMAPGRAPHICSVIEW_H

#ifndef GAGENTGRAPHICSITEM_H
#define GAGENTGRAPHICSITEM_H
#include "labexegraphicsmap_global.h"

#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QPropertyAnimation>
class GAgent;

int rand255();
QColor randRGB();

//! Typical size used to scale all of the items to a reference.
static QSizeF TypicalScreen(800, 600);
static double TypicalSpacingOnScreen(100.0);


/////////////////////////////////////////////////////////////////////
//! \brief The GAgentGraphicsItem class is the base class that defines a graphics item for representing a GAgent object.
/*!
*/
class LABEXEGRAPHICSMAP_EXPORT GAgentGraphicsItem : public QGraphicsSvgItem // QObject, public QGraphicsRectItem //
{
	Q_OBJECT

public:
    GAgentGraphicsItem(GAgent* pAgent, QGraphicsItem *parentItem = 0);
	~GAgentGraphicsItem();

	//! Returns the agent of this item
	GAgent* Agent() const { return m_pAgent; }
	//! Returns the parent item GAgentGraphicsItem of this item. 0 if none.
	GAgentGraphicsItem* ParentAgentGraphicsItem() const;
	//! Returns direct children GAgentGraphicsItem of this item.
	QList<GAgentGraphicsItem*> ChildAgentGraphicsItems() const;
	//! Returns the siblings GAgentGraphicsItem of this item.
	QSet<GAgentGraphicsItem*> SiblingAgentGraphicsItems();

	//! TEST function that would set the size of the rect OR SVG item
	void SetRect(const QRectF & rectangle);
	//! Overload that sets the size. The top-left corner is not moved.
	void SetRect(const QSizeF & size);
	void SetBrush(const QBrush & val) { m_Brush = val; }

	//! returns the rect that should bound our item
	QRectF boundingRect() const {return m_Rect;}
	//! Re-implemented to paint a simple rect by default
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

public slots:
	//! Starts an animation of the position to the newPos. Will emit FinishedAnimation() when finished. The returned QPropertyAnimation is parented to this item and will destroy itself upon finishing.
	QPropertyAnimation* AnimateMoveTo(QPointF newPos, int msecs);
	//! Starts an animation of the position by moving by moveBy. Will emit FinishedAnimation() when finished. The returned QPropertyAnimation is parented to this item and will destroy itself upon finishing.
	QPropertyAnimation* AnimateMoveBy(QPointF moveBy, int msecs);
	//! Moves to the newPos. 
	void MoveTo(QPointF newPos);

public:// public for the time being because of a bug in Qt 4 that prevents the QGraphicsItem::mouseDoubleClickEvent() from being called directly.
	//! Re-implemented
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
protected:
	//! Changes the opacity of the brush when painting
	void ChangeBrushOpacity( double opacity );


protected slots:
	//! Resizes the item so that it encompasses its children
	void EncompassChildren();

signals:
	//! Emitted when a graphics item of a Agent()'s child agent has been added to this item as a child. The remainingItemsToBeAdded may give an information on how many items are still going to be very very soon (e.g. How many remaining subfolders and files are being discovered/added as child of a given folder item).
	void ChildAgentGraphicsItemAdded(GAgentGraphicsItem* pAddedItem, int remainingItemsToBeAdded = 0);
	//! Emitted when a moving animation AnimateMoveTo() has finished.
	void FinishedAnimation();
	//! Emitted when the surface defined grew by more than a certain amount since the last time this signal was emitted
	void RectGrewNoticably(const QRectF & newRect);

private slots:
	//! Slots that reacts to a GAgent::ChildAgentAdded() signal with a Qt::QueuedConnection.
	void AgentEvent_ChildAgentAdded(GAgent* pAgent);
	//! Increment a counter representing the number of items that are to be added at the same time, before the actual item are created. This can help for e.g. the placement of groups of item
	void AgentEvent_IncrementAgentCounterForBatchAdding();
    //! Slots that reacts to a GAgent::UnParent() signal with a Qt::QueuedConnection.
    void AgentEvent_UnParent();

private:
	GAgent* m_pAgent;
	//! test of a member rect so that we can paint a simple rect instead of a svg.
	QRectF m_Rect;
	//! test of a member rect so that we can paint a simple rect instead of a svg.
	QBrush m_Brush;
	//! test of a member rect so that we can paint a simple rect instead of a svg.
	QPen m_Pen;
	//! Counter of items being added at the same time. See AgentEvent_IncrementAgentCounterForBatchAdding().
	int m_ItemsToBeAdded;
	//! surface when the signal RectGrewNoticably() was last emitted
	QRectF m_LastEmittedRectChange;

protected:
	//! For efficiency, this variable holds the surface of the boudingRectangle() so that it is not recomputed all the time in paint().
	double m_Surface;
	//! For efficiency, this variable holds an approximation of the space used in a 800x450 screen so that it is not recomputed all the time in paint().
	double m_ApproxPercentOfScreenOnScale1;

public:
	static QSvgRenderer m_StaticRenderer;

};

#endif // GAGENTGRAPHICSITEM_H

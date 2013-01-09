#include "GFireworksPlacerAgent.h"
#include <QVector2D>
#include <qmath.h>

GFireworksPlacerAgent::GFireworksPlacerAgent(QObject *parent)
	: GPlacerAgent(parent)
{
}

GFireworksPlacerAgent::~GFireworksPlacerAgent()
{
    terminate();
}

void GFireworksPlacerAgent::AddItemToPlace( GAgentGraphicsItem* pItemToPlace, int remainingItemsToBeAdded /*= 0*/ )
{
	GPlacerAgent::AddItemToPlace(pItemToPlace, remainingItemsToBeAdded);
	connect(pItemToPlace, SIGNAL(RectGrewNoticably(QRectF)), this, SLOT(Fire()), Qt::QueuedConnection);
	connect(pItemToPlace, SIGNAL(ChildAgentGraphicsItemAdded(GAgentGraphicsItem*)), this, SLOT(Fire()), Qt::QueuedConnection);
}

void GFireworksPlacerAgent::Fire()
{
	GAgentGraphicsItem* pFireItem = qobject_cast<GAgentGraphicsItem*>(sender());
	if(!pFireItem)
		return;
	GAgentGraphicsItem* pParentItem = pFireItem->ParentAgentGraphicsItem();
	//     if(!pParentItem)
	//         return;
	// disconnect so that we don't fire an item again and again as it grows while it is fired
	disconnect(pFireItem, SIGNAL(RectGrewNoticably(QRectF)), this, SLOT(Fire()));
	disconnect(pFireItem, SIGNAL(ChildAgentGraphicsItemAdded(GAgentGraphicsItem*)), this, SLOT(Fire()));

	// direction for firing
	QPointF fireDirection(pFireItem->pos());
	if(pParentItem)
		fireDirection += pParentItem->pos() - pParentItem->boundingRect().center();

	QLineF parentPosLine(QPointF(0,0), fireDirection);
	parentPosLine.setAngle(parentPosLine.angle()+(double(qrand()-qrand()))/double(RAND_MAX) * 60);

	QVector2D fireVector(parentPosLine.x2(),parentPosLine.y2());
	fireVector.normalize();
	QPointF unitFirePoint(fireVector.toPointF());

	double ManhattanParentRect = qSqrt(pFireItem->SiblingAgentGraphicsItems().count());
	double ManhattanItemRect = qSqrt(pFireItem->ChildAgentGraphicsItems().count());
	double randDist = 2.0 + qrand() % 2;
	double fireLength = TypicalScreen.width() * randDist * (ManhattanParentRect + ManhattanItemRect) * (ManhattanParentRect <= 1.1 ? 0.2 : 1.0);
	QPointF firePoint(fireLength*unitFirePoint);
	firePoint -= pFireItem->boundingRect().center();

	//! Needs to be called at a better time
	pFireItem->Agent()->UnParent();

	QPropertyAnimation* pAnim = pFireItem->AnimateMoveBy(firePoint, 500);
}




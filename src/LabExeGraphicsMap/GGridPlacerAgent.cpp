#include "GGridPlacerAgent.h"
#include <qmath.h>

GGridPlacerAgent::GGridPlacerAgent(QObject *parent) 
	: GPlacerAgent(parent)
	, m_TotalItems(0)
{
}

GGridPlacerAgent::~GGridPlacerAgent()
{
}

void GGridPlacerAgent::AddItemToPlace(GAgentGraphicsItem* pItem, int remainingItemsToBeAdded /*= 0*/)
{
	if(!pItem)
		return;
	GAgentGraphicsItem* pParent = pItem->ParentAgentGraphicsItem();
	if(!pParent)
		return;

	if(m_TotalItems == 0) {
		m_TotalItems += remainingItemsToBeAdded + 1;
		m_NumColumns = qSqrt(m_TotalItems) + 1;
	}
	int AlreadyPlaced = m_TotalItems - remainingItemsToBeAdded - 1;
	// ???????
	double spacing = TypicalSpacingOnScreen;
	double stepX = TypicalScreen.width() + spacing;
	double stepY = TypicalScreen.height() + spacing;
	double x = AlreadyPlaced % m_NumColumns;
	double y = AlreadyPlaced / m_NumColumns;
	x *= stepX;
	y *= stepY;

	QPointF newPosition(x, y);

	QPropertyAnimation* pAnim = pItem->AnimateMoveTo(newPosition, 1000);

	// trick to update the size of parents only for one animation
	if(remainingItemsToBeAdded == 0) {
// 		connect(pAnim, SIGNAL(valueChanged(QVariant)), pParent, SLOT(EncompassChildren()));
		FinishedPlacingItems(pParent);
	}
}




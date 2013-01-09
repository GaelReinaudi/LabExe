#ifndef GFORCEPLACERAGENT_H
#define GFORCEPLACERAGENT_H

#include "GPlacerAgent.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GForcePlacerAgent class is a GAgent class that takes care of placing items by simulating forces between the items.
/*!
*/
class GForcePlacerAgent : public GPlacerAgent
{
	Q_OBJECT

public:
	GForcePlacerAgent(QObject *parent);
	~GForcePlacerAgent();

	//! Re-implemented 
	GAgentGraphicsItem* ProvideNewAgentGraphicsItem(GAgentGraphicsItem* pParentItem);

protected:
	//! Re-implemented from QThread.
	void run();
	//! Re-implemented.
	void AddItemToPlace(GAgentGraphicsItem* pItemToPlace, int remainingItemsToBeAdded = 0);

private:
	QVector2D Force( GAgentGraphicsItem* pItem );
	//! Returns the vector of the shortest move so that the two rect items are in direct contact. InflateRect1 makes the calculation behave as if the rect 1 was inflated in all direction by InflateRect1.
	QVector2D ShortestMoveForEdgeToEdge(GAgentGraphicsItem* pIt1ToMove, GAgentGraphicsItem* pIt2, double InflateRect1 = 0.0);
	
};

#endif // GFORCEPLACERAGENT_H

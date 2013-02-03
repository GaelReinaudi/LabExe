#ifndef GSEQUENCEGRAPHICSITEM_H
#define GSEQUENCEGRAPHICSITEM_H

// TRY BASALT
#include "../../../MapExe/src/agent.h"

class GSequence;
class QDeclarativeItem;
// TRY BASALT
class GSequenceGraphicsItem : public QObject, public QGraphicsRectItem 
{
	Q_OBJECT

public:
	GSequenceGraphicsItem(GSequence* pSeq, QGraphicsItem* pParIt);;
	~GSequenceGraphicsItem();

protected slots:
	void Event_ChildAgentAdded(GAgent* pAgentAdded);
	void AddQmlToScene();

signals:
	void aaaaaaaaaaaaaaaaaaa();

private:
	GSequence* m_pSeq;
	QDeclarativeItem* m_pQMLitem;
};

#endif // GSEQUENCEGRAPHICSITEM_H

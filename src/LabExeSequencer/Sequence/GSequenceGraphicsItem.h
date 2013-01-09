#ifndef GSEQUENCEGRAPHICSITEM_H
#define GSEQUENCEGRAPHICSITEM_H

// TRY BASALT
#include "../../../MapExe/src/agent.h"

class GSequence;
// TRY BASALT
class GSequenceGraphicsItem : public QObject, public QGraphicsRectItem 
{
	Q_OBJECT

public:
	GSequenceGraphicsItem(GSequence* pSeq, QGraphicsItem* pParIt);;
	~GSequenceGraphicsItem();

protected slots:
	void Event_ChildAgentAdded(GAgent* pAgentAdded);

signals:
	void a();

private:
	GSequence* m_pSeq;
};

#endif // GSEQUENCEGRAPHICSITEM_H

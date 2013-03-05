#ifndef GSEQUENCEGRAPHICSITEM_H
#define GSEQUENCEGRAPHICSITEM_H
#include <QObject>
#include <QGraphicsRectItem>

class GSequence;
class QDeclarativeItem;
class QDeclarativeEngine;

class GSequenceGraphicsItem : public QObject, public QGraphicsRectItem 
{
	Q_OBJECT

public:
	GSequenceGraphicsItem(GSequence* pSeq, QGraphicsItem* pParIt);;
	~GSequenceGraphicsItem();

protected slots:
	void AddQmlToScene();

signals:
	void aaaaaaaaaaaaaaaaaaa();

private:
	GSequence* m_pSeq;
	QDeclarativeItem* m_pQMLitem;
	QDeclarativeEngine* m_pEngine;
};

#endif // GSEQUENCEGRAPHICSITEM_H

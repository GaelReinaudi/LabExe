#ifndef GEVENTGRAPHICSLINE_H
#define GEVENTGRAPHICSLINE_H

#include <QGraphicsLineItem>
class GSequenceEventItem;

class GEventGraphicsLine : public QObject, public QGraphicsLineItem
{
	Q_OBJECT

public:
	GEventGraphicsLine(GSequenceEventItem* pEvent, QGraphicsItem* parent);
	~GEventGraphicsLine();

public slots:
	void SetX(double newX) {setX(newX);}

private:
	
};

#endif // GEVENTGRAPHICSLINE_H

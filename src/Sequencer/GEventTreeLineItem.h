#ifndef GEVENTTREELINEITEM_H
#define GEVENTTREELINEITEM_H
#include "labexe_global.h"

#include <QtGui>

class LABEXE_EXPORT GEventTreeLineItem : public QGraphicsLineItem
{
public:
	GEventTreeLineItem(QGraphicsItem* pParentItem);
	~GEventTreeLineItem();

	//! adjust the vertical size/position
	virtual void UpdateVerticalPosition();

protected:
	//! reimplemented from QGraphicsItem to inform of changes in the item properties. Adequate measures can be taken e.g. for maintaing vertical position.
	virtual	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	
};

#endif // GEVENTTREELINEITEM_H

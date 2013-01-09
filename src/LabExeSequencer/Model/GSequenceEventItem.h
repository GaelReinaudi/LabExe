#ifndef GSEQUENCEEVENTITEM_H
#define GSEQUENCEEVENTITEM_H

#include <QStandardItem>
#include <QObject>
#include "param.h"
class QGraphicsItem;

class GSequenceEventItem : public QObject, public QStandardItem
{
//	Q_OBJECT

public:
	GSequenceEventItem();
	~GSequenceEventItem();
	QGraphicsItem* ProvideGraphicsItem(QGraphicsItem* parent) const;
	//! returns the parent QStandardItem cast to a GSequenceEventItem or 0 if no parent.
	GSequenceEventItem* ParentItem() const { return static_cast<GSequenceEventItem*>(QStandardItem::parent()); }
	//! returns a list of existing child in the given column
	QList<GSequenceEventItem*> ChildItems(int inCol) const;
	//! returns a list of existing child in all the row/column
	QList<GSequenceEventItem*> ChildItems() const;

	//! Re-implemented
	QStandardItem* clone() const {
		GSequenceEventItem* pNewEv = new GSequenceEventItem();
		pNewEv->m_Delay = m_Delay;
		return pNewEv;
	}

//private:
	GParamDouble* m_Delay;

	friend class GEventGraphicsLine;
};

#endif // GSEQUENCEEVENTITEM_H

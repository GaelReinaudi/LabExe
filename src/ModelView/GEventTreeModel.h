#ifndef GEventTreeModel_H
#define GEventTreeModel_H

#include <QAbstractItemModel>
class G_old_TimeEvent;

enum ColumnData{ColumnDevice, ColumnEvent, ColumnDelay};

class GEventTreeModel : public QAbstractItemModel
{
public:
	GEventTreeModel(QObject *parent);
	~GEventTreeModel();

	void SetRootEvent(G_old_TimeEvent* pRootEvent);

	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
	G_old_TimeEvent* getItem(const QModelIndex &index) const;
	G_old_TimeEvent* m_pRootEvent;
};

#endif // GEventTreeModel_H

#include "GEventTreeModel.h"
#include "Instruction/G_old_Instruction.h"
#include "Device/GDevice.h"

GEventTreeModel::GEventTreeModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	m_pRootEvent = 0;
}

GEventTreeModel::~GEventTreeModel()
{

}

void GEventTreeModel::SetRootEvent( G_old_TimeEvent* pRootEvent )
{
	if(pRootEvent)
		m_pRootEvent = pRootEvent;
}

G_old_TimeEvent* GEventTreeModel::getItem(const QModelIndex &index) const
{
	if(index.isValid()) {
		G_old_TimeEvent* pEvent = static_cast<G_old_TimeEvent*>(index.internalPointer());
		if(pEvent) 
			return pEvent;
	}
	return m_pRootEvent;
}

QVariant GEventTreeModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	G_old_TimeEvent* pEvent = getItem(index);
	
	if(index.column() == ColumnDevice && pEvent->inherits("G_old_Instruction"))
		return (static_cast<G_old_Instruction*>(pEvent))->Device()->Name();
	if(index.column() == ColumnEvent)
		return "evvv";
	if(index.column() == ColumnDelay)
		return pEvent->DelayFromParent();
	return "ghj";
}

QVariant GEventTreeModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{
	if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		if(section == ColumnDelay)
			return "Delay";
		return "sdf";//m_RootEvent->data(section);
	}
	return QVariant();
}

QModelIndex GEventTreeModel::index( int row, int column, const QModelIndex &parent /*= QModelIndex()*/ ) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	G_old_TimeEvent* parentItem = getItem(parent);

	G_old_TimeEvent* childItem = parentItem->ChildEvent(row);
	if(childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex GEventTreeModel::parent( const QModelIndex &index ) const
{
	if(!index.isValid())
		return QModelIndex();

	G_old_TimeEvent* childItem = getItem(index);
	G_old_TimeEvent* parentItem = childItem->ParentEvent();

	if(parentItem == m_pRootEvent)
		return QModelIndex();

	return createIndex(parentItem->ThisChildNumber(), 0, parentItem);
}

int GEventTreeModel::rowCount( const QModelIndex &parent /*= QModelIndex()*/ ) const
{
	G_old_TimeEvent* parentItem = getItem(parent);

	return parentItem->childCount();
}

int GEventTreeModel::columnCount( const QModelIndex &parent /*= QModelIndex()*/ ) const
{
	if(parent.isValid())
		return 3;
	return 4;//m_RootEvent->columnCount();
}

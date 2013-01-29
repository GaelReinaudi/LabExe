#include <QtGui>
#include <QtXml>

#include "GSeqModel.h"
#include "../Sequence/GEventNode.h"
#include "../Sequence/GSynchEvent.h"

GSeqModel::GSeqModel(GSequence* pSequence, QObject *parent)
	: QAbstractItemModel(parent)
	, m_pSequence(pSequence)
{
// 	QFileSystemModel a(this);
// 	a.canFetchMore(QModelIndex());
}

GSeqModel::~GSeqModel()
{
}

int GSeqModel::columnCount(const QModelIndex & parent) const
{
	return 3;
}

QVariant GSeqModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	GEventNode* node = nodeForIndex(index);

	switch(index.column()) {
		 case 0:
			 return node->UniqueSystemID();
		 case 1:
			 return node->data();
		 case 2:
			 return rowForNode(node);
		 default:
			 return QVariant();
	}
}

Qt::ItemFlags GSeqModel::flags(const QModelIndex &index) const
{
// 	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);
//	theFlags |= Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	// drop-able
	theFlags |= Qt::ItemIsDropEnabled;

	// if no valid index
	if(!index.isValid()) {
		return theFlags;
	}
	// editable 
	theFlags |= Qt::ItemIsEditable;
	// drag-able
	theFlags |= Qt::ItemIsDragEnabled;

	return theFlags;
}

QVariant GSeqModel::headerData(int section, Qt::Orientation orientation,
							  int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			 case 0:
				 return tr("Name");
			 case 1:
				 return tr("Attributes");
			 case 2:
				 return tr("Value");
			 default:
				 return QVariant();
		}
	}

	return QVariant();
}

QModelIndex GSeqModel::index(int row, int column, const QModelIndex & parent) const
{
	if(!hasIndex(row, column, parent))
		return QModelIndex();

	GEventNode* parentNode = nodeForIndex(parent);
	GEventNode* childNode = parentNode->ChildNodes().at(row);
	return createIndex(row, column, childNode);
}

QModelIndex GSeqModel::parent(const QModelIndex & child) const
{
	GEventNode* childNode = nodeForIndex(child);
	GEventNode* parentNode = childNode->ParentNode();

	if(parentNode == m_pSequence->RootTimeEvent()) {
		return QModelIndex();
	}

	int row = rowForNode(parentNode);
	int col = 0;
	return createIndex(row, col, parentNode);
}

int GSeqModel::rowCount(const QModelIndex & parent) const
{
	GEventNode* parentNode = nodeForIndex(parent);
	return parentNode->ChildNodes().count();
}

QModelIndex GSeqModel::indexForNode(GEventNode* node) const
{
	if(node == m_pSequence->RootTimeEvent()) {
		return QModelIndex();
	}
	int row = rowForNode(node);
	int col = 0;
	QModelIndex theIndex = createIndex(row, col, node);
	qDebug() << "indexForNode = " << theIndex;
	return theIndex;
}

GEventNode* GSeqModel::nodeForIndex(const QModelIndex & index) const
{
	if(index.isValid()){
		return static_cast<GEventNode*>(index.internalPointer());
	}
	return m_pSequence->RootTimeEvent();
}

int GSeqModel::rowForNode(GEventNode* node) const
{
	return node->ParentNode()->ChildNodes().indexOf(node);
}

bool GSeqModel::setData( const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/ )
{
	if(!index.isValid() || role != Qt::EditRole) {
		return false;
	}
	GEventNode* childNode = nodeForIndex(index);
//	node->text = value.toString();
	qDebug() << index.column();
	qDebug() << value;
	QModelIndex topLeft = index;
	QModelIndex botRight = index;
	emit dataChanged(topLeft, botRight);
	return true;
}

void GSeqModel::insertNode( GEventNode* parentNode, int pos, GEventNode* node )
{
	QModelIndex parent = indexForNode(parentNode);

	const QList<GEventNode*> listEv = parentNode->ChildNodes();
	const int numEv = listEv.count();
	// if the pos is negative, we will just append the node at the end 
	if(pos < 0 || pos > numEv) {
		pos = numEv;
	}
	int firstRow = pos;
	int lastRow = pos;

	beginInsertRows(parent, firstRow, lastRow);
	parentNode->InsertChildEvent(node, pos);
	endInsertRows();
}

void GSeqModel::removeNode( GEventNode* node )
{
	GEventNode* parentNode = node->ParentNode();
	const QModelIndex parent = indexForNode(parentNode);
	int pos = rowForNode(node);
	int firstRow = pos;
	int lastRow = pos;
	beginRemoveRows(parent, firstRow, lastRow);
	node->SetParent(0);
	endRemoveRows();
}

Qt::DropActions GSeqModel::supportedDropActions() const
{
	return Qt::MoveAction;// |Qt::CopyAction;
}

QStringList GSeqModel::mimeTypes() const
{
	QStringList theTypes;
	theTypes << "LabExe/EventNodes.list";
	theTypes << "LabExe/parameter.single";
	return theTypes;
}

// TEST
GEventNode* tempNodeMime = 0;

QMimeData* GSeqModel::mimeData( const QModelIndexList & indexes ) const
{
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	foreach(const QModelIndex & index, indexes) {
		if(index.isValid()) {
			GEventNode* node = nodeForIndex(index);
			if(index.column() == 0 && node) {
				QString text = node->UniqueSystemID();
				stream << text;
			}
			tempNodeMime = node;
		}
	}
	QMimeData* mimeData = new QMimeData();
	mimeData->setData("LabExe/EventNodes.list", encodedData);
	return mimeData;
}

bool GSeqModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
	if(action == Qt::IgnoreAction) {
		return true;
	}
	if(!data->hasFormat("LabExe/EventNodes.list")) {
		return false;
	}
	if(column >= columnCount(parent)) {
		return false;
	}
	GEventNode* parentNode = nodeForIndex(parent);
	QByteArray encodedData = data->data("LabExe/EventNodes.list");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);
	while(!stream.atEnd()) {
		QString text;
		stream >> text;
		qDebug() << text;
// 		GEventNode* node = tempNodeMime;
		GEventNode* node = EventManagerInstance()->GetEvent(text);
		if(node)
			insertNode(parentNode, row, node);
	}
	return true;
}

bool GSeqModel::insertRows( int row, int count, const QModelIndex & parent /*= QModelIndex()*/ )
{
	return false;
	beginInsertRows(parent, row, row+count);
	GEventNode *parentNode = nodeForIndex(parent);
	for(int i=0; i<count; i++) {
		new GEventNode(parentNode);
	}
	endInsertRows();
	return true;
}

bool GSeqModel::removeRows( int row, int count, const QModelIndex & parent /*= QModelIndex()*/ )
{

	return false;


	GEventNode* parentNode = nodeForIndex(parent);
	const QList<GEventNode*> listEv = parentNode->ChildNodes();

	Q_ASSERT_X(row >= 0, "GSeqModel::removeRows", "row < 0");
	Q_ASSERT_X(row + count <= listEv.count(), "GSeqModel::removeRows", "row + count > list.count()");
	
	beginRemoveRows(parent, row, row+count);
	for(int i = 0; i < count; i++) {
		GEventNode* node = listEv.at(row);
		removeNode(node);
	}
	endRemoveRows();
	return true;
}











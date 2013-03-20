#ifndef GSEQMODEL_H
#define GSEQMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include <QFileSystemModel>

#include "../Sequence/GSequence.h"

class GEventNode;

// take a look at this http://qt.digia.com/Product/Learning/Education/elearning/online-training/Qt-Essentials---Widget-Edition/
// see the examples-resources folder
 
class GSeqModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	GSeqModel(GSequence* pSequence, QObject *parent);
	~GSeqModel();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &child) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	// for editable model http://qt-project.org/videos/watch/model_view_ii_part_4_custom_tree_model
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	// for insert remove model http://qt-project.org/videos/watch/model_view_ii_part_4_custom_tree_model
	void insertNode(GEventNode* parentNode, int pos, GEventNode* node);
	void removeNode(GEventNode* node);
	// for drag and drop, idem
	Qt::DropActions supportedDropActions() const;
	QStringList mimeTypes() const;
	QMimeData* mimeData(const QModelIndexList & indexes) const;
	bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());

protected: 
	// helper functions from video    http://qt-project.org/videos/watch/model_view_ii_part_4_custom_tree_model
	QModelIndex indexForNode(GEventNode* node) const;
	GEventNode* nodeForIndex(const QModelIndex & index) const;
	int rowForNode(GEventNode* node) const;

private:
	GSequence* m_pSequence;
// 	DommmmmmmmmmmmmmItem *rootItem;
};

#endif // GSEQMODEL_H

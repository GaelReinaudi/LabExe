#ifndef GSEQUENCEMODEL_H
#define GSEQUENCEMODEL_H

#include <QAbstractTableModel>
#include <QGraphicsScene>
class G_old_Sequence;
class GDevice;

class GSequenceModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	GSequenceModel(QObject *parent);
	~GSequenceModel();

	void SetSequence(G_old_Sequence* pseq);
	QGraphicsScene* Scene() { return &m_Scene; }

	int rowCount( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount( const QModelIndex & parent = QModelIndex() ) const;
	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

protected slots:
	//! Updates the positions of the DeviceGraphics in the scene so that they are laying on top of each other.
	//! If pFromThatDeviceDown is given, the position is updated only from that device down, ignoring the ones before.
	void UpdateDeviceGraphicsPositions(GDevice* pFromThatDeviceDown = 0);

private:
	//! The QGraphicsScene is where the items are going to be positioned and ordered.
	QGraphicsScene m_Scene;
	G_old_Sequence* m_pSequence;
};

#endif // GSEQUENCEMODEL_H

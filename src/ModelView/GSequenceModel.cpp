#include "GSequenceModel.h"
#include "Sequencer/G_old_Sequence.h"
#include "Sequencer/GGraphicsDeviceSequenceItem.h"

GSequenceModel::GSequenceModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	m_pSequence = 0;
}

GSequenceModel::~GSequenceModel()
{
	SetSequence(0);
}

void GSequenceModel::SetSequence( G_old_Sequence* pseq )
{
	if(pseq == m_pSequence)
		return;
	if(m_pSequence) 
		m_Scene.removeItem(m_pSequence->TimeOrigin()->EventGraphicsItem());
	m_pSequence = pseq;
	if(m_pSequence) {
		m_Scene.addItem(m_pSequence->TimeOrigin()->EventGraphicsItem());
	}
	else
		return;
	// Let's set the vertical position of the GGraphicsDeviceSequenceItem's
	UpdateDeviceGraphicsPositions();
	connect(m_pSequence, SIGNAL(DeviceJustAdded(GDevice*)), this, SLOT(UpdateDeviceGraphicsPositions(GDevice*)));
	return;
}

void GSequenceModel::UpdateDeviceGraphicsPositions( GDevice* pFromThatDeviceDown /*= 0*/)
{
	int indexOfTheStartingDevice = m_pSequence->m_Devices.indexOf(pFromThatDeviceDown);
	foreach(GDevice* pDev, m_pSequence->m_Devices) {
 		GGraphicsDeviceSequenceItem* pThatDevGrah = m_pSequence->m_DevicesGraphicsItem.value(pDev, 0);
		// if there are no devices before this one in the list, we set the position at 0
		int NthDevIndex = m_pSequence->m_Devices.indexOf(pDev);
		if(NthDevIndex >= indexOfTheStartingDevice) {
			if(NthDevIndex <= 0)
				pThatDevGrah->setPos(0, 0);
			// else, we set the position bellow the previous device
			else {
				GGraphicsDeviceSequenceItem* pPreviousDeviceGraph = m_pSequence->m_DevicesGraphicsItem.value(m_pSequence->m_Devices.at(NthDevIndex - 1));
				pThatDevGrah->PositionBelow(pPreviousDeviceGraph);
			}
		}
	}
}

int GSequenceModel::rowCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	if(!m_pSequence || parent.isValid())
		return 0;
	return m_pSequence->m_DevicesGraphicsItem.count();
}

int GSequenceModel::columnCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	if(!m_pSequence || parent.isValid())
		return 0;
	return 1;
}

QVariant GSequenceModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole */ ) const
{
	return QVariant();
}


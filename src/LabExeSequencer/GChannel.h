#ifndef GCHANNEL_H
#define GCHANNEL_H

#include <QObject>
#include "ToolBox/GSerializable.h"
#include "labexe_global.h"

class GSequence;
class GSynchEvent;
class GInstruction;

#include <QAbstractListModel>

/////////////////////////////////////////////////////////////////////
//! \brief The GChannel class defines a channel in a sequence. 
/*!
It is the base class for different type of channels. Channels are meant to be filled 
with GInstruction of the corresponding type (like a GDigitalOutInstruction for a GDigitalOutChannel).

It has the following functionalities:
- contains a GraphicsItem that is part of the GSequenceGraphicsScene, and that will allow the display of instructions and the user interactions.
- It will manage the instructions at the time it needs to be translated in a device specific data. 
(e.g. an analog channel will be able to group with other analog channels to fill a matrix of double for the PXI system). 
*/
class GChannel : public QObject, public GSerializable
{
	Q_OBJECT

public:
	GChannel(GSequence* parentSequence);
	~GChannel();

	//! Returns the sequence this channel is part of.
	GSequence* Sequence();

public:
	//! Reimplemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	void InterpretSettings(QSettings& fromQsettings);

private:
};

// //! from example http://qt-project.org/doc/qt-4.8/declarative-modelviews-abstractitemmodel-model-h.html
// class GChannelModel : public QAbstractListModel
// {
// 	Q_OBJECT
// public:
// 	enum ChannelRoles {
// 		TypeRole = Qt::UserRole + 1,
// 		SizeRole
// 	};
// 
// 	GChannelModel(QObject *parent = 0)
// 		: QAbstractListModel(parent)
// 	{
// 		QHash<int, QByteArray> roles;
// 		roles[TypeRole] = "uniqueID";
// 		roles[SizeRole] = "size";
// 		setRoleNames(roles);
// 	}
// 
// 	void AddChannel(GChannel* pChan) {
// 		beginInsertRows(QModelIndex(), rowCount(), rowCount());
// 		m_Channels << pChan;
// 		endInsertRows();
// 	}
// 
// 	int rowCount(const QModelIndex & parent = QModelIndex()) const {
// 		return m_Channels.count();
// 	}
// 
// 	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const {
// 		if (index.row() < 0 || index.row() > m_Channels.count())
// 			return QVariant();
// 
// 		const GChannel* pChan = m_Channels[index.row()];
// 		if(role == TypeRole)
// 			return pChan->UniqueSystemID();
// 		else if(role == SizeRole)
// 			return 545;//pChan->UniqueSystemID();
// 		return QVariant();
// 	}
// 
// private:
// 	QList<GChannel*> m_Channels;
// };

#endif // GCHANNEL_H

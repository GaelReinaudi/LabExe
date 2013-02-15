#ifndef GPRIVATEQOBJECTINGSERIALIZABLE_H
#define GPRIVATEQOBJECTINGSERIALIZABLE_H
#include "labexe_global.h"

#include <QObject>
#include <QSettings>

class GSerializable;

/////////////////////////////////////////////////////////////////////
//! \brief The GPrivateQObjectInGSerializable class is used internally by GSerializable in order to be able to use signal and slots internally.
/*!
*/
class GPrivateQObjectInGSerializable : public QObject
{
	Q_OBJECT

public:
	GPrivateQObjectInGSerializable(GSerializable* parentSerializable);
	~GPrivateQObjectInGSerializable();

private slots:
	//! Used to open the group strCurrentGroupAtTimeOfCall in the RequestLatterReadSettings() call
	void OpenGroupInQsettingsForLatterReadCurrentGroup(QSettings* pFromQsettings, QString strCurrentGroupAtTimeOfCall);
	//! Used to assign a new unique ID after the object has been fully constructed
	void LatterAssignNewUniqueID();
	//! used to forward the signal up to the serializable
	void Event_UniqueSystemIDChanged();

signals:
	//! Emitted in the InterpretSettings() function. The strCurrentGroupAtTimeOfCall has to be the group in the QSetting object that was explored when PopulateSettings() was called and triggered this signal.
	void RequestLatterReadSettings(QSettings* pFromQsettings, QString strCurrentGroupAtTimeOfCall);
	//! Emitted in GSerializable constructor in order to assign a new unique ID after the object has been fully constructed
	void RequestLatterAssignNewUniqueID();
	//! 
	void UniqueSystemIdChanged();

private:
	GSerializable* m_ParentSerializable;

	friend class GSerializable;
};

#endif // GPRIVATEQOBJECTINGSERIALIZABLE_H

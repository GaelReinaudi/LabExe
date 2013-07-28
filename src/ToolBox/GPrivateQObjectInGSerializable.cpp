#include "GPrivateQObjectInGSerializable.h"
#include "GSerializable.h"

GPrivateQObjectInGSerializable::GPrivateQObjectInGSerializable(GSerializable *parentSerializable)
	: QObject(0)
	, m_ParentSerializable(0)
{
	m_ParentSerializable = parentSerializable;
	connect(this, SIGNAL(RequestLatterReadSettings(QSettings*, QString)), this, SLOT(OpenGroupInQsettingsForLatterReadCurrentGroup(QSettings*, QString)), Qt::QueuedConnection);
	connect(this, SIGNAL(RequestLatterAssignNewUniqueID()), this, SLOT(LatterAssignNewUniqueID()), Qt::QueuedConnection);
	connect(this, SIGNAL(UniqueSystemIdChanged()), this, SLOT(Event_UniqueSystemIDChanged()));
}

GPrivateQObjectInGSerializable::~GPrivateQObjectInGSerializable()
{

}

void GPrivateQObjectInGSerializable::OpenGroupInQsettingsForLatterReadCurrentGroup( QSettings* pFromQsettings, QString strCurrentGroupAtTimeOfCall )
{
	if(!m_ParentSerializable)
		return;
	if(!pFromQsettings)
		return;
	try {
		pFromQsettings->beginGroup(strCurrentGroupAtTimeOfCall);
		m_ParentSerializable->DelayedInterpretSettings(*pFromQsettings);
		pFromQsettings->endGroup();
	}
	catch(...){

	}
}

void GPrivateQObjectInGSerializable::LatterAssignNewUniqueID()
{
	QObject* pObj = dynamic_cast<QObject*>(m_ParentSerializable);
	if(pObj && pObj->metaObject())
		m_ParentSerializable->AssignNewUniqueID(pObj->metaObject()->className());
	else
		m_ParentSerializable->AssignNewUniqueID("BBB");
}

void GPrivateQObjectInGSerializable::Event_UniqueSystemIDChanged()
{
	m_ParentSerializable->Event_UniqueSystemIDChanged();
}


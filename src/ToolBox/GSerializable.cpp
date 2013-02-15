#include "GSerializable.h"
#include "GPrivateQObjectInGSerializable.h"
#include "GObjectFactory.h"
#include <QFileInfo>
#include <QStringList>
#include <QDebug>
#include <QUuid>

//#define USE_COUNTER_FOR_UNIQUE_ID
#define USE_POINTER_ADDRESS_FOR_UNIQUE_ID
#define APPEND_TIME_FOR_UNIQUE_ID
QHash<QString, QObject*> GSerializable::m_DeSerializedTypeIDs;

QString ChildSeparator("(");

GObjectFactory* ObjectFactoryInstance()
{
	return &GObjectFactorySing::Instance();
}

bool RegisterInObjectFactory( const QString & className, QObject*(objectCreator)(QObject*) )
{
	return ObjectFactoryInstance()->Register(className, objectCreator);
}

GSerializable::GSerializable(bool doNotUseInternalQObject /*= false*/)
	: m_pSettings(0)
	, m_pObjectForConnections(0)
{
	// This makes the unique m_uniqueKey that can be used conveniently in a hash
	UniqueSystemKey();

	AssignNewUniqueID("_");

	if(!doNotUseInternalQObject) {
		m_pObjectForConnections = new GPrivateQObjectInGSerializable(this);
		// 08-17-2012
		emit m_pObjectForConnections->RequestLatterAssignNewUniqueID();
	}
}

GSerializable::GSerializable( const QString & globalSystemID, bool doNotUseInternalQObject /*= false*/ )
	: m_pSettings(0)
	, m_pObjectForConnections(0)
	, m_pIsRelativeTo_ParentSerializable(0)
{
	// This makes the unique m_uniqueKey that can be used conveniently in a hash
	UniqueSystemKey();

	if(!doNotUseInternalQObject) {
		m_pObjectForConnections = new GPrivateQObjectInGSerializable(this);
	}
	if(globalSystemID == "") {
		AssignNewUniqueID("AAA");
		if(m_pObjectForConnections)
			emit m_pObjectForConnections->RequestLatterAssignNewUniqueID();
	}
	else {
		ChangeUniqueSystemID(globalSystemID);
	}
}

GSerializable::GSerializable( const QString & relativeID, GSerializable* pParentSerializable )
	: m_pSettings(0)
	, m_pObjectForConnections(0)
	, m_pIsRelativeTo_ParentSerializable(0)
{
	Q_ASSERT_X(false, "GSerializable::GSerializable", "You might want to check that you should go there. 567613");
// 	// This makes the unique m_uniqueKey that can be used conveniently in a hash
// 	UniqueSystemKey();
// 
// 	m_pObjectForConnections = new GPrivateQObjectInGSerializable(this);
// 	QString ID;
// 	if(!pParentSerializable) {
// 		qWarning() << "The parent for defining a global Identifier is NULL in GSerializable::GSerializable";
// 		ID = "NoParentSerializable";
// 	}
// 	else {
// 		m_pIsRelativeTo_ParentSerializable = pParentSerializable;
// 		ID = pParentSerializable->UniqueSystemID();
// 	}
// 
// 	if(ID == "") {
// 		qWarning() << "The parent has no ID";
// 		ID = "ParentWithoutID";
// 	}
// 	ID += relativeID != "" ? relativeID : QUuid::createUuid().toString();
// 	ChangeUniqueSystemID(ID);
}

GSerializable::GSerializable( const QString & relativeID, QObject* pObjSerializable )
	: m_pSettings(0)
	, m_pObjectForConnections(0)
	, m_pIsRelativeTo_ParentSerializable(0)
{
	// This makes the unique m_uniqueKey that can be used conveniently in a hash
	UniqueSystemKey();

	m_pObjectForConnections = new GPrivateQObjectInGSerializable(this);
	QString parentID;
	QString ID;
	GSerializable* pParentSerializable = dynamic_cast<GSerializable*>(pObjSerializable);
	if(!pParentSerializable) {
//		qWarning() << "The QObject* pObjSerializable is not a GSerializable";
		parentID = "NoParentSerializable";
	}
	else {
		m_pIsRelativeTo_ParentSerializable = pParentSerializable;
		parentID = pParentSerializable->UniqueSystemID();
	}

	if(!m_pIsRelativeTo_ParentSerializable) {
		ID = parentID;
		ID += ChildSeparator;
	}

	ID += (relativeID != "") ? relativeID : QUuid::createUuid().toString();
	m_UniqueSystemID = ID;
	Event_UniqueSystemIDChanged();

	// if the parent ID changes and our ID is relative to it, we could schedule an update so 
	// that Event_UniqueSystemIDChanged() gets called and applies necessary changes
	if(m_pIsRelativeTo_ParentSerializable && this->m_pObjectForConnections && m_pIsRelativeTo_ParentSerializable->m_pObjectForConnections) {
		QObject::connect(
			  m_pIsRelativeTo_ParentSerializable->m_pObjectForConnections
			, SIGNAL(UniqueSystemIdChanged())
			, this->m_pObjectForConnections
			, SIGNAL(UniqueSystemIdChanged())
//			, Qt::QueuedConnection
			);
	}
}

GSerializable::~GSerializable()
{
	if(m_pSettings)
		delete m_pSettings;
	m_pSettings = 0;
	if(m_pObjectForConnections)
		delete m_pObjectForConnections;
	m_pObjectForConnections = 0;
}

void GSerializable::ChangeUniqueSystemID(const QString & uniqueIdentifierName)
{
	if(uniqueIdentifierName == m_UniqueSystemID)
		return;
	m_pIsRelativeTo_ParentSerializable = 0;
	m_UniqueSystemID = uniqueIdentifierName; 
// 	Event_UniqueSystemIDChanged();
	
	if(m_pObjectForConnections)
		emit m_pObjectForConnections->UniqueSystemIdChanged();
}

QString GSerializable::UniqueSystemID() const
{
	QString theCurrentUniqueID;
// 	return m_UniqueSystemID;
// 
	if(!m_pIsRelativeTo_ParentSerializable)
		return m_UniqueSystemID;

	return m_pIsRelativeTo_ParentSerializable->UniqueSystemID() + ChildSeparator + m_UniqueSystemID;
}

QString GSerializable::UniqueSystemKey()
{
	if(m_uniqueKey == "") {
		m_uniqueKey = QString::number(reinterpret_cast<quintptr>(this), 16);
		m_uniqueKey += QString(".");
		m_uniqueKey += QDateTime::currentDateTime().toString("yyMMdd.hhmmss");
	}
	return m_uniqueKey;
}

/////////////////////////////////////////////////////////////////////
/*!
This function has to be reimplemented in derivate class in order to save specific settings. 
When reimplemented, don't forget to call the BaseClasse::PopulateSettings() in order to save the settings defined in the base class.
\param: QSettings & settingsToBeUsed : the QSettings to write in.
ex:\code
void SomeClass::PopulateSettings( QSettings& inQsettings )
{
	BaseClasse::PopulateSettings(inQsettings);
	inQsettings.setValue("From", aDouble);
	inQsettings.setValue("StepNumber", anInt);
}
\endcode
*////////////////////////////////////////////////////////////////////
void GSerializable::PopulateSettings( QSettings& inQsettings )
{
	inQsettings.setValue("ID", UniqueSystemID());
}

void GSerializable::WriteToPermanentStorage()
{
	if(!m_pSettings)
		return;
	// removes anything that in the current group
	m_pSettings->remove("");
	PopulateSettings(*m_pSettings);
	m_pSettings->sync();
}

/////////////////////////////////////////////////////////////////////
/*!
This function has to be reimplemented in derivate class in order to read specific settings. 
When reimplemented, don't forget to call the BaseClasse::InterpretSettings() in order to read the settings defined in the base class.
\param: QSettings & fromQsettings : the QSettings to read from.
ex:\code
void SomeClass::InterpretSettings( QSettings& inQsettings )
{
	BaseClasse::InterpretSettings(inQsettings);
	myDouble = inQsettings.value("From", aDefaultValueDouble);
	myInt = inQsettings.value("StepNumber", aDefaultValueInt);
}
\endcode
*////////////////////////////////////////////////////////////////////
void GSerializable::InterpretSettings( QSettings& fromQsettings )
{
	QString restoreID = fromQsettings.value("ID").toString();
	if(restoreID != "") {
		ChangeUniqueSystemID(restoreID);
		Q_ASSERT_X(false, "GSerializable::InterpretSettings", "You might want to check that you should go there. 565613");
	}
}

void GSerializable::LatterReadCurrentGroup( QSettings& fromQsettings )
{
	QString strCurrentGroup = fromQsettings.group();
	if(m_pObjectForConnections)
		emit m_pObjectForConnections->RequestLatterReadSettings(&fromQsettings, strCurrentGroup);
}

void GSerializable::SetFile( const QString fileName )
{
	QFile file(fileName);
	QFileInfo FileInfo(file);
	if(!FileInfo.exists()) {
		file.open(QFile::WriteOnly | QFile::Text);
	}
	FileInfo.setFile(file);
	if(!FileInfo.exists()) {
		qCritical("The file \"%s\" does not seem to be on your system.", fileName);
		return;
	}
	if(m_pSettings)
		delete m_pSettings;
	m_pSettings = new QSettings(fileName, QSettings::IniFormat);
}

bool GSerializable::ReadSettingsFromFile( QString fileName, QString subGroup /*= ""*/)
{
	QFileInfo FileInfo(fileName);
	if(!FileInfo.exists()) {
// 		qCritical("The file \"%d\" does not seem to be on your system.", fileName);
// 		return false;
	}

	if(m_pSettings)
		delete m_pSettings;
	m_pSettings = new QSettings(fileName, QSettings::IniFormat);

	QSettings& tempSettings = *m_pSettings;
	if(subGroup == "")
		InterpretSettings(tempSettings);
	else {
		tempSettings.beginGroup(subGroup);
		InterpretSettings(tempSettings);
		tempSettings.endGroup();
	}
	return true;
}

QString GSerializable::AssignNewUniqueID(QString baseName)
{
	baseName += QString("-") + UniqueSystemKey();
	ChangeUniqueSystemID(baseName);
	return baseName;
}

QString GSerializable::AssignNewUniqueID(QObject* pObject)
{
	QString strBase;
	if(!pObject) {
		strBase = "0";
// 		qWarning() << "pointer NULL for obtaining the class name in \"GSerializable::AssignNewUniqueID\"" << "It might be that the object do not inherit from QObject";
	}
	else if(!pObject->metaObject()) {
		strBase = "No-MetaObject";
		qWarning() << "No meta-object to use for obtaining the class name in \"GSerializable::AssignNewUniqueID\"" << "Check that the MACRO Q_OBJECT is used.";
	}
	else
		strBase = pObject->metaObject()->className();

	return AssignNewUniqueID(strBase);
}

QString GSerializable::GetTypeFromUniqueTypeID(const QString & strTypeID)
{
	QString strType;
// #ifdef USE_COUNTER_FOR_UNIQUE_ID
	// if there is "{", it means that we have something made with a QUUID, so we should read it this way
	if(strTypeID.contains("{"))
		strType = strTypeID.left(strTypeID.lastIndexOf("{"));
	else
		strType = strTypeID.left(strTypeID.lastIndexOf("-"));
// #else
// 	strType = strTypeID.left(strTypeID.lastIndexOf("{"));
// #endif
	return strType;
}

QString GSerializable::ChangeUniqueTypeID(const QString & strTypeID, const QString & newType)
{
	QString newStrTypeID = strTypeID;
	newStrTypeID.replace(GetTypeFromUniqueTypeID(strTypeID), newType);
	return newStrTypeID;
}

/////////////////////////////////////////////////////////////////////
/*!
In order to de-serialize the list of objects, the object's class has to be registered 
with the G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE macro. see the GObjectFactory doc.
\param:  QSettings & fromQsettings : the QSettings from which to read the list of objects
\param:  QString keyString : the name under which the list is saved 
\param:  QObject * pCommonParent : the parent use to re-parent the created objects. They'll all have the same.
*////////////////////////////////////////////////////////////////////
void GSerializable::RestoreListOfObjects( QSettings& fromQsettings, QString keyString, QObject* pCommonParent )
{
	// get the list of typeID`s. ex:("GSynchEvent-4e48880","GChannelSynchEvent-476b168")
	QStringList strListTypeID = fromQsettings.value(keyString).toStringList();
	foreach(QString str_TypeID, strListTypeID) {
		RestoreObject(fromQsettings, str_TypeID, pCommonParent);
	}
}

/////////////////////////////////////////////////////////////////////
/*!
In order to de-serialize an objects, the object's class has to be registered 
with the G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE macro. see the GObjectFactory doc.
When an Object is Restored by this function, using its uniqueTypeID:
- a static QHash maps the uniqueTypeID used for the de-serialization to the actual pointer of the instantiated object.
- the pointer can be retrieved so that other objects can use it by knowing the uniqueTypeID. See RestoredObject();

\param:  QSettings & fromQsettings : the QSettings from which to read the list of objects
\param:  QString uniqueTypeID : the key name under which the object was saved. It should have been made using AssignNewUniqueID().
\param:  QObject * pParent : the parent use to re-parent the created object.
*////////////////////////////////////////////////////////////////////
void GSerializable::RestoreObject( QSettings &fromQsettings, QString uniqueTypeID, QObject* pParent )
{
	// the type. ex: "GChannelSynchEvent"
	QString strType = GetTypeFromUniqueTypeID(uniqueTypeID);
	// call the GObjectFactory!
	QObject* pNewU = ObjectFactoryInstance()->CreateNewObject(strType, pParent);
	if(pNewU)
		m_DeSerializedTypeIDs.insert(uniqueTypeID, pNewU);
	GSerializable* pSer = dynamic_cast<GSerializable*>(pNewU);
	fromQsettings.beginGroup(uniqueTypeID);
	if(pSer) {
		pSer->ChangeUniqueSystemID(uniqueTypeID);
		pSer->InterpretSettings(fromQsettings);
	}
	fromQsettings.endGroup();
}

QObject* GSerializable::RestoredObject(const QString & uniqueTypeID)
{
	QObject* pObj = m_DeSerializedTypeIDs.value(uniqueTypeID, 0);
	if(!pObj)
		qWarning() << uniqueTypeID 
		<< "\" could not be mapped to an existing QObject*."
		<< "\r\n Check it has been restored with \"RestoreObject()\"";
	return pObj;
}
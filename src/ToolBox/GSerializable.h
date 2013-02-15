#ifndef GSERIALIZABLESETTINGS_H
#define GSERIALIZABLESETTINGS_H
#include "../labexe_global.h"

#include <QSettings>
#include <QString>
#include <QStringList>
#include <QDebug>

class GObjectFactory;
//! Returns the object factory
LABEXE_EXPORT GObjectFactory* ObjectFactoryInstance(); 
//! Registers a class so that it is available in the object factory.
LABEXE_EXPORT bool RegisterInObjectFactory(const QString & className, QObject*(objectCreator)(QObject*));

class GPrivateQObjectInGSerializable;

/////////////////////////////////////////////////////////////////////
//! \brief The GSerializable class defines functions in order to save/retrieve values in/from a QSettings.
/*!
Classes inheriting from GSerializable can thus be easily used by the framework when it comes to save data. 
It does not inherit QObject so that it can be multiply inherited by classes derived of QObject.
*/
class LABEXE_EXPORT GSerializable
{
public:
	//! Constructor. It will not have a UniqueSystemID(). If doNotUseInternalQObject == true, there will not be any QObject and signal and slot available within this instance of GSerializable. This is only used when instantiating a GSerializable before the QMetaObject system is working.
	GSerializable(bool doNotUseInternalQObject = false);
	//! Constructor. It will have the UniqueSystemID() provided in globalSystemID. If doNotUseInternalQObject == true, there will not be any QObject and signal and slot available within this instance of GSerializable. This is only used when instantiating a GSerializable before the QMetaObject system is working.
	GSerializable(const QString & globalSystemID, bool doNotUseInternalQObject = false);
	//! Constructor. It will have the UniqueSystemID() defined by the pParentSerializable->UniqueSystemID() followed by the relativeID.
	GSerializable(const QString & relativeID, GSerializable* pParentSerializable);
	GSerializable(const QString & relativeID, QObject* pObjSerializable);
	virtual ~GSerializable();

	//! Sets a specific file for writing the data to. It will create its own instance of QSettings to take care of future read/write operations.
	void SetFile(const QString fileName);
	//! Returns the QSettings associated with this. 0 if none were specified. It can be an internal QSettings if SetFile() was used to specify a file.
	QSettings* Settings() const { return m_pSettings; }

	//! Write the settings if a QSettings object was associated, e.g. using an internal QSettings object set by using SetFile() or ...
	void WriteToPermanentStorage();
	//! Read the settings from a file. if subGroup is specified, it will only look in that subGroup. See the QSettings documentation.
	bool ReadSettingsFromFile(QString fileName, QString subGroup = "");

	//! Save the list of items, using unique ID`s in order to uniquely identify and save each item in the list as well. 
	template<typename U> 
	static void SaveListAndItems(QSettings& inQsettings, QString keyString, QList<U*> listObjectSettings);
	//! Reads a list of items, using the unique TypeID`s created when using SaveListAndItems(). The Items are instantiated by the GObjectFactory, using a common pCommonParent.
	static void RestoreListOfObjects(QSettings& fromQsettings, QString keyString, QObject* pCommonParent);
	//! Re-instantiate (using the GObjectFactory) and read settings from the fromQsettings under the key uniqueTypeID. The parent of the object is set to pParent
	static void RestoreObject(QSettings &fromQsettings, QString uniqueTypeID, QObject* pParent);
	//! Convenient function to retrieve the type (class name) from a strTypeID made by AssignNewUniqueID().
	static QString GetTypeFromUniqueTypeID(const QString & strTypeID);
	//! Convenient function to change the type (class name) in a strTypeID made by AssignNewUniqueID().
	static QString ChangeUniqueTypeID(const QString & strTypeID, const QString & newType);
	//! retrieve the pointer to an object if it was re-instantiated by RestoreObject().
	static QObject* RestoredObject(const QString & uniqueTypeID);

	//! Saves the settings in the inQsettings by opening a group named UniqueSystemID() and calling the implementation of PopulateSettings().
	void SaveDeviceSettings(QSettings& inQsettings, bool doOpenIdGroup = true) {
		if(doOpenIdGroup)
			inQsettings.beginGroup(UniqueSystemID());
		PopulateSettings(inQsettings);
		if(doOpenIdGroup)
			inQsettings.endGroup();	}
	//! Readds the settings from the fromQsettings by opening a group named UniqueSystemID() and calling the implementation of InterpretSettings().
	void ReadDeviceSettings(QSettings& fromQsettings, bool doOpenIdGroup = true) {
		if(doOpenIdGroup)
			fromQsettings.beginGroup(UniqueSystemID());
		InterpretSettings(fromQsettings);
		if(doOpenIdGroup)
			fromQsettings.endGroup();	}

protected:
	//! Populates or completes a QSettings object to allow permanent storage through the QSettings class. 
	virtual void PopulateSettings(QSettings& inQsettings) = 0;
	//! Reads from the provided QSettings. 
	virtual void InterpretSettings(QSettings& fromQsettings) = 0;
	//! Call this function from within InterpretSettings() when some of the de-serialization from the current group needs to take place latter. It will call DelayedInterpretSettings() from a Qt::QueuedConnection, and read the QSettings within the group you are when calling LatterReadCurrentGroup().
	void LatterReadCurrentGroup(QSettings& fromQsettings);

protected:
	//! Re-implement this when some of the de-serialization needs to take place latter. Queued connection triggered by LatterReadCurrentGroup(). Be careful, the reference fromQsettings must still exist at the moment of the actual de-serializing.
    virtual void DelayedInterpretSettings(QSettings& fromQsettings) {Q_UNUSED(fromQsettings);}
	//! Convenient function to make a unique ID and change this unique ID using the provided string as a base for the name. 
	QString AssignNewUniqueID(QString baseName);
	//! Convenient function to make a unique ID and change this unique ID using the provided object to find the base for the name. 
	QString AssignNewUniqueID(QObject* pObject);

private:
	//! the last QSettings used for writing/reading to/from file.
	QSettings* m_pSettings;
	//! maps the instantiated objects with their uniqueTypeID.
	static QHash<QString, QObject*> m_DeSerializedTypeIDs;


public:
	//! Returns the unique system name that can identify this object without ambiguity. 
	QString UniqueSystemID() const;
	//! Returns a unique key (useful for hash) to identify this object. It is made of <hexadecimal address>.<yyMMdd>.<HHmmss> 
	QString UniqueSystemKey();
	//! Changes the name of the Unique ID. It then calls Event_UniqueSystemIDChanged() that can be re-implemented e.g. for adding the new one in a ...ManagerInstance().
	void ChangeUniqueSystemID(const QString & uniqueIdentifierName);

protected:
	//! Called after the unique system ID changed so that derivate class may do some extra adjustment, like e.g. for updating an object manager. The default implementation does nothing.
	virtual void Event_UniqueSystemIDChanged() {}

private:
	//! Unique name that can be used to identify without ambiguity the object in the system. It has to be the same from one instance of the program to another, or on any computer the program might be launched on.
	QString m_UniqueSystemID;
	//! this key is made unique at creation and is adequate for a hash (in order to index this object)
	QString m_uniqueKey;
	//! If the ID is relative to the parent, this holds the pointer to the parent serializable object
	GSerializable* m_pIsRelativeTo_ParentSerializable;

private:
	//! Internal QObject to be able to use signals and slots
	GPrivateQObjectInGSerializable* m_pObjectForConnections;
	friend class GPrivateQObjectInGSerializable;
};


/////////////////////////////////////////////////////////////////////
/*!
Saves a list of object that can be later de-serialized using RestoreListOfObjects().
The objects in the list must inherit QObject and their class must use the macro Q_OBJECT.

This makes it easy to save tree structures if it is called on each level of the tree. 
First a list of unique string ID (one for each item!) will be saved inQsettings under the given keyString.
Then each item will be saved inQsettings under the previously defined UniqueID keys.
It is a template because the type of the object of the list has to be given. (seehttp://www.parashift.com/c++-faq-lite/proper-inheritance.html#faq-21.2)
\return: void : 
\param:  QSettings & inQsettings : the QSettings in which to write the data
\param:  QString keyString : the key under which to store the data (like in QSettings::setValue).
\param:  QList<GSerializable * > listObjectSettings : the list of objects to serialize. 
*////////////////////////////////////////////////////////////////////
template<typename U> 
void GSerializable::SaveListAndItems(QSettings& inQsettings, QString keyString, QList<U*> listObjectSettings)
{
	// first the list of TypeID`s
	QStringList strListUniqueID;
	foreach(U* pObj, listObjectSettings) {
		// if it is a list of GSerializable objects, we use the UniqueSystemID
		GSerializable* pSer = dynamic_cast<GSerializable*>(pObj);
		if(pSer) {
			if(pSer->m_UniqueSystemID != "") {
				strListUniqueID += pSer->m_UniqueSystemID;
				continue;
			}
		}
		// else
		// try to convert the obj to a QObject. It will be 0 otherwise and trigger a warning in AssignNewUniqueID().
		QObject* pQObject = dynamic_cast<QObject*>(pObj);
		QString uniqueID;
		if(pSer) {
			uniqueID = pSer->AssignNewUniqueID(pQObject);
		}
		else {
			uniqueID = QString("KKKKKKKKKKKKKKKKKKKKKKK") + QString("-") + QString::number(reinterpret_cast<quintptr>(pQObject), 16);
		}
		strListUniqueID += uniqueID;
	}
	inQsettings.setValue(keyString, strListUniqueID);

	// get the TypeID that was made some lines above to beginGroup() and PopulateSettings()
	foreach(U* pObj, listObjectSettings) {
		// try to convert the obj to a GSerializable.
		GSerializable* pSer = dynamic_cast<GSerializable*>(pObj);
		if(pSer) {
			QString str_TypeID = strListUniqueID.at(listObjectSettings.indexOf(pObj));
			inQsettings.beginGroup(str_TypeID);
			pSer->PopulateSettings(inQsettings);
			inQsettings.endGroup();
		}
	}
}


#endif // GSERIALIZABLESETTINGS_H

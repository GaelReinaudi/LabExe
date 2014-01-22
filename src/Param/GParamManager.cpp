#include "GParamManager.h"
#include "GParam.h"
#include <QMimeData>
#include <QSettings>
#include <QtWidgets>


GParamManager::GParamManager( void )
	: GSerializable("ParamManager", true) // we don't use the internal QObject as it is going to be instantiated before the program and the Qt dll actually quicks in. 
{

}

QList<QString> GParamManager::RegisteredParamTypes() const
{
	std::vector<QString> ids = ParamManagerInstance()->RegisteredIds();
	QList<QString> typlist;
	for(int i = 0; i < (int)ids.size(); i++) {
		typlist.append(ids[i]);
	}
	return typlist;
}

void GParamManager::Add( GParam* pParam )
{
	insert(pParam->UniqueSystemID(), pParam);
}

GParam* GParamManager::GetParam( const QMimeData* theMimeData ) const
{
	if(!theMimeData)
		return 0;
	QByteArray encodedData = theMimeData->data("LabExe/parameter.single");
	QDataStream paramStream(&encodedData, QIODevice::ReadOnly);
	if(paramStream.atEnd()) 
		return 0;	
	QString paramUniqueID;
	paramStream >> paramUniqueID;

	return GetParam(paramUniqueID);
}

GParam* GParamManager::GetParam( QString keyString, QSettings* fromQsettings )
{
	if(!fromQsettings)
		return 0;
	return GetParam(fromQsettings->value(keyString, "").toString());
}

GParam* GParamManager::CreateNewParam( QString className, QString uniqueParamID, QObject *parent )
{
	return ParamManagerInstance()->CreateObject( className, uniqueParamID, parent);
}

GParam* GParamManager::CreateNewParam( QString className, QObject *parent )
{
	return CreateNewParam(className, QUuid::createUuid().toString(), parent);
}

void GParamManager::PopulateSettings( QSettings& inQsettings )
{
	// clears everything relative to GParamManager in the file
	inQsettings.remove("");
}

void GParamManager::InterpretSettings( QSettings& fromQsettings )
{
    Q_UNUSED(fromQsettings);
}

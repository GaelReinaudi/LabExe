#include "GParamBucket.h"
#include "GParamNum.h"
#include "GParamString.h"
#include "GParamBool.h"
#include "GParamDouble.h"
#include "GParamManager.h"
#include "GParamBucketWidget.h"
#include "GParamBucketTreeWidget.h"
#include <QTimer>

GParamBucket::GParamBucket(QString theName, QObject *parent, GParam::Properties paramOptions /*= NoOption*/)
	: GParam(theName, parent, paramOptions)
	, m_IsFreezed(false)
	, m_NumUpdates(0)
	, m_SomeUpdateSignalCount(0)
	, m_DelayUpdateSignal(0)
{
	setObjectName(theName);
	Init();

	AddExtraFieldBool("trig");
}

GParamBucket::~GParamBucket()
{

}

void GParamBucket::Init()
{
	connect(this, SIGNAL(RequestDropParamLater(QString)), this, SLOT(DropParamByID(QString)), Qt::QueuedConnection);
}

bool GParamBucket::DropParamByID( QString uniqueParamID )
{
	// if freeze, no drop by ID
	if(m_IsFreezed)
		return false;
	GParam* pPar = ParamManagerInstance()->GetParam(uniqueParamID);
	if(!pPar) {
		qWarning() << "Couldn't retrieve the parameter" << uniqueParamID <<"in GParamBucket::DropParamByID()";
		return false;
	}
	AddParam(pPar);
	return true;
}

void GParamBucket::AddParamLater( GParam* pParam )
{
	if(!pParam)
		return;
	emit RequestDropParamLater(pParam->UniqueSystemID());
}

bool GParamBucket::AddParam( GParam* pParam )
{
	if(!pParam) {
		qWarning() << "GParamBucket::AddParam(), the pointer is 0. 642614";
		return false;
	}
	if(Contains(pParam)) {
// 		qWarning() << "The param is already in the bucket! One could implement a moving mechanism to remove the param and re-append it at the end. 644614";
		return false;
	}

	m_Params.append(pParam);
	// make all the extra fields
	for(int i = 0; i < m_ExtraFieldsClass.count() ; i++) {
		QString className = m_ExtraFieldsClass.value(i);
		QString fieldName = m_ExtraFieldsName.value(i);
		// if there is or was a previous associated field, we can reuse them
		if(ExtraParam(pParam, fieldName)) {
			continue;
		}
		m_Param_Field_ExtraParams[pParam][fieldName] = ParamManagerInstance()->CreateNewParam(className, pParam->UniqueSystemID() + fieldName, this);
	}

	// special for triggering.
	// if there is a "trig" field, we connect the bool to the UpdateTriggeringScheme()
	GParamBool* pTrigBool = ExtraParamBool(pParam, "trig");
	if(pTrigBool) {
		connect(pTrigBool, SIGNAL(ValueDidChange(bool)), this, SLOT(UpdateTriggeringScheme()));
	}
	// starts the triggering
	SetParamTriggeringConnection(pParam, true);

	emit ParamAdded(pParam);
	return true;
}

bool GParamBucket::RemoveParam( GParam* pParam )
{
	if(!pParam) {
		qWarning() << "GParamBucket::RemoveParam(), the pointer is 0. 642618";
		return false;
	}
	if(!Contains(pParam)) {
		qWarning() << "The param is not in the bucket and thus can't be removed! 644619";
		return false;
	}
	// the index of the param before removal
	int indexParam = m_Params.indexOf(pParam);

	// stops the triggering
	SetParamTriggeringConnection(pParam, false);
	// removes the param
	if(!m_Params.removeOne(pParam))
		return false;
	// tells about it
	emit ParamRemoved(indexParam, pParam);

	// destroys the extra params
	for(int i = 0; i < m_ExtraFieldsClass.count() ; i++) {
		QString fieldName = m_ExtraFieldsName.value(i);
		GParam* pExtraPar = ExtraParam(pParam, fieldName);
		// removes the map item for the field name before deleting the corresponding param
		m_Param_Field_ExtraParams[pParam].remove(fieldName);
		if(pExtraPar) {
 			pExtraPar->deleteLater();
		}
	}
	// removes the map for that param
	m_Param_Field_ExtraParams.remove(pParam);
	return true;
}

void GParamBucket::SetParamTriggeringConnection(GParam* pParam, bool doUseAsTrigger)
{
	if(doUseAsTrigger) {
		// if numerical, we make some connections
		if(qobject_cast<GParamNum*>(pParam))
			connect(pParam, SIGNAL(ValueUpdated(double)), this, SLOT(EventSomeValueUpdated()), Qt::UniqueConnection);
	}
	else {
		// if numerical, we make some dis-connections
		if(qobject_cast<GParamNum*>(pParam))
			disconnect(pParam, SIGNAL(ValueUpdated(double)), this, SLOT(EventSomeValueUpdated()));
	}
	GParamBool* pTrigBool = ExtraParamBool(pParam, "trig");
	if(pTrigBool) {
		pTrigBool->SetParamValue(doUseAsTrigger);
	}
}

void GParamBucket::UpdateTriggeringScheme()
{
	foreach(GParam* pParam, Params()) {
		GParamBool* pTrigBool = ExtraParamBool(pParam, "trig");
		if(pTrigBool) {
			bool doTrigger = pTrigBool->BoolValue();
			SetParamTriggeringConnection(pParam, doTrigger);
		}
	}
}

GParamBucketWidget* GParamBucket::ProvideParamBucketWidget( QWidget* parentWidget, Qt::Orientation orientation /*= Qt::Vertical*/ )
{
 	GParamBucketWidget* pWidBucket = new GParamBucketWidget(parentWidget, this, orientation);
//	GParamBucketTreeWidget* pWidBucket = new GParamBucketTreeWidget(parentWidget, this);
	return pWidBucket;
}

GParamBucketTreeWidget* GParamBucket::ProvideParamBucketTreeWidget(QWidget* parentWidget)
{
	GParamBucketTreeWidget* pWidBucket = new GParamBucketTreeWidget(parentWidget, this);
	return pWidBucket;	
}

QWidget* GParamBucket::ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid /*= Default*/)
{
 	return ProvideParamBucketTreeWidget(forWhichParent);
//	return ProvideParamBucketWidget(forWhichParent);
}

void GParamBucket::Freeze( bool doFreeze /*= true*/ )
{
	m_IsFreezed = doFreeze;
}

void GParamBucket::PopulateSettings( QSettings& inQsettings )
{
	GParam::PopulateSettings(inQsettings);
// 	inQsettings.beginGroup(UniqueSystemID());
// 	// temp keep this to maintain file compatibility 
// 	//////////////////////////////////////////////////
// 	GParam* pPar = Params().value(0);
// 	if(pPar) {
// 		inQsettings.setValue("param-held", pPar->UniqueSystemID());
// 	}
// 	//////////////////////////////////////////////////
	QStringList paramStrList;
	foreach(GParam* pPar, Params()) {
		if(pPar)
			paramStrList << pPar->UniqueSystemID();
	}
	inQsettings.setValue("params", paramStrList);

	inQsettings.beginGroup("extras");
	// if some params are child of this bucket, it saves them (just like GDevice does)
	foreach(GParam* pPar, findChildren<GParam*>()) {
// 		pPar->PopulateSettings(inQsettings);
		pPar->SaveDeviceSettings(inQsettings);
	}
	inQsettings.endGroup();
// 	inQsettings.endGroup();
}

void GParamBucket::InterpretSettings( QSettings& fromQsettings )
{
	GParam::InterpretSettings(fromQsettings);
// 	fromQsettings.beginGroup(UniqueSystemID());
// 	// temp keep this to maintain file compatibility 
// 	//////////////////////////////////////////////////////////////////////
// 	if(!fromQsettings.contains("params")) {								//
// 		QString parID = fromQsettings.value("param-held").toString();	//
// 		if(parID != "")													//
// 			emit RequestDropParamLater(parID);							//
// 	} else {															//
// 	//////////////////////////////////////////////////////////////////////
	QStringList parIDs = fromQsettings.value("params").toStringList();
	if(parIDs.isEmpty())
		parIDs = fromQsettings.value("params-held").toStringList();
	foreach(QString strID, parIDs) {
		if(strID == "")
			continue;
		emit RequestDropParamLater(strID);
	}
// 	//////////////////////////////////////////////////////////////////////
// 	}																	//
// 	//////////////////////////////////////////////////////////////////////
	fromQsettings.beginGroup("extras");
	// if params are child of this bucket, it restores them (just like GDevice does). but latter so that the params are existing already.
	LatterReadCurrentGroup(fromQsettings);
	fromQsettings.endGroup();

// 	fromQsettings.endGroup();
}

void GParamBucket::DelayedInterpretSettings( QSettings& fromQsettings )
{
	GParam::DelayedInterpretSettings(fromQsettings);
	foreach(GParam* pPar, findChildren<GParam*>()) {
// 		pPar->InterpretSettings(fromQsettings);
		//TOGO: false just for compatibility with previous version
		pPar->ReadDeviceSettings(fromQsettings);
	}
}

// bool GParamBucket::IsParamTriggering( GParam* pParam )
// {
// 	bool wasConnected = disconnect(pParam, SIGNAL(ValueUpdated(double)), this, SLOT(EventSomeValueUpdated()));
// 	if(wasConnected)
// 		connect(pParam, SIGNAL(ValueUpdated(double)), this, SLOT(EventSomeValueUpdated()), Qt::UniqueConnection);
// 	return wasConnected;
// }

void GParamBucket::EventSomeValueUpdated()
{
	m_SomeUpdateSignalCount++;
	// if one param changes, we should queue an update signal. By the time it is actually called, the count might have gotten up.
	if(m_SomeUpdateSignalCount == 1)
		QTimer::singleShot(m_DelayUpdateSignal, this, SLOT(EventAboutToSendUpdateSignal()));
	//TODO
}

void GParamBucket::EventAboutToSendUpdateSignal()
{
	++m_NumUpdates;
	int nParUpdated = m_SomeUpdateSignalCount;
	m_SomeUpdateSignalCount = 0;
	emit BucketUpdatedValues(nParUpdated);
}

GParam* GParamBucket::ExtraParam( GParam* pParam, const QString & field ) const
{
	return m_Param_Field_ExtraParams.value(pParam).value(field, 0);
}

GParamNum* GParamBucket::ExtraParamNum( GParam* pParam, const QString & field ) const
{
	return qobject_cast<GParamNum*>(m_Param_Field_ExtraParams.value(pParam).value(field, 0));
}

GParamString* GParamBucket::ExtraParamString( GParam* pParam, const QString & field ) const
{
	return qobject_cast<GParamString*>(m_Param_Field_ExtraParams.value(pParam).value(field, 0));
}

GParamBool* GParamBucket::ExtraParamBool( GParam* pParam, const QString & field ) const
{
	return qobject_cast<GParamBool*>(m_Param_Field_ExtraParams.value(pParam).value(field, 0));
}

void GParamBucket::AddExtraFieldBool( const QString & fieldName )
{
	QString theFieldUsed = fieldName;
	if(theFieldUsed.isEmpty() || m_ExtraFieldsName.contains(theFieldUsed))
		theFieldUsed.append(QString("#%1").arg(m_ExtraFieldsName.count()));
	m_ExtraFieldsClass += "GParamBool";
	m_ExtraFieldsName += theFieldUsed;
}

void GParamBucket::AddExtraFieldString( const QString & fieldName )
{
	QString theFieldUsed = fieldName;
	if(theFieldUsed.isEmpty() || m_ExtraFieldsName.contains(theFieldUsed))
		theFieldUsed.append(QString("#%1").arg(m_ExtraFieldsName.count()));
	m_ExtraFieldsClass += "GParamString";
	m_ExtraFieldsName += theFieldUsed;
}

void GParamBucket::AddExtraFieldDouble( const QString & fieldName )
{
	QString theFieldUsed = fieldName;
	if(theFieldUsed.isEmpty() || m_ExtraFieldsName.contains(theFieldUsed))
		theFieldUsed.append(QString("#%1").arg(m_ExtraFieldsName.count()));
	m_ExtraFieldsClass += "GParamDouble";
	m_ExtraFieldsName += theFieldUsed;
}

void GParamBucket::RemoveExtraField( const QString & fieldName )
{
	int idexField = m_ExtraFieldsName.indexOf(fieldName);
	m_ExtraFieldsName.removeAt(idexField);
	m_ExtraFieldsClass.removeAt(idexField);
}

QVector<GParamNum*> GParamBucket::ParamNums(const QVector<bool> & accessMask /*= QVector<bool>()*/) const
{
	static bool warningNotOnlyNums = false;
	QVector<GParamNum*> listPar;
	listPar.reserve(8);
	int i = 0;
	foreach(GParam* pPar, Params()) {
		// 0 pointer if not num
		GParamNum* pParNum = qobject_cast<GParamNum*>(pPar);
		// if no access mask value or if it is true, we consider this param
		if(accessMask.value(i, true)) {
			listPar.append(pParNum);
			if(!pParNum)
				warningNotOnlyNums = true;
		}
		++i;
	}
	if(warningNotOnlyNums)
		qDebug() << "Malformed bucket! not all params are numerical when asking ParamNums(). 816154";
	return listPar;
}

GVectorDouble const GParamBucket::DoubleValues(const QString & boolMaskField /*= ""*/) const
{
	GVectorDouble listRet;
	// if no mask, a quick loop to return all the values, 0.0s if not numerical params
	if(boolMaskField.isEmpty()) {
		foreach(GParamNum* pParNum, ParamNums()) {
			listRet.append(pParNum ? pParNum->DoubleValue() : 0.0);
		}
	}
	else {
		foreach(GParamNum* pParNum, ParamNums()) {
			GParamBool* pParMask = ExtraParamBool(pParNum, boolMaskField);
			// if it doesn't correspond to a field, bye for this one
			if(!pParMask) {
				continue;
			}
			bool isTrue = pParMask->BoolValue();
			// if it is not true, bye for this one
			if(!isTrue) {
				continue;
			}
			listRet.append(pParNum->DoubleValue());
		}
	}
	return listRet;
}

void GParamBucket::SetValues(const GVectorDouble & theValuesToSet, const QVector<bool> & accessMask /*= QVector<bool>()*/)
{
	int i = 0;
	foreach(GParamNum* pParNum, ParamNums(accessMask)) {
		if(pParNum) {
			pParNum->SetParamValue(theValuesToSet[i]);
		}
		++i;
	}

// 	// if no mask, we give all true`s
// 	if(accessMask.isEmpty()) {
// 		accessMask = QVector<bool>(nVal, true);
// 	}
// 	int nMask = accessMask.count();
// 
// 	QList<GParamNum*> listPar = ParamNums();
// 	int nPar = listPar.count();
// 	int nVal = theValuesToSet.count();
// 	for(int i = 0; i < qMin(qMin(nPar, nVal), nMask); i++) {
// 		GParamNum* pParNum = listPar[i];
// 		if(pParNum && accessMask[i])
// 			listPar.at(i)->SetParamValue(theValuesToSet[i]);
// 	}
}

QList<GParam*> GParamBucket::ExtraParamList( const QString & forThatField )
{
	QList<GParam*> listPar;
	foreach(GParam* pPar, Params()) {
		listPar.append(ExtraParam(pPar, forThatField));
	}
	return listPar;
}

const QVector<bool> GParamBucket::ExtraParamBoolMask( const QString & forThatField ) const
{
	QVector<bool> listBool(Params().count());
	int i = 0;
	foreach(GParam* pPar, Params()) {
		GParamBool* pParBool = qobject_cast<GParamBool*>(ExtraParam(pPar, forThatField));
		listBool[i] = pParBool ? pParBool->BoolValue() : false;
		++i;
	}
	return listBool;
}

GParamNum* GParamBucket::ParamNum( int indexParam ) const
{
	return qobject_cast<GParamNum*>(m_Params.value(indexParam));
}

QVector<GParamNum*> GParamBucket::ExtraParamsNum( QString field, const QString & boolMaskField /*= ""*/ ) const
{
	QVector<GParamNum*> listNums;
	foreach(GParam* pPar, Params()) {
		GParamNum* pParNum = ExtraParamNum(pPar, field);
		// if there is a mask given
		if(!boolMaskField.isEmpty()) {
			GParamBool* pParMask = ExtraParamBool(pPar, boolMaskField);
			// if it doesn't correspond to a field, bye for this one
			if(!pParMask) {
				continue;
			}
			bool isTrue = pParMask->BoolValue();
			// if it is not true, bye for this one
			if(!isTrue) {
				continue;
			}
		}
		listNums.append(pParNum);
	}
	return listNums;
}
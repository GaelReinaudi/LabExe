#include "GParam.h"
#include "GParamLabel.h"
#include "GParamManager.h"	
#include "Device/GDevice.h"	
#include "GParamSettingsWidget.h"

GParamManager* ParamManagerInstance()
{
	return &GParamManagerSing::Instance();
}

GParam* GetParamFromParamManager( QString uniqueID )
{
	return ParamManagerInstance()->GetParam(uniqueID);
}

void AddToParamManager( GParam* pParam )
{
	ParamManagerInstance()->Add(pParam);
}

GParam::GParam(QString theName, QObject *parent, GParam::Properties paramOptions /*= NoOption*/)
	: QObject(parent) 
	, GSerializable(theName, parent)
	, m_Options(paramOptions)
	, m_Units("")
	, m_DisplayUnits(false)
{
	ParamInit();
	SetName(theName);
}

GParam::~GParam()
{
}

void GParam::ParamInit()
{
	GDevice* pDev = qobject_cast<GDevice*>(parent());
	GSerializable* pSer = dynamic_cast<GSerializable*>(parent());
	// test
// 	if(!pDev)
// 		qDebug() << Name() << "Param's parent is not a GDevice";
// 	if(!pSer)
// 		qDebug() << Name() << "Param's parent is not a GSerializable";

	if(ParamManagerInstance()->Contains(UniqueSystemID())) {
// 		if(pDev)
// 			qWarning() << "In device" << pDev->Name() << "GParam has not been given a unique identifier:" << UniqueSystemID() << "already exists";
// 		else if(pSer)
// 			qWarning() << "In serialized" << "with ID" << pSer->UniqueSystemID() 
// 			<< "GParam has not been given a unique identifier:" << UniqueSystemID() << "already exists";
	}
	else
		ParamManagerInstance()->Add(this);

	// by default, the ParamValueWasUpdated() triggers the ParamUpdateCompletion() signal.
	SetExternalCompletionSignal(this, SIGNAL(ParamValueWasUpdated(bool)));
}

QString GParam::Name() const
{
	return objectName();
}

void GParam::SetName( QString theName )
{
	setObjectName(theName);
	emit NameChanged(theName);
}

QLabel* GParam::ProvideNewLabel(QWidget* forWhichParent, LabelWidget::Options someOptions /*= LabelWidget::NoOption*/)
{
	// we tell the label that this is the param it labels (used, eg for drag & drops)
	GParamLabel* pLabel = new GParamLabel(this, forWhichParent);
	pLabel->setToolTip(m_Description);

	// we connect the following so that the GParam can provide a context menu
	connect(pLabel, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ProvideParamMenu(const QPoint&)));

	return pLabel;
}

/////////////////////////////////////////////////////////////////////
/*!
	\return:   bool : true if successful, just like QT's connect()
	\param: const QObject * sender : the sender of the signal
	\param: const char * signal : the signal has to have a bool as argument like DeviceDidTheThing(bool successfully)
	\param: Qt::ConnectionType type : QT's connection type
ex:\code
	UnParam.SetExternalCompletionSignal(pMotor, SIGNAL(FinishedMoveToTargetPosition(bool)));
\endcode
*////////////////////////////////////////////////////////////////////
bool GParam::SetExternalCompletionSignal( const QObject* sender, const char* signal, Qt::ConnectionType type /*= Qt::AutoConnection*/ )
{
	//TODO could check if new connection successful before breaking the old one: if(connect) then if(disconnect all) then connect
	disconnect(this, SIGNAL(ParamUpdateCompletion(bool)));
	if(connect(sender, signal, this, SIGNAL(ParamUpdateCompletion(bool)), type))
		return true;
	else
		return false;
}

void GParam::ProvideParamMenu( const QPoint & pos ) const
{
	QMenu menu;
	// was the sender a GParamLabel ?
	GParamLabel* pParLab = qobject_cast<GParamLabel*>(sender());
	PopulateParamMenu(&menu, pParLab);
	if(pParLab)
		menu.exec(pParLab->mapToGlobal(pos));
}

void GParam::PopulateParamMenu( QMenu* pTheMenu, GParamLabel* pSenderLabel /*= 0*/ ) const
{
	pTheMenu->addActions(pSenderLabel->actions());
}

GParamSettingsWidget* GParam::PopupSettingWidget()
{
	GParamSettingsWidget* pWid = new GParamSettingsWidget(this, 0);
	pWid->move(QCursor::pos() + QPoint(0, 20));
	// for clean-up
	pWid->setAttribute(Qt::WA_DeleteOnClose, true);
	pWid->setWindowFlags(Qt::Popup);
	pWid->show();
	return pWid;
}

void GParam::PopulateSettings( QSettings& inQsettings )
{
// 	inQsettings.beginGroup(UniqueSystemID());
	if(!m_Units.isEmpty())
		inQsettings.setValue("Units", m_Units);
	if(m_DisplayUnits)
		inQsettings.setValue("dispUnit", 1);
// 	inQsettings.endGroup();
}

void GParam::InterpretSettings( QSettings& fromQsettings )
{
// 	fromQsettings.beginGroup(UniqueSystemID());
	SetUnits(fromQsettings.value("Units", "").toString());
	ShowUnits(fromQsettings.value("dispUnit", false).toBool());
// 	fromQsettings.endGroup();
}

void GParam::SetUnits( QString strUnits )
{
	m_Units = strUnits;
// 	if(m_Units.isEmpty())
// 		m_Units += "--";
	emit UnitsChanged(m_Units);
}

void GParam::ShowUnits( bool doShowUnits )
{
	m_DisplayUnits = doShowUnits;
 	emit UnitDisplayChanged(doShowUnits);
}

QString GParam::StringContent( char format /*= 'g'*/, int precision /*= 6*/ )
{
	QVariant::Type theType = type();
	switch(theType) {
		case QVariant::Double: 
			return QString::number(toDouble(), format, precision);
		case QVariant::String:
			return toString();
		case QVariant::Int:
			return QString::number(toInt());
	}
	return "";
}








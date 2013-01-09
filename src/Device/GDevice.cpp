#include "GDevice.h"
#include "GDeviceManager.h"												
#include "GDeviceWidget.h"
#include "Param/GParam.h"
#include <QInputDialog>

GDeviceManager* DeviceManagerInstance()
{
	return &(GDeviceManagerSing::Instance());
}

GDevice* GetDeviceFromDeviceManager( QString uniqueIdentifierName )
{
	return DeviceManagerInstance()->GetDevice(uniqueIdentifierName);
}

void AddToDeviceShelf( GDevice* pDevice )
{
	DeviceManagerInstance()->AddDeviceToShelf(pDevice);
}

bool RegisterHardDeviceInManager( const QString & className, GDevice*(deviceCreator)(QString, QObject*))
{
	bool isOk = DeviceManagerInstance()->Register(className, deviceCreator);
	return isOk;
}

bool RegisterProgDeviceInManager( const QString & className, GDevice*(deviceCreator)(QString, QObject*) )
{
	bool isOk = DeviceManagerInstance()->Register(className, deviceCreator);
	if(isOk)
		DeviceManagerInstance()->RegisterProgDevice(className);
	return isOk;
}

void LoadPluginsInDeviceManager()
{
	DeviceManagerInstance()->LoadDevicePlugins();
}

/////////////////////////////////////////////////////////////////////
/*!
The uniqueIdentifierName is a name that can be used to identify the device without ambiguity. 
\param: QString uniqueIdentifierName : a unique universal name that used to identify the GDevice. It has to be the same from one instance to the program to another, or on any computer the program might be launched on.
\param: QObject * parent : Parent object.
\exeption:	if the uniqueIdentifierName already exists as an identifier for any of the GDevice's.
ex:\code
ADeviceClass( "PXI_1/Card6535_1/Ao_21", pParent );
\endcode
*////////////////////////////////////////////////////////////////////
GDevice::GDevice(QString uniqueIdentifierName, QObject *parent)
	: QThread(parent)
	, GSerializable(uniqueIdentifierName)
{
	AddToDeviceManager();
	// note that this ReName() call will be overwritten when the device is created through the device manager (see G_REGISTER_HARD_DEVICE_CLASS), 
	// but not if it is created manually or by another device, like in the SrYbExe::DevicesToShelf() or in GPhidgetAdvancedServoModule::CreateSubDevicesServos().
	ReName(uniqueIdentifierName);

	// warn if parent = 0. Added to a cleanup list to insure will be destroyed.
	if(!parent) {
		DeviceManagerInstance()->m_CleanUpDevices.add(this);
		qDebug() << "device" << metaObject()->className() << UniqueSystemID() << "Created with parent = 0.";
	}
	connect(this, SIGNAL(PrivateTriggerLatterInitialization()), this, SLOT(LatterInitialization()), Qt::QueuedConnection);
	emit PrivateTriggerLatterInitialization();
	disconnect(this, SIGNAL(PrivateTriggerLatterInitialization()));

	m_Color = QColor(
		(qrand() * 255) / RAND_MAX, 
		(qrand() * 255) / RAND_MAX, 
		(qrand() * 255) / RAND_MAX);

}

GDevice::~GDevice()
{
	DeviceManagerInstance()->Remove(UniqueSystemID());
}

/////////////////////////////////////////////////////////////////////
/*!
The ProvideNewDeviceGroupBox()function returns a GDevice widget that is going to be 
populated by the derived PopulateDeviceWidget(). This widget is intended to represent 
and manage the device. 
	\return:   GDeviceWidget* : the returned GDeviceWidget.
	\param: QWidget * inWhichWidget : this sets the widget that is going to parent (hence display) the returned GDeviceWidget. 
	\param: QBoxLayout::Direction orientation : sets how the layout of the GDeviceWidget manages the display (QBoxLayout::LeftToRight, QBoxLayout::RightToLeft, QBoxLayout::TopToBottom, QBoxLayout::BottomToTop).
*////////////////////////////////////////////////////////////////////
GDeviceWidget* GDevice::ProvideNewDeviceGroupBox( QWidget* inWhichWidget, QBoxLayout::Direction orientation /*= QBoxLayout::LeftToRight*/ )
{
	// we create the appropriate QGroupBox with connections to the device
	GDeviceWidget* pGroupBoxDeviceWidget = new GDeviceWidget(this, inWhichWidget);
	// we make a layout with the orientation
	QBoxLayout* DeviceLayout = new QBoxLayout(orientation);
	pGroupBoxDeviceWidget->setLayout(DeviceLayout);
	DeviceLayout->setSpacing(1);
	DeviceLayout->setContentsMargins(1, 1, 1, 1);

	// call the virtual function that populates the widget
	PopulateDeviceWidget(pGroupBoxDeviceWidget);

	return pGroupBoxDeviceWidget;
}

void GDevice::ProvideContextMenu( const QPoint & pos ) const
{
	QMenu menu;
	// was the sender a GDeviceWidget ?
	GDeviceWidget* pDevWid = qobject_cast<GDeviceWidget*>(sender());
	PopulateContextMenu(&menu, pDevWid);
	QWidget* pSender = qobject_cast<QWidget*>(sender());
	if(pSender)
		menu.exec(pSender->mapToGlobal(pos));
}

void GDevice::PopulateContextMenu( QMenu* pTheMenu, GDeviceWidget* pSenderDeviceWidget /*= 0*/ ) const
{
	pTheMenu->addAction("Re-name", this, SLOT(ReName()));
	pTheMenu->addSeparator();

	pTheMenu->addAction(":-)");
	// if the request came from a GDeviceWidget, we can add the actions() in its list
	if(pSenderDeviceWidget)
		pTheMenu->addActions(pSenderDeviceWidget->actions());
}

void GDevice::ReName( QString val )
{
	if(val == "" || val == objectName())
		return;
	setObjectName(val);
	emit NameChanged(objectName());
}

void GDevice::ReName()
{
	bool bOK;
	ReName(QInputDialog::getText(&m_DummyWidgetToParentWidgets, "New name", objectName(), QLineEdit::Normal, Name(), &bOK));
}

void GDevice::PopulateSettings( QSettings& inQsettings )
{
	GSerializable::PopulateSettings(inQsettings);
	// class name
	inQsettings.setValue("Class", metaObject()->className());
	// parent ID if it is a GDevice
	QString itsParentID("");
	GDevice* pItsDeviceParent = qobject_cast<GDevice*>(parent());
	if(pItsDeviceParent)
		itsParentID = pItsDeviceParent->UniqueSystemID();
	inQsettings.setValue("Parent-Device-ID", itsParentID);
	// Name
	inQsettings.setValue("Name", Name());

	foreach(GParam* pPar, ChildParams()) {
// 		pPar->PopulateSettings(inQsettings);
		pPar->SaveDeviceSettings(inQsettings);
	}
}

void GDevice::InterpretSettings( QSettings& fromQsettings )
{
	QString nameDev = fromQsettings.value("Name", "").toString();
	if(nameDev != "")
		ReName(nameDev);
	foreach(GParam* pPar, ChildParams()) {
// 		pPar->InterpretSettings(fromQsettings);
		pPar->ReadDeviceSettings(fromQsettings);
	}
	emit ParamJustInterpretSettings();
}

QList<GParam*> GDevice::ChildParams() const
{
	QList<GParam*> listParamChildren;
	foreach(QObject* pObj, QObject::children()) {
		GParam* pPar = qobject_cast<GParam*>(pObj);
		if(pPar)
			listParamChildren.append(pPar);
	}
	return listParamChildren;
}

void GDevice::Event_UniqueSystemIDChanged(const QString & newSystemID)
{
	AddToDeviceManager();
}

void GDevice::AddToDeviceManager()
{
	QString theUniqueID = UniqueSystemID();
	// put it in the device manager
	if(DeviceManagerInstance()->Contains(theUniqueID))
		qWarning() << "GDevice has not been given a unique identifier:" << theUniqueID << "already exists";
	else
		DeviceManagerInstance()->Add(this);
}

QString GDevice::ShelfName() const
{
	QString theNameForShelving = metaObject()->className();
	QRegExp capitals("[A-Z0-9]");
	// remove a "G" at the beginning if it is followed by another capital
	if(theNameForShelving.indexOf("G") == 0 && capitals.indexIn(theNameForShelving, 1))
		theNameForShelving.remove(0, 1);
	// let's separate the capitalized worlds
	int pos = 1;
	while((pos = capitals.indexIn(theNameForShelving, pos)) > 0) {
		// if it is several numbers in a row we should probably not break it down with spaces
		if(pos - 1 >= 0 && theNameForShelving[pos].isNumber() && theNameForShelving[pos - 1].isNumber()) {
			pos++;
			continue;
		}
		theNameForShelving.insert(pos, " ");
		pos += 2;
	}

	return theNameForShelving;
}

void GDevice::DragEnterEvent( QDragEnterEvent *event, GDeviceWidget* pOnWidget )
{
	event->ignore();
}

void GDevice::DropEvent( QDropEvent *event, GDeviceWidget* pOnWidget )
{
	event->ignore();
}
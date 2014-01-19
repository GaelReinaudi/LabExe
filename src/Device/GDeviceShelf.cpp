#include "GDeviceShelf.h"
#include "GDevice.h"
#include "GDeviceWidget.h"
#include "GDeviceListModel.h"
#include "GDeviceManager.h"												
#include<QPushButton>

GDeviceShelf::GDeviceShelf(QWidget *parent)
	: QWidget(parent)
	, m_UpdateOnShow(true)
{
	setupUi(this);

	// using qt demo config dialog

	m_PagesWidget = stackedWidget;

	// then this, to re-enable the drag&drop that was disabled by setMovement(QListView::Static)
	m_pListDevicesWidget->setDragDropMode(QAbstractItemView::DragOnly);
      
	connect(m_pListDevicesWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(ChangePage(QListWidgetItem*,QListWidgetItem*)));
	m_pListDevicesWidget->setCurrentRow(0);

	QPushButton *closeButton = new QPushButton(tr("Close"));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	setWindowTitle(tr("Device shelves"));
}

GDeviceShelf::~GDeviceShelf()
{

}

void GDeviceShelf::ChangePage(QListWidgetItem* current, QListWidgetItem* previous)
{
	if(!current)
		current = previous;

	m_PagesWidget->setCurrentIndex(m_Map_ListItem_PageIndex.value(current));
}

void GDeviceShelf::AddDevices( QList<GDevice*> listDevices, bool WarnIfAlreadyOnShelf /*= true*/ )
{
	foreach(GDevice* pDev, listDevices) {
		AddDevice(pDev, WarnIfAlreadyOnShelf);
	}
}

void GDeviceShelf::AddDevice( GDevice* pDevice, bool WarnIfAlreadyOnShelf /*= true*/ )
{
	if(!pDevice) {
		qWarning() << "Trying to shelf a device having a null pointer. 9722286.";
		return;
	}
	if(WarnIfAlreadyOnShelf && m_ListShelvedDevices.contains(pDevice)) {
		qDebug() << "Device\"" << pDevice->Name() << "\" was already on the shelf.";
		return;
	}
	
	// make a new item, for which we will set the Qt::UserRole data for drag and drop capabilities
	QListWidgetItem* pDevItem = new QListWidgetItem(m_pListDevicesWidget);
	pDevItem->setData(Qt::UserRole, QVariant::fromValue(pDevice->UniqueSystemID()));

//	pDevItem->setIcon(QIcon("../../src/images/phidgets.png"));
	QString theText = pDevice->Name();
	pDevItem->setText(theText);
	pDevItem->setTextAlignment(Qt::AlignHCenter);
	pDevItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);

	QGroupBox* pDevGroupBox = new QGroupBox(pDevice->Name());
	int theNewIndex = m_PagesWidget->addWidget(pDevGroupBox);
	
	m_Map_ListItem_PageIndex.insert(pDevItem, theNewIndex);
	
	// let's add it in the m_ListShelvedDevices, used to keep track of shelved devices and for updates
	m_ListShelvedDevices.append(pDevice);

	QBoxLayout *vbox = new QVBoxLayout(pDevGroupBox);
	vbox->setContentsMargins(0, 0, 0, 0);
	// add a device widget
	GDeviceWidget* pDevWid = pDevice->ProvideNewDeviceGroupBox(pDevGroupBox);
	vbox->addWidget(pDevWid);
	QWidget* pExtraSettings = pDevice->ProvideNewExtraSettingsWidget(pDevGroupBox);
	if(pExtraSettings)
		vbox->addWidget(pExtraSettings);

	// if there are subdevices we should add the device to the shelf and query the list of subdevices and embed them in a new children shelf.
	if(pDevice->HasSubDevices()) {
		GDeviceShelf* pSubShelf = new GDeviceShelf(pDevGroupBox);
		pDevWid->layout()->addWidget(pSubShelf);
		pSubShelf->AddDevices(pDevice->SubDevices());
		pSubShelf->show();
	}
	else
		vbox->addStretch();
}

void GDeviceShelf::FillWithProgDeviceClasses()
{
	// first, we will NOT want to update the shelf (thus cleaning it) because it is a shelf for GProgDevice (which will not be re-added by update who calls AddDevice()).
	m_UpdateOnShow = false;

	QStringList ProgDeviceClassList = DeviceManagerInstance()->RegisteredProgDevices();
	foreach(QString strClassName, ProgDeviceClassList) {
// TO DEBUG		// test to put a sample widget there
		GDevice* pDev = DeviceManagerInstance()->CreateNewProgDevice(strClassName, this);
		if(!pDev) {
			qWarning() << "Couldn't create the program device" << strClassName;
			continue;
		}
		QWidget* pDevGroupBox = pDev->ProvideNewDeviceGroupBox(m_PagesWidget);

		int theNewIndex = m_PagesWidget->addWidget(pDevGroupBox);

		// make a new item, for which we will set the Qt::UserRole data for drag and drop capabilities
		QListWidgetItem *pDevItem = new QListWidgetItem(m_pListDevicesWidget);
		pDevItem->setText(pDev->ShelfName());
		pDevItem->setData(Qt::UserRole, QVariant::fromValue(strClassName));
		pDevItem->setTextAlignment(Qt::AlignHCenter);
		pDevItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
// 		pDevItem->setIcon(QIcon("../../src/images/save.png"));

		m_Map_ListItem_PageIndex.insert(pDevItem, theNewIndex);
	}
	m_pListDevicesWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void GDeviceShelf::CleanUp()
{
	m_pListDevicesWidget->clear();

	int nPage = m_PagesWidget->count();
	for(int i = 0; i < nPage; i++) {
		QWidget* pWid = m_PagesWidget->widget(0);
		m_PagesWidget->removeWidget(pWid);
		if(pWid)
			delete pWid;
	}

	m_ListShelvedDevices.clear();
}

void GDeviceShelf::Update()
{
	if(!m_UpdateOnShow)
		return;
	QList<GDevice*> listDevices = m_ListShelvedDevices;
	CleanUp();
	AddDevices(listDevices, false);
}

void GDeviceShelf::showEvent( QShowEvent * event )
{
	// if this is the very top shelf showing up, Update()!
	if(parentWidget())
		return;
	Update();
	QWidget::showEvent(event);
}

void GDeviceShelf::Show()
{
	show();
	QApplication::setActiveWindow(this);
}

#include "GBenchDockWidget.h"
#include "Device/GDeviceWidget.h"
#include "Device/GDevice.h"
#include "GLabCommand.h"
#include "GWorkBench.h"
#include "ToolBox/GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GBenchDockWidget, QWidget)

GBenchDockWidget::GBenchDockWidget(QWidget *parent, Qt::DockWidgetArea initialDockingPosition /*= Qt::NoDockWidgetArea*/)
	: QDockWidget(parent)
	, m_pBench(qobject_cast<GWorkBench*>(parent))
{
	setWindowTitle("na");
	setObjectName("na");

	setContentsMargins(0, 0, 0, 0);

	if(!m_pBench)
		return;

	if(initialDockingPosition == Qt::NoDockWidgetArea) {
		m_pBench->addDockWidget(Qt::RightDockWidgetArea, this);
	} // called when dropping a device on the bench
	else if(initialDockingPosition == Qt::AllDockWidgetAreas) {
		m_pBench->addDockWidget(Qt::RightDockWidgetArea, this);
		setFloating(true);
	}
	else {
		m_pBench->addDockWidget(initialDockingPosition, this);
	}
	m_pBench->viewMenu->addAction(toggleViewAction());

	// by default the dock will be destroyed on close
	// this will be set to false when the first device is put in the dock
    setAttribute(Qt::WA_DeleteOnClose, true);
}

void GBenchDockWidget::InsertDeviceWidget(GDeviceWidget* pDevWid)
{
	if(!pDevWid)
		return;
	// the device
	GDevice* pTheDevice = pDevWid->Device();
	if(!pTheDevice)
		return;
	// now we insert the widget
	// if this is the first widget of the dock, we insert it as the main widget of the dock
	int numDev = EmbeddedDevices().count();
	switch(numDev) {
	case 0:
		setWidget(pDevWid);
		setWindowTitle(pTheDevice->Name());
//		setObjectName(pTheDevice->UniqueSystemID());
		setAttribute(Qt::WA_DeleteOnClose, false); // see the constructor
		connect(pTheDevice, SIGNAL(NameChanged(QString)), this, SLOT(ChangeTitle(QString)));
		break;
	default:
		qCritical() << "Case not yet handled.  81641";
		break;
	}
	// TO CONTINUE
	// in the future, we could create a GDevicePanel and put the device widgets in it

	m_Devices.append(pTheDevice);

	// and take appropriate measure when the device widget is deleted
    auto con = connect(pDevWid, &GDeviceWidget::destroyed, this, [this, pTheDevice](){ this->DeviceWidgetRemoved(pTheDevice); });
    connect(this, &QObject::destroyed, [con](){ disconnect(con); });
}

void GBenchDockWidget::DeviceWidgetRemoved( GDevice* pTheDevice )
{
	// remove from the dock
	m_Devices.removeOne(pTheDevice);
	// what to do next
	int numDev = EmbeddedDevices().count();
	switch(numDev) {
	case 0: // delete the Dock since it is empty of devices
		deleteLater();
		// change the freaking name of the dock to "" so that it doesn't get saved 
		// and restored every time you close/open the program, accumulating data 
		// in the saveState() section of the bench.
		setObjectName("");
		break;
	}
}

void GBenchDockWidget::PopulateSettings( QSettings& inQsettings )
{
	// set the freaking name of the dock to ... so that it doesn't accumulate data 
	// in the saveState() section of the bench.
	QString dockObjName("Dock-%1");
	setObjectName(dockObjName.arg(m_pBench->BenchDynamicDocks().indexOf(this)));

	inQsettings.setValue("objectName", objectName());
	inQsettings.setValue("title", windowTitle());
// 	inQsettings.setValue("Embedded-Devices", EmbeddedDevicesID());
}

void GBenchDockWidget::InterpretSettings( QSettings& fromQsettings )
{
	setObjectName( fromQsettings.value("objectName").toString());
	setWindowTitle( fromQsettings.value("title").toString());
// 	QStringList devlist = fromQsettings.value("Embedded-Devices").toStringList();
}

QList<GDevice*> GBenchDockWidget::EmbeddedDevices() const
{
    auto container = m_Devices;
    std::sort(container.begin(), container.end());
    container.erase(std::unique(container.begin(), container.end()),
                    container.end());
    return container;
}

QStringList GBenchDockWidget::EmbeddedDevicesID() const
{
	QStringList strList;
	foreach(GDevice* pDev, EmbeddedDevices()) {
		if(pDev)
			strList << pDev->UniqueSystemID();
	}
	return strList;
}

void GBenchDockWidget::ChangeTitle( QString newTitle )
{
	setWindowTitle(newTitle);
}

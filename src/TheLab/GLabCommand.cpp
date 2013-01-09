#include "GLabCommand.h"
#include "GWorkBench.h"
#include "Device/GDevice.h"
#include "Device/GDeviceWidget.h"

GCommandAddDevice::GCommandAddDevice( GDevice* theDevice, GWorkBench* theLab, QUndoCommand *parent /*= 0*/ )
: QUndoCommand(parent)
, m_pAddedWidget(0)
, m_pInWhichWidget(0)
{
	m_pDevice = theDevice;
	m_pBench = theLab;
}

GCommandAddDevice::GCommandAddDevice( GDevice* theDevice, GWorkBench* theLab, QWidget* pForWhichWidget, QUndoCommand *parent /*= 0*/ )
: QUndoCommand(parent)
, m_pAddedWidget(0)
{
	m_pDevice = theDevice;
	m_pBench = theLab;
	m_pInWhichWidget = pForWhichWidget;
}

GCommandAddDevice::~GCommandAddDevice()
{

}

void GCommandAddDevice::undo()
{
	if(!m_pBench || !m_pDevice)
		return;
	m_pBench->RemoveDevice(m_pDevice, false);
	setText(QObject::tr("Add device \"%1\"").arg(m_pDevice->Name()));
}

void GCommandAddDevice::redo()
{
	if(!m_pBench || !m_pDevice)
		return;
	// if it is already in the workbench
	if(m_pBench->m_MapDevicesWidgets.contains(m_pDevice)) {
		qWarning("%s : This device is already in the Lab.", m_pDevice->Name());
		return;
	}
	// get a new device widget
	m_pAddedWidget = m_pDevice->ProvideNewDeviceGroupBox(m_pBench);
	if(!m_pAddedWidget)
		return;

	m_pBench->m_MapDevicesWidgets.insert(m_pDevice, m_pAddedWidget);

	m_pBench->ConnectAndShowDeviceWidget(m_pAddedWidget, m_pInWhichWidget);

	// lets inform the workbench when a widget has been moved by hand
	// m_pLab->connect(pDevWid, SIGNAL(FinishedMoveByHand(QPoint)), m_pLab, SLOT(DeviceWidgetMovedByHand(QPoint)));
	setText(QObject::tr("Add device \"%1\"").arg(m_pDevice->Name()));
}

GCommandRemoveDevice::GCommandRemoveDevice( GDevice* theDevice, GWorkBench* theLab, QUndoCommand *parent /*= 0*/ )
	: m_pBench(theLab)
	, m_pDevice(theDevice)
{
}

GCommandRemoveDevice::~GCommandRemoveDevice()
{
}

void GCommandRemoveDevice::undo()
{
	foreach(GDeviceWidget* pDevWid, m_DeviceWidgetsAndParents.keys()) {
		GDeviceWidget* NewpDevWid = m_pDevice->ProvideNewDeviceGroupBox(m_pBench);
		m_pBench->m_MapDevicesWidgets.insert(m_pDevice, NewpDevWid);
		m_pBench->ConnectAndShowDeviceWidget(NewpDevWid, m_DeviceWidgetsAndParents.value(pDevWid));
		NewpDevWid->move(m_DeviceWidgetsAndPosition.value(pDevWid));
		if(m_DeviceWidgetsWasShown.value(pDevWid))
			NewpDevWid->show();
	}
	setText(QObject::tr("Remove device \"%1\"").arg(m_pDevice->Name()));
}

void GCommandRemoveDevice::redo()
{
	m_DeviceWidgetsAndParents.clear();
	m_DeviceWidgetsAndPosition.clear();
	m_DeviceWidgetsWasShown.clear();
	QList<GDeviceWidget*> listAllDevWid = m_pBench->m_MapDevicesWidgets.values(m_pDevice);
	foreach(GDeviceWidget* pDevWid, listAllDevWid) {
		m_DeviceWidgetsAndParents.insert(pDevWid, pDevWid->parentWidget());
		m_DeviceWidgetsAndPosition.insert(pDevWid, pDevWid->pos());
		m_DeviceWidgetsWasShown.insert(pDevWid, !pDevWid->isHidden());
	}
	// cleaning the device's widget(s) from the user interface.
	m_pBench->m_MapDevicesWidgets.remove(m_pDevice);
	foreach(GDeviceWidget* pDevWid, m_DeviceWidgetsAndParents.keys())
//		pDevWid->setParent(0);
		delete pDevWid;
	setText(QObject::tr("Remove device \"%1\"").arg(m_pDevice->Name()));
}
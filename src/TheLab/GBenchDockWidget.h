#ifndef GBENCHDOCKWIDGET_H
#define GBENCHDOCKWIDGET_H
#include "labexe_global.h"

#include "ToolBox/GSerializable.h"
#include <QDockWidget>

class GDevice;
class GDeviceWidget;
class GWorkBench;

/////////////////////////////////////////////////////////////////////
//! \brief The GBenchDockWidget class defines a dock widget that works with a workbench. 
/*!
It is a QDockWidget that can contain one (and more in the future...) GDeviceWidget object.
The devices that are handled by a GBenchDockWidget (through a GDeviceWidget provided by ProvideNewDeviceGroupBox()) is returned by EmbeddedDevices().

This class will:
- insert a device widget in its gui, as the main widget if there is only one device (in the future, there will bw support for more than one device).
- destroy itself when its last device has been removed.
- add a checkable menu entry in the View menu of its workbench.
- be serialized and de-serialized by the workbench (through save state).
- the bench will also de-serialize the EmbeddedDevices().
*/
class LABEXE_EXPORT GBenchDockWidget : public QDockWidget, public GSerializable
{
	Q_OBJECT

public:
	GBenchDockWidget(QWidget *parent, Qt::DockWidgetArea initialDockingPosition = Qt::NoDockWidgetArea);
	~GBenchDockWidget();

public:
	//! gives the list of devices that have a deviceWidget in this Dock
	QList<GDevice*> EmbeddedDevices() const;
	//! Gives the string list of UniqueID that corresponds to EmbeddedDevices()
	QStringList EmbeddedDevicesID() const;
	//! inserts the device widget in this dock
	void InsertDeviceWidget(GDeviceWidget* pDevWid);

protected:
	//! Re-implemented
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented
	virtual void InterpretSettings(QSettings& fromQsettings);

protected slots:
	//! called by the destruction of a device widget. pObjDev should be the pointer to the Device()
	void DeviceWidgetRemoved(QObject* pObjDev);
	//! called to change the title
	void ChangeTitle(QString newTitle);

private:
	//! the parent bench
	GWorkBench* m_pBench;
	//! the devices embedded in this dock
	QList<GDevice*> m_Devices;
};

#endif // GBENCHDOCKWIDGET_H

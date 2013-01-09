#ifndef GWORKBENCH_H
#define GWORKBENCH_H
#include "labexe_global.h"

#include "ToolBox/GSerializable.h"
#include "GWorkBenchManager.h"
#include <QMainWindow>
#include <QUndoStack>

namespace Ui {class GWorkBenchClass;};
class GDevice;
class GDeviceWidget;
class GBenchDockWidget;

/////////////////////////////////////////////////////////////////////
//! \brief The GWorkBench class defines a workbench. 
/*!
It is a QMainWindow that is used to display devices ui.
It can :
- Add() or Remove() devices from its UI, 
- allow the Drag and Drop of GDevices from a GDeviceListWidget (or GDeviceListModel).
- allow the docking of devicewidgets in GBenchDockWidget`s.
- Saves some settings and the settings of the devices. For this, you have to re-implement PopulateSettings(QSettings&) and InterpretSettings(QSettings&).
*/
class LABEXE_EXPORT GWorkBench : public QMainWindow, public GSerializable
{
	Q_OBJECT

public:
	GWorkBench(QWidget *parent);
	virtual ~GWorkBench();

	//! Returns a list of the devices in the same order as they were added historically.
	QList<GDevice*> DeviceList();

public slots:
	//! Add the device using the undo/redo framework if putOnStack = true.
	virtual GDeviceWidget* AddDevice(GDevice* pTheDevice, QPoint whereInParent = QPoint(0, 0), bool putOnStack = true);
	//! Add the device in the bench, but create a docking widget for it.
	virtual GBenchDockWidget* AddDeviceInNewDock(GDevice* pTheDevice, bool putOnStack = true);
	//! Add the device in an existing dock.
	virtual void AddDeviceInDock(GDevice* pTheDevice, GBenchDockWidget* pDock, bool putOnStack = true);
	//! Removes the GDevice object to the list of GDevice's managed by the GWorkBench.
	virtual void RemoveDevice(GDevice* pTheDevice, bool putOnStack = true);
	//! Removes the GDevice using its uniqueIdentifierName.
	void RemoveDevice(QString uniqueIdentifierName);
	//! Saves the workbench to file
	bool saveFile(const QString &fileName);
	//! Exports a screenshot of the bench to file
	void ExportScreenShot(const QString & screenFileName);

protected:
	//! Returns the device widget corresponding to the pTheDevice in this bench
	GDeviceWidget* DeviceWidget(GDevice* pTheDevice) {return m_MapDevicesWidgets.value(pTheDevice);}
	//! Re-implemented
	void closeEvent(QCloseEvent *event);
	//! Re-implemented in order to accept dropping in this widget
	void dragEnterEvent(QDragEnterEvent *event); 
	//! Re-implemented in order to accept dropping in this widget
	void dropEvent(QDropEvent *event);

protected slots:
	//! Called to provide a specific "latter" initialization: when the program goes back to the main event loop after the creation of this bench. Default implementation does nothing.
	virtual void LatterBenchInitialization() {};

protected:
	//! Re-implemented.
	virtual void PopulateSettings( QSettings& inQsettings );
	//! Re-implemented.
	virtual void InterpretSettings( QSettings& fromQsettings );

signals:
	//! emitted when the bench saved itself in the given filePath
	void Saved(QString filePath = "");

private:
	void ConnectAndShowDeviceWidget(GDeviceWidget* pDevWid, QWidget* pInWhichWidget = 0);
	//! gives the list of the GBenchDockWidget`s in this bench
	QList<GBenchDockWidget*> BenchDynamicDocks();
	void createActions();
	void createMenus();
	bool maybeSave();
	void setCurrentFile(const QString &fileName);
	QMenu *fileMenu;
	QMenu *viewMenu;
	QAction *saveAction;
	QAction *saveAsAction;
	QString curFile;

private slots:
	void InitBench();
	bool save();
	bool saveAs();
	// 	// creates and insert a new dock in the bench and adds some associated features to the menu
// 	GBenchDockWidget* CreateNewDockWidget();

// signals:
// 	//! Emitted when a Device has been dropped in this bench
// 	void DeviceDropped(GDevice* pDevice, QPoint thePos);

protected:
	Ui::GWorkBenchClass *pUiBench;
private:
	//! For keeping track of when a device was added. The order of reloading might indeed matter a lot.
	QMap<GDevice*, QString> m_DateDeviceAdded;
	//! For keeping track of which widget is of which device
	QMultiMap<GDevice*, GDeviceWidget*> m_MapDevicesWidgets;
	//! A stack of QCommand and other stuffs for the undo/redo framework.
	QUndoStack m_UndoStack;

friend class GSequencerBench;
friend class GSequencingBench;
friend class GImagingBench;
friend class GOptimizingBench;
friend class GCommandAddDevice;
friend class GCommandRemoveDevice;
friend class GLabControlPanel;
friend class GBenchDockWidget;
};


#endif // GWORKBENCH_H

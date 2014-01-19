#ifndef GDEVICESHELF_H
#define GDEVICESHELF_H
#include "labexe_global.h"

#include <QtWidgets>
#include "ui_GDeviceShelf.h"

class GDevice;

using namespace Ui;

/////////////////////////////////////////////////////////////////////
//! \brief The GDeviceShelf defines a widget in  which we can "shelve" GDevice`s objects.
/*!
Shelving means:
- displaying in an intuitive way the devices registered by the system
- giving basic information about the devices in use (to come soon actually)
- providing drag and drop capabilities to put GDevice`s objects on GWorkBenches
*/
class GDeviceShelf : public QWidget, public GDeviceShelfClass
{
	Q_OBJECT

public:
	GDeviceShelf(QWidget *parent = 0);
	~GDeviceShelf();

	//! add one device on the shelf
	void AddDevice(GDevice* pDevice, bool WarnIfAlreadyOnShelf = true);
	//! add the devices on the shelf
	void AddDevices(QList<GDevice*> listDevices, bool WarnIfAlreadyOnShelf = true);
	//! add the GProgDevice classes name on the shelf (the one registered with G_REGISTER_NEW_PROG_DEVICE_CLASS). Those are not object pointers, but will be useful if a bench needs a new instance of such a class (using the device manager).
	void FillWithProgDeviceClasses();

public slots:
	void ChangePage(QListWidgetItem *current, QListWidgetItem *previous);

protected slots:
	//! Updates the devices by calling CleanUp() then AddDevice() using the devices previously shelved
	void Update();
	//! Show the shelf an makeit the active window
	void Show();

protected:
	//! re-implemented
	void showEvent( QShowEvent * event );

private:
	//! Cleans up the shelf
	void CleanUp();

	//! the devices shelved
	QList<GDevice*> m_ListShelvedDevices;
	QMap<QListWidgetItem*, int> m_Map_ListItem_PageIndex;
	QStackedWidget* m_PagesWidget;
	// useful for NOT updating the shelf (thus cleaning it) when it is a shelf for GProgDevice
	bool m_UpdateOnShow;
};

#endif // GDEVICESHELF_H

#ifndef GLABCONTROLPANEL_H
#define GLABCONTROLPANEL_H
#include "labexe_global.h"

#include <QMainWindow>
#include <QDir>
#include <QObjectCleanupHandler>
#include "ToolBox/GSerializable.h"
namespace Ui {class GLabControlPanelClass;};
class GWorkBench;
class QListWidgetItem;
class QListWidget;
class QDockWidget;
class GDeviceShelf;
class GDevice;

/////////////////////////////////////////////////////////////////////
//! \brief The GLabControlPanel class is the access point for starting various environment in the lab.
/*!
It can :
- display or hide a GDeviceShelf, where all the declared devices are stored so that you can use it on workbenches
- create or recall GWorkBench's, where you can slide devices
- start instances of userdefined environment, like for a camera interface.
*/
class LABEXE_EXPORT GLabControlPanel : public QMainWindow//, public GSerializableSettings
{
	Q_OBJECT

public:
	GLabControlPanel(QWidget *parent = 0);
	virtual ~GLabControlPanel();

	//! adds the device on the hardware device shelf
	void AddDeviceToShelf(GDevice* pDev);

public slots:
	//! Produces a new GWorkBench environment. Will ask what type of bench to make. see G_REGISTER_WORKBENCH_CLASS().
	void MakeNewWorkBench();
	//! Adds a workbench by reading a file. If strFilePath is not provided, a file dialog will be opened. Returns the created workbench, 0 if fails.
	GWorkBench* AddWorkBenchFromFile(QString strFilePath = "", bool thenShow = true);
	//! Removes a WorkBench from the list. Uses sender()->parent() to figure out the QListWidget that emitted the delete signal.
	void DeleteWorkBenchItem();
	//! should be re-implemented in order to look for an upadte of the program. Default does nothing.
	virtual void CheckForUpdate() {}
	//! checks if a guided tour should be started and does it if so. default does nothing for now.
	virtual void GuidedTourWorkBench() {}

protected:
	//! reimplemented, called before closing
	void closeEvent(QCloseEvent *event);
	//! saves the benches in the QSettings object
	virtual void SaveBenches(QSettings &panelSettings);
	//! restores the benches from the QSettings object
	virtual void RestoreBenches(QSettings &panelSettings);
	//! Inserts a name and pointer of a WorkBench in the pListWorkBenches.
	virtual void InsertWorkBenchInList(GWorkBench* pWB);
	//! returns the debugging dock widget
	QDockWidget* DebugDockWidget();
	//! returns the list widget that holds the list of active workbenches
	QListWidget* WorckbenchListWidget();

private slots:
	//! Save the control panel settings to file
	void Save();

	//! Save the control panel and all the workbenches settings
	void SaveAllWorkbenches();
	//! Save the control panel and all the workbenches settings in the designated folder. If justExportCopy, then the current saving file paths are not changed.
	void SaveAllAs(bool justExportCopy = false, const QString & folderPath = "");
	//! Exports the screen shot of the active workbenches in a tree structure of sub-folders in the folder designated by SaveAllAs()
	void ExportScreenshots();
	//! makes a screen shot of the whole desktop and saves it if the fileSaveName is given.
	QPixmap ScreenShotDesktop(const QString & fileSaveName = "");
	//! Shows the corresponding workbench if any.
	void ShowWorkBench(QListWidgetItem* pItem);
	//! Called after the construction of the class because you can't call virtual functions from a constructor. 
	void InitializeAfterConstructor();

	//! This slot is called via Qt::QueuedConnection by the message handler so that messages from other threads can be sent to the main thread and be displayed.
	void ToStaticDebugMessageHandler(int type, QByteArray messageByteArray) const;;

signals:
	//! this signal is connected via Qt::QueuedConnection to call ToStaticDebugMessageHandler() so that messages from other threads can be sent to the main thread and be displayed.
	void OtherThreadDebugMessage(int type, QByteArray messageByteArray);

private:
	//! For convenience, gives back the GWorkBench that corresponds to the QListWidgetItem
	GWorkBench* WorkBenchFromListItem( QListWidgetItem* pItem );
	//! A special message function to handle Debug, Warning, Critical and Fatal messages from Qt. See qInstallMsgHandler in Qt's doc.
	static void DebugMessageHandler(QtMsgType type, const char *msg);
	// This is the class name of the instance of the control panel. ex: "SrYbExe", or "WinLabExe".
	QString ProgramName_DontUseDuringConstruction();

private:
	QDir m_DefaultSavingDir;
	Ui::GLabControlPanelClass *ui;
	//! Custom item role so that we can save e.g. pointers in the item of a QListWidgetItem.
	enum GItemRole {
		WorkBenchPointer = 100,
		SettingsFile,
		kErroafasfafafafagdarMalfoadgagdrmedInput,
	};
	//! The main device shelf of the lab.
	GDeviceShelf* m_pDeviceShelf;
	//! The e-device shelf: the shelf where the GProgDevice`s should appear
	GDeviceShelf* m_pProgDeviceShelf;
	//! instance of this class used for debug message connection from other threads
	static GLabControlPanel* m_LabInstance;
	//! tells to accept debug messages
	static bool m_AcceptDebugMessages;

	//! intended to help cleanup when creating Workbenches with parent = 0
	mutable QObjectCleanupHandler m_CleanUpWorkBench;

};	

#endif // GLABCONTROLPANEL_H

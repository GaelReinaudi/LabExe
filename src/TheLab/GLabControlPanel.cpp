#include "GLabControlPanel.h"
#include "GWorkBenchManager.h"
#include "Param/GParamManager.h"
#include "Param/GParam.h"
#include "Device/GDeviceManager.h"												
#include "device.h"
#include "Device/GDeviceShelf.h"
#include "ui_GLabControlPanel.h"

#include "GWorkBench.h"
#include <QDir>

bool m_CloseWorkBenchWhenDeletedFromList = true;
GLabControlPanel* GLabControlPanel::m_LabInstance = 0;

// #include "Sequencer/GLabSequencer.h"

QTextEdit* pGlobalTextEditToDisplayDebugMessages = 0;
int NumDebugMessages = 0;
bool GLabControlPanel::m_AcceptDebugMessages = true;

GLabControlPanel::GLabControlPanel(QWidget *parent)
	: QMainWindow(parent)
	, m_DefaultSavingDir(QDir::home())
	, m_pProgDeviceShelf(new GDeviceShelf(0))
{
    ui = new Ui::GLabControlPanelClass();
	ui->setupUi(this);
	ui->pMenuView->addAction(DebugDockWidget()->toggleViewAction());

	// device manager shelf:
	m_pDeviceShelf = DeviceManagerInstance()->HardwareDeviceShelf();

	connect(ui->actionSaveAllWorkbenches, SIGNAL(triggered()), this, SLOT(SaveAllWorkbenches()));
	connect(ui->actionSave_all_as, SIGNAL(triggered()), this, SLOT(SaveAllAs()));
	connect(ui->actionExport_wb_Screenshots, SIGNAL(triggered()), this, SLOT(ExportScreenshots()));
	connect(ui->actionCheck_for_update, SIGNAL(triggered()), this, SLOT(CheckForUpdate()));

	pGlobalTextEditToDisplayDebugMessages = ui->pDebugTextEdit;
	// to handle Qt warning and other messages (like the one generated when qDebug, qWarning and qCritical are used):
	//	QErrorMessage::qtHandler();
	// or this, for more flexible behavior, like sending Debug messages to a special QTextEdit.
    qInstallMessageHandler(GLabControlPanel::DebugMessageHandler);
    connect(this, SIGNAL(OtherThreadDebugMessage(int, QString)), this, SLOT(ToStaticDebugMessageHandler(int, QString)), Qt::QueuedConnection); // queued!
	m_LabInstance = this;

	connect(ui->pButtonCreateNewWorkBench, SIGNAL(clicked()), this, SLOT(MakeNewWorkBench()));
	connect(ui->pWorkBenchFromFile, SIGNAL(clicked()), this, SLOT(AddWorkBenchFromFile()));

	// create the shortcut after the list widget has been created
	// pressing DEL activates the deleteItem() slot only when list widget has focus
	QShortcut* DelShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->pListWorkBenches);
	connect(DelShortcut, SIGNAL(activated()), this, SLOT(DeleteWorkBenchItem()));
	// and double click shows the workbench
	connect(ui->pListWorkBenches, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(ShowWorkBench(QListWidgetItem*)));

	// to open the device shelves
	connect(ui->pButtonDeviceShelf, SIGNAL(clicked()), m_pDeviceShelf, SLOT(Show()));
	connect(ui->pButtonProgDeviceShelf, SIGNAL(clicked()), m_pProgDeviceShelf, SLOT(Show()));

	// nooooooo:	No Call to a virtual function in c-tor !  InitializeAfterConstructor(); NO!
	// yeeeeees: initialization through a queued connection, meaning it will happen after this constructor, once the program comes back to the event loop.
	QTimer::singleShot(0, this, SLOT(InitializeAfterConstructor()));

	// Opening guided tour
	QTimer::singleShot(1000, this, SLOT(GuidedTourWorkBench()));
}

GLabControlPanel::~GLabControlPanel()
{
	m_AcceptDebugMessages = false;
	pGlobalTextEditToDisplayDebugMessages = 0;
	delete ui;
	delete m_pProgDeviceShelf;
	// clears the devices that were added to the shelf (device manager). Indeed, the device manager destructor is not called directly because of the fact that it is from the loki library.
	DeviceManagerInstance()->m_CleanUpDevices.clear();
}

void GLabControlPanel::InitializeAfterConstructor()
{
	QString strProgramName = ProgramName_DontUseDuringConstruction();
	QCoreApplication::setOrganizationName("LabExe");
	QCoreApplication::setApplicationName(strProgramName);

	// restore some settings
	QSettings windowSettings(strProgramName, "GLabControlPanel");
	restoreGeometry(windowSettings.value("geometry").toByteArray());
	restoreState(windowSettings.value("state").toByteArray());
	m_pDeviceShelf->restoreGeometry(windowSettings.value("shelf-geometry").toByteArray());
	m_pProgDeviceShelf->restoreGeometry(windowSettings.value("e-shelf-geometry").toByteArray());
	QString defaultpath = windowSettings.value("default-directory").toString();
  	if(!defaultpath.isEmpty())
  		m_DefaultSavingDir.setPath(defaultpath);

	// for the e-device shelf, we 
	m_pProgDeviceShelf->FillWithProgDeviceClasses();

	// load the workbenches by getting the filenames where they are saved
	QString fileNameControlPanel = strProgramName + "_ControlPanel.ini";

	QString pathSaveControlPanel = m_DefaultSavingDir.absoluteFilePath(fileNameControlPanel);
	QSettings panelSettings(pathSaveControlPanel, QSettings::IniFormat);

	RestoreBenches(panelSettings);

	// now we get some properties of some params. 
	ParamManagerInstance()->ReadDeviceSettings(panelSettings);

	// hide the debugging combobox if the first line is not commented
	ui->pDevicesComboBox->hide();
	ui->pDevicesComboBox->view()->setDragDropMode(QAbstractItemView::DragOnly);
	ui->pDevicesComboBox->setModel(new GDeviceListModel(ui->pDevicesComboBox));
	// Let's populate the combo with names and the pointer to the corresponding GDevice's. (We use QVariant for that).
	foreach(GDevice* pDev, DeviceManagerInstance()->AllDevices())
		ui->pDevicesComboBox->addItem(pDev->Name(), QVariant::fromValue(pDev->UniqueSystemID()));
}

void GLabControlPanel::Save()
{
	QString strProgramName = ProgramName_DontUseDuringConstruction();

	// records some settings in the registry
	QSettings windowSettings(strProgramName, "GLabControlPanel");
	windowSettings.setValue("geometry", saveGeometry());
	windowSettings.setValue("state", saveState());
	windowSettings.setValue("shelf-geometry", m_pDeviceShelf->saveGeometry());
	windowSettings.setValue("e-shelf-geometry", m_pProgDeviceShelf->saveGeometry());
	windowSettings.setValue("default-directory", m_DefaultSavingDir.absolutePath());

	// some settings into a file in the m_DefaultSavingDir
	QString fileNameControlPanel = strProgramName + "_ControlPanel.ini";
	QString pathSaveControlPanel = m_DefaultSavingDir.absoluteFilePath(fileNameControlPanel);
	QSettings panelSettings(pathSaveControlPanel, QSettings::IniFormat);
	panelSettings.remove("");

	SaveBenches(panelSettings);

	// now we save some properties of some params using the ParamManager. 
	ParamManagerInstance()->SaveDeviceSettings(panelSettings);
}

void GLabControlPanel::closeEvent(QCloseEvent *event)
{
	// since this is closing, we want to avoid multiple saving of the same thing
	foreach(QListWidgetItem* pitemWB, ui->pListWorkBenches->findItems("*", Qt::MatchWildcard)) {
		GWorkBench* pWB = WorkBenchFromListItem(pitemWB);
		if(pWB) {
			disconnect(pWB, SIGNAL(Saved()), this, SLOT(Save()));
		}
	}

	Save();
	qApp->closeAllWindows();
	QTimer::singleShot(1000, qApp, SLOT(closeAllWindows()));

    qInstallMessageHandler(0);

	QMainWindow::closeEvent(event);
}

GWorkBench* GLabControlPanel::AddWorkBenchFromFile(QString strFilePath /*= ""*/, bool thenShow /*= true*/)
{
	if(strFilePath == ""){
		strFilePath = QFileDialog::getOpenFileName(this, tr("Open Workbench"), "", tr("Workbench (*.wb)"));
	}
	if(strFilePath == "") {
// 		qWarning() << "AddWorkBenchFromFile: The file wasn't found";
		return 0;
	}

	GWorkBench* pWbToRet = WorkBenchManagerInstance()->CreateNewWorkBenchFromFile(strFilePath, 0);
	// the parent is zero so we add the bench to the cleanup object so that it gets destroyed
	m_CleanUpWorkBench.add(pWbToRet);
	if(pWbToRet) {
		pWbToRet->SetFile(strFilePath);
		pWbToRet->ReadSettingsFromFile(strFilePath);
		InsertWorkBenchInList(pWbToRet);
		if(thenShow)
			pWbToRet->show();
	}
	return pWbToRet;
}

void GLabControlPanel::MakeNewWorkBench()
{
	// if there is more than one type of workbench, what kind of workbench
	QStringList items = WorkBenchManagerInstance()->RegisteredTypes();
	bool ok;
	QString strBenchType = QInputDialog::getItem(this, tr("Please chose a workbench type"), tr("Workbench type:"), items, 0, false, &ok);
	if(!ok)
		return;

	QString wbName = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm - ");
	bool bOK;
	wbName = QInputDialog::getText(this, "Name the new workbench", "descriptive name", QLineEdit::Normal, wbName, &bOK);
	if(!bOK)
		return;

	GWorkBench* pWB = WorkBenchManagerInstance()->CreateNewWorkBench(strBenchType, 0);
	// the parent is zero so we add the bench to the cleanup object so that it gets destroyed
	m_CleanUpWorkBench.add(pWB);
	if(!pWB)
		return;
	pWB->setObjectName(wbName);
	pWB->show();

	// insert the corresponding bench in the list
	InsertWorkBenchInList(pWB);
}

void GLabControlPanel::DeleteWorkBenchItem()
{
	// the parent of the sender QShortuct must have been set to the QListWidget containing the workbenches
	QListWidget* pParentListWidget = qobject_cast<QListWidget*>(sender()->parent());
	if(pParentListWidget)
		foreach(QListWidgetItem* pItem, pParentListWidget->selectedItems()) {
			// delete the corresponding GWorkBench if m_CloseWorkBenchWhenDeletedFromList
			bool ClosingSuccesful = true;
			if(m_CloseWorkBenchWhenDeletedFromList) {
				GWorkBench* pWB = WorkBenchFromListItem(pItem);
				if(pWB)
					ClosingSuccesful = pWB->close();
			}
			// remove the strBenchType from the list (unless the maybe-closing was requested but failed)
			if(ClosingSuccesful)
				pParentListWidget->takeItem(pParentListWidget->row(pItem));
	}
}

GWorkBench* GLabControlPanel::WorkBenchFromListItem( QListWidgetItem* pItem )
{
	QVariant varrrr = pItem->data(WorkBenchPointer);
	return qobject_cast<GWorkBench*>(varrrr.value<QObject*>());
}

void GLabControlPanel::InsertWorkBenchInList( GWorkBench* pWB )
{
	if(!pWB)
		return qCritical("The address of the workbench is 0");
	QListWidgetItem *newItem = new QListWidgetItem;
	newItem->setText(pWB->objectName());
	// set the link with the workbench
	QObject* pObj = pWB;
	newItem->setData(WorkBenchPointer, QVariant::fromValue(pObj));
	// add it in the list
	ui->pListWorkBenches->addItem(newItem);

	// makes the Save()`ing occur when a workbench saves in a given file
	connect(pWB, SIGNAL(Saved()), this, SLOT(Save()));
}

void GLabControlPanel::DebugMessageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
	if(!m_AcceptDebugMessages)
		return;
	// cannot be called from another thread than the main thread
	if(!QCoreApplication::instance())
		return;
	if(QThread::currentThread() != QCoreApplication::instance()->thread()) {
        QString messageByteArray(msg);
		emit m_LabInstance->OtherThreadDebugMessage(type, messageByteArray);
		return;
	}
	QErrorMessage* pErMess = new QErrorMessage(0);
	pErMess->setAttribute(Qt::WA_DeleteOnClose);

	switch (type) {
	 case QtDebugMsg:
		 NumDebugMessages++;
// #ifndef QT_NO_DEBUG
		 if(pGlobalTextEditToDisplayDebugMessages)
			 pGlobalTextEditToDisplayDebugMessages->append(QString("Debug %2: %1").arg(msg).arg(NumDebugMessages, 2));
// #endif
		 delete pErMess;
		 break;
	 case QtWarningMsg:
		 NumDebugMessages++;
// #ifndef QT_NO_DEBUG
		 if(pGlobalTextEditToDisplayDebugMessages)
			 pGlobalTextEditToDisplayDebugMessages->append(QString("Warning %2: %1").arg(msg).arg(NumDebugMessages, 2));
// #endif
		 delete pErMess;
		 break;
// 	 case QtWarningMsg:
// 		 pErMess->showMessage(QString("Warning: %1\n").arg(msg));
// 		 break;
	 case QtCriticalMsg:
		 pErMess->showMessage(QString("Critical: %1\n").arg(msg));
		 break;
	 case QtFatalMsg:
		 pErMess->showMessage(QString("Fatal: %1\n").arg(msg));
		 abort();
	}
}

void GLabControlPanel::ToStaticDebugMessageHandler( int type, QString messageByteArray ) const
{
    QMessageLogContext unused;
    DebugMessageHandler(static_cast<QtMsgType>(type), unused, messageByteArray);
}

void GLabControlPanel::SaveAllWorkbenches()
{
	Save();
	foreach(QListWidgetItem* pitemWB, ui->pListWorkBenches->findItems("*", Qt::MatchWildcard)) {
		GWorkBench* pWB = WorkBenchFromListItem(pitemWB);
		if(pWB) {
			pWB->save();
		}
	}
}

void GLabControlPanel::ShowWorkBench( QListWidgetItem* pItem )
{
	GWorkBench* pWB = WorkBenchFromListItem(pItem);
	if(!pWB)
		return;
	
	pWB->show();
	QApplication::setActiveWindow(pWB);
}

void GLabControlPanel::SaveAllAs(bool justExportCopy /*= false*/, const QString & folderPath /*= ""*/)
{
	//bug, crashes my program 60s after closing the dialog
	//QString folderName = QFileDialog::getExistingDirectory(this, tr("Select folder"), m_DefaultSavingDir.absolutePath());
	QString folderName = folderPath;
	if(folderPath.isEmpty()) {
		QFileDialog* pDialog = new QFileDialog(0, "Select folder", m_DefaultSavingDir.absolutePath());
		pDialog->setFileMode(QFileDialog::Directory);
		pDialog->setOption(QFileDialog::ShowDirsOnly);
		pDialog->exec();
		QStringList lFol = pDialog->selectedFiles();
		if(!lFol.isEmpty())
			folderName = lFol.at(0);
	}
	if(folderName.isEmpty())
		return;

	// list of filenames before we actually save them in case we must preserve the default file paths (justExportCopy = true)
	QMap<GWorkBench*, QString> benchesFileNamesWorkBenches;
	QString theDefaultSaveDirPAth = m_DefaultSavingDir.absolutePath();

	m_DefaultSavingDir.setPath(folderName);
	foreach(QListWidgetItem* pitemWB, ui->pListWorkBenches->findItems("*", Qt::MatchWildcard)) {
		GWorkBench* pWB = WorkBenchFromListItem(pitemWB);
		if(pWB) {
			// if there is no Settings(), we should ask you if you want to save
			if(!pWB->Settings())
				pWB->maybeSave();
			// and now, is there a Settings()?
			if(pWB->Settings()) {
				QString benchFileName = pWB->Settings()->fileName();
				benchesFileNamesWorkBenches[pWB] = benchFileName;
			}
			// the default file name if nothing better is proposed
			QString defaultNameSave = m_DefaultSavingDir.absoluteFilePath(pWB->objectName().remove(":") + ".wb");
			pWB->SetFile(defaultNameSave);
		}
	}
	// save everything
	SaveAllWorkbenches();
	//revert if justExportCopy
	if(justExportCopy) {
		m_DefaultSavingDir.setPath(theDefaultSaveDirPAth);
		foreach(GWorkBench* pWB, benchesFileNamesWorkBenches.keys()) {
			pWB->SetFile(benchesFileNamesWorkBenches[pWB]);
		}
	}
	// save everything
	SaveAllWorkbenches();
}

void GLabControlPanel::ExportScreenshots()
{
	QString folderName = QDateTime::currentDateTime().toString("yyyy/MM-MMM/yyyy-MM-dd/'conf'-HHmmss");
	if(!m_DefaultSavingDir.mkpath(folderName))
		return;
	QString foldPath = m_DefaultSavingDir.path() + QString("/") + folderName;
	foldPath = QDir::cleanPath(foldPath);
	QDir dirSave = QDir(foldPath);
	QString preDateName = QDateTime::currentDateTime().toString("yyyy-MM-dd-");

	SaveAllAs(true, foldPath);

	// goes through the list of workbenches to screen-shot them
	foreach(QListWidgetItem* pitemWB, ui->pListWorkBenches->findItems("*", Qt::MatchWildcard)) {
		GWorkBench* pWB = WorkBenchFromListItem(pitemWB);
		if(pWB) {
			QString screenFileName = preDateName + pWB->objectName().remove(":") + ".png";
			pWB->ExportScreenShot(dirSave.absoluteFilePath(screenFileName));
		}
		ScreenShotDesktop(dirSave.absoluteFilePath(preDateName + "desktop"));
	}
}

QPixmap GLabControlPanel::ScreenShotDesktop(const QString & fileSaveName /*= ""*/)
{
	QDesktopWidget* desktop = QApplication::desktop();
	QPixmap scrShotDesk = QPixmap::grabWindow(desktop->winId(), 0, 0, desktop->width(), desktop->height());
	if(!fileSaveName.isEmpty()) {
		scrShotDesk.save(fileSaveName + ".png", "PNG");
	}
	return scrShotDesk;
}

void GLabControlPanel::AddDeviceToShelf( GDevice* pDev )
{
	m_pDeviceShelf->AddDevice(pDev);
}

QString GLabControlPanel::ProgramName_DontUseDuringConstruction()
{
	// This is the class name of the instance of the control panel. ex: "SrYbExe", or "WinLabExe".
	QString strProgramName = metaObject()->className();
	if(strProgramName == "")
		strProgramName = "LabExe";
	return strProgramName;
}

void GLabControlPanel::SaveBenches( QSettings &panelSettings )
{
	// for all the workbenches in the list save the filenames where they are saved or about to be saved. 
	// same for SequencerBenches
	QStringList listFileNamesWorkBenches;
	// map to set tell if a bench is visible
	QMap<QString, bool> mapBenchFile_Visible;
	// 	QStringList listFileNamesSequencerBenches;
	foreach(QListWidgetItem* pitemWB, ui->pListWorkBenches->findItems("*", Qt::MatchWildcard)) {
		GWorkBench* pWB = WorkBenchFromListItem(pitemWB);
		if(pWB) {
			// if there is no Settings(), we should ask you if you want to save
			if(!pWB->Settings())
				pWB->maybeSave();
			// and now, is there a Settings()?
			if(pWB->Settings()) {
				QString benchFileName = pWB->Settings()->fileName();
				listFileNamesWorkBenches << benchFileName;
				mapBenchFile_Visible.insert(pWB->objectName(), pWB->isVisible());
			}
		}
	}
	panelSettings.beginGroup("WorkBenches");
	panelSettings.setValue("files", listFileNamesWorkBenches);
	panelSettings.endGroup();

	// status of the benches
	foreach(QString benchName, mapBenchFile_Visible.keys()) {
		panelSettings.beginGroup(benchName);
		panelSettings.setValue("wasVisible", mapBenchFile_Visible.value(benchName, false));
		panelSettings.endGroup();
	}
}

void GLabControlPanel::RestoreBenches( QSettings &panelSettings )
{
	panelSettings.beginGroup("WorkBenches");
	QStringList listFileNamesWorkBenches = panelSettings.value("files").toStringList();
	panelSettings.endGroup();
	// then create them and read the files
	foreach(QString filename, listFileNamesWorkBenches) {
		GWorkBench* pWB = AddWorkBenchFromFile(filename, false);
		if(pWB) {
			// the status of the bench
			panelSettings.beginGroup(pWB->objectName());
			bool wasVisible = panelSettings.value("wasVisible", false).toBool();
			panelSettings.endGroup();
			if(wasVisible)
				pWB->show();
		}
	}
}

QDockWidget* GLabControlPanel::DebugDockWidget()
{
	return ui->pDebugDock;
}

QListWidget* GLabControlPanel::WorckbenchListWidget()
{
	return ui->pListWorkBenches;
}

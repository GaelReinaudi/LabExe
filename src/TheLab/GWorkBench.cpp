#include "GWorkBench.h"
#include "ui_GWorkBench.h"
#include "Device/GDevice.h"
#include "Device/GDeviceWidget.h"
#include "Device/GDeviceManager.h"												
#include "GLabCommand.h"
#include "GBenchDockWidget.h"

G_REGISTER_WORKBENCH_CLASS(GWorkBench)

GWorkBench::GWorkBench(QWidget *parent)
	: QMainWindow(parent) // 2012-01-10 to have a separate entry in the task bar
{
	InitBench();
    setStatusBar(nullptr);

	// test for nested docks
	setDockNestingEnabled(true);
}

void GWorkBench::InitBench()
{
	pUiBench = new Ui::GWorkBenchClass();
	pUiBench->setupUi(this);
	// undo/redo view
	pUiBench->m_pUndoView->setStack(&m_UndoStack);
	pUiBench->m_pDockUndo->close();

	createActions();
	createMenus();

	viewMenu->addAction(pUiBench->m_pDockUndo->toggleViewAction());

	QTimer::singleShot(0, this, SLOT(LatterBenchInitialization()));
}

GWorkBench::~GWorkBench()
{
	delete pUiBench;
}

void GWorkBench::closeEvent( QCloseEvent *event )
{
	WriteToPermanentStorage();
	emit Saved();
	QMainWindow::closeEvent( event );
}

GDeviceWidget* GWorkBench::AddDevice( GDevice* pTheDevice, QPoint whereInParent /*= QPoint(0, 0)*/, bool putOnStack /*= true*/ )
{
	QWidget* pSenderWidget = qobject_cast<QWidget*>(sender());

	GCommandAddDevice* pAddCommand = new GCommandAddDevice(pTheDevice, this, pSenderWidget);
	if(putOnStack)
		m_UndoStack.push(pAddCommand);
	else
		pAddCommand->redo();

	GDeviceWidget* pDevWid = pAddCommand->AddedWidget();
	if(!pDevWid)
        return nullptr;

	m_DateDeviceAdded[pTheDevice] = QDateTime::currentDateTime().toString(Qt::ISODate);

	// TODO
	// should add GProgDevice to a CleanupHandler so that they get destroyed if the bench is deleted
	// other wise, numerical expression keep updating after the bench is deleted

	// moves to the point if possible
	pDevWid->move(whereInParent);
	return pDevWid;
}

GBenchDockWidget* GWorkBench::AddDeviceInNewDock( GDevice* pTheDevice, bool putOnStack /*= true*/ )
{
    Q_UNUSED(putOnStack)
    GDeviceWidget* pDevWid = AddDevice(pTheDevice);
	if(!pDevWid)
        return nullptr;
	GBenchDockWidget* pNewDock = new GBenchDockWidget(this, Qt::AllDockWidgetAreas);
	pNewDock->InsertDeviceWidget(pDevWid);
	return pNewDock;
}

void GWorkBench::AddDeviceInDock( GDevice* pTheDevice, GBenchDockWidget* pDock, bool putOnStack /*= true*/ )
{
    Q_UNUSED(putOnStack)
    if(!pTheDevice)
		return;
	if(!pDock) {
		qWarning() << "GWorkBench::AddDeviceInDock : The GBenchDockWidget* pointer is 0! 875494";
		return;
	}

	GDeviceWidget* pDevWid = AddDevice(pTheDevice);
	if(!pDevWid)
		return;
	pDock->InsertDeviceWidget(pDevWid);
}

void GWorkBench::ConnectAndShowDeviceWidget( GDeviceWidget* pDevWid, QWidget* pInWhichWidget /*= 0*/ )
{
	// adding RemoveDevice capabilities to the devicewidget context menu by adding an action
	QAction* pRemoveAction = new QAction("Remove from bench", this);
	pDevWid->addAction(pRemoveAction);
    auto con = connect(pRemoveAction, &QAction::triggered, [this, pDevWid](){ this->RemoveDevice(pDevWid->Device()->UniqueSystemID()); });
    connect(this, &QObject::destroyed, [con](){ QObject::disconnect(con); });

	if(!pInWhichWidget)
		pInWhichWidget = this;
	pDevWid->setParent(pInWhichWidget);
	pDevWid->show();
}

void GWorkBench::RemoveDevice( GDevice* pDevice, bool putOnStack /*= true*/ )
{
	if(!pDevice || !m_DateDeviceAdded.contains(pDevice))
		return;
 	GCommandRemoveDevice* pRemoveCommand = new GCommandRemoveDevice(pDevice, this);
 	if(putOnStack)
 		m_UndoStack.push(pRemoveCommand);
 	else
 		pRemoveCommand->redo();

	m_DateDeviceAdded.remove(pDevice);
}

void GWorkBench::RemoveDevice( QString uniqueIdentifierName )
{
	QList<GDevice*> listDEvTest = DeviceManagerInstance()->AllDevices();
	GDevice* pDev = GetDeviceFromDeviceManager(uniqueIdentifierName);
	if(pDev)
		RemoveDevice(pDev);
}

void GWorkBench::PopulateSettings(QSettings& inQsettings)
{
	// clean every data in the current group
	inQsettings.remove("");

	inQsettings.setValue("WorkBench-Type", metaObject()->className());
	inQsettings.setValue("objectName", objectName());
	inQsettings.setValue("isVisible", isVisible());

	// saves the GBenchDockWidget created dynamically 
	GSerializable::SaveListAndItems<GBenchDockWidget>(inQsettings, "Dynamic-Docks", BenchDynamicDocks());

	// the list of devices, devices'ID, and GBenchDock in which they appear. The 2 latter are filled in the foreach loop over every devices.
	QList<GDevice*> listDev = DeviceList();
	QList<QString> listOfAllBenchDevicesIDs;
	QMap<GDevice*, QString> mapDevice_BenchDockID;
	foreach(GDevice* pDev, listDev) {
		listOfAllBenchDevicesIDs.append(pDev->UniqueSystemID());
		// get if the widget in which the device is graphically embedded is one of the GBenchDockWidget. "" else.
        GBenchDockWidget* qWid = nullptr;
		foreach(GBenchDockWidget* pDock, BenchDynamicDocks()) {
			if(pDock->EmbeddedDevices().contains(pDev)) {
				qWid = pDock;
				break;
			}
		}
		mapDevice_BenchDockID.insert(pDev, qWid ? qWid->UniqueSystemID() : "");
	}
	// then we save those informations
	inQsettings.beginGroup("BenchDevices");
	inQsettings.setValue("NumberBenchDevices", m_DateDeviceAdded.count());
	inQsettings.setValue("m_BenchDevices", QVariant(listOfAllBenchDevicesIDs));
	inQsettings.endGroup();

	// and some other info, directly in the section of the device
	foreach(GDevice* pDev, listDev) {
		inQsettings.beginGroup(pDev->UniqueSystemID());
		inQsettings.setValue("BenchDockID", mapDevice_BenchDockID.value(pDev));
		inQsettings.setValue("DateAdded", m_DateDeviceAdded[pDev]);
		inQsettings.endGroup();
		pDev->SaveDeviceSettings(inQsettings);
	}

	// general settings
	inQsettings.beginGroup("Geometry");
	inQsettings.setValue("geometry", saveGeometry());
	inQsettings.setValue("windowState", saveState());
	inQsettings.endGroup();
}

void GWorkBench::InterpretSettings(QSettings& fromQsettings)
{
	// restores the GBenchDockWidget
	GSerializable::RestoreListOfObjects(fromQsettings, "Dynamic-Docks", this);

	QString objectName = fromQsettings.value("objectName").toString();
	setObjectName(objectName);
	setWindowTitle(objectName);
	bool wasVisible = fromQsettings.value("isVisible").toBool();
	if(!wasVisible)
		hide();

	fromQsettings.beginGroup("BenchDevices");
//	int numDevices = fromQsettings.value("NumberBenchDevices", 0).toInt();
	QList<QString> listOfAllBenchDevicesIDs = fromQsettings.value("m_BenchDevices").toStringList();
	fromQsettings.endGroup();

	foreach(QString strDevID, listOfAllBenchDevicesIDs) {
		if(strDevID == "") {
			continue;
		}
		fromQsettings.beginGroup(strDevID);
		GDevice* pDev = GetDeviceFromDeviceManager(strDevID);
		// if doesn't exist, try to make it
		if(!pDev) {
			QString className = fromQsettings.value("Class").toString();
			GDevice* itsParent = GetDeviceFromDeviceManager(fromQsettings.value("Parent-Device-ID").toString());
			pDev = DeviceManagerInstance()->CreateNewDevice(className, strDevID, itsParent);
		}
		if(pDev) {
			// if it is part of a Dock, we create it inside
			QString strDockID = fromQsettings.value("BenchDockID").toString();
            GBenchDockWidget* pDock = nullptr;
			if(strDockID != "")
				pDock = qobject_cast<GBenchDockWidget*>(GSerializable::RestoredObject(strDockID));
			if(pDock)
				AddDeviceInDock(pDev, pDock);
			else // else we just add it the default way.
				AddDevice(pDev);

			// the date it was added to a bench
			QString dateAdded = fromQsettings.value("DateAdded", "0").toString();
			m_DateDeviceAdded[pDev] = dateAdded;
		}
		fromQsettings.endGroup();

		if(pDev) {
			pDev->ReadDeviceSettings(fromQsettings);
		}
	}
	m_UndoStack.clear();

	fromQsettings.beginGroup("Geometry");
	restoreGeometry(fromQsettings.value("geometry").toByteArray());
	restoreState(fromQsettings.value("windowState").toByteArray());
	fromQsettings.endGroup();
}

//////////////////////////////////	// taken from Qt MainWindow example

void GWorkBench::createActions()
{
	saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAction->setShortcuts(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save the document to disk"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setShortcuts(QKeySequence::SaveAs);
	saveAsAction->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
}

void GWorkBench::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
// 	fileMenu->addAction(newAct);
// 	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
// 	fileMenu->addSeparator();
// 	fileMenu->addAction(exitAct);

// 	menuBar()->addSeparator();
// 
// 	helpMenu = menuBar()->addMenu(tr("&Help"));
// 	helpMenu->addAction(aboutAct);
// 	helpMenu->addAction(aboutQtAct);
	viewMenu = menuBar()->addMenu(tr("&View"));
}

bool GWorkBench::save()
{
	if (!Settings() || Settings()->fileName().isEmpty()) {
		return saveAs();
	} else {
		return saveFile(Settings()->fileName());
	}
}

bool GWorkBench::saveAs()
{
	QString defaultNameSave = objectName().remove(":");
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Workbench"), defaultNameSave, tr("Workbench (*.wb)"));
	if (fileName.isEmpty())
		return false;

	return saveFile(fileName);
}

bool GWorkBench::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if(!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return false;
	}

// 	QTextStream out(&file);
// #ifndef QT_NO_CURSOR
// 	QApplication::setOverrideCursor(Qt::WaitCursor);
// #endif
// 	out << textEdit->toPlainText();
// #ifndef QT_NO_CURSOR
// 	QApplication::restoreOverrideCursor();
// #endif

	//G
	SetFile(fileName);
	WriteToPermanentStorage();
	emit Saved(fileName);

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

bool GWorkBench::maybeSave()
{	
	// taken from Qt MainWindow example
	if(true /*textEdit->document()->isModified()*/) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, objectName(),
			// "The document has been modified.\n"
			"Do you want to save any changes?",
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

void GWorkBench::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	setWindowModified(false);

	QString shownName = curFile;
	if(curFile.isEmpty())
		shownName = "untitled.txt";
	setWindowFilePath(shownName);
}

void GWorkBench::dragEnterEvent( QDragEnterEvent *event )
{
	QMainWindow::dragEnterEvent(event);
	const QMimeData* pData = event->mimeData();
	if(pData->hasFormat("LabExe/device.single"))
		event->accept();
}

void GWorkBench::dropEvent( QDropEvent *event )
{
	GDevice* pTheDevice = DeviceManagerInstance()->GetDevice(event->mimeData());
	if(!pTheDevice)
		return;
	AddDeviceInNewDock(pTheDevice);
}

QList<GBenchDockWidget*> GWorkBench::BenchDynamicDocks()
{
	QList<GBenchDockWidget*> listDynamicDocks;
	foreach(QObject* pObj, QObject::children()) {
		GBenchDockWidget* pBenchDock = qobject_cast<GBenchDockWidget*>(pObj);
		if(pBenchDock)
			listDynamicDocks.append(pBenchDock);
	}
	return listDynamicDocks;
}

QList<GDevice*> GWorkBench::DeviceList()
{
	QList<GDevice*> listReturn;
	QList<QString> dates = m_DateDeviceAdded.values();
    std::sort(dates.begin(), dates.end());
	foreach(QString date, dates) {
		foreach(GDevice* pDev, m_DateDeviceAdded.keys(date)) {
			if(!listReturn.contains(pDev))
				listReturn.append(pDev);
		}
	}
	return listReturn;
}

void GWorkBench::ExportScreenShot(const QString & screenFileName)
{
    QPixmap pixWid = grab();
	pixWid.save(screenFileName, "PNG");
}










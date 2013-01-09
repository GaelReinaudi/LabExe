#include "GFileSystemDevice.h"
#include "GFileSystemCrawler.h"

G_REGISTER_PROG_DEVICE_CLASS(GFileSystemDevice)

GFileSystemDevice::GFileSystemDevice(QObject *parent, QString uniqueIdentifierName /*= ""*/)
	: GProgDevice(parent, uniqueIdentifierName)
	, GHasAgent(new GFileSystemCrawler(this)) //can't do that because of creation order// , m_pFileSystemModel))
	, m_Path("Path", this)
{
	// is the parent a GDeviceShelf? Because if it is, we might not want to use the crawler agent's QFileSystemModel
	// gets the crawler to use its QFileSystemModel object
	GFileSystemCrawler* pCrawler = qobject_cast<GFileSystemCrawler*>(Agent());
	if(pCrawler && !IsShelvedInstance())
		m_pFileSystemModel = pCrawler->SystemModel();
	else {
// 		qWarning() << "This class was supposed to use the QFileSystemModel of the GFileSystemCrawler agent. Creating a new one.";
		m_pFileSystemModel = new QFileSystemModel(this);
	}

	m_Path = QDir::homePath();
// 	connect(&m_Path, SIGNAL(ValueDidChange(QString)), this, SLOT(ChangeRootDirectory(QString)));
 	ChangeRootDirectory(m_Path);

}

GFileSystemDevice::~GFileSystemDevice()
{

}

void GFileSystemDevice::PopulateDeviceWidget(GDeviceWidget* theDeviceWidget)
{
	QBoxLayout* pLay = new QHBoxLayout();
	theDeviceWidget->AddSubLayout(pLay);
	QTreeView *tree = new QTreeView(theDeviceWidget);
	tree->setModel(m_pFileSystemModel);
	tree->setRootIndex(m_pFileSystemModel->index(m_Path));
	pLay->addWidget(tree);
}

void GFileSystemDevice::PopulateSettings( QSettings& inQsettings )
{
	GProgDevice::PopulateSettings(inQsettings);
}

void GFileSystemDevice::ReadFromSettings( QSettings& fromQsettings )
{
	GProgDevice::ReadFromSettings(fromQsettings);
}

void GFileSystemDevice::ChangeRootDirectory(QString newPath)
{
	m_pFileSystemModel->setRootPath(newPath);
}
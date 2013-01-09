#include "GWorkBenchManager.h"
#include "GWorkBench.h"

GWorkBench* GWorkBenchManager::CreateNewWorkBench( QString className, QWidget *parent )
{
	return WorkBenchManagerInstance()->CreateObject( className, parent);
}

GWorkBenchManager* WorkBenchManagerInstance()
{
	return &(GWorkBenchManagerSing::Instance());
}

QStringList GWorkBenchManager::RegisteredTypes() const
{
	std::vector<QString> ids = WorkBenchManagerInstance()->RegisteredIds();
	QStringList typlist;
	for(int i = 0; i < (int)ids.size(); i++) {
		typlist.append(ids[i]);
	}
	return typlist;
}

GWorkBench* GWorkBenchManager::CreateNewWorkBenchFromFile( QString fileName, QWidget *parent )
{
	QSettings setting(fileName, QSettings::IniFormat);
	QString strType = setting.value("WorkBench-Type").toString();
	if(strType == ""){
		qWarning() << QString("The file %1 doesn't exist or doesn't contain the \"WorkBench-Type\" information. No bench will be created. The local execution path is %2").arg(fileName).arg(QDir().absolutePath());
		return 0;
	}
	GWorkBench* pWb = CreateNewWorkBench(strType, parent);
	return pWb;
}

// 
// void GWorkBenchManager::LoadWorkBenchToolPlugins()
// {
// 	QDir pluginsDir = QDir(qApp->applicationDirPath());
// 	pluginsDir.cd("WorkBenchToolPlugins");
// 	QStringList pluginFileNames;
// 
// 	foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
// 		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName), this);
// 		QObject *objInstance = loader.instance();
// 		GWorkBenchToolPlugin* pPlugin = qobject_cast<GWorkBenchToolPlugin*>(objInstance);
// 		// if there is a pPlugin, we insert its class name in the m_MapNameWorkBenchToolPlugins so that we can find the plugin from its class name. used to (de)serialize.
// 		if(pPlugin) {
// 			pluginFileNames += fileName;
// 			if(pPlugin)
// 				m_MapNameWorkBenchToolPlugins.insert(pPlugin->metaObject()->className(), pPlugin);
// 		}
// 	}
// }


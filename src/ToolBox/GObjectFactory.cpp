#include "GObjectFactory.h"
#include "GSerializable.h"

QObject* GObjectFactory::CreateNewObject( QString className, QObject *parent )
{
	return ObjectFactoryInstance()->CreateObject( className, parent);
}

QStringList GObjectFactory::RegisteredTypes() const
{
	std::vector<QString> ids = ObjectFactoryInstance()->RegisteredIds();
	QStringList typlist;
	for(int i = 0; i < (int)ids.size(); i++) {
		typlist.append(ids[i]);
	}
	return typlist;
}
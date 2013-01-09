#ifndef GFILESYSTEMCRAWLER_H
#define GFILESYSTEMCRAWLER_H

#include "GFolderAgent.h"
#include "GFileSystemModel.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GFolderAgent class is a GAgent for representing folders in the map system.
/*!
more...
*/
class GFileSystemCrawler : public GFolderAgent
{
 	Q_OBJECT

public:
	//! Constructor that makes this crawler use a provided QFileSystemModel for exploring the file system. If pModelToUse == 0, a new QFileSystemModel is created
	GFileSystemCrawler(QObject *parent, GFileSystemModel* pModelToUse = 0);
	~GFileSystemCrawler();

	//! Returns a pointer to the file system model.
	QFileSystemModel* SystemModel() const { return m_pSystemModel; }

protected:
	//! Re-implemented to point to the GFileSystemModel* m_pSystemModelused by this crawler
	GFileSystemModel* FileSystem() {return m_pSystemModel;}

protected slots:
	void EventDirectoryLoaded(QString path);

private:
	//! The QFileSystemModel object that actually explores the file system
	GFileSystemModel* m_pSystemModel;
};

#endif // GFILESYSTEMCRAWLER_H

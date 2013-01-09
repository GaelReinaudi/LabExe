#include "GFolderAgent.h"
#include "GFolderGraphicsItem.h"
#include "GRandomPlacerAgent.h"
#include "GGrasshopperPlacerAgent.h"
#include "GGridPlacerAgent.h"
#include "GForcePlacerAgent.h"
#include "GFireworksPlacerAgent.h"

#include "GFileSystemModel.h"

G_REGISTER_PROG_AGENT_CLASS(GFolderAgent)

GFolderAgent::GFolderAgent(QObject *parent)
	: GUriAgent(parent)
	, m_pFileSystem(0)
{
// 	new GRandomPlacerAgent(this);
	new GGridPlacerAgent(this);

	static bool firstpass = true;
	if(firstpass) {
		firstpass = false;
		qsrand(5554);
	}
    switch(3 + 0*qrand() % 2) {
	case 0:
		new GGridPlacerAgent(this);
		break;
	case 1:
		new GGrasshopperPlacerAgent(this);
		break;
	case 2:
		new GForcePlacerAgent(this);
		break;
	case 3:
		new GFireworksPlacerAgent(this);
		break;
	}
}

GFolderAgent::~GFolderAgent()
{

}

GAgentGraphicsItem* GFolderAgent::ProvideNewAgentGraphicsItem(GAgentGraphicsItem* pParentItem)
{
	GFolderGraphicsItem* pFoldIt = new GFolderGraphicsItem(this, pParentItem);
	return pFoldIt;
}

void GFolderAgent::Event_DoubleClicked(GAgentGraphicsItem *pOnWhichItem)
{
    FileSystem()->OpenFolder(Info());
}

GFileSystemModel* GFolderAgent::FileSystem()
{
	if(m_pFileSystem)
		return m_pFileSystem;

	GFolderAgent* pParFolderAgent = qobject_cast<GFolderAgent*>(ParentAgent());
	if(pParFolderAgent)
		return pParFolderAgent->FileSystem();
	
	m_pFileSystem = new GFileSystemModel(this);
	m_pFileSystem->setRootPath(Info().path());
	return m_pFileSystem;
}
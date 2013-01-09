#include "GFileAgent.h"
#include "GFileGraphicsItem.h"

GFileAgent::GFileAgent(QObject* parent)
	: GUriAgent(parent)
{
}

GFileAgent::~GFileAgent()
{

}

GAgentGraphicsItem* GFileAgent::ProvideNewAgentGraphicsItem(GAgentGraphicsItem* pParentItem)
{
	GFileGraphicsItem* pFileIt = new GFileGraphicsItem(this, pParentItem);
    return pFileIt;
}


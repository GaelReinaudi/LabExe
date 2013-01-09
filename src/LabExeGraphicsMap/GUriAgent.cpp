#include "GUriAgent.h"
#include <QGraphicsEllipseItem>

GUriAgent::GUriAgent(QObject *parent)
	: GAgent(parent)
{

}

GUriAgent::~GUriAgent()
{

}

GAgentGraphicsItem* GUriAgent::ProvideNewAgentGraphicsItem(GAgentGraphicsItem* pParentItem)
{
	return 0;
}

void GUriAgent::SetInfo( QFileInfo val )
{
	m_Info = val; 
	emit FileInfoChanged(m_Info);
}
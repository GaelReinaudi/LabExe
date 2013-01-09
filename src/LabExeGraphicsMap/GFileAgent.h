#ifndef GFILEAGENT_H
#define GFILEAGENT_H

#include "GUriAgent.h"

class GFileAgent : public GUriAgent
{
//    Q_OBJECT
public:
    GFileAgent(QObject* parent);
    ~GFileAgent();

    //! Re-implemented
    virtual GAgentGraphicsItem* ProvideNewAgentGraphicsItem(GAgentGraphicsItem* pParentItem);

private:
    
};

#endif // GFILEAGENT_H

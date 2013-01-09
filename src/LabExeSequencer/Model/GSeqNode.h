#ifndef GSEQNODE_H
#define GSEQNODE_H

#include <QObject>
#include <QDomNode>

// CRTP !
template<typename DerivedClass, typename BaseNode>
class GNode : public BaseNode
{
public:
	GNode(QObject *parent) {
// 		QObject::connect(static_cast<QObject*>(this), SIGNAL(d()), static_cast<QObject*>(this), SLOT(s()));
	}
	~GNode() {
	}

private:
	
};

class GSeqNode : public QObject, public GNode<GSeqNode, QDomNode>
{
public:
	GSeqNode(QObject *parent, GSeqNode* parentNode) 
		: QObject(parent)
		, GNode<GSeqNode, QDomNode>(parent)
	{
	}
	~GSeqNode() {
	}

private:

};

#endif // GSEQNODE_H

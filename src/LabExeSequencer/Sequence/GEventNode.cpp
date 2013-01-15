#include "GEventNode.h"
#include "ToolBox/GObjectFactory.h"
#include "Param/GParam.h"
#include <QPen>

GEventManager* EventManagerInstance()
{
	return &(GEventManagerSing::Instance());
}

// G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GEventNode, GEventNode)

GEventNode::GEventNode(GEventNode* pParentNode)
	: GAgent(pParentNode)
// 	QObject(pParentNode) // object parent at least for now because of the auto saving thing (object factory)
// 	, GSerializable()
	, QGraphicsLineItem(pParentNode)
{
	if(pParentNode)
		pParentNode->InsertChildEvent(this);

	QPen ChannelPen(Qt::red, 2);
	// Next line ensures the width of the line is always the same on the screen, no matter what scale or zooming you use.
	ChannelPen.setCosmetic(true);
	setPen(ChannelPen);

	setFlag(QGraphicsItem::ItemIsSelectable);
	// to make the item movable 
	setFlag(QGraphicsItem::ItemIsMovable);
	// to inform of position change relative to parent. See GSynchEventGraphicsItem::itemChange().
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);

	UpdateVerticalPosition();

	EventManagerInstance()->Add(this);
}

GEventNode::~GEventNode()
{

}

void GEventNode::InsertChildEvent( GEventNode* node, int atPos /*= -1*/ )
{
	Q_ASSERT(node);

	node->setParentItem(this);
	node->setParent(this);

	// if not at the end, we have to figure out the event to stack it before
	if(atPos >= 0) {
		const QList<GEventNode*> listEv = ChildNodes();
		const int numEv = listEv.count();
		if(atPos < numEv) {
			GEventNode* pEventToPush = listEv.at(atPos);
			node->stackBefore(pEventToPush);
		}
	}
}

void GEventNode::UpdateVerticalPosition()
{
	// if their is a parent item, we draw the line within the pParentItem->boundingRect(). 
	double heightLine = 11.4159;
	if(parentItem())
		heightLine = parentItem()->boundingRect().height();
	setLine(0.0, 0.0, 0.0, heightLine);
}

QVariant GEventNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
	// corrects the position about to be changed so that the event can't be moved vertically.
	if(change == ItemPositionChange) {
		// value is the new position.
		QPointF newPos = value.toPointF();
		// resets the vertical position to what it was before: it prevents vertical motion.
		newPos.setY(pos().y());
		// prevents going out of the sequence at negative X
		newPos.setX(qMax(0.0, newPos.x()));
		return newPos;
	}
	return QGraphicsLineItem::itemChange(change, value);
}

const QList<GEventNode*> GEventNode::ChildNodes() const
{
	QList<GEventNode*> theNodeList;
	foreach(QObject* item, QObject::children()) {
//	foreach(QGraphicsItem* item, childItems()) {
// 		GEventNode* pNode = qgraphicsitem_cast<GEventNode*>(item);
		GEventNode* pNode = qobject_cast<GEventNode*>(item);
		if(pNode)
			theNodeList.append(pNode);
	}
	return theNodeList;
}

void GEventNode::PopulateSettings( QSettings& inQsettings )
{
// 	GSerializable::PopulateSettings(inQsettings);
	QList<GEventNode*> listEvents = ChildNodes();
	foreach(GParam* pPar, ChildParams()) {
		pPar->SaveDeviceSettings(inQsettings);
	}
	GSerializable::SaveListAndItems<GEventNode>(inQsettings, "children", listEvents);
	
	inQsettings.setValue("DelayFromParent", x());
}

void GEventNode::InterpretSettings( QSettings& fromQsettings )
{
	GSerializable::InterpretSettings(fromQsettings);
	GSerializable::RestoreListOfObjects(fromQsettings, "children", this);
	foreach(GParam* pPar, ChildParams()) {
		//TOGO: false just for compatibility with previous version
		pPar->ReadDeviceSettings(fromQsettings);
	}
}

QList<GParam*> GEventNode::ChildParams() const
{
	QList<GParam*> listParamChildren;
	foreach(QObject* pObj, QObject::children()) {
		GParam* pPar = qobject_cast<GParam*>(pObj);
		if(pPar)
			listParamChildren.append(pPar);
	}
	return listParamChildren;
}

void GEventNode::Event_UniqueSystemIDChanged( const QString & newSystemID )
{
	EventManagerInstance()->Add(this);
}

#include "GSynchEvent.h"
#include "GSequence.h"
#include "Param/GParamDouble.h"
#include "Param/GDoubleSpinBox.h"
#include "ToolBox/GObjectFactory.h"

G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GSynchEvent, GSynchEvent)

GSynchEvent::GSynchEvent(GSynchEvent* parentEventSynchro)
	: GEventNode(parentEventSynchro)
{
	m_pDelay = new GParamDouble("delay", this);
	connect(this, SIGNAL(DelayChanged(double)), m_pDelay, SLOT(SetParamValue(double)));
	connect(m_pDelay, SIGNAL(ValueUpdated(double)), this, SLOT(SetDelayFromParent(double)));

//test
	pText = new QGraphicsSimpleTextItem("update", this);
	pText->setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

GSynchEvent::~GSynchEvent()
{
}

QList<GSynchEvent*> GSynchEvent::ChildrenEvents()
{
	QList<GSynchEvent*> listEventChildren;
	foreach(QObject* pObj, QObject::children()) {
		GSynchEvent* pEvent = qobject_cast<GSynchEvent*>(pObj);
		if(pEvent)
			listEventChildren.append(pEvent);
	}
	return listEventChildren;
}

double GSynchEvent::GlobalXPosition()
{
	return scenePos().x();
}

void GSynchEvent::PopulateSettings( QSettings& inQsettings )
{
	GEventNode::PopulateSettings(inQsettings);
// 	QList<GSynchEvent*> listEvents = ChildrenEvents();
// 	inQsettings.setValue("DelayFromParent", x());
// 	GSerializable::SaveListAndItems<GSynchEvent>(inQsettings, "EventChildren", listEvents);
}

void GSynchEvent::InterpretSettings( QSettings& fromQsettings )
{
	GEventNode::InterpretSettings(fromQsettings);
// 	setX(fromQsettings.value("DelayFromParent", 0.0).toDouble());
// 	GSerializable::RestoreListOfObjects(fromQsettings, "EventChildren", this);
}

QVariant GSynchEvent::itemChange( QGraphicsItem::GraphicsItemChange change, const QVariant &value )
{
	if(change == ItemPositionHasChanged) {
		double newDelay = value.toPointF().x();
		emit DelayChanged(newDelay);
		pText->setText(QString::number(newDelay));
	}
 	if(change == ItemScenePositionHasChanged) {
		double newAbsPos = value.toPointF().x();
		emit AbsoluteTimeChanged(newAbsPos);
 	}
	return GEventNode::itemChange(change, value);
}

void GSynchEvent::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
	QMenu menu;
	m_pDelay->ProvideNewParamSpinBox(&menu)->show();
	QString dddd("Delay: %1");
	dddd = dddd.arg(DelayFromParent());
	menu.addAction(dddd);
	menu.exec(event->screenPos());
}

void GSynchEvent::SetDelayFromParent( double RelativeDelay )
{
	setX(RelativeDelay);
}
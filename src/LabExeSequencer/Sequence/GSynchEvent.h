#ifndef GSYNCHEVENT_H
#define GSYNCHEVENT_H

#include <QObject>
#include "GEventNode.h"

class GSequence;
class GParamDouble;

/////////////////////////////////////////////////////////////////////
//! \brief The GSynchEvent class defines an event that can be used to synchronize other events in a tree-like structure. 
/*!
It has the following functionalities:
- is a QGraphicsLineItem that is part of the sequence GSequenceGraphicsScene. This allows 
the elegant management of the tree-structure of events and of there relative positions. 
It also allows the display of the event and some user interactions.
- the tree-like structure of the events defines the synchronization of each event: a parent triggers all its children.
- an additional DelayFromParent() can be defined to delay the event being triggered from its parent.
So technically speaking, GSynchEvent inherits from QGraphicsLineItem. It can be then put in the GSequence's EventTreeScene().
*/
class GSynchEvent : public GEventNode
{
	Q_OBJECT

public:
	//! construct an event, child of (=synchronized with) parentEventSynchro.
	GSynchEvent(GSynchEvent* parentEventSynchro);
	//! destrucotr
	~GSynchEvent();

	//! returns the graphics item GEventNode that represent this event in the event tree scene
	GEventNode* GraphicsEventTreeItem() {return this;}
	//! returns the children that are of type GSynchEvent. Use this to navigate down the tree structure of events.
	QList<GSynchEvent*> ChildrenEvents();
	//! Returns the event delay relatively to the parent event (defined with SynchronizeWith()).
	double DelayFromParent() const {return x();}
	//! Returns the global position of the event in the sequence. Calls the graphics item position in the GSequenceGraphicsScene.
	double GlobalXPosition();

	QVariant data() const {return DelayFromParent();}


public:
	//! Reimplemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	virtual void InterpretSettings(QSettings& fromQsettings);

public slots:
	//! Tells the event to be delayed by that amount relatively to the parent event.
	void SetDelayFromParent(double RelativeDelay);


signals:	
	//! Emitted when the event DelayFromParent() changed.
	void DelayChanged(double newDelay);
	//! Emitted when the event time in the sequence changed, i.e. when the event moves in the sequence's tree-scene. PS: emitted only if setFlag(ItemSendsScenePositionChanges);
	void AbsoluteTimeChanged(double newAbsoluteTime);

protected:
	//! reimplemented from GEventNode to inform of changes in the item properties.
	virtual	QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
	//! Reimplemented from QGraphicsItem. Calls the GChannel function to handle that.
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	//! delay from the parent node to this node
	GParamDouble* m_pDelay;
	// test
	QGraphicsSimpleTextItem* pText;

private:

};

#endif // GSYNCHEVENT_H

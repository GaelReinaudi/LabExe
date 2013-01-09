#ifndef GSYNCHEVENT_H
#define GSYNCHEVENT_H

#include <QObject>
#include "GEventTreeLineItem.h"
#include "ToolBox/GSerializable.h"

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
class GSynchEvent : public QObject, public GEventTreeLineItem, public GSerializable
{
	Q_OBJECT

public:
	//! construct an event, child of (=synchronized with) parentEventSynchro.
	GSynchEvent(GSynchEvent* parentEventSynchro);
	//! construct an event as a root event of a GSequence.
	GSynchEvent(GSequence* pSequence);
	~GSynchEvent();

	//! returns the graphics item GEventTreeLineItem that represent this event in the event tree scene
	GEventTreeLineItem* GraphicsEventTreeItem() {return this;}
	//! returns the children that are of type GSynchEvent. Use this to navigate down the tree structure of events.
	QList<GSynchEvent*> ChildrenEvents();
	//! Returns the event delay relatively to the parent event (defined with SynchronizeWith()).
	double DelayFromParent() {return x();}
	//! Returns the global position of the event in the sequence. Calls the graphics item position in the GSequenceGraphicsScene.
	double GlobalXPosition();

public:
	//! Reimplemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	virtual void ReadFromSettings(QSettings& fromQsettings);

public slots:
	//! Tells the event to be delayed by that amount relatively to the parent event.
	void SetDelayFromParent(double RelativeDelay);


signals:	
	//! Emitted when the event DelayFromParent() changed.
	void DelayChanged(double newDelay);
	//! Emitted when the event time in the sequence changed, i.e. when the event moves in the sequence's tree-scene. PS: emitted only if setFlag(ItemSendsScenePositionChanges);
	void AbsoluteTimeChanged(double newAbsoluteTime);

protected:
	//! reimplemented from GEventTreeLineItem to inform of changes in the item properties.
	virtual	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	//! Reimplemented from QGraphicsItem. Calls the GChannel function to handle that.
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	//! 
	GParamDouble* m_pDelay;
	// test
	QGraphicsSimpleTextItem* pText;

private:

};

#endif // GSYNCHEVENT_H

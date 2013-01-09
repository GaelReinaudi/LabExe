#ifndef GSEQUENCE_H
#define GSEQUENCE_H

#include <QAction>
#include "GSequenceGraphicsScene.h"
#include "ToolBox/GSerializable.h"

class GChannel;
class GSynchEvent;

/////////////////////////////////////////////////////////////////////
//! \brief The GSequence class defines a set of instruction that are linked or synchronized in a tree like structure. 
/*!
Each instruction has a parent() triggering event and can have one or several event that will be able to trigger further instructions.
Each instruction has to be assigned to one of the GSequence's channel. 

The sequence owns a GSequenceGraphicsScene object that will allow the display of the sequence in a GSequenceGraphicsView.
There are actually two scenes associated with a GSequence:
- one is for the representation of the GSynchEvent`s and allows some display and user interactions through a GSequenceGraphicsView.
- one is for the representation of the GSynchEvent`s, GChannel`s and GInstruction`s and allows some more display and user interactions through a GSequenceGraphicsView.
- those 2 scenes are coupled together to reflect the same information, e.g. have a sceneRect() that reflects the sequence Length().
*/
class GSequence : public QObject, public GSerializable
{
	Q_OBJECT

public:
	enum ChannelType {AskType, AnalogChannel = 100, DigitalChannel = 200, CustomChannelType = 300};

public:
	GSequence(QObject *parent = 0);
	~GSequence();

	//! Returns the length (duration) of the sequence.
	double Length() const {return 33.14159;}
	//! Convenient function to return a list of GChannel out of a list of various graphics items in a GSequenceScene
	static QList<GChannel*> GetChannels(QList<QGraphicsItem*> listItems);
	//! Convenient function to return a list of GSynchEvent out of a list of various graphics items, e.g. from a selection in a GSequenceScene
	static QList<GSynchEvent*> GetEvents(QList<QGraphicsItem*> listItems);
	//! Convenient function to return a pointer to the only selected GSynchEvent in the tree-scene. If more than one, or none, returns 0.
	GSynchEvent* GetTheSelectedEvent();
	//! Returns the scene that holds the event items in a tree structure.
	GSequenceGraphicsScene* EventTreeScene() const { return m_pEventTreeScene; }
	//! Returns the scene that holds the Channels and channel-event, instructions, ect...
	GSequenceGraphicsScene* ChannelScene() const { return m_pChannelScene; }
	//! Returns the root time event
	GSynchEvent* RootTimeEvent() const { return m_pRootTimeEvent; }

public slots:
	//! Creates and adds a channel to the sequence and returns a pointer to the new GChannel. 0 if no channel was created.
	GChannel* CreateNewChannel(ChannelType theType = AskType);
	//! Adds the channel to the channel list
	void AddChannel( GChannel* pChan );
	//! Create a new event in the sequence. If you don't provide the parent event and/or the channel owning the event, you will have to do it graphically.
	GSynchEvent* CreateNewEvent(GSynchEvent* pParentEvent = 0, GChannel* pAssignedChannelForGraphics = 0);

public:
	//! Reimplemented.
	virtual void PopulateSettings( QSettings& inQsettings );
	//! Reimplemented.
	virtual void ReadFromSettings( QSettings& fromQsettings );

signals:
	//! emitted when the Length() of the sequence changed.
	void LengthChanged(double newLength);

private:
	//! The list of GChannel`s managed by this sequence.
	QList<GChannel*> m_ChannelList;
	//! This is the root event of the sequence, the one that defines t = 0. It is at the top of the instruction tree.
	GSynchEvent* m_pRootTimeEvent;
	//! There are two scenes associated with a GSequence. This one is for the representation of the GSynchEvent`s and will allow some display and user interactions through a QGraphicsView.
	GSequenceGraphicsScene* m_pEventTreeScene;
	//! There are two scenes associated with a GSequence. This one is for the representation of the GSynchEvent`s, GChannel`s and GInstruction`s which will allow some more display and user interactions through a QGraphicsView.
	GSequenceGraphicsScene* m_pChannelScene;

private:
	//! Makes some actions that can be triggered e.g. by a QMenu.
	void createActions();

public:
	//! action to make and add a new channel to this sequence. Will call CreateNewChannel().
	QAction* m_pActionNewChannel;
	//! action to make and add a new event to this sequence.  Will call CreateNewEvent().
	QAction* m_pActionNewEvent;

	friend class GSequenceGraphicsScene;
	friend class GSequenceScene;
};

#endif // GSEQUENCE_H

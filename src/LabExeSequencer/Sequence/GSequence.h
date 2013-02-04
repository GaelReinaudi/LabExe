#ifndef GSequence_H
#define GSequence_H

#include <QAction>
#include "GEvScene.h"
#include "ToolBox/GSerializable.h"
#include "GSequence.h"
#include "param.h"

#include "GSequenceGraphicsItem.h"

// TRY BASALT
#include "../../../MapExe/src/agent.h"

class GChannel;
class GSynchEvent;
class GEventNode;


/////////////////////////////////////////////////////////////////////
//! \brief The GSequence class defines a set of instruction that are linked or synchronized in a tree like structure. 
/*!
Each instruction has a parent() triggering event and can have one or several event that will be able to trigger further instructions.
Each instruction has to be assigned to one of the GSequence's channel. 

The sequence owns a GEvScene object that will allow the display of the sequence in a GSequenceGraphicsView.
There are actually two scenes associated with a GSequence:
- one is for the representation of the GSynchEvent`s and allows some display and user interactions through a GSequenceGraphicsView.
- one is for the representation of the GSynchEvent`s, GChannel`s and GInstruction`s and allows some more display and user interactions through a GSequenceGraphicsView.
- those 2 scenes are coupled together to reflect the same information, e.g. have a sceneRect() that reflects the sequence Length().
*/
class GSequence : public GContentAgent<GSequence, GSequenceGraphicsItem>
{
	Q_OBJECT
	
public:
	enum ChannelType {AskType, AnalogChannel = 100, DigitalChannel = 200, CustomChannelType = 300};

public:
	GSequence(QObject *parent = 0);
	~GSequence();


	//! Implemented
	virtual bool InjectInitializationData(const QVariantList & theDataList) {return false;}
	//! Implemented
	virtual QVariantList ProvideInitializationData() {return QVariantList();}


	//! Returns the length (duration) of the sequence.
	Q_INVOKABLE double Length() const {return 53.14159;}
	//! Returns the length (duration) of the sequence.
	Q_INVOKABLE int NumChannels() const {return m_ChannelList.size();}
	//! Convenient function to return a pointer to the only selected GSynchEvent in the tree-scene. If more than one, or none, returns 0.
	GSynchEvent* GetTheSelectedEvent();
	//! Returns the scene that holds the event items in a tree structure.
	GEvScene* EventTreeScene() const { return m_pEventTreeScene; }
	//! Returns the scene that holds the Channels and channel-event, instructions, etc...
	GEvScene* ChannelScene() const { return m_pChannelScene; }
	//! Returns the root time event
	GSynchEvent* RootTimeEvent() const { return EventTreeScene()->m_pRootEvent; }

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
	virtual void InterpretSettings( QSettings& fromQsettings );

	//! Convenient function to return a list of GChannel out of a list of various graphics items in a GEvScene
	static QList<GChannel*> FilterItemChannels(QList<QGraphicsItem*> listItems);
	//! Convenient function to return a list of GSynchEvent out of a list of various graphics items, e.g. from a selection in a GEvScene
	static QList<GSynchEvent*> FilterItemEvents(QList<QGraphicsItem*> listItems);

signals:
	//! emitted when the Length() of the sequence changed.
	void LengthChanged(double newLength);

private:
	//! The list of GChannel`s managed by this sequence.
	QList<GChannel*> m_ChannelList;
	//! There are two scenes associated with a GSequence. This one is for the representation of the GSynchEvent`s and will allow some display and user interactions through a QGraphicsView.
	GEvScene* m_pEventTreeScene;
	//! There are two scenes associated with a GSequence. This one is for the representation of the GSynchEvent`s, GChannel`s and GInstruction`s which will allow some more display and user interactions through a QGraphicsView.
	GEvScene* m_pChannelScene;

private:
	//! Makes some actions that can be triggered e.g. by a QMenu.
	void createActions();

public:
	//! action to make and add a new channel to this sequence. Will call CreateNewChannel().
	QAction* m_pActionNewChannel;
	//! action to make and add a new event to this sequence.  Will call CreateNewEvent().
	QAction* m_pActionNewEvent;

public:
	GParamDouble m_Length;

	friend class GEvScene;
};

#endif // GSequence_H

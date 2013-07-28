#ifndef GSequence_H
#define GSequence_H

#include <QAction>
#include "GEvScene.h"
#include "ToolBox/GSerializable.h"
#include "GSequence.h"
#include "param.h"

#include "GSequenceGraphicsItem.h"
#include <QDeclarativeListProperty>

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
class GSequence : public QObject
{
	Q_OBJECT
	
public:
	enum ChannelType {AskType, AnalogChannel = 100, DigitalChannel = 200, CustomChannelType = 300};

public:
	GSequence(QObject* parent = 0);
	~GSequence();

	Q_PROPERTY(double length READ Length NOTIFY LengthChanged);
	Q_PROPERTY(int channelCount READ NumChannels NOTIFY NumChannelsChanged);
	Q_PROPERTY(QDeclarativeListProperty<GChannel> channels READ Channels);

	//! Implemented
	virtual bool InjectInitializationData(const QVariantList & theDataList) {
		Q_UNUSED(theDataList);
		return false;
	}
	//! Implemented
	virtual QVariantList ProvideInitializationData() {return QVariantList();}


	//! Returns the length (duration) of the sequence.
	Q_INVOKABLE double Length() const {return 53.14159;}

	//! Returns the length (duration) of the sequence.
	Q_PROPERTY(int numChannels READ NumChannels NOTIFY NumChannelsChanged)
	int NumChannels() const {return m_ChannelList.size();}
	//! Returns the length (duration) of the sequence.
	double Length() const {return 53.14159;}
	//! Returns the channels as a property list
	QDeclarativeListProperty<GChannel> Channels() { return QDeclarativeListProperty<GChannel>(this, m_ChannelList); }
	//! return the ith channel
	GChannel* Channel(int index) const { return m_ChannelList.at(index); }

	//! Returns the scene that holds the event items in a tree structure.
	GEvScene* EventTreeScene() const { return m_pEventTreeScene; }
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

signals:
	//! emitted when the Length() of the sequence changed.
	void LengthChanged(double newLength);
	//! emitted when the number of channels changed
	void NumChannelsChanged(int newNumChannels);

private:
	//! The list of GChannel`s managed by this sequence.
	QList<GChannel*> m_ChannelList;
	//! There are two scenes associated with a GSequence. This one is for the representation of the GSynchEvent`s and will allow some display and user interactions through a QGraphicsView.
	GEvScene* m_pEventTreeScene;

public:
	GParamDouble m_Length;

	friend class GEvScene;
};

#endif // GSequence_H

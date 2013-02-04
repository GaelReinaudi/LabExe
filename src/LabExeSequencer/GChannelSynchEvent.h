#ifndef GCHANNELSYNCHEVENT_H
#define GCHANNELSYNCHEVENT_H
#include "labexe_global.h"

#include "Sequence/GSynchEvent.h"

class GChannel;
class GSynchEventGraphicsItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GChannelSynchEvent class is a GSynchEvent that is associated with a GChannel of a GSequence.
/*!
It is still displayed in the tree-scene of the sequence and provides the same functionalities as GSynchEvent.
It adds the following functionalities:
- has an extra QGraphicsItem, namely GSynchEventGraphicsItem, so that it can be also 
displayed in the sequence scene, within a channel, with some other user interactions.
- ...
*/
class GChannelSynchEvent : public GSynchEvent
{
	Q_OBJECT

public:
	//! Construct a GSynchEvent and assigns it to the channel pAssignedChannel. If no channel is provided, the sequence scene should own this event item.
	GChannelSynchEvent(GSynchEvent* parentEventSynchro, GChannel* pAssignedChannel = 0);
	~GChannelSynchEvent();

	//! Returns the channel the event is associated with. 0 if no channel.
	GChannel* Channel() const { return m_pChannel; }

public:
	//! Reimplemented from GSerializableSettings. Writes the settings to permanent storage using the provided QSettings. 
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializableSettings. Reads the settings from permanent storage using the provided QSettings. 
	virtual void InterpretSettings(QSettings& fromQsettings);
	//! assigns a channel to this event. Places it in the channel graphics item.
	void AssignChannel( GChannel* pChan );

private slots:
	//! update the position of the m_pEventGraphicsItem in the channel graphics item. 
	void UpdatePositionChannelEventItem( double newAbsoluteTime );

protected:
	//! For painting itself and interacting in the sequence QGraphicsView.
	GSynchEventGraphicsItem* m_pEventGraphicsItem;
	//! the pointer to the assigned channel
	GChannel* m_pChannel;

};

#endif // GCHANNELSYNCHEVENT_H

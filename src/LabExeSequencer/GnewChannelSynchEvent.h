#ifndef GCHANNELSYNCHEVENT_H
#define GCHANNELSYNCHEVENT_H
#include "labexe_global.h"

#include "Sequence/GSynchEvent.h"

class GnewChannel;
class GnewSynchEventGraphicsItem;

/////////////////////////////////////////////////////////////////////
//! \brief The GnewChannelSynchEvent class is a GSynchEvent that is associated with a GnewChannel of a GSequence.
/*!
It is still displayed in the tree-scene of the sequence and provides the same functionalities as GSynchEvent.
It adds the following functionalities:
- has an extra QGraphicsItem, namely GnewSynchEventGraphicsItem, so that it can be also 
displayed in the sequence scene, within a channel, with some other user interactions.
- ...
*/
class GnewChannelSynchEvent : public GSynchEvent
{
	Q_OBJECT

public:
	//! Construct a GSynchEvent and assigns it to the channel pAssignedChannel. If no channel is provided, the sequence scene should own this event item.
	GnewChannelSynchEvent(GSynchEvent* parentEventSynchro, GnewChannel* pAssignedChannel = 0);
	~GnewChannelSynchEvent();

	//! Returns the channel the event is associated with. 0 if no channel.
	GnewChannel* Channel() const { return m_pChannel; }

public:
	//! Reimplemented from GSerializableSettings. Writes the settings to permanent storage using the provided QSettings. 
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializableSettings. Reads the settings from permanent storage using the provided QSettings. 
	virtual void InterpretSettings(QSettings& fromQsettings);
	//! assigns a channel to this event. Places it in the channel graphics item.
	void AssignChannel( GnewChannel* pChan );

private slots:
	//! update the position of the m_pEventGraphicsItem in the channel graphics item. 
	void UpdatePositionChannelEventItem( double newAbsoluteTime );

protected:
	//! For painting itself and interacting in the sequence QGraphicsView.
	GnewSynchEventGraphicsItem* m_pEventGraphicsItem;
	//! the pointer to the assigned channel
	GnewChannel* m_pChannel;

};

#endif // GCHANNELSYNCHEVENT_H

#ifndef GCHANNEL_H
#define GCHANNEL_H

#include <QObject>
#include "GnewChannelGraphicsItem.h"
#include "ToolBox/GSerializable.h"
#include "labexe_global.h"

class GSequence;
class GSynchEvent;
class GnewInstruction;

#include <QMenu>

/////////////////////////////////////////////////////////////////////
//! \brief The GnewChannel class defines a channel in a sequence. 
/*!
It is the base class for different type of channels. Channels are meant to be filled 
with GnewInstruction of the corresponding type (like a GDigitalOutInstruction for a GDigitalOutChannel).

It has the following functionalities:
- contains a GraphicsItem that is part of the GSequenceGraphicsScene, and that will allow the display of instructions and the user interactions.
- It will manage the instructions at the time it needs to be translated in a device specific data. 
(e.g. an analog channel will be able to group with other analog channels to fill a matrix of double for the PXI system). 
*/
class GnewChannel : public QObject, public GSerializable
{
	Q_OBJECT

public:
	GnewChannel(GSequence* parentSequence);
	~GnewChannel();

	//! Returns the sequence this channel is part of.
	GSequence* Sequence();
	//! Returns a pointer to the graphics item of this channel
	GnewChannelGraphicsItem* ChannelGraphicsItem() const { return m_pChannelItem; }

public slots:
	//! Creates and adds a new GChannelSynchEvent to the sequence, assigned to this Channel, and returns a pointer to the new GSynchEvent. 0 if no event was created.
	GSynchEvent* CreateNewSynchEvent(GSynchEvent* pParentEvent = 0);
	//! Creates and adds a new instruction to the sequence using the registered type of instruction. registered means declared with G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GDerivateInstruction, GSynchEvent). Returns the new instruction, 0 if not successful.
	GnewInstruction* CreateNewInstruction(QString strRegisteredType = "", GSynchEvent* pParentEvent = 0);

public:
	//! Reimplemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	void InterpretSettings(QSettings& fromQsettings);

private:
	//! Makes some actions that can be triggered e.g. by a QMenu.
	virtual void createActions();
	//! action to add a new event to this channel
	QAction* m_pActionNewEvent;
	//! action to add a new instruction to this channel
	QAction* m_pActionNewInstruction;
	//! the graphical representation of the channel in a GSequenceGraphicsScene.
	GnewChannelGraphicsItem* m_pChannelItem;
};

#endif // GCHANNEL_H

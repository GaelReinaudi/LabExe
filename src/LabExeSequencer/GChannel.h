#ifndef GCHANNEL_H
#define GCHANNEL_H

#include <QObject>
#include "GChannelGraphicsItem.h"
#include "ToolBox/GSerializable.h"
#include "labexe_global.h"

class GSequence;
class GSynchEvent;
class GInstruction;

#include <QMenu>

/////////////////////////////////////////////////////////////////////
//! \brief The GChannel class defines a channel in a sequence. 
/*!
It is the base class for different type of channels. Channels are meant to be filled 
with GInstruction of the corresponding type (like a GDigitalOutInstruction for a GDigitalOutChannel).

It has the following functionalities:
- contains a GraphicsItem that is part of the GSequenceGraphicsScene, and that will allow the display of instructions and the user interactions.
- It will manage the instructions at the time it needs to be translated in a device specific data. 
(e.g. an analog channel will be able to group with other analog channels to fill a matrix of double for the PXI system). 
*/
class GChannel : public QObject, public GSerializable
{
	Q_OBJECT

public:
	GChannel(GSequence* parentSequence);
	~GChannel();

	//! Returns the sequence this channel is part of.
	GSequence* Sequence();
	//! Returns a pointer to the graphics item of this channel
	GChannelGraphicsItem* ChannelGraphicsItem() const { return m_pChannelItem; }

public:
	//! Reimplemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	void InterpretSettings(QSettings& fromQsettings);

private:
	//! the graphical representation of the channel in a GSequenceGraphicsScene.
	GChannelGraphicsItem* m_pChannelItem;
};

#endif // GCHANNEL_H

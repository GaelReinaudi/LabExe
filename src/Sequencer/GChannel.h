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
class LABEXE_EXPORT GChannel : public QObject, public GSerializable
{
	Q_OBJECT

public:
	GChannel(GSequence* parentSequence);
	~GChannel();

	//! Returns the sequence this channel is part of.
	GSequence* Sequence();
	//! Returns a pointer to the graphics item of this channel
	GChannelGraphicsItem* ChannelGraphicsItem() const { return m_pChannelItem; }

public slots:
	//! Creates and adds a new GChannelSynchEvent to the sequence, assigned to this Channel, and returns a pointer to the new GSynchEvent. 0 if no event was created.
	GSynchEvent* CreateNewSynchEvent(GSynchEvent* pParentEvent = 0);
	//! Creates and adds a new instruction to the sequence using the registered type of instruction. registered means declared with G_REGISTER_IN_FACTORY_WITH_PARENT_TYPE(GDerivateInstruction, GSynchEvent). Returns the new instruction, 0 if not successful.
	GInstruction* CreateNewInstruction(QString strRegisteredType = "", GSynchEvent* pParentEvent = 0);

// protected:
// 	//! Takes care of the contextMenuEvent coming from the GChannelGraphicsItem
// 	virtual void ContextMenuEvent( QGraphicsSceneContextMenuEvent * event );

public:
	//! Reimplemented from GSerializable. Writes the settings to permanent storage using the provided QSettings. 
	void PopulateSettings(QSettings& inQsettings);
	//! Reimplemented from GSerializable. Reads the settings from permanent storage using the provided QSettings. 
	void ReadFromSettings(QSettings& fromQsettings);

public:
	// TO MOVE TO A CLASS GMenuPopulator ?
	//! Populate a QMenu to allow some action to be executed on this channel. Can (Should) be re-implemented to reflect more action for derived channels.
	virtual void PopulateContextMenu(QMenu* pMenu);
	// TO MOVE TO A CLASS GMenuPopulator ?
	QMenu m_Menu;

private:
	//! Makes some actions that can be triggered e.g. by a QMenu.
	virtual void createActions();
	//! action to add a new event to this channel
	QAction* m_pActionNewEvent;
	//! action to add a new instruction to this channel
	QAction* m_pActionNewInstruction;
	//! the graphical representation of the channel in a GSequenceGraphicsScene.
	GChannelGraphicsItem* m_pChannelItem;
};

#endif // GCHANNEL_H

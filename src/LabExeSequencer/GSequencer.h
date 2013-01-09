#ifndef GSequencer_H
#define GSequencer_H

#include "Device/GProgDevice.h"
#include "Model/GSeqModel.h"

// TRY BASALT
#include "../../../MapExe/src/agent.h"

class GSequence;
class GSequence;

class GSequencer : public GProgDevice
{
	Q_OBJECT

public:
	GSequencer(QObject* pParent, QString uniqueIdentifierName = "");
	~GSequencer();

	//! Returns the sequence of this sequencer
	GSequence* Sequence() const { return m_pSequence; }
	//! Reimplemented to provide a GnewSequencerWidget
	GDeviceWidget* ProvideNewDeviceGroupBox(QWidget* inWhichWidget, QBoxLayout::Direction orientation = QBoxLayout::LeftToRight);

public:
	//! Reimplemented.
	void PopulateSettings( QSettings& inQsettings );
	//! Reimplemented.
	void InterpretSettings( QSettings& fromQsettings );

protected:
	//! Re-implemented
	void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget) {}

protected slots:
	void DebugIndex( const QModelIndex & index );

private:
	//! this sequencer has one unique sequence.
	GSequence* m_pSequence;
	GSequence* m_pSeq;
	GSeqModel* m_pModel;

	GMapGraphicsScene* m_pScene;

	friend class GnewSequencerWidget;
};

#endif // GSequencer_H

#ifndef GSequencer_H
#define GSequencer_H

#include "Device/GProgDevice.h"
#include "Model/GSeqModel.h"

class GSequence;

class GSequencer : public GProgDevice
{
	Q_OBJECT

public:
	GSequencer(QObject* pParent, QString uniqueIdentifierName = "");
	~GSequencer();

	//! Returns the sequence of this sequencer
	GSequence* Sequence() const { return m_pSequence; }
	//! Reimplemented to provide a GSequencerWidget
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
	GSeqModel* m_pModel;

	QGraphicsScene* m_pScene;

	friend class GSequencerWidget;
};

#endif // GSequencer_H

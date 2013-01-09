#ifndef GSEQUENCERBENCH_H
#define GSEQUENCERBENCH_H

#include "GWorkBench.h"
#include "Sequencer/GSequence.h"
class GSequencerWidget;

/////////////////////////////////////////////////////////////////////
//! \brief The GSequencerBench class defines a workbench intended to control a Sequencer. 
/*!
It is a GWorkBench where the central widget has been set to a GSequencerWidget.
*/
class GSequencerBench : public GWorkBench
{
	Q_OBJECT

public:
	GSequencerBench(QWidget *parent);
	~GSequencerBench();

public:
	//! Reimplemented.
	virtual void PopulateSettings( QSettings& inQsettings );
	//! Reimplemented.
	virtual void ReadFromSettings( QSettings& fromQsettings );

protected slots:
	//! meant to display the position of the mouse in the GSequenceGraphicsScene. Default displays the info in the status bar.
	void DisplayHoverInScene(QPointF pointHover);


private:
	//! the sequencer widget to display the sequence
	GSequencerWidget* m_pSequencerWidget;
	//! this sequencer has one unique sequence.
	GSequence m_Sequence;

	void createMenus();

};

#endif // GSEQUENCERBENCH_H

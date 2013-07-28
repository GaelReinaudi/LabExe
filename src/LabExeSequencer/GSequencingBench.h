#ifndef GSequencingBench_H
#define GSequencingBench_H

#include "TheLab/GWorkBench.h"
class GSequencer;

/////////////////////////////////////////////////////////////////////
//! \brief The GSequencingBench class defines a workbench intended to control a Sequencer. 
/*!
It is a GWorkBench where the central widget has been set to a GSequencerWidget.
*/
class GSequencingBench : public GWorkBench
{
	Q_OBJECT

public:
	GSequencingBench(QWidget *parent);
	~GSequencingBench();

public:
	//! Reimplemented.
	virtual void PopulateSettings( QSettings& inQsettings );
	//! Reimplemented.
	virtual void InterpretSettings( QSettings& fromQsettings );

protected:
	//! Re-implemented
	virtual void LatterBenchInitialization();

protected slots:
	//! meant to display the position of the mouse in the GSequenceGraphicsScene. Default displays the info in the status bar.
	void DisplayHoverInScene(QPointF pointHover);

private:
	//! the optimizer device
	GSequencer* m_pSequencer;
	//! creates the m_pSequencer if necessary
	void CreateSequencer();
};

#endif // GSequencingBench_H

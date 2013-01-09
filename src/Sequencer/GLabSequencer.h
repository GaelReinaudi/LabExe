#ifndef GLABSEQUENCER_H
#define GLABSEQUENCER_H

#include "TheLab/GWorkBench.h"
#include "ui_GLabSequencer.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GLabSequencer is a special type of GWorkBench that defines an interface to manipulate sequences of instrucitons.
/*!
It is a GWorkBench that displays in particular:
- a large view representing a sequence of instructions for different channels,
- a way to map those channel to actual output devices
*/
class GLabSequencer : public GWorkBench
{
	Q_OBJECT

public:
	GLabSequencer(QWidget *parent = 0);
	~GLabSequencer();

	void MakeTheSequence();

private:
	Ui::GLabSequencerClass ui;
};

#endif // GLABSEQUENCER_H

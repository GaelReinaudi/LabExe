#ifndef GSequencerWidget_H
#define GSequencerWidget_H
#include "labexesequencer_global.h"

#include "ui_GnewSequencerWidget.h"
#include "Device/GDeviceWidget.h"
#include "GSequencer.h"

class GnewSequencerWidget : public GDeviceWidget, public Ui::GnewSequencerWidget
{
	Q_OBJECT

public:
	GnewSequencerWidget(GSequencer* pTheSeq, QWidget* parent = 0);
	~GnewSequencerWidget();

private:
	GSequencer* m_pSequencer;
}; 

#endif // GSequencerWidget_H

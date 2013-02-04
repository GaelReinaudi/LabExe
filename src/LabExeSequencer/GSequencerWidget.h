#ifndef GSequencerWidget_H
#define GSequencerWidget_H
#include "labexesequencer_global.h"

#include "ui_GSequencerWidget.h"
#include "Device/GDeviceWidget.h"
#include "GSequencer.h"

class GSequencerWidget : public GDeviceWidget, public Ui::GSequencerWidget
{
	Q_OBJECT

public:
	GSequencerWidget(GSequencer* pTheSeq, QWidget* parent = 0);
	~GSequencerWidget();

private:
	GSequencer* m_pSequencer;
}; 

#endif // GSequencerWidget_H

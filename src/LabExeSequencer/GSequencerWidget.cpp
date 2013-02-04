#include "GSequencerWidget.h"
#include "Sequence/GSequence.h"

GSequencerWidget::GSequencerWidget(GSequencer* pTheSeq, QWidget* parent /*= 0*/)
	: GDeviceWidget(pTheSeq, parent)
	, m_pSequencer(pTheSeq)
{
	setupUi(this);

	if(!m_pSequencer->Sequence())
		return;

	GSequenceView* pSequenceView = new GSequenceView(this);
	pSeqViewLayout->addWidget(pSequenceView);
	pSequenceView->setScene(m_pSequencer->Sequence()->ChannelScene());
	pEventsView->setScene(m_pSequencer->Sequence()->EventTreeScene());

	connect(pButtonNewEvent, SIGNAL(clicked()), m_pSequencer->Sequence(), SLOT(CreateNewEvent()));
}

GSequencerWidget::~GSequencerWidget()
{

}

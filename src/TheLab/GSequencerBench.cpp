#include "GSequencerBench.h"
#include "Sequencer/GSequencerWidget.h"

// G_REGISTER_WORKBENCH_CLASS(GSequencerBench)

GSequencerBench::GSequencerBench(QWidget *parent)
	: GWorkBench(parent)
{
	m_pSequencerWidget = new GSequencerWidget();
	setCentralWidget(m_pSequencerWidget);

	// set the sequence's scene to the sequenceview.
	m_pSequencerWidget->pSequenceView->setScene(m_Sequence.ChannelScene());
	// set the sequence's event-scene to the other sequenceview.
	m_pSequencerWidget->pSequenceEventsView->setScene(m_Sequence.EventTreeScene());

	// connects to display the hover position in the status bar
	statusBar()->show();
	statusBar()->showMessage(tr("Ready"));
	connect(m_Sequence.EventTreeScene(), SIGNAL(HoverInScene(QPointF)), this, SLOT(DisplayHoverInScene(QPointF)), Qt::QueuedConnection);

	createMenus();

	connect(m_pSequencerWidget->pButtonNewEvent, SIGNAL(clicked()), &m_Sequence, SLOT(CreateNewEvent()));
}

GSequencerBench::~GSequencerBench()
{

}

void GSequencerBench::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&Sequence"));
	fileMenu->addAction(m_Sequence.m_pActionNewChannel);
}

void GSequencerBench::DisplayHoverInScene( QPointF pointHover )
{
	QStatusBar* pSB = statusBar();
	QString message("Pos: (%1 , %2)");
	pSB->showMessage(message.arg(pointHover.x()).arg(pointHover.y()));
}

void GSequencerBench::PopulateSettings( QSettings& inQsettings )
{
	GWorkBench::PopulateSettings(inQsettings);
	// save the sequence informations
	m_Sequence.PopulateSettings(inQsettings);
}

void GSequencerBench::ReadFromSettings( QSettings& fromQsettings )
{
	GWorkBench::ReadFromSettings(fromQsettings);
	// read the sequence informations
	m_Sequence.ReadFromSettings(fromQsettings);
}





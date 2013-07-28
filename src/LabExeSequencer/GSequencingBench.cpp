#include "GSequencingBench.h"
#include "GSequencer.h"
#include "GSequencerWidget.h"
#include "Sequence/GSequence.h"

G_REGISTER_WORKBENCH_CLASS(GSequencingBench)

GSequencingBench::GSequencingBench(QWidget *parent)
	: GWorkBench(parent)
	, m_pSequencer(0)
{
}

GSequencingBench::~GSequencingBench()
{

}

void GSequencingBench::DisplayHoverInScene( QPointF pointHover )
{
	QStatusBar* pSB = statusBar();
	QString message("Pos: (%1 , %2)");
	pSB->showMessage(message.arg(pointHover.x()).arg(pointHover.y()));
}

void GSequencingBench::PopulateSettings( QSettings& inQsettings )
{
	GWorkBench::PopulateSettings(inQsettings);
	// manually save the device id of the optimizer because we will want to know if an optimizer was saved
	inQsettings.setValue("sequencerDeviceID", m_pSequencer->UniqueSystemID());
}

void GSequencingBench::InterpretSettings( QSettings& fromQsettings )
{
	GWorkBench::InterpretSettings(fromQsettings);

	// if there was no sequencer saved in the bench and that we should create one
	QString sequID = fromQsettings.value("sequencerDeviceID", "").toString();
	if(sequID == "") {
		CreateSequencer();
	}
	// if it is there, we just update the pointer to it
	else {
		GDevice* pDev = GetDeviceFromDeviceManager(sequID);
		m_pSequencer = qobject_cast<GSequencer*>(pDev);
	}
}

void GSequencingBench::LatterBenchInitialization()
{
	// if there is no optimizer at this point, we add one
	if(!m_pSequencer)
		CreateSequencer();
	else
		setCentralWidget(DeviceWidget(m_pSequencer));

	createMenus();

	// connects to display the hover position in the status bar
	statusBar()->show();
	statusBar()->showMessage(tr("Ready"));
	connect(m_pSequencer->Sequence()->EventTreeScene(), SIGNAL(HoverInScene(QPointF)), this, SLOT(DisplayHoverInScene(QPointF)), Qt::QueuedConnection);
}

void GSequencingBench::CreateSequencer()
{
	if(m_pSequencer) {
		delete m_pSequencer;
	}
	m_pSequencer = new GSequencer(this);
	GDeviceWidget* pDevWid = AddDevice(m_pSequencer);
	setCentralWidget(pDevWid);
}
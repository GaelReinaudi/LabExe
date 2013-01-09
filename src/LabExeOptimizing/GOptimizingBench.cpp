#include "GOptimizingBench.h"
#include "TheLab/GBenchDockWidget.h"
#include "Device/GDeviceManager.h"
#include "GOptimizingWidget.h"
#include "GOptimizer.h"

 G_REGISTER_WORKBENCH_CLASS(GOptimizingBench)

GOptimizingBench::GOptimizingBench(QWidget *parent)
	: GWorkBench(parent)
	, m_pOpt(0)
{
}

GOptimizingBench::~GOptimizingBench()
{

}

void GOptimizingBench::PopulateSettings( QSettings& inQsettings )
{
	GWorkBench::PopulateSettings(inQsettings);
  	// manually save the device id of the optimizer because we will want to know if an optimizer was saved
	inQsettings.setValue("optimizerDeviceID", m_pOpt->UniqueSystemID());
}

void GOptimizingBench::InterpretSettings( QSettings& fromQsettings )
{
	GWorkBench::InterpretSettings(fromQsettings);

	// if there was no optimizer saved in the bench and that we should create one
	QString optimID = fromQsettings.value("optimizerDeviceID", "").toString();
	if(optimID == "") {
		CreateOptimizer();
	}
	// if it is there, we just update the pointer to it
	else {
		GDevice* pDev = GetDeviceFromDeviceManager(optimID);
		m_pOpt = qobject_cast<GOptimizer*>(pDev);
	}
}

void GOptimizingBench::LatterBenchInitialization()
{
	// if there is no optimizer at this point, we add one
	if(!m_pOpt)
		CreateOptimizer();
	else
		setCentralWidget(DeviceWidget(m_pOpt));
}

void GOptimizingBench::CreateOptimizer()
{
	if(m_pOpt)
		delete m_pOpt;
	m_pOpt = new GOptimizer(this);
	GDeviceWidget* pDevWid = AddDevice(m_pOpt);
	setCentralWidget(pDevWid);
}
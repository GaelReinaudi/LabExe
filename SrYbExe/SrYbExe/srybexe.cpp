#include "srybexe.h"
#ifdef Q_OS_WIN32
#include "LabExeImaging/labexeimaging.h"
#include "LabExeSequencer/labexesequencer.h"
#include "LabExeOptimizing/labexeoptimizing.h"
#include <WinSparkle/winsparkle.h>
#endif
 
#ifdef Q_OS_WIN32
namespace{ 
// int uhgf = _crtBreakAlloc = 872437;
//	int gfd = _CrtSetDbgFlag(0);
}
#endif
 
SrYbExe::SrYbExe(QWidget *parent, Qt::WFlags flags)
	: GLabControlPanel(parent)
{
	Q_UNUSED(flags);

	// loads the plugins that defined derived classes of GDevice
	LoadPluginsInDeviceManager();

	// that makes the code enter those respective dlls so that the workbenches get registered.
#ifdef Q_OS_WIN32
    LabExeImaging();
    LabExeSequencer();
    LabExeOptimizing();
#endif

#ifdef Q_OS_WIN32
	// Initialize WinSparkle as soon as the app itself is initialized, right before entering the event loop:
    win_sparkle_set_appcast_url("http://labexe.com/SrYbExeAutoUpdate.xml");
	wchar_t company_name[] = L"LabExe";
	wchar_t app_name[] = L"SrYbExe";
	wchar_t app_version[] = L"2.16.0";
    win_sparkle_set_app_details(company_name, app_name, app_version);
    win_sparkle_init();
#endif
}

SrYbExe::~SrYbExe()
{
#ifdef Q_OS_WIN32
   win_sparkle_cleanup();
#endif
}

void SrYbExe::CheckForUpdate()
{
#ifdef Q_OS_WIN32
  win_sparkle_check_update_with_ui();
#endif
}

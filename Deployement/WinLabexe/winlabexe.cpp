#include "winlabexe.h"
#include "labexeimaging/labexeimaging.h"
// #include "LabExeOptimizing/labexeoptimizing.h"

#include <WinSparkle/winsparkle.h>

WinLabexe::WinLabexe(QWidget *parent, Qt::WFlags flags)
	: GLabControlPanel(parent)
{
	// that makes the code enter those respective dlls so that the workbenches get registered.
	LabExeImaging();
// 	LabExeOptimizing();

	// Initialize WinSparkle as soon as the app itself is initialized, right before entering the event loop:
	win_sparkle_set_appcast_url("http://labexe.com/WinLabexe32AutoUpdate.xml");
	wchar_t company_name[] = L"LabExe";
	wchar_t app_name[] = L"WinLabexe";
	wchar_t app_version[] = L"2.8.2";
	win_sparkle_set_app_details(company_name, app_name, app_version);
	win_sparkle_init();
}

WinLabexe::~WinLabexe()
{
	win_sparkle_cleanup();
}

void WinLabexe::CheckForUpdate()
{
	win_sparkle_check_update_with_ui();
}
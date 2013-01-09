#ifndef WINLABEXE_H
#define WINLABEXE_H

#include "labexe.h"

class WinLabexe : public GLabControlPanel
{
	Q_OBJECT

public:
	WinLabexe(QWidget *parent = 0, Qt::WFlags flags = 0);
	~WinLabexe();

	//! Re-implemented
	void CheckForUpdate();
};

#endif // WINLABEXE_H

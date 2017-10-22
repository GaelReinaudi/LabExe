#ifndef SRYBEXE_H
#define SRYBEXE_H

#include "labexe.h"

class SrYbExe : public GLabControlPanel
{
	Q_OBJECT

public:
	SrYbExe(QString controlPanelIniFilePath, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~SrYbExe();

	//! Re-implemented
	void CheckForUpdate();

protected:
	QList<GDevice*> DevicesToShelf();
};

#endif // SRYBEXE_H

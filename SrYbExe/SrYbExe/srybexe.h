#ifndef SRYBEXE_H
#define SRYBEXE_H


#include "TheLab/GLabControlPanel.h"
class SrYbExe : public GLabControlPanel
{
	Q_OBJECT

public:
    SrYbExe(QString controlPanelIniFilePath, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~SrYbExe();

	//! Re-implemented
	void CheckForUpdate();

protected:
	QList<GDevice*> DevicesToShelf();
};

#endif // SRYBEXE_H

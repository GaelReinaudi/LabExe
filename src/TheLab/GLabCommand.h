#ifndef GLABCOMMAND_H
#define GLABCOMMAND_H

#include <QUndoCommand>
#include <QHash>
#include "Device/GDeviceCommand.h"

class GDevice;
class GWorkBench;
class GDeviceWidget;

class GCommandAddDevice : public QUndoCommand
{
public:
	GCommandAddDevice(GDevice* theDevice, GWorkBench* theLab, QUndoCommand *parent = 0);
	GCommandAddDevice(GDevice* theDevice, GWorkBench* theLab, QWidget* pForWhichWidget, QUndoCommand *parent = 0);
	~GCommandAddDevice();
	void undo();
	void redo();
	//! returns the widget that was added, 0 if none
	GDeviceWidget* AddedWidget() const { return m_pAddedWidget; }

private:
	GDeviceWidget* m_pAddedWidget;
	GDevice* m_pDevice;
	GWorkBench* m_pBench;
	QWidget* m_pInWhichWidget;
};

class GCommandRemoveDevice : public QUndoCommand
{
public:
	GCommandRemoveDevice(GDevice* theDevice, GWorkBench* theLab, QUndoCommand *parent = 0);
	~GCommandRemoveDevice();
	void undo();
	void redo();
private:
	GDevice* m_pDevice;
	GWorkBench* m_pBench;
	QHash<GDeviceWidget*, QWidget*> m_DeviceWidgetsAndParents;
	QHash<GDeviceWidget*, QPoint> m_DeviceWidgetsAndPosition;
	QHash<GDeviceWidget*, bool> m_DeviceWidgetsWasShown;
};

#endif // GLABCOMMAND_H

#ifndef GDEVICECOMMAND_H
#define GDEVICECOMMAND_H
#include "labexe_global.h"

#include <QUndoCommand>
#include <QPoint>

class QWidget;
class GDevice;

/////////////////////////////////////////////////////////////////////
//! This File contains definition of QUndoCommand so that a GDevice can Undo/Redo some operations.
/*!
*/

class GCommandWidgetMove : public QUndoCommand
{
public:
	GCommandWidgetMove(QWidget *theWidget, const QPoint &oldPos, QUndoCommand *parent = 0);
	~GCommandWidgetMove();
	void undo();
	void redo();
private:
	QWidget* m_pWidget;
	QPoint m_OldPosition;
	QPoint m_NewPosition;
};

class GCommandDeviceReName : public QUndoCommand
{
public:
	GCommandDeviceReName(GDevice *theDevice, const QString &oldName, QUndoCommand *parent = 0);
	~GCommandDeviceReName();
	void undo();
	void redo();
private:
	GDevice* m_pDevice;
	QString m_OldName;
	QString m_NewName;
};

#endif // GDEVICECOMMAND_H

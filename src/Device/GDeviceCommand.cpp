#include "GDeviceCommand.h"
#include "GDevice.h"
#include <QWidget>

GCommandWidgetMove::GCommandWidgetMove(QWidget *theWidget, const QPoint &oldPos, QUndoCommand *parent)
: QUndoCommand(parent)
{
	m_pWidget = theWidget;
	m_NewPosition = m_pWidget->pos();
	m_OldPosition = oldPos;
}

GCommandWidgetMove::~GCommandWidgetMove()
{

}

void GCommandWidgetMove::undo()
{
	m_pWidget->move(m_OldPosition);
	setText(QObject::tr("Move %1 at (%2,%3)").arg("widget").arg(m_NewPosition.x()).arg(m_NewPosition.y()));
}

void GCommandWidgetMove::redo()
{
	m_pWidget->move(m_NewPosition);
	setText(QObject::tr("Move %1 at (%2,%3)").arg("widget").arg(m_NewPosition.x()).arg(m_NewPosition.y()));
}

//************************************************************************
//************************************************************************
//************************************************************************

GCommandDeviceReName::GCommandDeviceReName( GDevice *theDevice, const QString &oldName, QUndoCommand *parent /*= 0*/ )
{
	m_pDevice = theDevice;
	m_NewName = m_pDevice->Name();
	m_OldName = oldName;
}

GCommandDeviceReName::~GCommandDeviceReName()
{

}

void GCommandDeviceReName::undo()
{
	m_pDevice->ReName(m_OldName);
	setText(QObject::tr("Rename %1 \"%2\"").arg("device").arg(m_NewName));
}

void GCommandDeviceReName::redo()
{
	m_pDevice->ReName(m_NewName);
	setText(QObject::tr("Rename %1 \"%2\"").arg("device").arg(m_NewName));
}

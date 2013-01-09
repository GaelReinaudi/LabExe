#ifndef GDEVICEWIDGET_H
#define GDEVICEWIDGET_H
#include "labexe_global.h"

#include <QtGui>

class GDevice;
class GDeviceWidgetImpl;

/////////////////////////////////////////////////////////////////////
//! \brief The GDeviceWidget class defines a widget meant to contain GUI for a GDevice.
/*!

*/
class LABEXE_EXPORT GDeviceWidget : public QFrame
{
	Q_OBJECT

public:
	GDeviceWidget(GDevice* pOfWhichDevice, QWidget *parent);
	~GDeviceWidget();

	//! Returns the device that is managed by this widget.
	GDevice* Device() const { return m_pDevice; }
	//! Adds a QGroupBox in the GDeviceWidget. It is aimed to help organizing the widgets in there! You can provide a Layout that will help the QGroupBox organizing itself
	QGroupBox* AddSubGroupBox(QString Named, QLayout* groupLayout = 0);
	//! Same as AddSubGroupBox, but it just adds the layout, without a surrounding QGroupBox
	void AddSubLayout(QLayout* groupLayout);
	//! Tels the GroupBox of the GDevice to not display the Name() of the GDevice.
	void DontDisplayTitle();

public slots:
	//! Changes the name of the groupbox of this GDeviceWidget
	void Rename(QString newName);
	//! Makes the device widget enable or disable. Actually, only the inside of the QGroupBox is enable or disable so that the outside frame keeps some responsivity to context menu event.
	void Enable(bool doEnable);

protected:
	//! takes a screenshot of the widget and returns the QPixmap
	QPixmap screenShot();

protected:
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	//! Re-implemented in order to accept dropping in this widget
	void dragEnterEvent(QDragEnterEvent *event); 
	//! Re-implemented in order to accept dropping in this widget
	void dropEvent(QDropEvent *event);

signals:
// 	void FinishedMoveByHand(QPoint oldPos);

private:
	//! Implementation for the members
	GDeviceWidgetImpl* m_Impl;

	GDevice* m_pDevice;
	QPoint m_WidgetPosWhenMousePressed;
	QPoint m_MousePressedGlobalPos;
	QPoint m_MousePressedInWidgetPos;
	QPoint m_MousePressedInParentPos;
};

#endif // GDEVICEWIDGET_H

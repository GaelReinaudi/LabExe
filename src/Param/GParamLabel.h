#ifndef GPARAMLABEL_H
#define GPARAMLABEL_H
#include "labexe_global.h"

#include <QLabel>
#include <QtWidgets>

class GParam;

/////////////////////////////////////////////////////////////////////
//! \brief The GParamLabel class allow the display of a param name on the GUI. 
/*!
A GParamLabel is associateed with one GParam.
A GParam can be associated with one (or many) GParamLabel (inherits QLabel) that 
are provided by the function ProvideNewLabel(). The GParamLabel display a name and can be used 
e.g. for draging and droping the GParam in useful widgets (like GVariatorWidgetContainer).
*/
class GParamLabel : public QLabel
{
	Q_OBJECT

public:
	GParamLabel(GParam* pTheParam, QWidget *parent);
	~GParamLabel();
	const GParam* Param() const { return m_pParam; }

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

protected slots:
	//! highlights this widget by changing its color
	void StartHighlight(int msDuration = 0);
	//! remove the highlight on this widget by changing its color back to normal
	void StopHighlight();
protected:
    void enterEvent(QEnterEvent *event );
	void leaveEvent( QEvent* event );

private:
	QPoint m_DragStartPosition;
	GParam* m_pParam;
	QPalette m_InititalPalette;
	friend class GParam;
};

#endif // GPARAMLABEL_H

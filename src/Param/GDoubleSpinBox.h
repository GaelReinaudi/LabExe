#ifndef GDOUBLESPINBOX_H
#define GDOUBLESPINBOX_H
#include "labexe_global.h"

#include <QDoubleSpinBox>
#include <QtGui>

class GParamDouble;

/////////////////////////////////////////////////////////////////////
//! \brief The GDoubleSpinBox class is a Qt QDoubleSpinBox with some added functionalities...
/*!
like being aware of the GParamDouble it might be controlling.
*/
class LABEXE_EXPORT GDoubleSpinBox : public QDoubleSpinBox
{
	Q_OBJECT
public:
	GDoubleSpinBox(QWidget *parent);
	GDoubleSpinBox(GParamDouble* theControlledParam, QWidget *parent);
	~GDoubleSpinBox();

protected:
	void mousePressEvent( QMouseEvent * event );
	void mouseReleaseEvent( QMouseEvent * event );
	void mouseMoveEvent( QMouseEvent * event );
	void leaveEvent( QEvent * event );

public slots:
	//! Sets the range of the spinbox. If DontEmitChangeSignal, then the signal ValueChangedSignificantly() wont be emitted if the value has to be adjusted.
	void SetRange(double newMin, double newMax, bool DontEmitChangeSignal = false);
	//! Sets the decimal of the spinbox according to the one of the param.
	void SetDecimal(int nDec);
	//! Sets the step size of the spinbox according to the one of the param.
	void SetStep( double theStep );
	//! sets the units if the param is ShowingUnits();
	void UpdateUnits();

private slots:
	//! Slot intended to update the value in the box, but without sending the signal. It prevents infinite updating loops.
	void SetValue_WithoutSignal(const double& valueToDisplay);
	//! Same as SetValue_WithoutSignal(), but gets the value from the param itself.
	void UpdateValue_WithoutSignal();
	//! This slot is here to process the Qt valueChanged signal and re-emit a ValueChangedSignificantly unless the SetValue_WithoutSignal() slot was called.
	void ReProcessValueChangedSiganl(const double& newValue);

private:
	//! is used to emit/not emit ValueChangedSignificantly() when valueChanged(). See SetValue_WithoutSignal().
	bool m_ShouldEmit_ValueChangedSignificantly;

signals:
	//! emitted when valueChanged(), unless the SetValue_WithoutSignal() slot was called.
	void ValueChangedSignificantly(double);

private:
	void Init();
	QPoint m_DragStartPosition;
	GParamDouble* m_pParam;
};

#endif // GDOUBLESPINBOX_H

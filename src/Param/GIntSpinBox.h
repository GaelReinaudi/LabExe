#ifndef GINTSPINBOX_H
#define GINTSPINBOX_H
#include "labexe_global.h"

#include <QSpinBox>
#include <QtGui>

class GParamInt;

/////////////////////////////////////////////////////////////////////
//! \brief The GIntSpinBox class is a Qt QSpinBox with some added functionalities...
/*!
like being aware of the GParamInt it might be controlling.
*/
class LABEXE_EXPORT GIntSpinBox : public QSpinBox
{
	Q_OBJECT
public:
	GIntSpinBox(QWidget *parent);
	GIntSpinBox(GParamInt* theControlledParam, QWidget *parent);
	~GIntSpinBox();

public slots:
	//! Sets the range of the spinbox. If DontEmitChangeSignal, then the signal ValueChangedSignificantly() wont be emitted if the value has to be adjusted.
	void SetRange(double newMin, double newMax, bool DontEmitChangeSignal = false);
	//! Sets the step size of the spinbox according to the one of the param.
	void SetStep( double theStep );
	//! sets the units if the param is ShowingUnits();
	void UpdateUnits();

private slots:
	//! Slot intended to update the value in the box, but without sending the signal. It prevents infinite updating loops.
	void SetValue_WithoutSignal(const int& valueToDisplay);
	//! Same as SetValue_WithoutSignal(), but gets the value from the param itself.
	void UpdateValue_WithoutSignal();
	//! This slot is here to process the Qt valueChanged signal and re-emit a ValueChangedSignificantly unless the SetValue_WithoutSignal() slot was called.
	void ReProcessValueChangedSiganl(const int& newValue);

private:
	//! is used to emit/not emit ValueChangedSignificantly() when valueChanged(). See SetValue_WithoutSignal().
	bool m_ShouldEmit_ValueChangedSignificantly;

signals:
	//! emitted when valueChanged(), unless the SetValue_WithoutSignal() slot was called.
	void ValueChangedSignificantly(int);

private:
	void Init();
	GParamInt* m_pParam;
};

#endif // GINTSPINBOX_H

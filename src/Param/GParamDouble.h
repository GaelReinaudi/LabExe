#ifndef GPARAMDOUBLE_H
#define GPARAMDOUBLE_H
#include "labexe_global.h"

#include "GParamNum.h"

/////////////////////////////////////////////////////////////////////
//! \brief The GParamDouble class defines a GParamNum that holds a double.
/*!
*/
class LABEXE_EXPORT GParamDouble : public GParamNum
{
	Q_OBJECT

public:
	GParamDouble(QString theName, QObject* parent, GParam::Properties paramOptions = NoOption);
	~GParamDouble();

	virtual GParamDouble& operator=(double theValue) {SetParamValue(theValue); return *this;}
	virtual GParamDouble& operator=(int theValue) {SetParamValue(theValue); return *this;}
	virtual operator double() const {return DoubleValue();}

	//! Implemented
	double DoubleValue() const {
		return m_valDouble;
// 		m_MutexVariant.lock();
// 		double valCopy = m_valDouble;
// 		m_MutexVariant.unlock();
// 		return valCopy;
	}
	//! Implemented
	int IntValue() const {
		qDebug() << "conv to IntValue()";
		return int(m_valDouble);
	}
	//! Re-implemented
	GParamSettingsWidget* PopupSettingWidget();

public:
	//! For convenience. Just like ProvideNewParamWidget(), but it actually returns a GDoubleSpinBox
	GDoubleSpinBox* ProvideNewParamSpinBox(QWidget* forWhichParent);
	//! Implemented
	QWidget* ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid = Default);

public slots:
	//! Implemented
	void SetParamValue(const double& theNewValue, bool sendUpdateSignals = true, bool sendDisplayUpdateSignal = true);
	void SetParamValue(const int& theNewValue, bool sendUpdateSignals /*= true*/, bool sendDisplayUpdateSignal /*= true*/) {
		qDebug() << "set GParamDouble with int";
		SetParamValue(double(theNewValue), sendUpdateSignals, sendDisplayUpdateSignal);
	}

signals:
// 	//! Emitted when the value was updated (i.e. SetParamValue() was called).
// 	void ValueUpdated(const double& theNewValue);
// 	//! Emitted after ValueUpdated but only if the value did change, i.e. if(!gEqual(acceptedValue, oldVal))
// 	void ValueDidChange(const double& theNewValue);
// 	//! Emitted after ValueUpdated but this should only be connect to display things in the ui, it should not create an other emission from the ui, thus avoiding infinite loops.
// 	void ValueUpdateForDisplay(const double& theNewValue);

private:
	//! the actual value
	double m_valDouble;

};

#endif // GPARAMDOUBLE_H

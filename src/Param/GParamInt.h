#ifndef GPARAMINT_H
#define GPARAMINT_H
#include "labexe_global.h"

#include "GParamNum.h"
class QComboBox;

/////////////////////////////////////////////////////////////////////
//! \brief The GParamDouble class defines a GParamNum that represent adequately an integer.
/*!
*/
class LABEXE_EXPORT GParamInt : public GParamNum
{
	Q_OBJECT

public:
	GParamInt(QString theName, QObject* parent, GParam::Properties paramOptions = NoOption);
	~GParamInt();

	virtual GParamInt& operator=(int theValue) {SetParamValue(theValue); return *this;}
	virtual GParamInt& operator+=(int theAddValue) {SetParamValue(IntValue() + theAddValue); return *this;}
	virtual operator int() const {return IntValue();}

	//! Implemented
	int IntValue() const {
		return m_valInt;
// 		m_MutexVariant.lock();
// 		int valCopy = m_valInt;
// 		m_MutexVariant.unlock();
// 		return valCopy;
	}
	//! Implemented
	double DoubleValue() const {
		qDebug() << "conv to DoubleValue()";
		return double(m_valInt);
	}

	//! Re-implemented
	QVariant ToVariant() const {return QVariant(IntValue());}
	//! Implemented
	QString StringContent(char format = 'g', int precision = 6) const { return QString::number(IntValue());}

public:
	//! Implemented
	QWidget* ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid = Default);
	//! For convenience. Just like ProvideNewParamWidget(), but it actually returns a GDoubleSpinBox
	GIntSpinBox* ProvideNewParamSpinBox(QWidget* forWhichParent);
	//! For convenience. returns a combo box populated with the strings in the nameList
	QComboBox* ProvideNewParamComboBox(QWidget* forWhichParent, QStringList nameList);

public slots:
	//! Re-implemented. 
	void SetParamValue(const int& theNewValue, bool sendUpdateSignals = true, bool sendDisplayUpdateSignal = true);
	void SetParamValue(const double& theNewValue, bool sendUpdateSignals = true, bool sendDisplayUpdateSignal = true) {
		qDebug() << "set GParamInt with double";
		SetParamValue(int(theNewValue), sendUpdateSignals, sendDisplayUpdateSignal);
	}
	//! Implemented 
	virtual void SetFromVariant( QVariant varVal ) { SetParamValue(varVal.toInt()); }

protected:
	//! Re-implemented.
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented.
	virtual void InterpretSettings(QSettings& fromQsettings);

signals:
// 	//! Emitted when the value was updated (ie. SetParamValue() was called).
// 	void ValueUpdated(const int & theNewValue);
// 	//! Emitted after ValueUpdated but only if the value did change, i.e. if(!gEqual(acceptedValue, oldVal))
// 	void ValueDidChange(const int& theNewValue);
// 	//! Emitted after ValueUpdated but this should only be connect to display things in the ui, it should not create an other emission from the ui, thus avoiding infinite loops.
// 	void ValueUpdateForDisplay(const int& theNewValue);

private:
	//! the actual value
	int m_valInt;

};

#endif // GPARAMINT_H

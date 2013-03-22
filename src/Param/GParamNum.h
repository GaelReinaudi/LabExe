#ifndef GPARAMNUM_H
#define GPARAMNUM_H
#include "labexe_global.h"

#include "GParam.h"

class GDoubleSpinBox;

//! function to compare two doubles. Currently, it returns the result of qFuzzyCompare().
bool LABEXE_EXPORT gEqual(double val1, double val2);

//! the default minimum for a GParamNum
#define G_DEFAULT_RANGE_MIN (-9999999)
//! the default maximum for a GParamNum
#define G_DEFAULT_RANGE_MAX (9999999)

/////////////////////////////////////////////////////////////////////
//! \brief The GParamNum class defines a GParam that holds a numerical value.
/*!
It can be converted to a double type. and the operator=(double) can be used.
You can set a hard limit to the values the param can take. 
*/
class LABEXE_EXPORT GParamNum : public GParam
{
	Q_OBJECT

public:
	GParamNum(QString theName, QObject* parent, GParam::Properties paramOptions = NoOption);
	~GParamNum();

	virtual GParamNum& operator=(double theValue) {SetParamValue(theValue); return *this;}
	virtual GParamNum& operator+=(double theValue) {SetParamValue(DoubleValue() + theValue); return *this;}
	virtual GParamNum& operator-=(double theValue) {SetParamValue(DoubleValue() - theValue); return *this;}
	virtual double operator+(double theValue) {return DoubleValue() + theValue;}
	virtual double operator-(double theValue) {return DoubleValue() - theValue;}

	//! Returns the value as a double. A mutex protects the reading;
	double DoubleValue() const {
		m_MutexVariant.lock();
		double valCopy = m_valDouble;
		m_MutexVariant.unlock();
		return valCopy;
	}
	//! Returns the value as an int. A mutex protects the reading;
	int IntValue() const { return qRound(DoubleValue()); }
	//! Re-implemented
	QVariant ToVariant() const { return QVariant(DoubleValue()); }
	//! Implemented
	QString StringContent(char format = 'g', int precision = 6) const { return QString::number(DoubleValue(), format, precision);}

protected:
	//! Re-implemented.
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented.
	virtual void InterpretSettings(QSettings& fromQsettings);

public:
	// Some settings of the GParamNum

	//! Sets the range the Param can never go out of.
	void SetHardLimits(double minVal, double maxVal);
	//! The minimum of the hard limit
	double Minimum() {return m_ParamSettings.minimum();}
	//! The maximum of the hard limit
	double Maximum() {return m_ParamSettings.maximum();}
	//! Sets the typical step value by which someone would want to increment the param using the button or the scrolling. It is just going to be the spinbox step size. It is not the absolute precision of the param.
	void SetTypicalStep(double TypStep);
	//! The typical step value by which someone would want to increment the param using the button or the scrolling. It is just going to be the spinbox step size. It is not the absolute precision of the param.
	double TypicalStep() {return m_ParamSettings.singleStep();}
	//! Sets the absolute display precision of the param. It only affects the number of digits of the spinbox by using the setDecimals() function.
	void SetDisplayDecimals(int NumberOfDecimals);
	//! The absolute display precision of the param. It only affects the number of digits of the spinbox by using the setDecimals() function.
	int DisplayDecimals() {return m_ParamSettings.decimals();}
	//! Saves the settings of the param to the registry
	void SaveParamSettingsToRegistry();

	//! Provides a widget that will be used to control (read? or modify?) the param.
	GParamControlWidget* ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid = Default);

public slots:
	//! Sets a new value. If sendUpdateSignals, it will emit the signal ValueUpdated() and ValueDidChange() if it did change. If sendDisplayUpdateSignal, it will emit the signal ValueUpdateForDisplay().
	virtual void SetParamValue(const int& theNewValue, bool sendUpdateSignals = true, bool sendDisplayUpdateSignal = true);
	//! Sets a new value. If sendUpdateSignals, it will emit the signal ValueUpdated() and ValueDidChange() if it did change. If sendDisplayUpdateSignal, it will emit the signal ValueUpdateForDisplay().
	virtual void SetParamValue(const double& theNewValue, bool sendUpdateSignals = true, bool sendDisplayUpdateSignal = true);
	//! Implemented 
	virtual void SetParamValue( QVariant varVal ) { SetParamValue(varVal.toDouble()); }
	//! Restores the settings of the param from the registry if any. See SaveParamSettingsToRegistry().
	void RestoreParamSettingsFromRegistry();
	//! TEMPORARY workaround to emit many values in a single call that emits the ManyValuesAvailable() signal
	void EmitManyValues(const GVectorDouble & theValues) {emit ManyValuesAvailable(theValues);}

protected:

signals:
	//! Emitted when the value was updated (i.e. SetParamValue() was called).
	void ValueUpdated(const double& theNewValue);
	//! Emitted after ValueUpdated but only if the value did change, i.e. if(!gEqual(acceptedValue, oldVal))
	void ValueDidChange(const double& theNewValue);
	//! Emitted after ValueUpdated but this should only be connect to display things in the ui, it should not create an other emission from the ui, thus avoiding infinite loops.
	void ValueUpdateForDisplay(const double& theNewValue);
	//! Emitted when the hard limits changed.
	void HardLimitsChanged(double newMin, double newMax);
	//! Emitted when the decimal number changed.
	void DisplayDecimalsChanged(int newNumberOfDecimals);
	//! Emitted when the typical step size changed.
	void TypicalStepChanged(double newTypStep);
	//! TEMPORARY workaround to emit many values in a single signal
	void ManyValuesAvailable(const GVectorDouble & theValues);


private:

protected:
	//! the actual value
	double m_valDouble;
	//! this spinbox is used only to hold certain characteristics of this param, like step-size, decimal number, limits
	QDoubleSpinBox m_ParamSettings;

};

Q_DECLARE_METATYPE(GParamNum*)

#endif // GPARAMNUM_H

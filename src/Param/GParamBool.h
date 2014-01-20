#ifndef GPARAMBOOL_H
#define GPARAMBOOL_H
#include "labexe_global.h"

#include "GParam.h"
#include <QPushButton>
#include <QCheckBox>

/////////////////////////////////////////////////////////////////////
//! \brief The GParamNum class defines a GParam that holds a numericql value.
/*!
It can be converted to a double type. and the operator=(double) can be used.
You can set a hard limit to the values the param can take. 
*/
class LABEXE_EXPORT GParamBool : public GParam
{
	Q_OBJECT

public:
	GParamBool(QString theName, QObject* parent, GParam::Properties paramOptions = NoOption);
	~GParamBool();

	virtual GParamBool& operator=(bool theValue) {SetParamValue(theValue); return *this;}
	virtual operator bool() {return BoolValue();}

	//! Returns the value as a bool. A mutex protects the reading;
	bool BoolValue() const {
		return m_val;
		m_MutexVariant.lock();
		bool valCopy = m_val;
		m_MutexVariant.unlock();
		return valCopy;
	}
	//! Re-implemented
	QVariant ToVariant() const {return QVariant(BoolValue());}
	//! Implemented
    QString StringContent(char format = 'g', int precision = 6) const { Q_UNUSED(format); Q_UNUSED(precision); return BoolValue()?"true":"false";}

protected:
	//! Re-implemented.
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented.
	virtual void InterpretSettings(QSettings& fromQsettings);

public:
	//! Provides a widget that will be used to control (read? or modify?) the param.
	QWidget* ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid = Default);
	//! For convenience. Just like ProvideNewParamWidget(), but it actually returns a QPushButton. If theLabel is empty, it uses the name of the param.
	QPushButton* ProvideNewParamButton(QWidget* forWhichParent, QString theLabel = "");
	//! For convenience. Just like ProvideNewParamWidget(), but it actually returns a QCheckBox with a label
	QCheckBox* ProvideNewParamCheckBox(QWidget* forWhichParent, QString theLabel);
	//! For convenience. Just like ProvideNewParamCheckBox(), but it uses the name of the param for naming the checkbox
	QCheckBox* ProvideNewParamCheckBox(QWidget* forWhichParent);
	//! For convenience. Just like ProvideNewParamWidget(), but it returns a widget with 2 radio buttons inside it.
	QWidget* ProvideNewParamRadioButtons(QWidget* forWhichParent, QString onText, QString offText);

public slots:
	//! Sets a new value and emits the signal ValueUpdated(bool) 
	virtual void SetParamValue(bool theNewValue);
	//! Implemented 
	virtual void SetFromVariant( QVariant varVal ) { SetParamValue(varVal.toBool()); }

signals:
	//! Emitted when the value was updated (i.e. SetParamValue() was called).
	void ValueUpdated(bool theNewValue);
	//! Emitted when the value was updated, like ValueUpdated(), but with the opposite argument.
	void ValueUpdatedNegativeArgument(bool theOppositeOfTheNewValue);
	//! Emitted after ValueUpdated but only if the value did change, i.e. if(!gEqual(acceptedValue, oldVal))
	void ValueDidChange(bool theNewValue);

private:
	//! the actual value
	bool m_val;
	//! count of number of times it switched to true
	int m_SwitchTrueCount;
	
};

#endif // GPARAMBOOL_H

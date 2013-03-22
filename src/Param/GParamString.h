#ifndef GPARAMSTRING_H
#define GPARAMSTRING_H
#include "labexe_global.h"

#include "GParam.h"
#include <QLineEdit>
#include <QString>

/////////////////////////////////////////////////////////////////////
//! \brief The GParamString class defines a GParam that holds a string value.
/*!
*/
class LABEXE_EXPORT GParamString : public GParam
{
	Q_OBJECT

public:
	GParamString(QString theName, QObject* parent, GParam::Properties paramOptions = NoOption);
	~GParamString();

	virtual GParamString& operator=(const QString& theValue) { SetParamValue(theValue); return *this; }
	virtual operator QString() { return StringValue(); }

	//! Returns the value as a QString. A mutex protects the reading.
	QString StringValue() const;
	//! Re-implemented
	QVariant ToVariant() const {return QVariant(StringValue());}
	//! Implemented
	QString StringContent(char format = 'g', int precision = 6) const { return StringValue(); }

public:
	//! Provides a widget that will be used to control (read? or modify?) the param.
	GParamControlWidget* ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid = Default);
	//! For convenience. Just like ProvideNewParamWidget(), but it actually returns a QLineEdit
	QLineEdit* ProvideNewParamLineEdit(QWidget* forWhichParent);

protected:
	//! Re-implemented.
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented.
	virtual void InterpretSettings(QSettings& fromQsettings);

signals:
	//! Emitted when the value was updated (ie. SetParamValue() was called).
	void ValueUpdated(const QString& theNewValue);

protected slots:
	//! Sets a new value and emits the signal ValueUpdatedProgramatically(QString).
	virtual void SetParamValue(const QString& theNewValue);
	//! Implemented 
	virtual void SetParamValue( QVariant varVal ) { SetParamValue(varVal.toString()); }

private:
	//! the actual value
	QString m_val;
};

class GLineEditWithCompare : public QLineEdit
{
	Q_OBJECT

public:
	GLineEditWithCompare( QWidget * parent = 0 );
	GLineEditWithCompare( const QString & contents, QWidget * parent = 0 );

signals:
	//! Emitted with the value of the text if the return key is pressed
	void TextEditedAndReturn(const QString & text);

public slots:
	void SetTextIfDifferent(const QString & contents);

private slots:
	//! Used to emit the TextEditedAndReturn() signal
	void EmitOnFinishedEditting();

};

#endif // GPARAMSTRING_H
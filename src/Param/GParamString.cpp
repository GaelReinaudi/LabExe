#include "GParamString.h"
#include "GParamManager.h"

G_REGISTER_NEW_PARAM_CLASS(GParamString);

GParamString::GParamString(QString theName, QObject* parent, GParam::Properties paramOptions /*= NoOption*/)
	: GParam(theName, parent, paramOptions)
	, m_val("")
{
}

GParamString::~GParamString()
{
}

void GParamString::SetParamValue( const QString& theNewValue )
{
	QString oldVal = StringValue();

	// if the signal comes from a widget and the value is the same
	// let's ignore the order to prevent loops
	if(qobject_cast<QWidget*>(sender()))
		if(theNewValue == oldVal)
			return;

	m_MutexVariant.lock();
	m_val = theNewValue;
	m_MutexVariant.unlock();
	emit ValueUpdated(theNewValue);
}

QWidget* GParamString::ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid /*= Default*/)
{
	GLineEditWithCompare* pLineEdit = new GLineEditWithCompare(forWhichParent);
	if (Options() & GParam::ReadOnly) {
		pLineEdit->setReadOnly(true);
	}else {
		pLineEdit->setReadOnly(false);
		connect(pLineEdit, SIGNAL(TextEditedAndReturn(QString)), this, SLOT(SetParamValue(QString)));
	}

	if (Options() & GParam::HiddenText) {
		pLineEdit->setEchoMode(QLineEdit::Password);
	}

	if(optionWid == GParam::Minimal) {
		pLineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
		pLineEdit->setMaximumWidth(50);
//		pLineEdit->resize(30, pLineEdit->size().height());
	}

	connect(this, SIGNAL(ValueUpdated(QString)), pLineEdit, SLOT(SetTextIfDifferent(const QString&)));
	pLineEdit->setText(StringValue());
	return pLineEdit;
}

QLineEdit* GParamString::ProvideNewParamLineEdit( QWidget* forWhichParent)
{
	return qobject_cast<QLineEdit*>(ProvideNewParamWidget(forWhichParent));
}

QString GParamString::StringValue() const
{
	m_MutexVariant.lock();
	QString valCopy = m_val;
	m_MutexVariant.unlock();
	return valCopy;
}

void GParamString::PopulateSettings( QSettings& inQsettings )
{
	GParam::PopulateSettings(inQsettings);
	inQsettings.setValue("v", StringValue());
// 	//TOGO: false just for compatibility with previous version
// 	inQsettings.setValue(UniqueSystemID(), StringValue());
}

void GParamString::InterpretSettings( QSettings& fromQsettings )
{
	GParam::InterpretSettings(fromQsettings);
	SetParamValue(fromQsettings.value("v", "").toString());
	//TOGO: false just for compatibility with previous version
	if(!fromQsettings.contains("v"))
		SetParamValue(fromQsettings.value(UniqueSystemID(), "").toString());
	else
		return;
	if(!fromQsettings.contains(UniqueSystemID())) {
		fromQsettings.endGroup();
		SetParamValue(fromQsettings.value(UniqueSystemID(), "").toString());
		fromQsettings.beginGroup(UniqueSystemID());
	}
}

GLineEditWithCompare::GLineEditWithCompare( QWidget * parent /*= 0 */ ) 
	: QLineEdit(parent)
{
	connect(this, SIGNAL(editingFinished()), this, SLOT(EmitOnFinishedEditting()));
}

GLineEditWithCompare::GLineEditWithCompare( const QString & contents, QWidget * parent /*= 0 */ ) 
	: QLineEdit(contents, parent)
{
	connect(this, SIGNAL(editingFinished()), this, SLOT(EmitOnFinishedEditting()));
}

void GLineEditWithCompare::SetTextIfDifferent( const QString& contents )
{
	if(contents != text())
		setText(contents);
}

void GLineEditWithCompare::EmitOnFinishedEditting()
{
	emit TextEditedAndReturn(text());
}
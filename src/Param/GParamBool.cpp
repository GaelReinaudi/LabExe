#include "GParamBool.h"
#include "GParamManager.h"
#include <QPushButton>
#include <QtGui>

G_REGISTER_NEW_PARAM_CLASS(GParamBool);
 
GParamBool::GParamBool(QString theName, QObject* parent, GParam::Properties paramOptions /*= NoOption*/)
	: GParam(theName, parent, paramOptions)
	, m_SwitchTrueCount(0)
	, m_val(false)
{
	// ValueUpdated(bool) triggers the ParamValueWasUpdated(true) that will trigger (by default) the ParamUpdateCompletion(true), i.e. unless SetExternalCompletionSignal() sets an other signal.
	connect(this, SIGNAL(ValueUpdated(bool)), this, SIGNAL(ParamValueWasUpdated()));
}

GParamBool::~GParamBool()
{

}

void GParamBool::SetParamValue( bool theNewValue )
{
	bool oldVal = BoolValue();

	// if the signal comes from a widget and the value is the same
	// let's ignore the order to prevent loops
	if(qobject_cast<QWidget*>(sender()))
		if(theNewValue == oldVal)
			return;

	m_MutexVariant.lock();
	m_val = theNewValue;
	if(theNewValue)
		m_SwitchTrueCount++;
	m_MutexVariant.unlock();
	emit ValueUpdated(theNewValue);
	emit ValueUpdatedNegativeArgument(!theNewValue);
	if(oldVal != theNewValue) {
		emit ValueDidChange(theNewValue);
	}
}

GParamControlWidget* GParamBool::ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid /*= Default*/)
{
	if(optionWid == Minimal) {
		return ProvideNewParamCheckBox(forWhichParent, "");
	}
	QPushButton* pButton = new QPushButton(forWhichParent);
	pButton->setText(Name());
	if(Options() & GParam::ReadOnly) {
		pButton->setCheckable(false);
	}
	else {
		pButton->setCheckable(true);
		connect(pButton, SIGNAL(toggled(bool)), this, SLOT(SetParamValue(bool)));
	}

	connect(this, SIGNAL(ValueUpdated(bool)), pButton, SLOT(setChecked(bool)));
	pButton->setChecked(BoolValue());
	return pButton;
}

QPushButton* GParamBool::ProvideNewParamButton( QWidget* forWhichParent, QString theLabel /*= ""*/)
{
	QPushButton* pButton =  qobject_cast<QPushButton*>(ProvideNewParamWidget(forWhichParent/*, someOptions*/));
	if(pButton)
		if(theLabel != "")
			pButton->setText(theLabel);
	return pButton;
}

QCheckBox* GParamBool::ProvideNewParamCheckBox( QWidget* forWhichParent, QString theLabel)
{
	QCheckBox* pCheckBox = new QCheckBox(theLabel, forWhichParent);
	if(Options() & GParam::ReadOnly) {
		pCheckBox->setCheckable(false);
	}
	else {
		pCheckBox->setCheckable(true);
		connect(pCheckBox, SIGNAL(toggled(bool)), this, SLOT(SetParamValue(bool)));
	}

	connect(this, SIGNAL(ValueUpdated(bool)), pCheckBox, SLOT(setChecked(bool)));
	pCheckBox->setChecked(BoolValue());
	return pCheckBox;
}

QCheckBox* GParamBool::ProvideNewParamCheckBox( QWidget* forWhichParent )
{
	return ProvideNewParamCheckBox(forWhichParent, Name());
}

QWidget* GParamBool::ProvideNewParamRadioButtons( QWidget* forWhichParent, QString onText, QString offText )
{
	QWidget* pContWidget = new QWidget(forWhichParent);
	QHBoxLayout* pLay  = new QHBoxLayout(pContWidget);
	QRadioButton* pCheckBox = new QRadioButton(onText, pContWidget);
	QRadioButton* pOtherCheckBox = new QRadioButton(offText, pContWidget);
	pLay->addWidget(pCheckBox);
	pLay->addWidget(pOtherCheckBox);
	if(Options() & GParam::ReadOnly) {
		pCheckBox->setCheckable(false);
	}
	else {
		pCheckBox->setCheckable(true);
		connect(pCheckBox, SIGNAL(toggled(bool)), this, SLOT(SetParamValue(bool)));
	}
	connect(this, SIGNAL(ValueUpdated(bool)), pCheckBox, SLOT(setChecked(bool)));
	connect(this, SIGNAL(ValueUpdatedNegativeArgument(bool)), pOtherCheckBox, SLOT(setChecked(bool)));
	pCheckBox->setChecked(BoolValue());
	pOtherCheckBox->setChecked(!BoolValue());
	return pContWidget;
}

void GParamBool::PopulateSettings( QSettings& inQsettings )
{
	GParam::PopulateSettings(inQsettings);
	inQsettings.setValue("v", BoolValue());
// 	//TOGO: false just for compatibility with previous version
// 	inQsettings.setValue(UniqueSystemID(), BoolValue());
}

void GParamBool::InterpretSettings( QSettings& fromQsettings )
{
	GParam::InterpretSettings(fromQsettings);
	SetParamValue(fromQsettings.value("v", false).toBool());
	//TOGO: false just for compatibility with previous version
	if(!fromQsettings.contains("v"))
		SetParamValue(fromQsettings.value(UniqueSystemID(), false).toBool());
	else
		return;
	if(!fromQsettings.contains(UniqueSystemID())) {
		fromQsettings.endGroup();
		SetParamValue(fromQsettings.value(UniqueSystemID(), false).toBool());
		fromQsettings.beginGroup(UniqueSystemID());
	}
}
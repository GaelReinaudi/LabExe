#include "GParamInt.h"
#include "GParamManager.h"
#include "GIntSpinBox.h"
#include <QComboBox>

G_REGISTER_NEW_PARAM_CLASS(GParamInt);

GParamInt::GParamInt(QString theName, QObject* parent, GParam::Properties paramOptions /*= NoOption*/)
	: GParamNum(theName, parent, paramOptions)
// 	, m_valInt(0)
{
	SetDisplayDecimals(0);
}

GParamInt::~GParamInt()
{

}

QWidget* GParamInt::ProvideNewParamWidget( QWidget* forWhichParent, GParam::WidgetOptions optionWid /*= Default*/ )
{
    Q_UNUSED(optionWid);
    GIntSpinBox* pSpinBox = new GIntSpinBox(this, forWhichParent);
	pSpinBox->setValue(IntValue());
	// set some settings
	pSpinBox->SetStep(TypicalStep());
	pSpinBox->SetRange(Minimum(), Maximum(), true);

    if(Options() & GParam::NoButton) {
        pSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    }
    if(Options() & GParam::ReadOnly) {
        pSpinBox->setReadOnly(true);
        pSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    }
	else {
		connect(pSpinBox, SIGNAL(ValueChangedSignificantly(int)), this, SLOT(SetParamValue(int)));
	}

	connect(this, SIGNAL(HardLimitsChanged(double, double)), pSpinBox, SLOT(SetRange(double, double)));
	connect(this, SIGNAL(TypicalStepChanged(double)), pSpinBox, SLOT(SetStep(double)));

	connect(this, SIGNAL(ValueUpdateForDisplay(int)), pSpinBox, SLOT(SetValue_WithoutSignal(int)));
	connect(this, SIGNAL(RequestUpdateDisplay()), pSpinBox, SLOT(UpdateValue_WithoutSignal()));
	return pSpinBox;
}

GIntSpinBox* GParamInt::ProvideNewParamSpinBox( QWidget* forWhichParent )
{
	GIntSpinBox* pSpinBox = qobject_cast<GIntSpinBox*>(ProvideNewParamWidget(forWhichParent));
	return pSpinBox;
}

QComboBox* GParamInt::ProvideNewParamComboBox( QWidget* forWhichParent, QStringList nameList )
{
	QComboBox* pComboBox = new QComboBox(forWhichParent);
	pComboBox->addItems(nameList);
	pComboBox->setCurrentIndex(IntValue());

	connect(this, SIGNAL(ValueUpdateForDisplay(int)), pComboBox, SLOT(setCurrentIndex(int)));
	connect(pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SetParamValue(int)));

	return pComboBox;
}

void GParamInt::PopulateSettings( QSettings& inQsettings )
{
	GParam::PopulateSettings(inQsettings);
	inQsettings.setValue("v", IntValue());
// 	//TOGO: false just for compatibility with previous version
// 	inQsettings.setValue(UniqueSystemID(), IntValue());
}

void GParamInt::InterpretSettings( QSettings& fromQsettings )
{
	GParam::InterpretSettings(fromQsettings);
	SetParamValue(fromQsettings.value("v", 0.0).toInt());
	QString sfs = fromQsettings.group();
	//TOGO: false just for compatibility with previous version
	if(!fromQsettings.contains("v"))
		SetParamValue(fromQsettings.value(UniqueSystemID(), 0.0).toInt());
	else
		return;
	if(!fromQsettings.contains(UniqueSystemID())) {
		fromQsettings.endGroup();
		SetParamValue(fromQsettings.value(UniqueSystemID(), 0.0).toInt());
		fromQsettings.beginGroup(UniqueSystemID());
	}
}

void GParamInt::SetParamValue( const int& theNewValue, bool sendUpdateSignals /*= true*/, bool sendDisplayUpdateSignal /*= true*/ )
{
	// the hard limit is respected
	int acceptedValue = acceptedValue = qBound(int(m_ParamSettings.minimum()), theNewValue, int(m_ParamSettings.maximum()));

	bool didChange = (acceptedValue != m_valInt);
	m_valInt = acceptedValue;

	// first display to avoid sending the wrong order of signal when setParamValue re-enter itself (e.g. when a device sends a new value to display)
	if(sendDisplayUpdateSignal && didChange && canUpdateGui()) {
		emit ValueUpdateForDisplay(acceptedValue);
	}
	if(sendUpdateSignals) {
		emit ValueUpdated(theNewValue);
		emit ValueUpdated(double(theNewValue));
		// if the value really did change
		if(didChange) {
			emit ValueDidChange(acceptedValue);
			emit ValueDidChange(double(theNewValue));
		}
	}
}

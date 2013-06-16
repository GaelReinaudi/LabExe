#include "GParamNum.h"
#include "GDoubleSpinBox.h"
#include <QTimer>

int zzzzzzzzfdsasg = qRegisterMetaType<GVectorDouble>("GVectorDouble");

bool gEqual( double val1, double val2 )
{
	return qFuzzyCompare(val1, val2);
}

GParamNum::GParamNum(QString theName, QObject *parent, GParam::Properties paramOptions /*= NoOption*/)
	: GParam(theName, parent, paramOptions)
{
	m_ParamSettings.setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
	m_ParamSettings.setMinimum(G_DEFAULT_RANGE_MIN);
	m_ParamSettings.setMaximum(G_DEFAULT_RANGE_MAX);

	// ValueUpdated(double) triggers the ParamValueWasUpdated(true) that will trigger (by default) the ParamUpdateCompletion(true), i.e. unless SetExternalCompletionSignal() sets an other signal.
	connect(this, SIGNAL(ValueUpdated(double)), this, SIGNAL(ParamValueWasUpdated()));

	// delay loads the param settings if any in the registry
	QTimer::singleShot(0, this, SLOT(RestoreParamSettingsFromRegistry()));
}

GParamNum::~GParamNum()
{

}

void GParamNum::SetHardLimits( double minVal, double maxVal )
{
	m_ParamSettings.setRange(minVal, maxVal);
	emit HardLimitsChanged(Minimum(), Maximum());
}

void GParamNum::SetTypicalStep( double TypStep )
{
	m_ParamSettings.setSingleStep(TypStep); 
	emit TypicalStepChanged(TypStep);
}

void GParamNum::SetDisplayDecimals( int NumberOfDecimals )
{
	m_ParamSettings.setDecimals(NumberOfDecimals); 
	emit DisplayDecimalsChanged(NumberOfDecimals);
}

/////////////////////////////////////////////////////////////////////
/*!
The settings of this GParamNum is going to be saved in a place not specific to 
any instance of the program, namely the registry. 
This is because those settings might represent a security or a convenience that should 
not have to be set manually if a new instance of a program is started with different settings.
*////////////////////////////////////////////////////////////////////
void GParamNum::SaveParamSettingsToRegistry()
{
	QSettings saveInRegistry("LabExe", "Param Settings");
	saveInRegistry.beginGroup(UniqueSystemID());
	saveInRegistry.setValue("DispName", Name());
	saveInRegistry.setValue("Min", Minimum());
	saveInRegistry.setValue("Max", Maximum());
	saveInRegistry.setValue("Step", TypicalStep());
	saveInRegistry.setValue("Deci", DisplayDecimals());
	saveInRegistry.endGroup();
}

void GParamNum::RestoreParamSettingsFromRegistry()
{
	QSettings saveInRegistry("LabExe", "Param Settings");
	saveInRegistry.beginGroup(UniqueSystemID());
	if(saveInRegistry.contains("DispName"))
		SetName(saveInRegistry.value("DispName").toString());
	if(saveInRegistry.contains("Min"))
		SetHardLimits(saveInRegistry.value("Min").toDouble(), Maximum());
	if(saveInRegistry.contains("Max"))
		SetHardLimits(Minimum(), saveInRegistry.value("Max").toDouble());
	if(saveInRegistry.contains("Step"))
		SetTypicalStep(saveInRegistry.value("Step").toDouble());
	if(saveInRegistry.contains("Deci"))
		SetDisplayDecimals(saveInRegistry.value("Deci").toInt());
	saveInRegistry.endGroup();
}

// void GParamNum::SetParamValue( const int& theNewValue, bool sendUpdateSignals /*= true*/, bool sendDisplayUpdateSignal /*= true*/ )
// {
// 	int targetVal = theNewValue;
// 	int oldVal = IntValue();
// 	int acceptedValue = targetVal;
// 
// 	// the hard limit is respected
// 	if(true) {
// 		acceptedValue = qBound(int(m_ParamSettings.minimum()), acceptedValue, int(m_ParamSettings.maximum()));
// 	}
// 
// 	m_valDouble = acceptedValue;
// 
// 	// first display to avoid sending the wrong order of signal when setParamValue re-enter itself (e.g. when a device sends a new value to display)
// 	if(sendDisplayUpdateSignal) {
// 		emit ValueUpdateForDisplay(acceptedValue);
// 	}
// 	if(sendUpdateSignals) {
// 		emit ValueUpdated(acceptedValue);
// 		// if the value really did change
// 		if(acceptedValue != oldVal)
// 			emit ValueDidChange(acceptedValue);
// 	}
// }

void GParamNum::PopulateSettings( QSettings& inQsettings )
{
	GParam::PopulateSettings(inQsettings);
	inQsettings.setValue("v", DoubleValue());
// 	//TOGO: false just for compatibility with previous version
// 	inQsettings.setValue(UniqueSystemID(), DoubleValue());
}

void GParamNum::InterpretSettings( QSettings& fromQsettings )
{
	GParam::InterpretSettings(fromQsettings);
	SetParamValue(fromQsettings.value("v", 0.0).toDouble());
	QString sfs = fromQsettings.group();
	//TOGO: false just for compatibility with previous version
	if(!fromQsettings.contains("v"))
		SetParamValue(fromQsettings.value(UniqueSystemID(), 0.0).toDouble());
	else
		return;
	if(!fromQsettings.contains(UniqueSystemID())) {
		fromQsettings.endGroup();
		SetParamValue(fromQsettings.value(UniqueSystemID(), 0.0).toDouble());
		fromQsettings.beginGroup(UniqueSystemID());
	}
}


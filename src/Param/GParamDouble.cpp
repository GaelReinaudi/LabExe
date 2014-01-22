#include "GParamDouble.h"
#include "GParamManager.h"
#include "GParamDoubleSettingWidget.h"
#include "GDoubleSpinBox.h"

G_REGISTER_NEW_PARAM_CLASS(GParamDouble);
 
GParamDouble::GParamDouble(QString theName, QObject* parent, GParam::Properties paramOptions /*= NoOption*/)
	: GParamNum(theName, parent, paramOptions)
	, m_valDouble(0.0)
{
}

GParamDouble::~GParamDouble()
{

}

QWidget* GParamDouble::ProvideNewParamWidget(QWidget* forWhichParent, GParam::WidgetOptions optionWid /*= Default*/)
{
    Q_UNUSED(optionWid);
    GDoubleSpinBox* pSpinBox = new GDoubleSpinBox(this, forWhichParent);
	pSpinBox->setValue(DoubleValue());
	// set some settings
	pSpinBox->SetDecimal(DisplayDecimals());
	pSpinBox->SetStep(TypicalStep());
	pSpinBox->SetRange(Minimum(), Maximum(), true);

	if(Options() & GParam::ReadOnly) {
		pSpinBox->setReadOnly(true);
		pSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	}
	else {
		connect(pSpinBox, SIGNAL(ValueChangedSignificantly(double)), this, SLOT(SetParamValue(double)));
	}

	connect(this, SIGNAL(HardLimitsChanged(double, double)), pSpinBox, SLOT(SetRange(double, double)));
	connect(this, SIGNAL(DisplayDecimalsChanged(int)), pSpinBox, SLOT(SetDecimal(int)));
	connect(this, SIGNAL(TypicalStepChanged(double)), pSpinBox, SLOT(SetStep(double)));

	connect(this, SIGNAL(ValueUpdateForDisplay(double)), pSpinBox, SLOT(SetValue_WithoutSignal(double)));
	connect(this, SIGNAL(RequestUpdateDisplay()), pSpinBox, SLOT(UpdateValue_WithoutSignal()));
	return pSpinBox;
}

GDoubleSpinBox* GParamDouble::ProvideNewParamSpinBox( QWidget* forWhichParent)
{
	return qobject_cast<GDoubleSpinBox*>(ProvideNewParamWidget(forWhichParent/*, someOptions*/));
}

GParamSettingsWidget* GParamDouble::PopupSettingWidget()
{
	GParamSettingsWidget* pWid = GParamNum::PopupSettingWidget();
	GParamDoubleSettingWidget* pWidDouble = new GParamDoubleSettingWidget(this, 0);
	pWid->layout()->addWidget(pWidDouble);
	return pWid;
}

void GParamDouble::SetParamValue( const double& theNewValue, bool sendUpdateSignals /*= true*/, bool sendDisplayUpdateSignal /*= true*/ )
{
	// the hard limit is respected
	double acceptedValue = qBound(m_ParamSettings.minimum(), theNewValue, m_ParamSettings.maximum());

	bool didChange = !gEqual(acceptedValue, m_valDouble);
	m_valDouble = acceptedValue;

	// first display to avoid sending the wrong order of signal when setParamValue re-enter itself (e.g. when a device sends a new value to display)
	if(sendDisplayUpdateSignal && canUpdateGui()) {
		emit ValueUpdateForDisplay(acceptedValue);
	}
	if(sendUpdateSignals) {
		emit ValueUpdated(acceptedValue);
		// if the value really did change
		if(didChange) {
			emit ValueDidChange(acceptedValue);
		}
	}
}

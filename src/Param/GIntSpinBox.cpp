#include "GIntSpinBox.h"
#include "GParamInt.h"

GIntSpinBox::GIntSpinBox(QWidget *parent)
	: QSpinBox(parent)
{
	m_pParam = 0;
	Init();
}

void GIntSpinBox::Init()
{
	m_ShouldEmit_ValueChangedSignificantly = true;
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(ReProcessValueChangedSiganl(int)));
	setRange(G_DEFAULT_RANGE_MIN, G_DEFAULT_RANGE_MAX);
	setKeyboardTracking(false);
	setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);

	UpdateUnits();
	connect(m_pParam, SIGNAL(UnitsChanged(QString)), this, SLOT(UpdateUnits()));
	connect(m_pParam, SIGNAL(UnitDisplayChanged(bool)), this, SLOT(UpdateUnits()));
}

GIntSpinBox::GIntSpinBox( GParamInt* theControlledParam, QWidget *parent )
{
    Q_UNUSED(parent);
    m_pParam = theControlledParam;
	Init();
}

GIntSpinBox::~GIntSpinBox()
{

}

void GIntSpinBox::SetValue_WithoutSignal(const int& valueToDisplay)
{
	if(valueToDisplay == value())
		return;
	m_ShouldEmit_ValueChangedSignificantly = false;
	setValue(valueToDisplay);
	m_ShouldEmit_ValueChangedSignificantly = true;
}

void GIntSpinBox::UpdateValue_WithoutSignal()
{
	SetValue_WithoutSignal(m_pParam->IntValue());
}

void GIntSpinBox::ReProcessValueChangedSiganl( const int& newValue )
{
	if(m_ShouldEmit_ValueChangedSignificantly && newValue != m_pParam->IntValue())
		emit ValueChangedSignificantly(newValue);
}

void GIntSpinBox::SetRange( double newMin, double newMax, bool DontEmitChangeSignal /*= false*/ )
{
	if(DontEmitChangeSignal)
		m_ShouldEmit_ValueChangedSignificantly = false;
	setRange(newMin, newMax);
	m_ShouldEmit_ValueChangedSignificantly = true;
}

void GIntSpinBox::SetStep( double theStep )
{
	setSingleStep(theStep);
}

void GIntSpinBox::UpdateUnits()
{
	if(!m_pParam)
		return;
	QString thesuffix(" ");
	thesuffix += m_pParam->Units();

	if(m_pParam->ShowingUnits()) {
		setSuffix(thesuffix);
	}
	else {
		setSuffix("");
	}
}

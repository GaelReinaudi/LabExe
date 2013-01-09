#include "GDoubleSpinBox.h"
#include "GParamNum.h"
#include <QApplication>

GDoubleSpinBox::GDoubleSpinBox(QWidget *parent)
	: QDoubleSpinBox(parent)
{
	m_pParam = 0;
	Init();
}

GDoubleSpinBox::GDoubleSpinBox( GParamNum* theControlledParam, QWidget *parent )
{
	m_pParam = theControlledParam;
	Init();
}

void GDoubleSpinBox::Init()
{
	m_ShouldEmit_ValueChangedSignificantly = true;
	connect(this, SIGNAL(valueChanged(double)), this, SLOT(ReProcessValueChangedSiganl(double)));
	setRange(G_DEFAULT_RANGE_MIN, G_DEFAULT_RANGE_MAX);
	setDecimals(1);
	setKeyboardTracking(false);
	setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);

	UpdateUnits();
	connect(m_pParam, SIGNAL(UnitsChanged(QString)), this, SLOT(UpdateUnits()));
	connect(m_pParam, SIGNAL(UnitDisplayChanged(bool)), this, SLOT(UpdateUnits()));
}

GDoubleSpinBox::~GDoubleSpinBox()
{

}

void GDoubleSpinBox::mousePressEvent( QMouseEvent * event )
{
	QDoubleSpinBox::mousePressEvent(event);
	// from qt "drag & drop" page
	if(event->button() == Qt::LeftButton)
		m_DragStartPosition = event->pos();
}

void GDoubleSpinBox::mouseReleaseEvent( QMouseEvent * event )
{
	QDoubleSpinBox::mouseReleaseEvent(event);
}

void GDoubleSpinBox::mouseMoveEvent( QMouseEvent * event )
{
	QDoubleSpinBox::mouseMoveEvent(event);
	// from qt "drag & drop" page
	if(!(event->buttons() & Qt::LeftButton))
		return;
	if((event->pos() - m_DragStartPosition).manhattanLength() < QApplication::startDragDistance())
		return;
}

void GDoubleSpinBox::leaveEvent( QEvent * event )
{
	QDoubleSpinBox::leaveEvent(event);
}

void GDoubleSpinBox::SetValue_WithoutSignal(const double& valueToDisplay)
{
	if(valueToDisplay == value())
		return;
	m_ShouldEmit_ValueChangedSignificantly = false;
	setValue(valueToDisplay);
	m_ShouldEmit_ValueChangedSignificantly = true;
}

void GDoubleSpinBox::SetValue_WithoutSignal(const int& valueToDisplay)
{
	if(valueToDisplay == value())
		return;
	m_ShouldEmit_ValueChangedSignificantly = false;
	setValue(double(valueToDisplay));
	m_ShouldEmit_ValueChangedSignificantly = true;
}

void GDoubleSpinBox::ReProcessValueChangedSiganl( double newValue )
{
	if(m_ShouldEmit_ValueChangedSignificantly)
		emit ValueChangedSignificantly(newValue);
}

void GDoubleSpinBox::SetRange( double newMin, double newMax, bool DontEmitChangeSignal /*= false*/ )
{
	if(DontEmitChangeSignal)
		m_ShouldEmit_ValueChangedSignificantly = false;
	setRange(newMin, newMax);
	m_ShouldEmit_ValueChangedSignificantly = true;
}

void GDoubleSpinBox::SetDecimal( int nDec )
{
	setDecimals(nDec);
}

void GDoubleSpinBox::SetStep( double theStep )
{
	setSingleStep(theStep);
}

void GDoubleSpinBox::UpdateUnits()
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

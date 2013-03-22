#include "GParamInt.h"
#include "GParamManager.h"
#include "GDoubleSpinBox.h"

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

GDoubleSpinBox* GParamInt::ProvideNewParamSpinBox( QWidget* forWhichParent )
{
	GDoubleSpinBox* pSpinBox = qobject_cast<GDoubleSpinBox*>(ProvideNewParamWidget(forWhichParent));
	connect(this, SIGNAL(ValueUpdateForDisplay(int)), pSpinBox, SLOT(SetValue_WithoutSignal(int)));
	return pSpinBox;
}

QComboBox* GParamInt::ProvideNewParamComboBox( QWidget* forWhichParent, QStringList nameList )
{
	QComboBox* pComboBox = new QComboBox(forWhichParent);
	pComboBox->addItems(nameList);
	pComboBox->setCurrentIndex(IntValue());
	qDebug() << IntValue();

	connect(this, SIGNAL(ValueUpdateForDisplay(int)), pComboBox, SLOT(setCurrentIndex(int)));//, Qt::QueuedConnection);
	connect(pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SetParamValue(int)));

	return pComboBox;
}

void GParamInt::SetParamValue( const int& theNewValue, bool sendUpdateSignals /*= true*/, bool sendDisplayUpdateSignal /*= true*/ )
{
	int oldVal = IntValue();
	int acceptedValue = theNewValue;

	GParamNum::SetParamValue(theNewValue, sendUpdateSignals, sendDisplayUpdateSignal);

	// first display to avoid sending the wrong order of signal when setParamValue re-enter itself (e.g. when a device sends a new value to display)
	if(sendDisplayUpdateSignal) {
		emit ValueUpdateForDisplay(acceptedValue);
	}
	if(sendUpdateSignals) {
		emit ValueUpdated(theNewValue);
		// if the value really did change
		if(acceptedValue != oldVal)
			emit ValueDidChange(acceptedValue);
	}
}

void GParamInt::SetParamValue(const double& theNewValue, bool sendUpdateSignals /*= true*/, bool sendDisplayUpdateSignal /*= true*/)
{
	SetParamValue(qRound(theNewValue), sendUpdateSignals, sendDisplayUpdateSignal);
//	qDebug() << theNewValue << int(theNewValue) << qRound(theNewValue);
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

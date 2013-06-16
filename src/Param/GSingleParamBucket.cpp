#include "GSingleParamBucket.h"
#include "GSingleParamBucketWidget.h"
#include "GParamBucketTreeWidget.h"
#include "GParamManager.h"
#include "GParamDouble.h"
#include "GParamInt.h"

G_REGISTER_NEW_PARAM_CLASS(GSingleParamBucket);

GSingleParamBucket::GSingleParamBucket( QString theName, QObject *parent, GParam::Properties paramOptions /*= NoOption*/ )
	: GParamBucket(theName, parent, paramOptions)
	, m_pDefaultParam(0)
{
	m_pDefaultParam = new GParamDouble(theName, this);
	AddParam(m_pDefaultParam);
}

GSingleParamBucket::~GSingleParamBucket()
{

}

bool GSingleParamBucket::AddParam( GParam* pParam )
{
	GParam* pOldPar = Param();

	if(!GParamBucket::AddParam(pParam))
		return false;

	// now we remove the other (old) param
	if(Contains(pOldPar))
		RemoveParam(pOldPar);

	// if numerical, we make some connections
	GParamDouble* pParDbl = qobject_cast<GParamDouble*>(pParam);
	if(pParDbl) {
		connect(pParDbl, SIGNAL(ValueDidChange(double)), this, SIGNAL(ValueDidChange(double)));
		connect(pParDbl, SIGNAL(ValueUpdated(double)), this, SIGNAL(ValueUpdated(double)));
		connect(pParDbl, SIGNAL(ParamUpdateCompletion(bool)), this, SIGNAL(BucketUpdatedValues()));
		connect(pParDbl, SIGNAL(ManyValuesAvailable(GVectorDouble)), this, SIGNAL(ManyValuesAvailable(GVectorDouble)));
		// we even emit because a new value is there
		emit ValueUpdated(pParDbl->DoubleValue());
	}
	// if int, we make some connections
	GParamInt* pParInt = qobject_cast<GParamInt*>(pParam);
	if(pParInt) {
		connect(pParInt, SIGNAL(ValueDidChange(double)), this, SIGNAL(ValueDidChange(double)));
		connect(pParInt, SIGNAL(ValueUpdated(double)), this, SIGNAL(ValueUpdated(double)));
		connect(pParInt, SIGNAL(ValueDidChange(int)), this, SIGNAL(ValueDidChange(int)));
		connect(pParInt, SIGNAL(ValueUpdated(int)), this, SIGNAL(ValueUpdated(int)));
		connect(pParInt, SIGNAL(ParamUpdateCompletion(bool)), this, SIGNAL(BucketUpdatedValues()));
		// we even emit because a new value is there
		emit ValueUpdated(pParInt->IntValue());
	}
	return true;
}

bool GSingleParamBucket::RemoveParam( GParam* pParam )
{
	if(!GParamBucket::RemoveParam(pParam))
		return false;

	// if numerical, we make some dis-connections
	if(qobject_cast<GParamNum*>(pParam)) {
		disconnect(pParam, 0, this, 0);
	}
	return true;
}

GParamBucketWidget* GSingleParamBucket::ProvideParamBucketWidget( QWidget* parentWidget )
{
	GParamBucketWidget* pWid = new GSingleParamBucketWidget(parentWidget, this);
	return pWid;
}

GParamBucketTreeWidget* GSingleParamBucket::ProvideParamBucketTreeWidget( QWidget* parentWidget )
{
	GParamBucketTreeWidget* pWidBucket = new GParamBucketTreeWidget(parentWidget, this);
//	pWidBucket->setHeaderHidden(true);
	pWidBucket->ShowInfoItem(false);
	pWidBucket->setMaximumHeight(46);
	pWidBucket->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	return pWidBucket;
}

void GSingleParamBucket::SetParamValue( const double& theNewValue, bool sendUpdateSignals /*= true*/, bool sendDisplayUpdateSignal /*= true*/ )
{
	if(ParamNum())
		ParamNum()->SetParamValue(theNewValue, sendUpdateSignals, sendDisplayUpdateSignal);
}

double GSingleParamBucket::DoubleValue() const
{
	return ParamNum() ? ParamNum()->DoubleValue() : 0.0;
}

GParamNum* GSingleParamBucket::ParamNum() const
{
	return qobject_cast<GParamNum*>(Param());
}
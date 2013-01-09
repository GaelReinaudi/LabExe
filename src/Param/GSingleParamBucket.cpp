#include "GSingleParamBucket.h"
#include "GSingleParamBucketWidget.h"
#include "GParamBucketTreeWidget.h"
#include "GParamManager.h"
#include "GParamDouble.h"

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
	GParamNum* pParNum = qobject_cast<GParamNum*>(pParam);
	if(pParNum) {
		connect(pParNum, SIGNAL(ValueDidChange(double)), this, SIGNAL(ValueDidChange(double)));
		connect(pParNum, SIGNAL(ValueUpdated(double)), this, SIGNAL(ValueUpdated(double)));
		connect(pParNum, SIGNAL(ParamUpdateCompletion(bool)), this, SIGNAL(BucketUpdatedValues()));
		connect(pParNum, SIGNAL(ManyValuesAvailable(GVectorDouble)), this, SIGNAL(ManyValuesAvailable(GVectorDouble)));
		// we even emit because a new value is there
		emit ValueUpdated(pParNum->DoubleValue());
	}
	return true;
}

bool GSingleParamBucket::RemoveParam( GParam* pParam )
{
	if(!GParamBucket::RemoveParam(pParam))
		return false;

	// if numerical, we make some dis-connections
	if(qobject_cast<GParamNum*>(pParam)) {
		disconnect(pParam, SIGNAL(ValueDidChange(double)), this, SIGNAL(ValueDidChange(double)));
		disconnect(pParam, SIGNAL(ValueUpdated(double)), this, SIGNAL(ValueUpdated(double)));
		disconnect(pParam, SIGNAL(ParamUpdateCompletion(bool)), this, SIGNAL(BucketUpdatedValues()));
		disconnect(pParam, SIGNAL(ManyValuesAvailable(GVectorDouble)), this, SIGNAL(ManyValuesAvailable(GVectorDouble)));
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
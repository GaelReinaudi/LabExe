#ifndef GSINGLEPARAMBUCKET_H
#define GSINGLEPARAMBUCKET_H

#include "GParamBucket.h"
class GParamNum;

/////////////////////////////////////////////////////////////////////
//! \brief The GSingleParamBucket class defines a GParamBucket that can hold only one GParamNum object.
/*!
It will handle the parameter, forward its signals and slots... For instance:
- when the held param sends the signal ValueDidChange(), the bucket also emits it,
- when the held param sends the signal ValueUpdated(), the bucket also emits it,
- when the held param sends the signal BucketUpdatedValues(), the bucket also emits it,
- when calling the SetParamValue(), it calls the held param SetParamValue().

It only supports numerical params for now.
*/
class LABEXE_EXPORT GSingleParamBucket : public GParamBucket
{
	Q_OBJECT

public:
	//! constructor with a title that will be displayed as the title one the widget
	GSingleParamBucket(QString theName, QObject *parent, GParam::Properties paramOptions = NoOption);
	~GSingleParamBucket();

	//! Re-implemented to provide a widget that will hold a param widget and have some useful signal connections.
	virtual GParamBucketWidget* ProvideParamBucketWidget(QWidget* parentWidget);
	//! Re-implemented to minimize the size
	virtual GParamBucketTreeWidget* ProvideParamBucketTreeWidget(QWidget* parentWidget);

	//! returns the pointer to the held param. 0 if no param.
	GParam* Param() const { return Params().value(0); }
	//! returns the pointer to the held numerical param. 0 if no param or if the param is not a GParamNum.
	GParamNum* ParamNum() const;
	//! returns the param value as a double, or 0.0 if no param is in the bucket.
	double DoubleValue() const;
	//! Returns the default param to display in case we didn't drag anything in the bucket. 0 if none.
	GParamNum* DefaultParam() const { return m_pDefaultParam; }


public:
	//! Re-implemented so that the param dropped may be held, or replace the currently held param.
	virtual bool AddParam(GParam* pParam);
	//! Re-implemented.
	virtual bool RemoveParam(GParam* pParam);

public slots:
	//! Forwarding the param slot GParamNum::SetParamValue()
	virtual void SetParamValue(const double& theNewValue, bool sendUpdateSignals = true, bool sendDisplayUpdateSignal = true);

signals:
	//! Forwarding the param signal GParamNum::ValueDidChange()
	void ValueDidChange(const double& theNewValue);
	//! Forwarding the param signal GParamNum::ValueUpdated()
	void ValueUpdated(const double& theNewValue);
	//! Forwarding the param signal GParamNum::ValueDidChange()
	void ValueDidChange(const int& theNewValue);
	//! Forwarding the param signal GParamNum::ValueUpdated()
	void ValueUpdated(const int& theNewValue);
	//! TEMPORARY workaround. Forwarding the param signal GParamNum::ManyValuesAvailable()
	void ManyValuesAvailable(const GVectorDouble & theValues);

private:
	//! a default param to display before we drag one in this bucket
	GParamNum* m_pDefaultParam;
};

#endif // GSINGLEPARAMBUCKET_H

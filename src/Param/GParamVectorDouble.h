#ifndef GPARAMVECTORDOUBLE_H
#define GPARAMVECTORDOUBLE_H

#include "GParam.h"

class GParamVectorDouble : public GParam
{
	Q_OBJECT

public:
	GParamVectorDouble(QString theName, QObject* parent, GParam::Properties paramOptions = NoOption);
	~GParamVectorDouble();

	void SetParamValue(const int& theNewValue);
	void AppendValue(double newVal) { m_val.append(newVal); }

private:
	//! the actual value
	QVector<double> m_val;

};

#endif // GPARAMVECTORDOUBLE_H

#ifndef GPARAMVECTORDOUBLE_H
#define GPARAMVECTORDOUBLE_H

#include "GParam.h"

class GParamVectorDouble : public GParam
{
	Q_OBJECT

public:
	GParamVectorDouble(QString theName, QObject* parent, GParam::Properties paramOptions = NoOption);
	~GParamVectorDouble();

private:
	//! the actual value
	QVector<double> m_val;

};

#endif // GPARAMVECTORDOUBLE_H

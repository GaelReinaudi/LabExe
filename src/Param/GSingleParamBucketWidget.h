#ifndef GSINGLEPARAMBUCKETWIDGET_H
#define GSINGLEPARAMBUCKETWIDGET_H

#include "GParamBucketWidget.h"
#include <QtWidgets>

class GSingleParamBucket;

/////////////////////////////////////////////////////////////////////
//! \brief The GSingleParamBucketWidget class defines a GParamBucketWidget that can hold only one param widget
/*!
*/
class GSingleParamBucketWidget : public GParamBucketWidget
{
	Q_OBJECT

public:
	GSingleParamBucketWidget(QWidget *parent, GSingleParamBucket* theBucket);
	~GSingleParamBucketWidget();

protected:
	//! remove and delete the widgets held in this bucket widget.
	void Clear();
	//! Re-implemented so that the existing param widget get deleted.
	QGroupBox* AddParamWidget(GParam* pParam);

private:
	//! the layout that contains the param label and the param widget
	QBoxLayout* m_pLabelParamLayout;

};

#endif // GSINGLEPARAMBUCKETWIDGET_H

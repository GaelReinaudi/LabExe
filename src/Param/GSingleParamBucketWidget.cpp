#include "GSingleParamBucketWidget.h"
#include "GSingleParamBucket.h"

GSingleParamBucketWidget::GSingleParamBucketWidget(QWidget *parent, GSingleParamBucket* theBucket)
	: GParamBucketWidget(parent, theBucket)
{
}

GSingleParamBucketWidget::~GSingleParamBucketWidget()
{

}

void GSingleParamBucketWidget::Clear()
{
	QLayoutItem *child;
	while((child = layout()->takeAt(0)) != 0) {
		QWidget* pWidItem = child->widget();
		if(pWidItem)
			pWidItem->deleteLater();
		else
			delete child;
	}
}

QGroupBox* GSingleParamBucketWidget::AddParamWidget( GParam* pParam )
{
	Clear();
	return GParamBucketWidget::AddParamWidget(pParam);
}

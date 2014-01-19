#ifndef GPARAMBUCKETWIDGET_H
#define GPARAMBUCKETWIDGET_H

#include <QGroupBox>
#include <QHash>
#include <QPropertyAnimation>
#include <QVBoxLayout>

class GParam;
class GParamBucket;

/////////////////////////////////////////////////////////////////////
//! \brief The GParamBucketWidget class defines the widget that handle dropping of a GParam
/*!
When drag-and*dropping a GParamLabel object, this widget will emit the signal ParamLabelWasDropped().
This widget may also be associated with a GParamBucket. GParamBucket will ProvideParamBucketWidget(). 
In such a case, it will also emit the GParamBucket signal ParamLabelWasDropped().
*/
class GParamBucketWidget : public QGroupBox
{
	Q_OBJECT

	// test for the opacity animation
	Q_PROPERTY(int backgroundOpacity READ BackgroundOpacity WRITE SetBackgroundOpacity)
	int m_BackgroundOpacity;
	int BackgroundOpacity() const { return m_BackgroundOpacity; }
	void SetBackgroundOpacity(int val);
	//! Increment the background color opacity and starts a new fading out animation of the background. The incrementing is used so that a user can see how often the bucket is actually updated.
	void IncrementHighlightDecay();
	//! The animation currently running the fading out of the background
	QPropertyAnimation* m_pBackgroungAnimation;

public:
	GParamBucketWidget(QWidget *parent, GParamBucket* theBucket, Qt::Orientation orientation = Qt::Vertical);
	~GParamBucketWidget();

	//! Turns the highlight decay animation On/Off. This is On by default.
	void UseHighLightDecay(bool doTurnOn) {m_UseHighlightDecay = doTurnOn;}

protected:
	//! Returns the pointer to the actual bucket
	GParamBucket* Bucket() const { return m_Bucket; }
	//! reimplemented in order to accept dropping in this widget
	void dragEnterEvent(QDragEnterEvent *event); 
	//! reimplemented in order to accept dropping in this widget
	void dropEvent(QDropEvent *event);

protected slots:
	//! Enclose a new param label and param widget in a QGroupBox, and adds it in this bucket widget's layout(). It returns the QGroupBox so that derived class can add things in its internal layout().
	virtual QGroupBox* AddParamWidget(GParam* pParam);
	//! Deletes the param label and param widget from this bucket widget. the indexParamFromBucket is sent from the bucket signal, for information (it is used by other classes).
	virtual void RemoveParamWidget(int indexParamFromBucket, GParam* pParam);
	//! Resizes this widget, and its parent() if any. Notably called through a Qt::QueuedConnection after dropping a param.
	void ResizeThisAndParent();
	//! called when the bucket emitted the BucketUpdatedValues(). The argument should be the number of parameters that changed. 
	void EventBucketParamsUpdated(int nParamUpdated);

signals:
	//! Emitted when a Param has been dropped in this widget
	void ParamLabelWasDropped(GParam* pParam, QPoint thePos = QPoint(0,0));
	//! Emitted when a Param has been asked to leave this widget
	void ParamLabelRemoveActioned(GParam* pParam);

private slots:
	//! retrieves the GParam corresponding to the QAction pRemoveAction that is defined in AddParamWidget(). It will emit the signal ParamLabelRemoveActioned().
	void RemoveParamAction();

private:
	GParamBucket* m_Bucket;
	//! The map to know which widget to delete when the param is removed
	QHash<GParam*, QWidget*> m_MapParamWidget;
	//! do we use the highlight decay
	bool m_UseHighlightDecay;
};

#endif // GPARAMBUCKETWIDGET_H

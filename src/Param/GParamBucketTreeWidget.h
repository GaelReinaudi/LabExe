#ifndef GPARAMBUCKETTREEWIDGET_H
#define GPARAMBUCKETTREEWIDGET_H

#include <QHash>
#include <QMap>
#include <QCheckBox>
#include <QTreeWidget>
#include <QTableWidget>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
// #include <QAbstractItemModel>

class GParam;
class GParamBucket;

typedef QTreeWidget GBucketWidget;
typedef QTreeWidgetItem GBucketWidgetItem;
// typedef QTableWidget GBucketWidget;
// typedef QTableWidgetItem GBucketWidgetItem;

class GParamBucketTreeWidget : public GBucketWidget
{
	Q_OBJECT

public:
	GParamBucketTreeWidget(QWidget *parent, GParamBucket* theBucket);
	~GParamBucketTreeWidget();

	enum Column{Label, Widget, Extra};
	//! intended to be compatible with the old QGroupBox widget, to provide a Title
	void setTitle(QString newTitle) {}
	void UseHighLightDecay(bool) {}
	//! Shows an extra line in the bucket that contains some info like the number of times it was triggered and by how many updating parameters at once
	void ShowInfoItem(bool doShow);

protected:
	//! Returns the pointer to the actual bucket
	GParamBucket* Bucket() const { return m_Bucket; }
	//! Reimplemented in order to accept dropping in this widget.
	void dragEnterEvent(QDragEnterEvent *event); 
	//! Reimplemented in order to accept dropping in this widget.
	void dropEvent(QDropEvent *event);
	//! Reimplemented in order to get information about dropping in this widget.
	bool dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action);
	//! Re-implemented
	void mouseDoubleClickEvent(QMouseEvent* event);

protected slots:
	//! Enclose a new param label and param widget in a QGroupBox, and adds it in this bucket widget's layout(). It returns the QGroupBox so that derived class can add things in its internal layout().
	virtual void AddParamWidget(GParam* pParam);
	//! Deletes the param label and param widget from this bucket widget. the indexParamFromBucket is sent from the bucket signal, for information (it is used by other classes).
	virtual void RemoveParamWidget(int indexParamFromBucket, GParam* pParam);
	//! Called when the bucket emitted the BucketUpdatedValues(). The argument should be the number of parameters that changed. 
	void EventBucketParamsUpdated(int nParamUpdated);

private slots:
	//! Retrieves the GParam corresponding to the QAction pRemoveAction that is defined in AddParamWidget(). It will emit the signal ParamLabelRemoveActioned().
	void RemoveParamAction();


signals:
	//! Emitted when a Param has been dropped in this widget
	void ParamLabelWasDropped(GParam* pParam, QPoint thePos = QPoint(0,0));
	//! Emitted when a Param has been asked to leave this widget
	void ParamLabelRemoveActioned(GParam* pParam);
	//! Emitted when the check state of the param has changed
	void ParamCheckStateChanged(GParam* pParam, bool newState);

private:
	GParamBucket* m_Bucket;
	//! The map to know which widget to delete when the param is removed
	QMap<GParam*, GBucketWidgetItem*> m_MapParamItem;
	//! An item to provide infos about the bucket
	QTreeWidgetItem* m_pInfoItem;

};

#endif // GPARAMBUCKETTREEWIDGET_H
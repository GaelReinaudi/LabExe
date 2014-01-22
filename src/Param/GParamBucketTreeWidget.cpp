#include "GParamBucketTreeWidget.h"
#include "GParamManager.h"
#include "GParamLabel.h"
#include "GParamBucket.h"
#include <QDropEvent>
#include <QHeaderView>

GParamBucketTreeWidget::GParamBucketTreeWidget(QWidget *parent, GParamBucket* theBucket)
	: GBucketWidget(parent)
	, m_Bucket(theBucket)
	, m_pInfoItem(0)
{
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);

	// this line makes the widget accept drop from the outside....
	setDragDropMode(QAbstractItemView::InternalMove);

	setDefaultDropAction(Qt::MoveAction);
	setAnimated(true);
	setAutoScroll(true);
	setWordWrap(true);
	setAlternatingRowColors(true);

	// lets add the widgets of whatever param is already in this bucket at creation time
	foreach(GParam* pPar, m_Bucket->Params()) {
		AddParamWidget(pPar);
	}

	// informs the bucket that a param has been dropped / removed
	connect(this, SIGNAL(ParamLabelWasDropped(GParam*, QPoint)), theBucket, SLOT(AddParam(GParam*)));
	connect(this, SIGNAL(ParamLabelRemoveActioned(GParam*)), theBucket, SLOT(RemoveParam(GParam*)));
	// informs this widget to add / remove the param widget
	connect(theBucket, SIGNAL(ParamAdded(GParam*)), this, SLOT(AddParamWidget(GParam*)), Qt::QueuedConnection);
	connect(theBucket, SIGNAL(ParamRemoved(int, GParam*)), this, SLOT(RemoveParamWidget(int, GParam*)), Qt::QueuedConnection);
	// informs the bucket that some params updated
	connect(theBucket, SIGNAL(BucketUpdatedValues(int)), this, SLOT(EventBucketParamsUpdated(int)));

// 	// detect changes for the checkbox...
// 	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(Event_ItemChanged(QTreeWidgetItem*, int)));

	setColumnCount(Extra + m_Bucket->ExtraFieldsName().count());
// 	setHeaderLabels(QStringList() << "param" << "values" << m_Bucket->ExtraFieldsName());
	setHeaderLabels(QStringList() << m_Bucket->Name() << "values" << m_Bucket->ExtraFieldsName());
	resizeColumnToContents(Label);
	resizeColumnToContents(Widget);
	resizeColumnToContents(Extra);
	resizeColumnToContents(Extra + 1);

    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	header()->setStretchLastSection(false);
	setItemsExpandable(false);
// 	setColumnWidth(0, 70);
// 	setHeaderHidden(true);
	setIndentation(0);

//	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);

//	int nRow = m_Bucket->ParamCount();
// 	setRowCount(nRow);

	ShowInfoItem(true);
}

GParamBucketTreeWidget::~GParamBucketTreeWidget()
{

}

void GParamBucketTreeWidget::dragEnterEvent( QDragEnterEvent *event )
{
	GBucketWidget::dragEnterEvent(event);
	const QMimeData* pData = event->mimeData();
	if(pData->hasFormat("LabExe/parameter.single")) {
		event->accept();
	}
}

void GParamBucketTreeWidget::dropEvent( QDropEvent *event )
{
	GBucketWidget::dropEvent(event);
	GParam* pParam = ParamManagerInstance()->GetParam(event->mimeData());
	if(!pParam)
		return;
	emit ParamLabelWasDropped(pParam, event->pos());
}

bool GParamBucketTreeWidget::dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action)
{
	GBucketWidget::dropMimeData(parent, index, data, action);

qDebug() << "dropMimeData";
	return true;
}

void GParamBucketTreeWidget::AddParamWidget( GParam* pParam )
{
	if(!pParam)
		return;

	// let's add the param label 
	QLabel* pLabel = pParam->ProvideNewLabel(this);
	QWidget* pParWid = pParam->ProvideNewParamWidget(this);

	QTreeWidgetItem* paramTreeItem = new QTreeWidgetItem(this);

// 	QTreeWidgetItem *childItem = new QTreeWidgetItem(paramTreeItem);
// 	childItem->setText(0, tr("Oslo"));
// 	childItem->setText(1, tr("Yes"));

	QModelIndex indexParamLabel = indexFromItem(paramTreeItem, Label);
	QModelIndex indexParamWidget = indexFromItem(paramTreeItem, Widget);
	setIndexWidget(indexParamLabel, pLabel);
	setIndexWidget(indexParamWidget, pParWid);
	// for each extra field, we add a new extra param widget
	for(int i = 0; i < m_Bucket->ExtraFieldsName().count() ; i++) {
		QString fieldName = m_Bucket->ExtraFieldsName().value(i);
		GParam* extraPar = m_Bucket->ExtraParam(pParam, fieldName);
		QWidget* pExtraParWid = extraPar->ProvideNewParamWidget(this, GParam::Minimal);
		QModelIndex indexExtra = indexFromItem(paramTreeItem, Extra + i);
		setIndexWidget(indexExtra, pExtraParWid);
	}

	// if not frozen, we add a menu to remove the param
	if(!m_Bucket->IsFreezed()) {
		// let's add an Action to the Label so that it can be removed from the bucket
		QAction* pRemoveAction = new QAction("Remove from bucket", pLabel);
		pLabel->addAction(pRemoveAction);
		connect(pRemoveAction, SIGNAL(triggered()), this, SLOT(RemoveParamAction()));
	}
	m_MapParamItem.insert(pParam, paramTreeItem);

	// re-inserts the info item so that it stays the last one
	int infoInd = indexOfTopLevelItem(m_pInfoItem);
	if(infoInd != -1) {
		addTopLevelItem(takeTopLevelItem(infoInd));
	}
}

void GParamBucketTreeWidget::RemoveParamWidget(int indexParamFromBucket, GParam* pParam)
{
    Q_UNUSED(indexParamFromBucket);
    GBucketWidgetItem* pItem = m_MapParamItem.value(pParam);
	if(!pItem)
		return;
	delete pItem;
}

void GParamBucketTreeWidget::EventBucketParamsUpdated( int nParamUpdated )
{
	if(!m_pInfoItem) {
		return;
	}
	m_pInfoItem->setText(0, "update");
	m_pInfoItem->setText(1, QString::number(m_Bucket->NumUpdates()));
	m_pInfoItem->setText(2, QString::number(nParamUpdated));
}

void GParamBucketTreeWidget::RemoveParamAction()
{
	GParamLabel* pParLab = qobject_cast<GParamLabel*>(sender()->parent());
	if(!pParLab)
		return;
	GParam* pPar = const_cast<GParam*>(pParLab->Param());
	if(!pPar)
		return;

	emit ParamLabelRemoveActioned(pPar);
}

void GParamBucketTreeWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
    Q_UNUSED(event);
    if(!m_Bucket)
		return;
	m_Bucket->PopupSettingWidget();
}

void GParamBucketTreeWidget::ShowInfoItem( bool doShow )
{
	if(m_pInfoItem) {
		delete m_pInfoItem;
		m_pInfoItem = 0;
	}
	if(doShow) {
		// try to insert the info item. it will have to be moved when adding other items
		m_pInfoItem = new QTreeWidgetItem(this);
	}
}

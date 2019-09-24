#include "GParamBucketWidget.h"
#include "GParamManager.h"
#include "GParamLabel.h"
#include "param.h"
#include <QtWidgets>



GParamBucketWidget::~GParamBucketWidget()
{
	if(m_pBackgroungAnimation) {
		m_pBackgroungAnimation->stop();
		delete m_pBackgroungAnimation;
	}
}

void GParamBucketWidget::dragEnterEvent( QDragEnterEvent *event )
{
	QGroupBox::dragEnterEvent(event);
	if(m_Bucket->IsFreezed())
		return;
	const QMimeData* pData = event->mimeData();
	if(pData->hasFormat("LabExe/parameter.single")) {
		event->accept();
	}
}

void GParamBucketWidget::dropEvent( QDropEvent *event )
{
	QGroupBox::dropEvent(event);
	if(m_Bucket->IsFreezed())
		return;
	// extrapolation from qt help: "Using Drag and Drop with Item Views"
	GParam* pParam = ParamManagerInstance()->GetParam(event->mimeData());
	if(!pParam)
		return;
	emit ParamLabelWasDropped(pParam, event->pos());
}

void GParamBucketWidget::ResizeThisAndParent()
{
	adjustSize();
	if(parentWidget())
		parentWidget()->adjustSize();
}

QGroupBox* GParamBucketWidget::AddParamWidget( GParam* pParam )
{
	if(!pParam)
		return 0;

	// a group box for the param and its label
	QGroupBox* pParamContainingGroupbox = new QGroupBox(this);
	QBoxLayout* outerBox;
	outerBox = new QVBoxLayout(pParamContainingGroupbox);
	outerBox->setContentsMargins(1, 1, 1, 1);
	outerBox->setSpacing(1);

	QBoxLayout* layoutLabelParam = new QHBoxLayout();
	layoutLabelParam->setContentsMargins(0, 0, 0, 0);
	layoutLabelParam->setSpacing(1);
	outerBox->addLayout(layoutLabelParam);

	// let's add the param label 
	QLabel* pLabel = pParam->ProvideNewLabel(pParamContainingGroupbox);
	layoutLabelParam->addWidget(pLabel);
	// and the param widget
	layoutLabelParam->addWidget(pParam->ProvideNewParamWidget(pParamContainingGroupbox));

	if(!m_Bucket->IsFreezed()) {
		// let's add an Action to the Label so that it can be removed from the bucket
		QAction* pRemoveAction = new QAction("Remove from bucket", pLabel);
		pLabel->addAction(pRemoveAction);
		connect(pRemoveAction, SIGNAL(triggered()), this, SLOT(RemoveParamAction()));
	}

	layout()->addWidget(pParamContainingGroupbox);
	m_MapParamWidget.insert(pParam, pParamContainingGroupbox);

	return pParamContainingGroupbox;
}

void GParamBucketWidget::RemoveParamWidget(int indexParamFromBucket, GParam* pParam)
{
    Q_UNUSED(indexParamFromBucket);
    // group box widget to delete
	QWidget* pBox = m_MapParamWidget.take(pParam);
	if(pBox)
		pBox->deleteLater();
}

void GParamBucketWidget::RemoveParamAction()
{
	GParamLabel* pParLab = qobject_cast<GParamLabel*>(sender()->parent());
	if(!pParLab)
		return;
	GParam* pPar = const_cast<GParam*>(pParLab->Param());
	if(!pPar)
		return;

	emit ParamLabelRemoveActioned(pPar);
}

void GParamBucketWidget::EventBucketParamsUpdated( int nParamUpdated )
{
    Q_UNUSED(nParamUpdated);
    if(m_UseHighlightDecay)
		IncrementHighlightDecay();
}

void GParamBucketWidget::SetBackgroundOpacity( int val )
{
	m_BackgroundOpacity = val;
	QPalette Pal(palette());
	QColor theColor(Qt::yellow);
	theColor.setAlpha(m_BackgroundOpacity);
    Pal.setColor(QPalette::Window, theColor);
	setAutoFillBackground(true);
	setPalette(Pal);
}

void GParamBucketWidget::IncrementHighlightDecay()
{
	// we stop the previous animation
	if(m_pBackgroungAnimation) {
		m_pBackgroungAnimation->stop();
		delete m_pBackgroungAnimation;
	}
	// get the current opacity
	int opacNow = BackgroundOpacity();
	// increment it, here a percentage of the full 255 valued range. We limit it though.
	opacNow += 51;
	opacNow = qMin(255, opacNow);

	// new animation
	m_pBackgroungAnimation = new QPropertyAnimation(this, "backgroundOpacity");
	m_pBackgroungAnimation->setDuration(1000);
	m_pBackgroungAnimation->setStartValue(opacNow);
	m_pBackgroungAnimation->setEndValue(0);

	m_pBackgroungAnimation->start();
}

GParamBucketWidget::GParamBucketWidget(QWidget *parent, GParamBucket *theBucket, Qt::Orientation orientation)
    : QGroupBox(parent)
    , m_BackgroundOpacity(0)
    , m_pBackgroungAnimation(0)
    , m_Bucket(theBucket)
    , m_UseHighlightDecay(true)
{
    setAcceptDrops(true);

    if(!theBucket) {
        qWarning() << "Bucket passed is 0 in GParamBucketWidget::GParamBucketWidget()";
        return;
    }
    setTitle(theBucket->objectName());
    // layout
    QBoxLayout* mainBoxLayout;
    if(orientation == Qt::Vertical)
        mainBoxLayout = new QVBoxLayout(this);
    else
        mainBoxLayout = new QHBoxLayout(this);
    mainBoxLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    mainBoxLayout->setSpacing(1);
    mainBoxLayout->setContentsMargins(1, 0, 1, 0);

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

    connect(this, SIGNAL(ParamLabelWasDropped(GParam*, QPoint)), this, SLOT(ResizeThisAndParent()), Qt::QueuedConnection);

    // color
    if(m_Bucket->IsEmpty()) {
        QPalette Pal(palette());
        QColor theColor(Qt::green);
        // 	theColor.setAlpha(m_BackgroundOpacity);
        Pal.setColor(QPalette::Window, theColor);
        setAutoFillBackground(true);
        setPalette(Pal);
    }
}

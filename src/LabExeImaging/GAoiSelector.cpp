#include "GAoiSelector.h"
#include "GImageProcessor.h"
#include <QPushButton>
#include <QGraphicsItem>

// GAoiSelector::GAoiSelector(GImageGraphicsScene *parentScene)
// : QDialog(0)
// {
// 	setupUi(this);
// 
// }
GAoiSelector::GAoiSelector(GImageProcessor* pTheImageProcessor)
	: QDialog(0)
	, m_pImageProcessor(pTheImageProcessor)
{
	if(!m_pImageProcessor)
		qWarning() << "The image processor associated to this AOI selector is 0. 461816";

	setupUi(this);
	m_pTryCenterItem = new QGraphicsRectItem(-5, -5, 10, 10);
	m_pTryCenterItem->setFlag(QGraphicsItem::ItemIsMovable);
	m_pTryCenterItem->setPen(QPen(Qt::green));
	m_pAoiToBe = new QGraphicsRectItem(m_pTryCenterItem);
	m_pAoiToBe->setPen(QPen(Qt::blue));

//	QMetaObject::connectSlotsByName(this);
}

GAoiSelector::~GAoiSelector()
{
// 	if(m_pTryCenterItem)
// 		delete m_pTryCenterItem;
// 	if(m_pAoiToBe)
// 		delete m_pAoiToBe;
}

// QPushButton* GAoiSelector::ProvideNewOpeningButton( QWidget* inWhichWidget )
// {
// 	QPushButton* pBut = new QPushButton("Select AOI", inWhichWidget);
// 	connect(pBut, SIGNAL(clicked()), this, SLOT(open()));
// 
// 	return pBut;
// }

void GAoiSelector::showEvent( QShowEvent* e )
{
	QDialog::showEvent(e);
	QRectF currentAoiRect = m_pImageProcessor->AoiRect();
	InitAoiSelectorItems(currentAoiRect);
	return;
}

void GAoiSelector::closeEvent( QCloseEvent* e )
{
	QDialog::closeEvent(e);
	if(!m_pView || !m_pView->scene())
		return;
	m_pView->scene()->removeItem(m_pTryCenterItem);
//	m_pView->scene()->removeItem(m_pAoiToBe);
}

QRect GAoiSelector::ValidRect( QSize sizeImage )
{
	m_KnownFullImageSize = sizeImage;
	QRect imageRect(QPoint(0, 0), sizeImage);
// 	QSizeF wouldBeAoiSize = m_pImageProcessor->m_pAoiItem->rect().size();
// 	QPointF wouldBeAoiTopLeft = m_pImageProcessor->m_pAoiItem->rect().topLeft() + m_pImageProcessor->m_pAoiItem->scenePos();
// 	QRect wouldBeAoiRect = QRectF(wouldBeAoiTopLeft, wouldBeAoiSize).toRect();
	QRect wouldBeAoiRect = m_pImageProcessor->AoiRect().toRect();

	QRect intersectionRect = imageRect & wouldBeAoiRect;

	if(intersectionRect.isValid()) {
		return intersectionRect;
	}

	m_pImageProcessor->SetAoiRect(imageRect);
	return imageRect;
}

void GAoiSelector::on_WidthBox_valueChanged( int newWidth )
{
	double currentWidth = double(newWidth);
	double currentHeight = m_pAoiToBe->rect().height();

	// an AOI rect centered on the m_pTryCenterItem
	QRectF centeredAoiRect = QRectF(-currentWidth / 2, -currentHeight / 2, currentWidth, currentHeight);
	m_pAoiToBe->setRect(centeredAoiRect);
}

void GAoiSelector::on_HeightBox_valueChanged( int newHeight )
{
	double currentWidth = m_pAoiToBe->rect().width();
	double currentHeight = double(newHeight);

	// an AOI rect centered on the m_pTryCenterItem
	QRectF centeredAoiRect = QRectF(-currentWidth / 2, -currentHeight / 2, currentWidth, currentHeight);
	m_pAoiToBe->setRect(centeredAoiRect);
}

void GAoiSelector::on_ValidateButton_clicked()
{
	QPointF aoiTopLeftRelativeToScene = m_pAoiToBe->rect().topLeft() + m_pAoiToBe->scenePos();
	QSizeF sizeOfAOI = m_pAoiToBe->rect().size();
	m_pImageProcessor->SetAoiRect(QRectF(aoiTopLeftRelativeToScene, sizeOfAOI));

// 	QRectF aoiRectRelativeToItem(QPointF(0.0, 0.0), sizeOfAOI);
// 	m_pImageProcessor->m_pAoiItem->setPos(aoiTopLeftRelativeToScene);
// 	m_pImageProcessor->m_pAoiItem->setRect(aoiRectRelativeToItem);
}

void GAoiSelector::on_FullFrameButton_clicked()
{
	QRectF imageRect(QPoint(0, 0), m_KnownFullImageSize);
	InitAoiSelectorItems(imageRect);
}

void GAoiSelector::InitAoiSelectorItems( QRectF &currentAoiRect )
{
	if(!m_pView || !m_pView->scene())
		return;
	m_pView->scene()->addItem(m_pTryCenterItem);

	double currentWidth = currentAoiRect.width();
	double currentHeight = currentAoiRect.height();
	QPointF currentAoiCenter = currentAoiRect.center();

	// place the position of m_pTryCenterItem at the center of the aoi
	m_pTryCenterItem->setPos(currentAoiCenter);

	// an AOI rect centered on the m_pTryCenterItem
	QRectF centeredAoiRect = QRectF(-currentWidth / 2, -currentHeight / 2, currentWidth, currentHeight);
	m_pAoiToBe->setRect(centeredAoiRect);
	// sets the spin box values and the m_pAoiToBe.
	WidthBox->setValue(currentAoiRect.width());
	HeightBox->setValue(currentAoiRect.height());

	m_pView->centerOn(m_pTryCenterItem->pos());
}
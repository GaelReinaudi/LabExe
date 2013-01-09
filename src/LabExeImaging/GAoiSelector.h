#ifndef GAOISELECTOR_H
#define GAOISELECTOR_H

#include <QDialog>
#include "GImageGraphicsScene.h"
#include "ui_GAoiSelector.h"

class GImageProcessor;

using namespace Ui;
/////////////////////////////////////////////////////////////////////
//! \brief The GAoiSelector implements a QDialog that makes it easy to select an Area Of Interest in an Image.
/*!
This 
*/
class GAoiSelector : public QDialog, public GAoiSelectorClass
{
	Q_OBJECT

public:
// 	GAoiSelector(GImageGraphicsScene *parentScene);
	GAoiSelector(GImageProcessor* pTheImageProcessor);
	~GAoiSelector();

// 	//! Provides a button that will open this GAoiSelector. It will use the argument to connect the appropriate signals to the appropriate GImageGraphicsScene.
// 	QPushButton* ProvideNewOpeningButton(QWidget* inWhichWidget);
	//! returns a valid AOI. valid means that, given the sizeImage, the AOI will be a part of it, included in it. If it fails, it return the whole image rect.
	QRect ValidRect( QSize sizeImage );

protected:
	//! Reimplemented
	void showEvent(QShowEvent* e);
	//! Moves the AOI center item and try-to-be-AOI on the proposed AOI rect
	void InitAoiSelectorItems( QRectF &currentAoiRect );
	//! Reimplemented
	void closeEvent(QCloseEvent* e);

protected slots:
	//! slot called when the spin box changes
	void on_WidthBox_valueChanged(int newWidth);
	//! slot called when the spin box changes
	void on_HeightBox_valueChanged(int newHeight);
	//! slot called when the ValidateButton is clicked
	void on_ValidateButton_clicked();
	void on_FullFrameButton_clicked();

private:
	//! the associated image processor
	GImageProcessor* m_pImageProcessor;
	//! the graphics item that represents what we would like to be the center of the AOI.
	QGraphicsRectItem* m_pTryCenterItem;
	//! the graphics item that represents what we would be the AOI.
	QGraphicsRectItem* m_pAoiToBe;
	//! size of the images received by the processor, or invalid QSize if unkown.
	QSize m_KnownFullImageSize;
};

#endif // GAOISELECTOR_H

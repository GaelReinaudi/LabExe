#ifndef GInstructionView_H
#define GInstructionView_H

#include <QGraphicsView>

class G_old_Instruction;
class GCurveTableView;

/////////////////////////////////////////////////////////////////////
//! \brief The GInstructionView class is derived from QGraphicsView and allows to display informations about G_old_Instruction.
/*!
You set the G_old_Instruction to be displayed by using SetInstruction((). 
Then, the GInstructionView asks the G_old_Instruction to provide QGraphicsItemGroup 
that will be stored in a member QGraphicsScene, which is eventually displayed.
*/
class GInstructionView : public QGraphicsView
{
	Q_OBJECT

public:
	GInstructionView(QWidget *parent);
	~GInstructionView();
	void SetInstruction( G_old_Instruction* pInstruction );

private:
	//! The G_old_Instruction displayed
	G_old_Instruction* m_pInstruction;
	//! The scene that is displayed by the view. It is filled by asking the m_pInstruction to provide some QGraphicsItemGroup
	QGraphicsScene* m_pInsScene;

protected:
	//! Reimplemented function from QGraphicsView, in order to MakeSegmentFitExactly() when the view is resized.
	virtual void resizeEvent(QResizeEvent * event);
	virtual void mouseMoveEvent ( QMouseEvent * e );

protected slots:
	//! Update the item in the view and fit it tightly
	void UpdateView();
public slots:
	//! this function makes the view to be zoomed so that the G_old_Instruction occupies exactly the view area
	void FitInstructionInView();
};

#endif // GInstructionView_H

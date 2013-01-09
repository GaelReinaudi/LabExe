#include "GCurvePiece.h"
#include "GCurveBloc.h"

GCurvePiece::GCurvePiece(GCurveBloc *parent)
	: GCurve(parent)
{
	m_NumberParamNecessaryAndSufficient = HasToBeInitialized;
	connect(this, SIGNAL(ThisSegmentAmplitudeChanged()), this, SLOT(UpdateGraphicsItems()));
	connect(this, SIGNAL(ThisSegmentDurationChanged()), this, SLOT(UpdateGraphicsItems()));
}

GCurvePiece::~GCurvePiece()
{

}

GCurvePiece* GCurvePiece::SetDuration( double TheDuration )
{
	ReOrderImportanceOfParameters(theduration);
	if(TheDuration == m_Duration)
		return this;
	m_Duration = TheDuration;
	AdjustOtherParameters();
	emit ThisSegmentDurationChanged();
	return this;
}

GCurvePiece* GCurvePiece::SetInitialValue( double TheInitial )
{
	ReOrderImportanceOfParameters(initial);
	if(TheInitial == m_InitialValue)
		return this;
	m_InitialValue = TheInitial;
	AdjustOtherParameters();
	emit ThisSegmentAmplitudeChanged();
	return this;
}

GCurvePiece* GCurvePiece::SetFinalValue( double TheFinal )
{
	ReOrderImportanceOfParameters(final);
	if(TheFinal == m_FinalValue)
		return this;
	m_FinalValue = TheFinal;
	AdjustOtherParameters();
	emit ThisSegmentAmplitudeChanged();
	return this;
}

/////////////////////////////////////////////////////////////////////
/*!
When a parameter is adjusted by the user, it means that it is important.
This parameter is then put first in the list of important parameters.
It means that, some other parameters will go down in this list. 
This can change the parameters that will be adjusted eventually.
	\return    void : 
	\param     const int TheParamJustAdjusted : the parameter (from an enum) representing the parameter that has just been adjusted.
*/
void GCurvePiece::ReOrderImportanceOfParameters( const int TheParamJustAdjusted )
{
	if(!JustCheckingInitialization())
		return;
	int index = m_ListImportanceParam.indexOf(TheParamJustAdjusted);
	if(index < 0)
		return;
	// We move the parameter to the most important position of the ParamNecessaryAndSufficient 
	m_ListImportanceParam.move(index, 0);
}

bool GCurvePiece::JustCheckingInitialization() const
{
	bool IsOk = true;
	if(m_NumberParamNecessaryAndSufficient == HasToBeInitialized)
		IsOk = false;
	if(m_ListImportanceParam.size() == 0)
		IsOk = false;

	if(!IsOk)
	{
		QString str = "Derived class of GCurvePiece not well initialized!";
		QMessageBox msgBox(QMessageBox::Warning, "", str);
		msgBox.exec();
	}
	return IsOk;
}

//QGraphicsItemGroup* GCurvePiece::GetNewItemGroupForDrawing() const
//{
//	QGraphicsItemGroup* GroupToReturn = new QGraphicsItemGroup();
//	QGraphicsTextItem* textitem = new QGraphicsTextItem("N.A.");
//	GroupToReturn->addToGroup(textitem);
//	textitem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
//	return GroupToReturn;
//}

void GCurvePiece::MouseRightClicked( QPoint posScreen, QPoint posSegment )
{
	//m_ContextMenu.addAction("fsfsfqssdgmjpijpinpi");
	//m_ContextMenu.addAction("zaeaeae");
	//m_ContextMenu.popup(posScreen);
	return;
}

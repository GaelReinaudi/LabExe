#include "GCurveSegment.h"

GCurveSegment::GCurveSegment(GCurveBloc *parent)
	: GCurvePiece(parent)
{
	//! Don't forget this!
	InitializeListImportanceParam();
	m_Slope = 0;
	
	// extra init
	SetDuration(1.0);
	SetInitialValue(0.0);
	SetFinalValue(1.0);
}

GCurveSegment::~GCurveSegment()
{
}

void GCurveSegment::InitializeListImportanceParam()
{
	// default importance of the parameters
	m_ListImportanceParam << initial << final << theduration << slope;
	// Only 3 parameters are sufficient in order to define a GCurveSegment (unless you add some constraints).
	m_NumberParamNecessaryAndSufficient = 3;
}

GCurveSegment* GCurveSegment::SetSlope( double TheSlope )
{
	m_Slope = TheSlope;
 	ReOrderImportanceOfParameters(slope);
	AdjustOtherParameters();
	emit ThisSegmentAmplitudeChanged();
	return this;
}

/////////////////////////////////////////////////////////////////////
/*!
For a GLinSegment, depending on the order of importance of a ParametersOfLine, ONE redundant parameter has to be adjusted in order to be consistent.
The parameter is the last one in the m_ListImportanceParam. 
The function ReOrderImportanceOfParameters(TheParamJustAdjusted) might have been called before to change this ordering!
This function does that by using basic mathematical expression!
	\return    void : 
	\param     int TheParamJustAdjusted : the parameter (from an enum) that has just been adjusted
*/
void GCurveSegment::AdjustOtherParameters()
{
	int TheLeastImportantParam = m_ListImportanceParam.last();
	switch (TheLeastImportantParam)
	{
	case initial:
		m_InitialValue = m_FinalValue - m_Duration * m_Slope;
		break;
	case final:
		m_FinalValue = m_InitialValue + m_Duration * m_Slope;
		break;
	case theduration:
		m_Duration = (m_FinalValue - m_InitialValue) / m_Slope;
		emit ThisSegmentDurationChanged();
		break;
	case slope:
		m_Slope = (m_FinalValue - m_InitialValue) / m_Duration;
		break;
	}
}

QVariant GCurveSegment::GetThisSegmentQVariant()
{
	return qVariantFromValue(this);
}

QGraphicsItemGroup* GCurveSegment::GetNewItemGroupForDrawing() const
{
	QGraphicsItemGroup* GroupToReturn = new QGraphicsItemGroup();

	QGraphicsPathItem* pPathItem = new QGraphicsPathItem(m_PathItem.path());
//	QGraphicsPathItem* pPathItem = &m_PathItem;
	// let's add the QGraphicsPathItem
	GroupToReturn->addToGroup(pPathItem);

	// let's put some color
//	QLinearGradient gradient(0, - GetChannel()->m_LimitDown, 0, - GetChannel()->m_LimitUp);
	QLinearGradient gradient(0, - (-10), 0, - (10));
	gradient.setColorAt(0.0, Qt::blue);
	gradient.setColorAt(0.8, Qt::red);
	pPathItem->setBrush(gradient);

	return GroupToReturn;
}

QGraphicsItemGroup* GCurveSegment::GetNewItemGroupForParameters() const
{
	QGraphicsItemGroup* GroupToReturn = new QGraphicsItemGroup();
	// let's add a textitem
	QGraphicsTextItem* TextItem = new QGraphicsTextItem("3");
	TextItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);

	QDoubleSpinBox* pSpin = new QDoubleSpinBox();
	pSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);
	pSpin->setGeometry(0, 0, 50, 20);
	QGraphicsProxyWidget* pProxy = new QGraphicsProxyWidget();
	pProxy->setWidget(pSpin);
	pProxy->setFlag(QGraphicsItem::ItemIgnoresTransformations);
	pProxy->setPos(Duration()/2, - m_FinalValue/2 - m_InitialValue/2);
	
	//GroupToReturn->addToGroup(TextItem);
	GroupToReturn->addToGroup(pProxy);
	return GroupToReturn;
}

void GCurveSegment::UpdateGraphicsItems()
{
	QPainterPath ThePath;
	// ThePath is defined by the GCurvePiece parameters
	ThePath.moveTo(0, 0);
	ThePath.lineTo(0, -m_InitialValue);
	ThePath.lineTo(Duration(), -m_FinalValue);
	ThePath.lineTo(Duration(), 0);
	m_PathItem.setPath(ThePath);
}








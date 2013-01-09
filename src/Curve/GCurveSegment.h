#ifndef GCurveSegment_H
#define GCurveSegment_H

#include "GCurvePiece.h"

/////////////////////////////////////////////////////////////////////
//! \brief A GCurveSegment defines a line GCurvePiece:
/*!
	- a duration,
	- an initial value, 
	- a final value,
	- a slope

Those data seems to be redundant: if I give you an initial value, a slope and a duration, then you can guess the final value.
This can actually be used to allow the user to specify what parameters are important (= meaningful = to fix), 
and what parameter has to be adjusted automatically in case one of the important ones changes.
*/
class GCurveSegment : public GCurvePiece
{
	Q_OBJECT

public:
	GCurveSegment(GCurveBloc *parent);
	~GCurveSegment();

	//! Sets the duration of the GCurvePiece
//?	GCurveSegment* SetDuration(double TheDuration) {GCurvePiece::SetDuration(TheDuration); return this;}
	//! Sets the initial value of the GCurvePiece
//?	GCurveSegment* SetInitialValue(double TheInitial) {GCurvePiece::SetInitialValue(TheInitial); return this;}
	//! Sets the final value of the GCurvePiece
//?	GCurveSegment* SetFinalValue(double TheFinal) {GCurvePiece::SetFinalValue(TheFinal); return this;}

	//! Sets the slope of the GCurveSegment. The other parameters to be adjusted are set by the function SetPriorityParameter().
	GCurveSegment* SetSlope(double TheSlope);

private:
	//! This HAS TO BE CALLED when instantiating in order to give a default ordering for the parameters.
	void InitializeListImportanceParam();
	//! Adjusts the parameters defining the Line, trying to keep fix the parameters that have a high priority of importance.
	void AdjustOtherParameters();
	//! Returns a QVariant holding a pointer to this.
	QVariant GetThisSegmentQVariant();

	//! Updates the QGraphicsPathItem of the GCurvePiece accordingly to the values
	void UpdateGraphicsItems();
	//! Returns a group of new items that can be used to set a QGraphicsScene and draw a QGraphicsView. 
	QGraphicsItemGroup* GetNewItemGroupForDrawing() const;
	//! Returns a group of new items that represents the values that defines the GCurvePiece (it can be used to set a QGraphicsScene and draw a QGraphicsView). 
	QGraphicsItemGroup* GetNewItemGroupForParameters() const;


private:
	//! Optional parameters used to define the "line" shape
	double m_Slope;
	//! Enumeration of the additional parameters that can be used to define a GCurveSegment
	enum AdditionalParametersOfLine{slope};
};
Q_DECLARE_METATYPE(GCurveSegment*)

#endif // GCurveSegment_H

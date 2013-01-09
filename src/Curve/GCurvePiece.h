#ifndef GCurvePiece_H
#define GCurvePiece_H

#include <QObject>
#include "GCurve.h"

class GCurveBloc;

enum Helper{HasToBeInitialized = -3141592};
/////////////////////////////////////////////////////////////////////
//! \brief A GCurvePiece is a GCurve that defines a piece of GCurveBloc:
/*!
	- a duration,
	- an initial value, 
	- a final value,
	- a shape (ie: straight line, function,...),
	- maybe some additional parameters.

Those data seems to be redundant: if I give an initial value, a shape and a duration, then you can guess the final value.
This can actually be used to allow the user to specify what is important (=meaningful), 
and what has to be adjusted automatically in case one of the important value changes.

This class is abstract: in order to define the way that parameters are adjusted, this class needs to be derived.
*/
class GCurvePiece : public GCurve
{
	Q_OBJECT

public:
	GCurvePiece(GCurveBloc *parent);
	~GCurvePiece();

	//! Sets the duration of the GCurvePiece
	virtual GCurvePiece* SetDuration(double TheDuration);
	//! Returns the  duration of the GCurvePiece
	double Duration() const {return m_Duration;}
	//! Sets the initial value of the GCurvePiece
	virtual GCurvePiece* SetInitialValue(double TheInitial);
	//! Sets the final value of the GCurvePiece
	virtual GCurvePiece* SetFinalValue(double TheFinal);

protected:
	//! Changes the order of the important parameters knowing that TheParamJustAdjusted has just been adjusted (it means it is important).
	void ReOrderImportanceOfParameters( const int TheParamJustAdjusted );
	//! Adjusts the parameters defining the shape, trying to keep fix the parameters that have a high priority of importance.
	/*! It is pure virtual since you have to define this adjustment for your derived class (=for a given shape, e.g. Line, Expo, etc...).*/
	virtual void AdjustOtherParameters() = 0;
	//! This is called when instantiating in order to give a default ordering for the parameters
	/*! It is pure virtual since this depends on your derived class (=for a given shape, e.g. Line, Expo, etc...).
	This HAS TO BE CALLED when instantiating the derived class!*/
	virtual void InitializeListImportanceParam() = 0;

protected slots:
	//! Updates the QGraphicsItems of the GCurvePiece accordingly to the values
	virtual void UpdateGraphicsItems() = 0;

private:
	//! Called in order to check if the derived class did initialize properly
	bool JustCheckingInitialization() const;
	//! returns a QVariant holding a pointer to "this".
	//! it is useful for some function in the Model/View Architecture in Qt.
	/*! It is pure virtual since you have to define the type of the QVariant for your derived class (=for a given shape, e.g. Line, Expo, etc...).*/
	virtual QVariant GetThisSegmentQVariant() = 0;
	//! Called when the right button was clicked over the GCurvePiece
	void MouseRightClicked(QPoint posScreen, QPoint posSegment);

protected:
	//! The duration
	double m_Duration;
	//! Initial value 
	double m_InitialValue;
	//! Final value
	double m_FinalValue;
	//! List of int (from an enum) representing the parameters. 
	/*! The ordering goes from the most important (=most "to-keep-fix") to the least (= most "flex-adjustable").
	When one of the adjustable parameter is set by a method, it might goes up in the list so that it becomes important
	This HAS TO BE ADJUSTED in the pure virtual function InitializeListImportanceParam()!*/
	QList<int> m_ListImportanceParam;
	//! Number of parameters that are necessary and sufficient in order to define a given GCurvePiece.
	/*! This HAS TO BE ADJUSTED in the pure virtual function InitializeListImportanceParam()!*/
	int m_NumberParamNecessaryAndSufficient;
	/*! Enumeration of the parameters that can define a GCurveSegment.
	It starts at 1000 so that the AdditionalParametersOf_XXXXX (derived class) don't have the same index!*/
	enum ParametersOfSegment{initial = 1000, final, theduration};

	//! A GCurvePiece has a QGraphicsPathItem that represent it in SI units 
	//! so that it can be drawn in the Qt graphics-view framework.
	mutable QGraphicsPathItem m_PathItem;

signals:
	void ThisSegmentDurationChanged();
	void ThisSegmentAmplitudeChanged();

	friend class GCurveBloc;

};
Q_DECLARE_METATYPE(GCurvePiece*)

//// test
//class UndoComand_SetSegmentDuration : public QUndoCommand
//{
//public:
//	UndoComand_SetSegmentDuration(GCurvePiece *pSeg, double newValue)
//		: m_pSeg(pSeg), m_NewValue(newValue) 
//	{ setText(QString("Set duration to %1").arg(newValue, 0, 'G')); 
//	m_OldValue = m_pSeg->Duration();}
//	virtual void undo()
//	{ m_pSeg->SetDuration(m_OldValue); }
//	virtual void redo()
//	{ m_pSeg->SetDuration(m_NewValue); }
//private:
//	GCurvePiece *m_pSeg;
//	double m_NewValue;
//	double m_OldValue;
//
//};

#endif // GCurvePiece_H

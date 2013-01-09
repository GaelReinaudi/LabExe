#ifndef GCURVE_H
#define GCURVE_H

#include <QObject>
#include <QtGui>

typedef double GTimeUnit;

/////////////////////////////////////////////////////////////////////
//! \brief The GCurve class is an abstract class that defines a curve. 
/*!
A GCurve is used to define a time-dependent signal that can be used by a GDevice to output waveforms.
*/
class GCurve : public QObject
{
//	Q_OBJECT

public:
	GCurve(QObject *parent);
	~GCurve();
	//! Total duration of the GCurveBloc
	virtual double Duration() const = 0;

public:
	virtual QGraphicsItemGroup* GetNewItemGroupForDrawing() const = 0;
	virtual QGraphicsItemGroup* GetNewItemGroupForParameters() const = 0;

private:
	//! The QGraphicsItem that can represent the curve
	QGraphicsPathItem m_CurvePathItem;
	
};

#endif // GCURVE_H

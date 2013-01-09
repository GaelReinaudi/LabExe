#ifndef GCurveBloc_H
#define GCurveBloc_H

#include <QVector>
#include "GCurve.h"
class GCurvePiece;
typedef QVector<GCurvePiece*> GListOfCurvePieces;

/////////////////////////////////////////////////////////////////////
//! \brief A GCurveBloc defines a GCurve which is actually a list of GCurvePiece's (GListOfCurvePieces).
/*!
You can 
*/
class GCurveBloc : public GCurve, public GListOfCurvePieces
{
	Q_OBJECT

public:
	GCurveBloc(QObject *parent);
	~GCurveBloc();

	//! Returns a const pointer to the QList of GCurvePiece's (GCurveBloc inherits GListOfCurvePieces).
	const GListOfCurvePieces* thisListOfCurvePieces() const {return static_cast<const GListOfCurvePieces*>(this);}
	//! Total duration of the GCurveBloc
	GTimeUnit Duration() const;
	//! Duration of the Ith GCurvePiece of the GCurveBloc
	double Duration(const int IthCurvePiece) const;
	//! Number of GCurvePiece's
	int NumberSegments() const {return size();}
	//! Returns a pointer to the Nth GCurvePiece
	GCurvePiece* GetNthSegment(const int IthCurvePiece) const;

	//! Returns a QVariant that holds the pointer to this GCurveBloc
	QVariant GetThisCurveBlocQVariant();
	//! Returns a QVariant that holds the pointer to the Nth GCurvePiece of the GCurveBloc
	QVariant GetTheNthSegmentQVariant(const int NthSegment);
public:
	QGraphicsItemGroup* GetNewItemGroupForDrawing() const;
	QGraphicsItemGroup* GetNewItemGroupForParameters() const;

private:
	
public slots:
	void Clicked(const QModelIndex&);
	
signals:
	void ThisCurveBlocDurationChanged();

};
Q_DECLARE_METATYPE(GCurveBloc*)

#endif // GCurveBloc_H

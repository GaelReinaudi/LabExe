#include "GCurveBloc.h"
#include "GCurvePiece.h"

GCurveBloc::GCurveBloc(QObject *parent)
	: GCurve(parent)
{
}

GCurveBloc::~GCurveBloc()
{
}

//GCurveSegment* GCurveBloc::AppendNewLineSegment()
//{
//	GCurveSegment* unnewseg = new GCurveSegment(this);
//	append(unnewseg);
//	return unnewseg;
//}

GTimeUnit GCurveBloc::Duration() const
{
	double tottime = 0;
	foreach(GCurvePiece* pseg, *thisListOfCurvePieces()) {
		tottime += pseg->Duration();
	}
	return tottime;
}

double GCurveBloc::Duration( const int IthCurvePiece ) const
{
	return GetNthSegment(IthCurvePiece)->Duration();
}

GCurvePiece* GCurveBloc::GetNthSegment( const int IthCurvePiece ) const
{
	return at(IthCurvePiece);
}

QVariant GCurveBloc::GetThisCurveBlocQVariant()
{
	return qVariantFromValue(this);
}

QVariant GCurveBloc::GetTheNthSegmentQVariant( const int NthSegment )
{
	return at(NthSegment)->GetThisSegmentQVariant();
}

void GCurveBloc::Clicked(const QModelIndex& index)
{
	at(1);
}

QGraphicsItemGroup* GCurveBloc::GetNewItemGroupForDrawing() const
{
	QGraphicsItemGroup* pgroupe = new QGraphicsItemGroup();
	double tottime = 0;
	foreach(GCurvePiece* pseg, *thisListOfCurvePieces()) {
		QGraphicsItemGroup* seggroupe = pseg->GetNewItemGroupForDrawing();
		seggroupe->moveBy(tottime, 0);
		pgroupe->addToGroup(seggroupe);
		tottime += pseg->Duration();
	}
	return pgroupe;
}
QGraphicsItemGroup* GCurveBloc::GetNewItemGroupForParameters() const
{
	QGraphicsItemGroup* pgroupe = new QGraphicsItemGroup();
	double tottime = 0;
	foreach(GCurvePiece* pseg, *thisListOfCurvePieces()) {
		QGraphicsItemGroup* seggroupe = pseg->GetNewItemGroupForParameters();
		seggroupe->moveBy(tottime, 0);
		pgroupe->addToGroup(seggroupe);
		QGraphicsScene ascenefordestroy;
		ascenefordestroy.destroyItemGroup(seggroupe);
		tottime += pseg->Duration();
	}
	return pgroupe;
}

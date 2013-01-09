#include "GImageDouble.h"
#include <QMetaType>

int idGImageDouble = qRegisterMetaType<GImageDouble>("GImageDouble");

GImageDouble::GImageDouble()
	: QImage()
	, m_DateTimeCreated(QDateTime::currentDateTime())
{
	m_pSharedArray = QSharedPointer<GDoubleArray>(new GDoubleArray());
}

GImageDouble::GImageDouble( const QImage & image, bool fillDoubleArrayFromQImage /*= true*/)
	: QImage(image)
	, m_DateTimeCreated(QDateTime::currentDateTime())
{
	int hei = height();
	int wid = width();
	int Npix = wid * hei;
	m_pSharedArray = QSharedPointer<GDoubleArray>(new GDoubleArray(Npix));

	if(!fillDoubleArrayFromQImage)
		return;

	double* pDoubleArray = m_pSharedArray->data();
	int iTot = 0;
	for(int jLine = 0; jLine < hei; jLine++) {
		const uchar* scanL = image.constScanLine(jLine);
		for(int iCol = 0; iCol < wid; iCol++) {
			pDoubleArray[iTot] = scanL[iCol];
			iTot++;
		}
	}
}

GImageDouble::GImageDouble( int theWidth, int theHeight )
	: QImage(theWidth, theHeight, QImage::Format_Indexed8)
	, m_DateTimeCreated(QDateTime::currentDateTime())
{
	int hei = height();
	int wid = width();
	int Npix = wid * hei;
	m_pSharedArray = QSharedPointer<GDoubleArray>(new GDoubleArray(Npix));
}

GImageDouble::GImageDouble( int theWidth, int theHeight, double fillValue )
	: QImage(theWidth, theHeight, QImage::Format_Indexed8)
	, m_DateTimeCreated(QDateTime::currentDateTime())
{
	int hei = height();
	int wid = width();
	int Npix = wid * hei;
	GDoubleArray* pArr = new GDoubleArray(Npix);
	for(int i = 0; i < Npix ; i++) {
		(*pArr)[i] = fillValue;
	}
	m_pSharedArray = QSharedPointer<GDoubleArray>(pArr);
}

GImageDouble::GImageDouble( const GImageDouble & image )
	: QImage(image)
	, m_DateTimeCreated(image.m_DateTimeCreated)
{
	m_pSharedArray = image.m_pSharedArray;
}

GImageDouble & GImageDouble::operator=( const GImageDouble & image )
{
	QImage::operator=(image);
	m_DateTimeCreated = image.m_DateTimeCreated;
	m_pSharedArray = image.m_pSharedArray;
	return *this;
}

GImageDouble::~GImageDouble()
{

}

// GImageDouble GImageDouble::Clone() const
// {
// 
// }

GImageDouble GImageDouble::ExtractAoi( QRect aoirect ) const
{
	// if rectangle is out of the image
	if(!rect().contains(aoirect)) {
		qWarning() << "The AOI rectangle is out of the image. 64123846";
		// correction so that the calculation can still go on without crashing
		aoirect = aoirect.intersected(rect());
	}

	int hei = aoirect.height();
	int wid = aoirect.width();
	// create the GImageDouble with the good size
	// we specify false here so that the double data are not computed from the QImage since wee are going to do it ourself later
	GImageDouble imToReturn(copy(aoirect), false);
	// copy the double data
	// the image array to return
	double* pDoubleArray = imToReturn.DoubleArray().data();
	// the point in the original image that corresponds to the top left of the AOI
	double* pTopLeftStart = DoubleArray().data() + (aoirect.left() + width() * aoirect.top());
	// scanline is going to be incremented for each line of the AOI
	double* pScanLine = pTopLeftStart;
	int iTot = 0;
	for(int jLine = 0; jLine < hei; jLine++) {
		for(int iCol = 0; iCol < wid; iCol++) {
			pDoubleArray[iTot] = pScanLine[iCol];
			iTot++;
		}
		pScanLine += width();
	}

	return imToReturn;
}

GImageDouble GImageDouble::DataShifted( QPoint translation ) const
{
	if(translation == QPoint(0,0))
		return GImageDouble(*this);

	int aoiLeft = -translation.x();
	int aoiTop = -translation.y();
	QRect aoirect = rect().translated(aoiLeft, aoiTop);
	int hei = aoirect.height();
	int wid = aoirect.width();
	// create the GImageDouble with the good size
	// we specify false here so that the double data are not computed from the QImage since wee are going to do it ourself later
	GImageDouble imToReturn(copy(aoirect), false);
	// copy the double data
	// the image array to return
	double* pDoubleArray = imToReturn.DoubleArray().data();
	// the point in the original image that corresponds to the top left of the AOI
	int topLeftDataOffset = aoiLeft + wid * aoiTop;
	double* pTopLeftStart = DoubleArray().data() + topLeftDataOffset;
	// scanline is going to be incremented for each line of the AOI
	double* pScanLine = pTopLeftStart;
	int iTot = 0;
	for(int jLine = 0; jLine < hei; jLine++) {
		for(int iCol = 0; iCol < wid; iCol++) {
			if(jLine + aoiTop >= 0 && jLine + aoiTop < hei && iCol + aoiLeft >= 0 && iCol + aoiLeft < wid) {
				pDoubleArray[iTot] = pScanLine[iCol];
			} else {
				pDoubleArray[iTot] = 0.0;
			}
			iTot++;
		}
		pScanLine += wid;
	}

	return imToReturn;
}

/////////////////////////////////////////////////////////////////////
/*!
Fill the underlying QImage that is usually used for displaying the image.
\return: void : 
\param:  double RailDownForQImage : the value of the optical density that corresponds to the value 0 for the QImage.
\param:  double RailUp255ForQImage : the value of the optical density that corresponds to the value 255 for the QImage.
*////////////////////////////////////////////////////////////////////
void GImageDouble::FillQimageFromUsingDoubleArray( double RailDownForQImage, double RailUp255ForQImage )
{
	int hei = height();
	int wid = width();
	int Npix = wid * hei;

	double* pDoubleArray = DoubleArray().data();
	int iTot = 0;
	for(int jLine = 0; jLine < hei; jLine++) {
		uchar* scanL = scanLine(jLine);
		for(int iCol = 0; iCol < wid; iCol++) {
			if(pDoubleArray[iTot] <= RailDownForQImage)
				scanL[iCol] = 0;
			else if(pDoubleArray[iTot] >= RailUp255ForQImage)
				scanL[iCol] = 255;
			else
				scanL[iCol] = 255 * (pDoubleArray[iTot] - RailDownForQImage) / (RailUp255ForQImage - RailDownForQImage) ;
			iTot++;
		}
	}
}

double GImageDouble::PixelAt( int xPos, int yPos ) const
{
	double valRet;
	double* pDoubleArray = DoubleArray().data();
	int indexArray = xPos + yPos * width();
	if(indexArray < DoubleArray().count())
		valRet = DoubleArray().value(indexArray);
	return valRet;
}

GImageDouble & GImageDouble::operator+=( const GImageDouble & other )
{
	if(other.size() != size()) {
		qWarning() << "The sizes don't match for the GImageDouble::operator+=()";
		return *this;
	}

	int hei = height();
	int wid = width();
	int Npix = wid * hei;

	double* pDoubleArray = DoubleArray().data();
	double* pDoubleArrayOther = other.DoubleArray().data();
	for(int i = 0; i < Npix; i++) {
		pDoubleArray[i] += pDoubleArrayOther[i];
	}
	return *this;
}

GImageDouble & GImageDouble::operator-=( const GImageDouble & other )
{
	if(other.size() != size()) {
		qWarning() << "The sizes don't match for the GImageDouble::operator-=()";
			return *this;
	}

	int hei = height();
	int wid = width();
	int Npix = wid * hei;

	double* pDoubleArray = DoubleArray().data();
	double* pDoubleArrayOther = other.DoubleArray().data();
	for(int i = 0; i < Npix; i++) {
		pDoubleArray[i] -= pDoubleArrayOther[i];
	}
	return *this;
}

GImageDouble & GImageDouble::operator*=( double factor )
{
	if(factor == 1.0)
		return *this;

	double* pDoubleArray = DoubleArray().data();
	for(int i = 0; i < DoubleArray().count(); i++) {
		pDoubleArray[i] *= factor;
	}
	return *this;
}
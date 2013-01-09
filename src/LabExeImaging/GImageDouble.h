#ifndef GIMAGEDOUBLE_H
#define GIMAGEDOUBLE_H
#include "labexeimaging_global.h"

#include <QImage>
#include <QDebug>
#include <QSharedPointer>
#include <QVarLengthArray>
#include <QDateTime>

typedef QVarLengthArray<double> GDoubleArray;

class LABEXEIMAGING_EXPORT GImageDouble : public QImage
{
public:
	//! construct an empty image, i.e. empty QImage and empty double array.
	GImageDouble();
	//! construct an empty image, but calls QImage(theWidth, theHeight) and allocates the double array.
	GImageDouble(int theWidth, int theHeight);
	//! construct an empty image, but calls QImage(theWidth, theHeight) and allocates the double array. Fills the array with the given value.
	GImageDouble(int theWidth, int theHeight, double fillValue);
	//! construct an image from a QImage, i.e. calls QImage(QImage) and makes the corresponding double array if fillDoubleArrayFromQImage is true.
	GImageDouble(const QImage & image, bool fillDoubleArrayFromQImage = true);
	//! construct an image from a GImageDouble. increments the counter of the shared DoubleArray().
	GImageDouble(const GImageDouble & image);
	//! destructor
	~GImageDouble();

// 	//! Produces a deep copy of this image
// 	GImageDouble Clone() const;

	//! Returns the date and time the picture was created
	QDateTime DateTimeCreated() const { return m_DateTimeCreated; }

	GDoubleArray & DoubleArray() const {return *(m_pSharedArray);}
	//! returns a new GImageDouble from the valid aoirect within the original image.
	GImageDouble ExtractAoi(QRect aoirect) const;
	//! returns a new GImageDouble but with the data shifted by a 2D translation (x,y). As a result, some points leave the image, while some get in and are initialized with 0.0 values.
	GImageDouble DataShifted(QPoint translation) const;
	//! Fills the QImage (integer values) using the DoubleArray() and mapping the 0 to 255 scale using the range (RailDownForQImage to RailUp255ForQImage)
	void FillQimageFromUsingDoubleArray( double RailDownForQImage, double RailUp255ForQImage );
	//! Returns the double pixel value at the point (xPos, yPos).
	double PixelAt( int xPos, int yPos ) const;

	GImageDouble & operator=(const GImageDouble & other);
	GImageDouble & operator+=(const GImageDouble & other);
	GImageDouble & operator-=(const GImageDouble & other);
	GImageDouble & operator*=(double factor);

private:
	QSharedPointer<GDoubleArray> m_pSharedArray;
	//! Date and time the picture was created
	QDateTime m_DateTimeCreated;
};

#endif // GIMAGEDOUBLE_H

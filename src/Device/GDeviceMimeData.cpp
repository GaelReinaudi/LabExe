#include "GDeviceMimeData.h"
#include <QDebug>

GDeviceMimeData::GDeviceMimeData()
	: QMimeData()
{

}

GDeviceMimeData::~GDeviceMimeData()
{

}

// QStringList GDeviceMimeData::formats() const
// {
// 	return QMimeData::formats() << "image/png";
// }

QVariant GDeviceMimeData::retrieveData( const QString &mimeType, QMetaType type ) const
{
	emit dataRequested(mimeType);
// 	qDebug() << mimeType;

	return QMimeData::retrieveData(mimeType, type);
}

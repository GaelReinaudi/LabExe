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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QVariant GDeviceMimeData::retrieveData(const QString &mimeType, QMetaType type) const
#else
QVariant GDeviceMimeData::retrieveData(const QString &mimeType, QVariant::Type type) const
#endif
{
	emit dataRequested(mimeType);
// 	qDebug() << mimeType;

	return QMimeData::retrieveData(mimeType, type);
}

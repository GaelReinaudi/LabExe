#include "GDeviceListModel.h"
#include <QMimeData>

GDeviceListModel::GDeviceListModel(QObject *parent)
	: QStandardItemModel(parent)
{

}

GDeviceListModel::~GDeviceListModel()
{

}

QStringList GDeviceListModel::mimeTypes() const
{
	QStringList types;
// 	types << "LabExe/device.list";
	types << "LabExe/device.single";
	return types;
}

QMimeData *GDeviceListModel::mimeData(const QModelIndexList &indexes) const
{
	// from qt help: "Using Drag and Drop with Item Views"
	QMimeData* mimeData = new QMimeData();

// 	QByteArray encodedData;
// 	QDataStream stream(&encodedData, QIODevice::WriteOnly);
// 	QString text;
// 	foreach (QModelIndex index, indexes) {
// 		if (index.isValid()) {
// 			text = data(index, Qt::UserRole).toString();
// 			stream << text;
// 		}
// 	}

	// if there is only one device dragged
	QByteArray encodedData2;
	QDataStream stream2(&encodedData2, QIODevice::WriteOnly);
	if(indexes.count() == 1) {
		stream2 << data(indexes.at(0), Qt::UserRole).toString();
	}

//	mimeData->setData("LabExe/device.list", encodedData);
	mimeData->setData("LabExe/device.single", encodedData2);

//	mimeData->setText(text);
	return mimeData;
}

GDeviceListWidget::GDeviceListWidget( QWidget *parent )
	: QListWidget(parent)
{

}

GDeviceListWidget::~GDeviceListWidget()
{

}

QStringList GDeviceListWidget::mimeTypes() const
{
	QStringList types;
	types << "LabExe/device.single";
	return types;
}

QMimeData* GDeviceListWidget::mimeData( const QList<QListWidgetItem*> items ) const
{
	// from qt help: "Using Drag and Drop with Item Views"
	QMimeData* mimeData = new QMimeData();
 	QString text;

	QByteArray encodedData2;
	QDataStream stream2(&encodedData2, QIODevice::WriteOnly);
	if(items.count() == 1) {
		text = items.at(0)->data(Qt::UserRole).toString();
		stream2 << text;
	}

	mimeData->setData("LabExe/device.single", encodedData2);

	return mimeData;
}
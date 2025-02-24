#ifndef GDEVICELISTMODEL_H
#define GDEVICELISTMODEL_H
#include "labexe_global.h"

#include <QStandardItemModel>
#include <QListWidget>

/////////////////////////////////////////////////////////////////////
//! \brief The GDeviceListModel is just a derived class of QStandardItemModel.
/*!
It is intended for providing drag and drop capabilities through the use of QMimeData. See the Qt help concerning "drag and drop".
*/
class LABEXE_EXPORT GDeviceListModel : public QStandardItemModel
{
public:
	GDeviceListModel(QObject *parent);
	~GDeviceListModel();

	QStringList mimeTypes() const;
	QMimeData* mimeData(const QModelIndexList &indexes) const;
private:
	
};

/////////////////////////////////////////////////////////////////////
//! \brief The GDeviceListWidget is intended to provide the functionalities of GDeviceListModel for a QListWidget.
/*!
It is not meant to be used with a GDeviceListModel, but rather to provide the same functionality. 
Indeed, the model() of QListWidget is predefined and cannot be changed.
However, QListWidget can re-implement some drag and drop capabilities through the use of QMimeData.
*/
class GDeviceListWidget : public QListWidget
{
public:
	GDeviceListWidget(QWidget *parent = 0);
	~GDeviceListWidget();

	QStringList mimeTypes() const;
    QMimeData *mimeData(const QList<QListWidgetItem *> &items) const;
private:
	
};
#endif // GDEVICELISTMODEL_H

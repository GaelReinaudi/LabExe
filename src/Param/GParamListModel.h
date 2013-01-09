#ifndef GPARAMLISTMODEL_H
#define GPARAMLISTMODEL_H

#include <QStandardItemModel>

class GParamListModel : public QStandardItemModel
{
	Q_OBJECT

public:
	GParamListModel(QObject *parent);
	~GParamListModel();

private:
	
};

#endif // GPARAMLISTMODEL_H

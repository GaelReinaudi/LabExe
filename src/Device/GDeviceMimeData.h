#ifndef GDEVICEMIMEDATA_H
#define GDEVICEMIMEDATA_H

#include <QMimeData>

class GDeviceMimeData : public QMimeData
{
	Q_OBJECT

public:
	GDeviceMimeData();
	~GDeviceMimeData();

// 	QStringList formats() const;

signals:
	void dataRequested(const QString &mimeType) const;

protected:
    QVariant retrieveData(const QString &mimeType, QMetaType type) const;

private:
	
};

#endif // GDEVICEMIMEDATA_H

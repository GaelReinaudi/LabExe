#include "GInteractiveObject.h"
#include <QGraphicsProxyWidget>

GInteractiveObject::GInteractiveObject(QObject *parent)
	: QThread(parent)
	, GSerializable()
{
}

GInteractiveObject::~GInteractiveObject()
{

}

QGraphicsItem* GInteractiveObject::ProvideNewGraphicsItem(QGraphicsItem* pParentItem)
{
	QWidget* pWid = ProvideNewWidget(0);
	QGraphicsProxyWidget* pItem = new QGraphicsProxyWidget(pParentItem);
	pItem->setWidget(pWid);
	return pItem;
}
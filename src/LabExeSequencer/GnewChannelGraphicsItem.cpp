#include "GnewChannelGraphicsItem.h"
#include "GnewChannel.h"
#include "Sequence/GSequence.h"
#include <QGraphicsSceneMouseEvent>

double CHANNEL_GRAPHICS_ITEM_VERTICAL_SIZE = 10.0;

GnewChannelGraphicsItem::GnewChannelGraphicsItem(GnewChannel* theChannel)
	: QGraphicsRectItem()
	, QGraphicsLayoutItem()
	, m_pChannel(theChannel)
{
	if(!theChannel)
		qCritical() << "The GnewChannel can't be a zero pointer! 5791468";
	// Set a small size by default to not occupy space in the screen before it is resized. See sceneRect() in Qt help.
	setRect(0.0, 0.0, 0.000001, 0.000001);
	// color
	setBrush(QColor(255, 230, 230));

	setFlag(QGraphicsItem::ItemIsSelectable);
}

GnewChannelGraphicsItem::~GnewChannelGraphicsItem()
{

}

QSizeF GnewChannelGraphicsItem::sizeHint( Qt::SizeHint which, const QSizeF & constraint /*= QSizeF() */ ) const
{
	QSizeF sizeToReturn(Channel()->Sequence()->Length(), CHANNEL_GRAPHICS_ITEM_VERTICAL_SIZE);
	return sizeToReturn;
}

void GnewChannelGraphicsItem::setGeometry( const QRectF & rect )
{
	QRectF newGeom;
	newGeom.setSize(rect.size());
	setRect(newGeom);
	setPos(rect.topLeft());
}

void GnewChannelGraphicsItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	if(event->button() != Qt::RightButton)
		return;

//	if(event->modifiers() != Qt::ControlModifier)
		scene()->clearSelection();
	setSelected(true);

}

void GnewChannelGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
//??	todo faire une cascade de partie dans les menus en utilisant GSequenceGraphicsScene::contextMenuEvent() et itemAt()
	QMenu menu;
	Channel()->PopulateContextMenu(&menu);
	menu.exec(event->screenPos());
}

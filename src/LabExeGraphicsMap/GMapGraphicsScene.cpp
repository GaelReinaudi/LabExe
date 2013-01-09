#include "GMapGraphicsScene.h"
#include <QtGui>

GMapGraphicsScene::GMapGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
{
	// try optimization
	// this might make the scene thread-safe: https://bugreports.qt-project.org/browse/QTBUG-2534
	setItemIndexMethod(QGraphicsScene::NoIndex);
}

GMapGraphicsScene::~GMapGraphicsScene()
{

}

void GMapGraphicsScene::dragEnterEvent( QGraphicsSceneDragDropEvent * event )
{
	qDebug() << "GMapGraphicsScene::dragEnterEvent";
	qDebug() << event->possibleActions();
	qDebug() << event->proposedAction();
}

void GMapGraphicsScene::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{

}

void GMapGraphicsScene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{

}

#include "GFileSystemCrawler.h"

void GMapGraphicsScene::dropEvent( QGraphicsSceneDragDropEvent * event )
{
	const QMimeData* mimeData = event->mimeData();
	QStringList listOfUrls;
	if(mimeData->hasUrls()) {
		QList<QUrl> urlList = mimeData->urls();
		for (int i = 0; i < urlList.size() && i < 32; ++i) {
			QString url = urlList.at(i).toLocalFile();
			listOfUrls += url;
		}
		qDebug() << listOfUrls;
	}

	QPointF posDrop = event->scenePos();
	GFileSystemCrawler* pCrawl = new GFileSystemCrawler(this);

	pCrawl->SystemModel()->setRootPath(listOfUrls.first());

	// tell the image processor this is the bench it is part of
	QGraphicsItem* pAgentGraphItem = pCrawl->ProvideNewAgentGraphicsItem(0);
	addItem(pAgentGraphItem);
	pAgentGraphItem->setPos(posDrop);

}
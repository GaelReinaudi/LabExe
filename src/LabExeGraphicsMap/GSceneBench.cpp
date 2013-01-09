#include "GSceneBench.h"
#include "GMapGraphicsScene.h"
#include "GMapGraphicsView.h"
#include "GAgent.h"
#include "device.h"

G_REGISTER_WORKBENCH_CLASS(GSceneBench)

GSceneBench::GSceneBench(QWidget *parent)
	: GWorkBench(parent)
{
    setUnifiedTitleAndToolBarOnMac(true);
	m_pScene = new GMapGraphicsScene(this);
	m_pView = new GMapGraphicsView(this);
	m_pView->setScene(m_pScene);
	connect(m_pScene, SIGNAL(sceneRectChanged(QRectF)), m_pView, SLOT(UpdateFromSceneRect(QRectF)));
	setCentralWidget(m_pView);

//    this->showFullScreen();

// 	// this line defers the dropping action to the WorkBench so that we use the usual mechanism for dropping devices
// 	// this could be changed by deriving the m_pView so that we do better
// 	m_pView->setAcceptDrops(false);

// 	//test for high numbers of items
// 	for(int i = 0; i < 1000; i++) {
// 		QGraphicsRectItem* pItem = m_pScene->addRect(-50.0, -20.0, 100.0, 100.0);
// 		QLinearGradient gradient(0, - (-100), 0, - (10));
// 		gradient.setColorAt(0.0, Qt::blue);
// 		gradient.setColorAt(0.8, Qt::red);
// 		pItem->setBrush(gradient);
// 		RandomizePosition(pItem);
// 	}
//     //test for high numbers of items
//     for(int i = 0; i < 10000; i++) {
//         GGraphicsShinyRectItem* pItem = new GGraphicsShinyRectItem(0);
//         m_pScene->addItem(pItem);
//         RandomizePosition(pItem);
//     }
//     //test for GFolderGraphicsItem
//     GFolderGraphicsItem* GaelIsntAwesome = new GFolderGraphicsItem(0);
//     m_pScene->addItem(GaelIsntAwesome);
}

GSceneBench::~GSceneBench()
{

}

GDeviceWidget* GSceneBench::AddDevice( GDevice* pTheDevice, QPoint whereInParent /*= QPoint(0, 0)*/, bool putOnStack /*= true*/ )
{
	GDeviceWidget* pDevWid = GWorkBench::AddDevice(pTheDevice, whereInParent, putOnStack);

	// provide another device widget that will go in the scene
	// is the device a GAgent?
	GAgent* pAgent = qobject_cast<GAgent*>(pTheDevice);
	// or is it a device that GHasAgent?
	GHasAgent* pHasAgent = dynamic_cast<GHasAgent*>(pTheDevice);

	if(!pAgent)
		if(pHasAgent)
			pAgent = pHasAgent->Agent();

	// if no agent we try to mimic the widget in a proxy graphics item
//	if(!pAgent) {
//        // this could eventually be hidden in a would-be GDeviceMapGraphicsScene, by calling a would-be GDeviceMapGraphicsScene::addDevice(...)
//        QWidget* pOtherWid = pTheDevice->ProvideNewDeviceGroupBox(0);
//        QGraphicsItem* pItem = m_pScene->addWidget(pOtherWid);
//        // randomize the position
//        RandomizePosition(pItem);
//        return pDevWid;
//	}

	// but if it is
	// tell the image processor this is the bench it is part of
	QGraphicsItem* pAgentGraphItem = pAgent->ProvideNewAgentGraphicsItem(0);
	m_pScene->addItem(pAgentGraphItem);

	return pDevWid;
}

void GSceneBench::RandomizePosition( QGraphicsItem* pItem )
{
	// rand gives a number between 0 and RAND_MAX
	// this is a double between -1 and 1
	double x = 2.0 * (double(qrand() - RAND_MAX / 2) / double(RAND_MAX));
	double y = 2.0 * (double(qrand() - RAND_MAX / 2) / double(RAND_MAX));
	x *= 5000.0;
	y *= 5000.0;
	pItem->setPos(x,y);
}

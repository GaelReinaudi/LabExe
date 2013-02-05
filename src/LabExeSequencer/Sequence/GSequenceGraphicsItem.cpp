#include "GSequenceGraphicsItem.h"
#include "GSequence.h"
#include "GSynchEvent.h"

#include <QDeclarativeItem>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

static QFile qmlFile("../../src/LabExeSequencer/QML/sequence.qml");
static QString pathSeqQML = QFileInfo(qmlFile).absoluteFilePath();

GSequenceGraphicsItem::GSequenceGraphicsItem( GSequence* pSeq, QGraphicsItem* pParIt ) 
	: QGraphicsRectItem(pParIt)//(0.0, 0.0, 1000.0, 500.0)
	, m_pSeq(pSeq)
	, m_pQMLitem(0)
	, m_pEngine(0)
{
	connect(pSeq, SIGNAL(ChildAgentAdded(GAgent*)), this, SLOT(Event_ChildAgentAdded(GAgent*)));

//	m_pSeq->RootTimeEvent()->ProvideNewAgentWrappingItem(this);

	// just for debugging we make a new engine (don't delete it...) in order to reload the qml several times
	if(m_pEngine) {
		delete m_pEngine;
		m_pEngine = 0;
	}
	m_pEngine = new QDeclarativeEngine;
	m_pEngine->rootContext()->setContextProperty("seq", m_pSeq);

	//QMetaObject::invokeMethod(this, "AddQmlToScene", Qt::QueuedConnection);
	// for debugging we try to reload the qml so that we can see the changes as we save the qml file
	QTimer* pReloaderTimer = new QTimer(this);
	connect(pReloaderTimer, SIGNAL(timeout()), this, SLOT(AddQmlToScene()));
	pReloaderTimer->setInterval(1000);
	pReloaderTimer->start();
}

GSequenceGraphicsItem::~GSequenceGraphicsItem()
{
	if(m_pQMLitem) {
		scene()->removeItem(m_pQMLitem);
		delete m_pQMLitem;
		m_pQMLitem = 0;
	}
	if(m_pEngine) {
		delete m_pEngine;
		m_pEngine = 0;
	}
}

void GSequenceGraphicsItem::AddQmlToScene()
{
	// remove item in case we load it several times for debug purposes
	if(m_pQMLitem) {
		scene()->removeItem(m_pQMLitem);
		delete m_pQMLitem;
		m_pQMLitem = 0;
	}
	m_pEngine->clearComponentCache();

	QDeclarativeComponent component(m_pEngine, pathSeqQML);
	QObject *myObject = component.create();
	m_pQMLitem = qobject_cast<QDeclarativeItem*>(myObject);
	if(!m_pQMLitem) {
		qWarning() << "Couldn't make item from" << pathSeqQML;
		return;
	}
	scene()->addItem(m_pQMLitem);
}

void GSequenceGraphicsItem::Event_ChildAgentAdded( GAgent* pAgentAdded )
{
	qDebug() << "Event_ChildAgentAdded" << pAgentAdded;
	pAgentAdded->ProvideNewAgentWrappingItem(this);
}























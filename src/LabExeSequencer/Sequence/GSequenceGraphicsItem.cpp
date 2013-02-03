#include "GSequenceGraphicsItem.h"
#include "GSequence.h"
#include "GSynchEvent.h"

#include <QDeclarativeItem>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>


GSequenceGraphicsItem::GSequenceGraphicsItem( GSequence* pSeq, QGraphicsItem* pParIt ) 
	: QGraphicsRectItem(pParIt)//(0.0, 0.0, 1000.0, 500.0)
	, m_pSeq(pSeq)
	, m_pQMLitem(0)
{
	connect(pSeq, SIGNAL(ChildAgentAdded(GAgent*)), this, SLOT(Event_ChildAgentAdded(GAgent*)));

//	m_pSeq->RootTimeEvent()->ProvideNewAgentWrappingItem(this);

	QMetaObject::invokeMethod(this, "AddQmlToScene", Qt::QueuedConnection);
}

GSequenceGraphicsItem::~GSequenceGraphicsItem()
{

}

void GSequenceGraphicsItem::AddQmlToScene()
{
	QFile qmlFile("../../src/LabExeSequencer/QML/sequence.qml");
	QString pathSeqQML = QFileInfo(qmlFile).absoluteFilePath();

	QDeclarativeEngine* engine = new QDeclarativeEngine;
	engine->rootContext()->setContextProperty("seq", m_pSeq);

	QDeclarativeComponent component(engine, pathSeqQML);
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























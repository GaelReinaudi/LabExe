#include "GParamLabel.h"
#include "GParam.h"
#include <QApplication>

GParamLabel::GParamLabel(GParam* pTheParam, QWidget *parent )
	: QLabel(pTheParam->Name(), parent)
	, m_pParam(pTheParam)
{
	connect(pTheParam, SIGNAL(NameChanged(QString)), this, SLOT(setText(QString)));

	setContextMenuPolicy(Qt::ActionsContextMenu);

// 	setTextInteractionFlags(Qt::TextSelectableByMouse);

	// highlighting the labels:
	connect(m_pParam, SIGNAL(MouseEnteredParamLabel()), this, SLOT(StartHighlight()));
	connect(m_pParam, SIGNAL(MouseExitedParamLabel()), this, SLOT(StopHighlight()));

	m_InititalPalette = palette();

#ifdef QT_DEBUG
// 	setToolTip(m_pParam->UniqueSystemID());
	setStatusTip(m_pParam->UniqueSystemID());
// 	setWhatsThis(m_pParam->UniqueSystemID());
#endif
}

GParamLabel::~GParamLabel()
{

}

void GParamLabel::mousePressEvent(QMouseEvent *event)
{
	QLabel::mousePressEvent(event);
	Qt::KeyboardModifiers modif = event->modifiers();
	Qt::MouseButtons butts = event->buttons();
	// from qt "drag & drop" page
	if(butts != Qt::LeftButton || modif != Qt::NoModifier)
		return;

	m_DragStartPosition = event->pos();
}

void GParamLabel::mouseMoveEvent(QMouseEvent *event)
{
	QLabel::mouseMoveEvent(event);
	Qt::KeyboardModifiers modif = event->modifiers();
	Qt::MouseButtons butts = event->buttons();
	// from qt "drag & drop" page
	if(butts != Qt::LeftButton || modif != Qt::NoModifier)
		return;
	if((event->pos() - m_DragStartPosition).manhattanLength() < QApplication::startDragDistance())
		return;
	if(!m_pParam)
		return;

	QDrag* drag = new QDrag(this);

	// extrapolation from qt help: "Using Drag and Drop with Item Views"
	QMimeData* mimeData = new QMimeData();
	QByteArray encodedData;
	// we store the UniqueSystemID() in the mime's "LabExe/parameter.single"
	QDataStream streamParamName(&encodedData, QIODevice::WriteOnly);
	QString paramUniqueID = m_pParam->UniqueSystemID();
	// lets check that it is in the Param manager so that it can be used by whoever will read this ID
	if(!GetParamFromParamManager(paramUniqueID)) {
		qDebug() << paramUniqueID << " is not in the Param Manager";
	}
	if(paramUniqueID != "") {
		streamParamName << paramUniqueID;
	}
	mimeData->setData("LabExe/parameter.single", encodedData);
	drag->setMimeData(mimeData);
//	Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}

void GParamLabel::mouseDoubleClickEvent( QMouseEvent *event )
{
    Q_UNUSED(event);
    if(!m_pParam)
		return;
	// opens the setting widget of the param
	m_pParam->PopupSettingWidget();
}

void GParamLabel::StartHighlight( int msDuration /*= 0*/ )
{
    Q_UNUSED(msDuration);
    setAutoFillBackground(true);
	QPalette HighlightPalette(palette());
	HighlightPalette.setColor(QPalette::Base, Qt::magenta);
	HighlightPalette.setColor(QPalette::AlternateBase, Qt::magenta);
	HighlightPalette.setColor(QPalette::Window, Qt::magenta);
	setPalette(HighlightPalette);
}

void GParamLabel::StopHighlight()
{
// 	setAutoFillBackground(false);
// 	m_InititalPalette.setColor(QPalette::Base, Qt::transparent);
// 	m_InititalPalette.setColor(QPalette::AlternateBase, Qt::transparent);
	m_InititalPalette.setColor(QPalette::Window, Qt::transparent);
	setPalette(m_InititalPalette);
}

void GParamLabel::enterEvent( QEvent* event )
{
	QLabel::enterEvent(event);
	if(!m_pParam)
		return;
	emit m_pParam->MouseEnteredParamLabel();
}

void GParamLabel::leaveEvent( QEvent* event )
{
	QLabel::leaveEvent(event);
	if(!m_pParam)
		return;
	emit m_pParam->MouseExitedParamLabel();
}

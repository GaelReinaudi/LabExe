#include "GFileGraphicsItem.h"
#include "GFileAgent.h"
#include <QFont>
#include <QFontMetrics>

GFileGraphicsItem::GFileGraphicsItem(GFileAgent* pAgent, QGraphicsItem *parentItem /*= 0*/)
    : GAgentGraphicsItem(pAgent, parentItem)
    , m_FileNameItem(this)
{
	//! get informed of the changes to the file info
	connect(pAgent, SIGNAL(FileInfoChanged(QFileInfo)), this, SLOT(Event_FileInfoUpdated(QFileInfo)));
	Event_FileInfoUpdated(pAgent->Info());

	setFlag(QGraphicsItem::ItemIsMovable);
	setAcceptHoverEvents(true);
	SetRect(TypicalScreen);

// 	QFont fileNameFont;
// 	QString fileNameStr(pAgent->Info().fileName());
// 
// 	QFontMetrics fm(fileNameFont);
// 	int fileNameWidth=fm.width(fileNameStr);
// 	int currentFontSize = fileNameFont.pointSize();
// 	int correctFontSize = currentFontSize*boundingRect().width()/fileNameWidth;
// 	fileNameFont.setPointSize(correctFontSize);
// 	m_FileNameItem.setPlainText(fileNameStr);
// 	m_FileNameItem.setFont(fileNameFont);
}

GFileGraphicsItem::~GFileGraphicsItem()
{
}

void GFileGraphicsItem::Event_FileInfoUpdated( QFileInfo fileInfo )
{
	QString extFile = fileInfo.suffix().toLower();
	if(extFile == "jpg" || extFile == "png") {
		QPixmap pixImage(fileInfo.filePath());
		pixImage = pixImage.scaled(TypicalScreen.toSize(), Qt::KeepAspectRatio);
		new QGraphicsPixmapItem(pixImage, this);
		qDebug() << fileInfo.filePath();
	}
}

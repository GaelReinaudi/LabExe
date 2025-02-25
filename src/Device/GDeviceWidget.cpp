#include "GDeviceWidget.h"
#include "GDevice.h"
#include "GDeviceMimeData.h"

GDeviceWidget::GDeviceWidget(GDevice* pOfWhichDevice, QWidget *parent)
	: QFrame(parent)
	, m_pDevice(pOfWhichDevice)
{
	setFrameStyle(QFrame::Panel | QFrame::Raised);
	setContentsMargins(1, 1, 1, 1);

	setAcceptDrops(true);

	if(!m_pDevice)
		return;

	// make the GDeviceWidget emit a signal customContextMenuRequested when the context menu is requested.
	// you can plug this signal to the GDevice itself.
	setContextMenuPolicy(Qt::CustomContextMenu);
//	setContextMenuPolicy(Qt::ActionsContextMenu);

	// we connect the following so that the GDevice can provide a context menu

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), m_pDevice, SLOT(ProvideContextMenu(const QPoint&)));

	connect(m_pDevice, SIGNAL(IsAblenessMayHaveChanged(bool)), this, SLOT(Enable(bool)), Qt::QueuedConnection);
	// TODO: putback this when IsAble will be pure virtual (ie reimplemented in each GDevice).	setEnabled(m_pDevice->IsAble());

	// color
	setAutoFillBackground(true);
	// if no color for the device yet, we make one randomly
	QColor theBkgColor = m_pDevice->m_Color;
	theBkgColor.setAlpha(100);
	QPalette palette;
	palette.setColor(backgroundRole(), theBkgColor);
	setPalette(palette);
}

GDeviceWidget::~GDeviceWidget()
{

}

QGroupBox* GDeviceWidget::AddSubGroupBox( QString Named, QLayout* groupLayout /*= 0*/ )
{
	QGroupBox* pGroup = new QGroupBox(Named, this);
	layout()->addWidget(pGroup);
	if(groupLayout)
		pGroup->setLayout(groupLayout);
	return pGroup;
}

void GDeviceWidget::AddSubLayout( QLayout* groupLayout )
{
	if(groupLayout && qobject_cast<QBoxLayout*>(layout()))
		qobject_cast<QBoxLayout*>(layout())->addLayout(groupLayout);
}

void GDeviceWidget::mousePressEvent( QMouseEvent * event )
{
	QFrame::mousePressEvent(event);
    // Qt::KeyboardModifiers modif = event->modifiers();
	if(event->button() != Qt::LeftButton)
		return;
	m_WidgetPosWhenMousePressed = pos();
    m_MousePressedGlobalPos = event->globalPosition().toPoint();
	m_MousePressedInWidgetPos = event->pos();
	m_MousePressedInParentPos = mapToParent(m_MousePressedInWidgetPos);
}

void GDeviceWidget::mouseReleaseEvent( QMouseEvent * event )
{
	QFrame::mouseReleaseEvent(event);

	if(event->button() != Qt::LeftButton)
		return;
}

void GDeviceWidget::mouseMoveEvent( QMouseEvent *event )
{
	QFrame::mouseMoveEvent(event);
	// on a move with the left button clicked, we make a drag of the screenshot of this widget
	if(event->buttons() & Qt::LeftButton) {
		int distance = (event->pos() - m_MousePressedInWidgetPos).manhattanLength();
		if(distance >= QApplication::startDragDistance()) {
			// take the screenshot
			QPixmap pixScreen = screenShot();
			// from drag-and-drop example delayed encoding
			QDrag* drag = new QDrag(this);
			drag->setPixmap(pixScreen);
			drag->setHotSpot(m_MousePressedInWidgetPos);

			QMimeData* mimeData = new GDeviceMimeData();
			mimeData->setImageData(pixScreen);

// 			// connection to get more data only when the QMimeData actually needs them
// 			connect(mimeData, SIGNAL(dataRequested(QString)), this, SLOT(createData(QString)), Qt::DirectConnection);

			// data in bytearray
			QByteArray data;
			QBuffer buffer(&data);
			buffer.open(QIODevice::WriteOnly);
			pixScreen.save(&buffer, "PNG");
			buffer.close();
			mimeData->setData("image/png", data);

			// test for file url
			QList<QUrl> listFile;
			pixScreen.save("temp.png", "PNG");
			listFile.append(QString("file:///") + QFileInfo("temp.png").absoluteFilePath());
			mimeData->setUrls(listFile);

			drag->setMimeData(mimeData);
			drag->exec(Qt::CopyAction);
		}
	}
}

void GDeviceWidget::DontDisplayTitle()
{
// 	setTitle("");
// 	if(m_pDevice)
// 		disconnect(m_pDevice, SIGNAL(NameChanged(QString)), this, SLOT(Rename(QString)));
}

void GDeviceWidget::Rename( QString )
{
// 	setTitle(newName);	
// 	adjustSize();
}

void GDeviceWidget::Enable( bool doEnable )
{
	// get the children widgets of the QgroupBox
	QList<QWidget*> listChildrenWidget;
	foreach(QObject* pObj, children()) {
		QWidget* pWid = qobject_cast<QWidget*>(pObj);
		if(pWid)
			listChildrenWidget.append(pWid);
	}
	foreach(QWidget* pWid, listChildrenWidget) {
		pWid->setEnabled(doEnable);
	}
}

QPixmap GDeviceWidget::screenShot()
{
    QPixmap pixWid = grab();
	return pixWid;
}

void GDeviceWidget::dragEnterEvent( QDragEnterEvent *event )
{
	QFrame::dragEnterEvent(event);
	if(m_pDevice)
		m_pDevice->DragEnterEvent(event, this);
}

void GDeviceWidget::dropEvent( QDropEvent *event )
{
	QFrame::dropEvent(event);
	if(m_pDevice)
		m_pDevice->DropEvent(event, this);
}


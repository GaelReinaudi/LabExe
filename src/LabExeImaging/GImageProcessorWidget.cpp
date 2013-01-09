#include "GImageProcessorWidget.h"
#include "GImageProcessor.h"
#include "GAoiSelector.h"

GImageProcessorWidget::GImageProcessorWidget(GImageProcessor* pImageProcessor, QWidget *parent)
	: GDeviceWidget(pImageProcessor, parent)
{
	setupUi(this);
	if(!qobject_cast<GImageProcessor*>(pImageProcessor)) {
		qWarning() << "the GImageProcessor* pImageProcessor is not supposed to be null.    168137";
		return;
	}
	m_ShowCheckBox->setChecked(pImageProcessor->m_pAoiItem->isVisible());

	delete m_EnableCheckBox;
	delete m_ShowCheckBox;
	pTopLayout->insertWidget(0, pImageProcessor->m_DoDisplayAoi.ProvideNewParamCheckBox(this));
	pTopLayout->insertWidget(0, pImageProcessor->m_DoProcessImages.ProvideNewParamCheckBox(this));

	connect(m_pSelectAoiButton, SIGNAL(clicked()), pImageProcessor->m_pAoiSelector, SLOT(open()));

	connect(pImageProcessor, SIGNAL(NumImagesInputChanged(int)), m_pReceivedBox, SLOT(setValue(int)), Qt::QueuedConnection);
	connect(pImageProcessor, SIGNAL(NumImagesProcessedChanged(int)), m_pProcessedBox, SLOT(setValue(int)), Qt::QueuedConnection);
}

GImageProcessorWidget::~GImageProcessorWidget()
{

}

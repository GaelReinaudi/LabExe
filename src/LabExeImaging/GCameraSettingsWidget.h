#ifndef GCAMERASETTINGSWIDGET_H
#define GCAMERASETTINGSWIDGET_H
#include "labexeimaging_global.h"

#include "Device/GDeviceWidget.h"
namespace Ui {class GCameraSettingsWidgetClass;};

class GCamera;
class GParamLabel;

/////////////////////////////////////////////////////////////////////
//! \brief A GCameraSettingsWidget defines a widget to interact with the settings of a camera.
/*!

*/
class LABEXEIMAGING_EXPORT GCameraSettingsWidget : public GDeviceWidget
{
	Q_OBJECT

public:
	GCameraSettingsWidget(GCamera* pCamera, QWidget *parent = 0);
	~GCameraSettingsWidget();

	//! Returns the widget that is reserved for holding settings widgets specific to a given camera
	QWidget* CameraSpecificGroupBox();

public slots:
	//! this should be called to updates the display when the camera is initialized or when a setting changed some other parameter range (e.g. pixel clock can change the exposure range).
	void UpdateCapabilityDisplay();

protected:

private:
	GCamera* m_pCamera;
	Ui::GCameraSettingsWidgetClass *ui;
	QCheckBox* m_pTriggerCheckBox;
	QLabel* m_pTriggerLabel;
};

#endif // GCAMERASETTINGSWIDGET_H

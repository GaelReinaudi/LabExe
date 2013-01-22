/*! \file GDevice.h
* \brief Devices in LabExe
*
*/

#ifndef GDEVICE_H
#define GDEVICE_H
#include "labexe_global.h"

#include "ToolBox/GSerializable.h"

#include <QtWidgets>
#include <QObject>
#include <QList>

class GDeviceWidget;
class GParam;
class GDevice;
class GDeviceManager;

//! Returns the device manager
LABEXE_EXPORT GDeviceManager* DeviceManagerInstance(); 
//! Loads the plugins in the Agent manager
LABEXE_EXPORT void LoadPluginsInDeviceManager(); 
//! Returns the device whose uniqueIdentifierName matches in the device manager, 0 otherwise.
LABEXE_EXPORT GDevice* GetDeviceFromDeviceManager(QString uniqueIdentifierName); 
//! Adds the device to the hard device shelf.
LABEXE_EXPORT void AddToDeviceShelf(GDevice* pDevice); 
//! Registers the device into the device manager.
LABEXE_EXPORT bool RegisterHardDeviceInManager(const QString & className, GDevice*(deviceCreator)(QString, QObject*));
//! Registers the prog device into the device manager.
LABEXE_EXPORT bool RegisterProgDeviceInManager(const QString & className, GDevice*(deviceCreator)(QString, QObject*)); 


/////////////////////////////////////////////////////////////////////
//! \brief The GDevice class is the base abstract class of any device that can be used and displayed by a GWorkBench.
/*!
A device class must inherit this class and implement at least the pure virtual functions:
- PopulateDeviceWidget(), which defines how to populate a widget that will represent the gui of the device in workbenches

Parameters that are instantiated as being child of this device will be automatically serialized and de-serialized. 
If a special behavior is wanted, one can re-implement those functions:
- PopulateSettings(), which is called by the workbenches when serializing the settings to disk
- InterpretSettings(), which is called by the workbenches when de-serializing the settings from disk

Note that the body of those functions can be made extremely simple if using parameters object of the type GParam, such as:
- GParamInt, for controlling and displaying an integer value
- GParamDouble, for controlling and displaying a double value
- GParamString, for controlling and displaying a string value

When creating a new GHardDevice, you have to provide a uniqueIdentifierName that will identify it without ambiguity in the system.
You need to use the macro G_REGISTER_HARD_DEVICE_CLASS(OsciloTek2024B). 
This allows the device factory to create dynamically your type of device while reading settings from a file.
Every derived class of GDevice should have the Q_OBJECT macro in it so that the serialization process can have 
use the metaObject()->className() function.

\see \ref G_REGISTER_HARD_DEVICE_CLASS
*/
class LABEXE_EXPORT GDevice : public QThread, public GSerializable
{
	Q_OBJECT

public:
	//! Constructor. You have to provide a unique name to identify the physical device in the program
	GDevice(QString uniqueIdentifierName, QObject *parent);
	virtual ~GDevice();

	//! \todo{make pure virtual.}
	//! Tells if the device is able to work (eg: is present and is properly controlled). false by default! ?
	virtual bool IsAble() const {return false;} 
	//! Returns the name that is useful for the user.
	QString Name() const { return objectName(); }
	//! Provides a list of the sub-devices managed by this one. May be empty.
	virtual QList<GDevice*> SubDevices() const {QList<GDevice*> alist; return alist;}
	//! Returns true if there are any subdevice managed by this one.
	bool HasSubDevices() const {return !SubDevices().isEmpty();}
	//! Returns a new GDeviceWidget (that actually inherits QGroupBox widget) that is associated with a QBoxLayout with the specified orientation. 
	virtual GDeviceWidget* ProvideNewDeviceGroupBox(QWidget* inWhichWidget, QBoxLayout::Direction orientation = QBoxLayout::LeftToRight);
	//! Provides extra settings in a new widget
    virtual QWidget* ProvideNewExtraSettingsWidget(QWidget* parentWidget) {Q_UNUSED(parentWidget); return 0;}
	//! returns a list of all the parameters that are child of this device
	QList<GParam*> ChildParams() const;
	//! Returns the name that is to be displayed on the shelf
	virtual QString ShelfName() const;
	//! Color of the device that can be used by the gui interfaces or other graphics.
	QColor Color() const { return m_Color; }

protected:
	//! Re-implemented from GSerializable. It automatically serializes the parameters that are children of this device.
	virtual void PopulateSettings(QSettings& inQsettings);
	//! Re-implemented from GSerializable. It automatically de-serializes the parameters that are children of this device. 
	virtual void InterpretSettings(QSettings& fromQsettings);

public slots:
	//! Changes the name of the device. It is the name that is useful for the user.
	void ReName(QString val);
	//! Overload. Pops-up a dialog to rename the device.
	void ReName();

protected slots:
	//! Displays a context menu that will be populated by the reimplemented PopulateContextMenu(). pos is the position of the context menu event that the widget received.  
	void ProvideContextMenu( const QPoint & pos ) const;
	//! This virtual function is called only once, through a Qt::QueuedConnection from the constructor, in order to perform a latter initialization, once the program reaches the main event loop. The default implementation does nothing.
	virtual void LatterInitialization() {return;};

protected:
	//! Pure virtual function that has to be defined in order to populate the GDeviceWidget representing the device in the GUI.
	virtual void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget) = 0;
	//! Populates the context menu that was provided by ProvideContextMenu().
	virtual void PopulateContextMenu(QMenu* pTheMenu, GDeviceWidget* pSenderDeviceWidget = 0) const;
	//! If reimplemented by derived class, this is meant to provide a shorter default name for the device
	virtual QString DefaultShortName() const { return Name(); }
	//! Re-implemented to add the new ID in the DeviceManagerInstance(). It doesn't remove the previous one.
	void Event_UniqueSystemIDChanged(const QString & newSystemID);
	//! Function Called when a QWidget::dragEnterEvent() happens on a GDeviceWidget of this device.
	virtual void DragEnterEvent(QDragEnterEvent *event, GDeviceWidget* pOnWidget); 
	//! Function Called when a QWidget::dropEvent() happens on a GDeviceWidget of this device.
	virtual void DropEvent(QDropEvent *event, GDeviceWidget* pOnWidget);


signals:
	//! Emitted when the device Name() changed.
	void NameChanged(QString newname);
	//! Emitted when the IsAble'ness of the device may have changed. It should be emitted in user code eg. in a Attach() or Dettach() event handler.
	void IsAblenessMayHaveChanged(bool actualIsAbleness);
	//! Private! Emitted only once, in the constructor, in order to trigger LatterInitialization().
	void PrivateTriggerLatterInitialization();
	//! Emitted when the parameters have been read from file
	void ParamJustInterpretSettings();
	
private:
	//! adds this device in the device manager. If it was already added with another name, the old name is not removed. Both names will give this device when calling GDeviceManager::GetDevice();
	void AddToDeviceManager();
	QWidget m_DummyWidgetToParentWidgets;
	//! default color for the gui for this device background.
	QColor m_Color;

	friend class GDeviceWidget;
};

Q_DECLARE_METATYPE(GDevice*)


/*! \def G_REGISTER_HARD_DEVICE_CLASS(ClassName)
\brief This is the Macro that makes your derived class available to the device factory. 
You must put it in your derived class cpp file. If your device class is the main class introduced by a plugin, 
you should use the G_REGISTER_PLUGIN_PROG_DEVICE instead (in the plugin .h file).

It is this that allows you to de-serialize derived device using the name of the corresponding class. 
Take a look here: 
- "Modern C++ Design Generic Programming and Design Patterns Applied", Chapter 8: Object Factories.
*/
#define G_REGISTER_HARD_DEVICE_CLASS(ClassName)														\
	namespace																						\
{ 																									\
	int	nDevCreated = 0;																			\
	/*! Definition of a function that returns a new instance of a device !*/						\
	GDevice* CreateDevice(QString uniqueIdentifierName, QObject *parent)							\
	{																								\
		nDevCreated++;																				\
		GDevice* pNDev = 0;																			\
		pNDev =  new ClassName(uniqueIdentifierName, parent);										\
		return pNDev;																				\
	}																								\
	const bool regDe = RegisterHardDeviceInManager( # ClassName , CreateDevice);					\
}

/*! \def G_REGISTER_PROG_DEVICE_CLASS(ClassName)
\brief This is the Macro that makes your derived class from GProgDevice available to the device factory. 
You must put it in your derived class cpp file.If your device is defined in a plugin, 
you should use the G_REGISTER_PLUGIN_PROG_DEVICE instead (in the .h file).

This macro is like G_REGISTER_HARD_DEVICE_CLASS, but also register the GProgDevice using DeviceManagerInstance()->RegisterProgDevice(# ClassName);

The difference of those to macro is also justified by the fact that a GProgDevice doesn't take a QString uniqueID argument in the constructor.
*/
#define G_REGISTER_PROG_DEVICE_CLASS(ClassName)														\
	namespace																						\
{ 																									\
	int	nProgDevCreated = 0;																		\
	/*! Definition of a function that returns a new instance of a device !*/						\
	GDevice* CreateDevice(QString uniqueIdentifierName, QObject *parent)							\
	{																								\
		nProgDevCreated++;																			\
		GDevice* pNDev = 0;																			\
		pNDev = new ClassName(parent, uniqueIdentifierName);										\
		QString strName(pNDev->metaObject()->className());											\
		strName += QString("-%1").arg(nProgDevCreated);												\
		pNDev->ReName(strName);																		\
		return pNDev;																				\
	}																								\
	const bool regPrDe = RegisterProgDeviceInManager( # ClassName , CreateDevice);					\
}







class GDummyDevice: public GDevice 
{
public:
	GDummyDevice(QString uniqueIdentifierName, QObject *parent)	: GDevice(uniqueIdentifierName, parent) {}
	~GDummyDevice() {}

protected:
    void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget) {Q_UNUSED(theDeviceWidget);}

};

#endif // GDEVICE_H

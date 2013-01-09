/*! \file Tutorial-WS6-Implementation.h
* \brief A tutorial next
*
*/
/*!

\page NewDeviceImplementation Tutorial : Implementing the user device WS6

In this page we assume that you have setup the new project HighFinesseLabExe that will 
hold the classes necessary to control the wavelength-meter WS6. 

\section TocImplementNewDevice Table of content
- \ref ImplementGDeviceWS6
	- \ref WS6Constructor
	- \ref GDeviceUniqueID
- \ref WS6ImplementVirtualFunctions
	- \ref WS6PopulateWidget
	- \ref NoteAboutGParamDouble
	- \ref WS6ComplexDeviceWidget
- \ref WS6Serialization
	- \ref WS6DeviceSerialze
	- \ref WS6DeviceFactory


We are going to implement the functionality in the WS6. 
- Some are required by the GDevice abstract class. The c++ compiler won't let you 
compile if you don't implement the pure virtual functions of GDevice.
- Some will be meant to actually use the wavelength meter

\section ImplementGDeviceWS6 Re-implementing the GDevice functions
\subsection WS6Constructor The constructor
The GDevice class has one constructor with the following signature:
\code
//! Constructor. You have to provide a unique name to identify the physical device in the program
GDevice(QString uniqueIdentifierName, QObject *parent);
\endcode
As a consequence (and this is just plain c++ rule) you should initialize the GDevice 
in the constructor of the GWavelengthMeterWS6 by changing the following in the GWavelengthMeterWS6.cpp file:
\code
GWavelengthMeterWS6::GWavelengthMeterWS6(QObject *parent)
	: GDevice(parent)
{...}
\endcode
to the following (don't forget to also update the signature of the constructor in the GWavelengthMeterWS6.h file):
\code
GWavelengthMeterWS6::GWavelengthMeterWS6(QString uniqueIdentifierName, QObject *parent)
	: GDevice(uniqueIdentifierName, parent)
{...}
\endcode


\subsection GDeviceUniqueID About the unique ID of a device
\warning 
The GDevice class constructor takes a parameter QString uniqueIdentifierName. 
It will be your responsibility at construction time to provide a unique QString that will 
be used by the program to identify your device in a unique way. 

The best is usually to use some device-API functions to query a unique identifier specific to the device and that will never change.
For example, when creating the GPhidgetModule objects, the GPhidgetManager uses:
\code
CPhidget_getDeviceType(phidHandle, &deviceType); // phidget-API function to get the serial of the phidget type.
CPhidget_getSerialNumber(phidHandle, &serialNo); // phidget-API function to get the serial of the phidget. It is guaranteed to be unique.
....
QString thePhidgetUniqueID("%1%2%3");
thePhidString.arg(deviceType).arg(":").arg(serialNo); // assembling the strings
\endcode
in order to produce a unique name of the form "<type of the device>:<serial number>", e.g. : "PhidgetAdvancedServo:9926"

\note
Why is this useful? Because if you plug many devices to your computer, let's say many phidget modules, 
you don't want to give a new unique name to each one of them, and to remember them and which device has which name. 
Your code should rather (like described above) run a deterministic routine to name each device the same way it was named 
the last time you run your program, and the same way it will ever do it.



\section WS6ImplementVirtualFunctions Implementing the GDevice virtual functions
\subsection WS6PopulateWidget Implementing the GDevice::PopulateDeviceWidget()
You must implement the function GDevice::PopulateDeviceWidget(). This function will be called by the 
GUI when it needs to display the device functionality. 
- In the WS6 class declaration (GWavelengthMeterWS6.h), declare the function:
\code
protected:
	void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget);
\endcode
- In the GWavelengthMeterWS6.cpp, implement it. The argument theDeviceWidget is a pointer 
to a GDeviceWidget (inherits QWidget). You have to setup in the device widget whatever 
widget you will need to control your device, and to organize them in a layout. 
For example it could look like:
\code
void GWavelengthMeterWS6::PopulateDeviceWidget(GDeviceWidget* theDeviceWidget ) {
	// add a layout to the device widget
	QBoxLayout* pLay = new QHBoxLayout();
	theDeviceWidget->AddSubLayout(pLay);
	// add a spin box to the layout
	QDoubleSpinBox* pSpinBox = new QDoubleSpinBox(theDeviceWidget);
	pLay->addWidget(pSpinBox);
}
\endcode
However, doing so, you also would have to connect those control to the functionnality of the WS6 
by adding something looking like this:
\code
	// connection from gui to WS6
	connect(pSpinBox, SIGNAL(valueChanged(double)), this, SLOT(WhateverFunctionOfWS6(double)));
	// conenction from WS6 to gui
	conenct(this, SIGNAL(WhateverSignalInWS6(...)), someDisplay, SLOT(.......))
\endcode

\subsection NoteAboutGParamDouble Note about a better approach to connect the widget to the class
In the above example, a QSpin box is put in a layout in the widget. Then the connection with the 
rest of the program has to be made.
There exists a very convenient class in the LabExe framework: GParam. That class is made 
to interact appropriately with several part of the framework:
- drag and drop,
- having a name and label attached to it
- updating display widgets through signal and slot mechanism
- saving and restoring its own values through the serialization mechanism of the LabExe
- access to various tools programmed by users.

A good approach in this case is to use in the WS6 class a member:
\code
private:
	GParamDouble m_LastMeasure;
\endcode
You need to call the constructor of the GParamDouble within the constructor of GWavelengthMeterWS6:
\code
GWavelengthMeterWS6::GWavelengthMeterWS6(QString uniqueIdentifierName, QObject *parent)
	: GDevice(uniqueIdentifierName, parent)
	, m_LastMeasure("Channel 1", this)
{...}
\endcode
Please check the GParamDouble documentation to now more about its constructor.

Then, in the PopulateDeviceWidget function, you can call convenience functions from the GParamDouble class.
\code
void GWavelengthMeterWS6::PopulateDeviceWidget(GDeviceWidget* theDeviceWidget )
{
	QBoxLayout* pLay = new QHBoxLayout();
	theDeviceWidget->AddSubLayout(pLay);

	// the spin box is obtained through the GParamDouble and they are already connected together
	QDoubleSpinBox* pSpinBox = m_LastMeasure.ProvideNewParamSpinBox(theDeviceWidget);
	QLabel* nameLabel = m_LastMeasure.ProvideNewLabel(theDeviceWidget);
	pLay->addWidget(nameLabel);
	pLay->addWidget(pSpinBox);
}
\endcode

\subsection WS6ComplexDeviceWidget Another aproach for complex device widgets
This is an aproach to consider when the widget you are going to populate has to be complex,
meaning with a lot of controls, and a non trivial layout, like the widget for the GCamera device:
\image html NewDeviceImplement-01.PNG
In that case, it might be clearer to:
- make a new GUI class using the Qt GUI editor (here GCameraSettingsWidget class), 
with optionally some empty layout already well positioned in the widget
- re-implement the virtual function GDevice::ProvideNewDeviceGroupBox() to create and return a new instance of that class,
- implement the GDevice::PopulateDeviceWidget() function to populate the empty layouts of the GUI class

Here is the example for the GCamera class:
- the Qt GUI editor of the GCameraSettingsWidget class:
\image html NewDeviceImplement-02.PNG
where the red rectangles are empty layouts ready to be populated later with the appropriate QSpinBox
- the ProvideNewDeviceGroupBox function re-implemented:
\code
GDeviceWidget* GCamera::ProvideNewDeviceGroupBox( QWidget* inWhichWidget, QBoxLayout::Direction orientation) 
{
	GCameraSettingsWidget* pWidToReturn = new GCameraSettingsWidget(this, inWhichWidget);
	PopulateDeviceWidget(pWidToReturn);
	return pWidToReturn;
}
\endcode
where you notice that the PopulateDeviceWidget() function is being called
- the PopulateDeviceWidget() function would contain:
\code
	// sets the exposure spin box in the appropriate layout already in the ui: ui->pExposureLayout
	GDoubleSpinBox* pExpoBox = m_pCamera->m_Expo_ms.ProvideNewParamSpinBox(this);
	pWidCamera>ui->pExposureLayout->setWidget(0, QFormLayout::FieldRole, pExpoBox);
	// sets the gain spin box in the appropriate layout already in the ui: ui->pGainLayout
	GDoubleSpinBox* pGainBox = m_pCamera->m_Gain.ProvideNewParamSpinBox(this);
	pWidCamera->ui->pGainLayout->setWidget(0, QFormLayout::FieldRole, pGainBox);
	etc....
\endcode

This way, you separate the complexity of the layout from the actual parameters and their functionality.






\section WS6Serialization Making the device serializable, i.e. save its settings to file

\subsection WS6DeviceSerialze Making the device serializable

One thing you will want to have in your device, is the ability to save its parameter to 
a file when the program shuts down, so that it can read that file and restore itself 
when the program starts again. 

This is usually a difficult task to save a hierarchy of objects. You could read this 
great FAQ to know more: http://www.parashift.com/c++-faq-lite/serialization.html

However, Qt provides a very convenient way of saving data, which removes most of the difficulty. 
The LabExe framework makes use of this in order to implement its own way of saving:
- workbenches data,
- devices data and placement in the workbenches,
- parameters data,
- hierarchy of devices and ownership of parameters (e.g. a device might need a parameter choosed by the user 
at run time, like in an optimizer or a PID loop).

In order to save your specific device settings, you must re-implement the PopulateSettings() and InterpretSettings() methods.


\subsection WS6DeviceFactory Making the device de-serializable by the device factory
Another very important functionality of the LabExe framework is that GDevices can be instanciated 
by the program itself. What does it mean? Let's imagine a user creates a new instance of a device 
at runtime:
- let's say he makes a new PidLoop device for controlling a parameter of the 
program. 
- When closing the program, the device will be serialized and there will be written somewhere that this device has to be re-created during de-serialization. 
- When the program starts again, the framework will have to instanciate the given device by itself and restore its state.

This is a task that uses what is known as the "Factory Pattern": A class is in charge of knowing 
what object can be made by the program, and how to do it. To know more about patterns, google search "design pattern".

This class in the Framework is the GDeviceManager class. FYI, this class is also a singleton, google search "singleton pattern".

To make a device available to the device manager, the macro G_REGISTER_HARD_DEVICE_CLASS has to be inserted in the cpp file of the class:
\code
#include "GWavelengthMeterWS6.h"
G_REGISTER_HARD_DEVICE_CLASS(GWavelengthMeterWS6)
\endcode













*/
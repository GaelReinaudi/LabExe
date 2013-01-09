/*! \file Tutorial-WS6-Subdevices.h
* \brief A tutorial next
*
*/
/*!

\page NewDeviceSubdevices Tutorial : Making sub-devices to control complex/modular devices

In this page we assume that you have:
- setup the new project HighFinesseLabExe that will hold the classes necessary to control the wavelength-meter WS6 (\ref NewDeviceInSolution),
- followed the procedure to implement the functionality in the WS6 (\ref NewDeviceImplementation). 

\section TocSubdevicesNewDevice Table of content
- \ref SubDeviceIdea
	- \ref SubDeviceExample
	- \ref SubDeviceDefHierarchy
- \ref SubDeviceStructure
	- \ref SubDeviceStructuralBehavior
	- \ref CreateNewClassWS6Channel
- \ref ImplementGDeviceWS6Channel
	- \ref ImplementGDeviceWS6ChannelConstructor
	- \ref ImplementGDeviceWS6ChannelWidget
	- \ref ImplementGDeviceWS6Communication
- \ref ImplementGDeviceWS6AsParent
	- \ref ImplementGDeviceWS6AsParentConstructor
	- \ref ImplementGDeviceWS6AsParentShelf
	- \ref ImplementGDeviceWS6UpdatingChannels


We are going to describe the recommended procedure to make a device have sub-devices. What is a sub-device? 
A sub-device would be a device that may appear to the user as being a part of a device, being contained in it and dependent 
of it. 

\section SubDeviceIdea The idea of a sub-device

\subsection SubDeviceExample An example

The wavelength-meter WS6 is the actual physical device that is plugged to the computer. 
It is thus normal that a class represents (encapsulate) this entity in the program.
One of the functionality of the WS6 is to measure and return the wavelength of any of the 8 fibers plugged to it.
even though there are no clear separation of those 8 channel in the WS6 API, it would make sens to have 
an additional class in the project in order to represent the idea of a channel in the WS6. Indeed:


\subsection SubDeviceDefHierarchy defining the hierarchy

A more or less delicate task is to define what will be the hierarchy in the program, i.e. what 
will be defined as being a sub-device, what task it will be capable of, and what will remain in the parent "device".
In the case where the device API is already object oriented, well compartmented, you should probably try to 
keep the provided hierarchy. This is then rather easy.

In the case of the WS6, the C API (not object oriented) doesn't reveal any obvious code 
compartmentalization. It is up to the user (programmer).

The WS6 physical device is defined by the class GWavelengthMeterWS6 (see the other tutorials). 
We are now defining the GWS6Channel class that will be coded to encapsulate functionalities 
specific to the channel.


\section SubDeviceStructure Setup the structure

\subsection SubDeviceStructuralBehavior Structural behavior

Here is what should happen when the program starts:
- In SrYbExe.cpp, the GWavelengthMeterWS6 only instance gets created (see \ref WS6UseInSrYbExe).
- In the GWavelengthMeterWS6 constructor, each of the 8 channel objects have to be created and given a unique name,
- for this, since the GWavelengthMeterWS6 object should have a Unique name already, the obvious would be to use that name appended with a number from 1 to 8.

Communication between the WS6 class and the channel class:
- The channel class should contain the parameter (GParamDouble) that represents the measured wavelength,
- but it is probably the WS6 class that should actually use the API function that measure all the wavelengths.
- So, a separate thread in GWavelengthMeterWS6 check for the wavelengths and calls a method of the appropriate channel object.

\subsection CreateNewClassWS6Channel Create a class to represent a WS6 channel

Just like in \ref CreateNewClassWS6, add a class to the HighFinesseLabExe project by right-clicking the project:
- go to Add then Class
- choose a QtClass and call it "GWS6Channel"
- set the Base class as GDevice. 
- un-check the lower case file names
- leave insert Q_OBJECT checked

The wizard then creates the GWS6Channel.h file. You have to change the line:
\code
#include <GDevice>
\endcode
to the following:
\code
#include "device.h"
\endcode

\section ImplementGDeviceWS6Channel Implementing the GWS6Channel class

\subsection ImplementGDeviceWS6ChannelConstructor The constructor

Just like in \ref ImplementGDeviceWS6, change the following in the GWS6Channel.cpp file:
\code
GWS6Channel::GWS6Channel(QObject *parent)
	: GDevice(parent)
{...}
\endcode
to the following (don't forget to also update the signature of the constructor in the GWS6Channel.h file):
\code
GWS6Channel::GWS6Channel(QString uniqueIdentifierName, QObject *parentWS6)
	: GDevice(uniqueIdentifierName, parentWS6)
{...}
\endcode
and don't forget to:
\code
#include "GWavelengthMeterWS6.h"
\endcode

\warning 
It will be your responsibility at construction time to provide a unique QString that will 
be used by the program to identify your device in a unique way. see \ref ImplementGDeviceWS6AsParent.

\subsection ImplementGDeviceWS6ChannelWidget Implementing the PopulateDeviceWidget()

Just like in \ref WS6ImplementVirtualFunctions, you must implement the function GDevice::PopulateDeviceWidget().
- In the channel class declaration (GWS6Channel.h), declare the function:
\code
protected:
	void PopulateDeviceWidget(GDeviceWidget* theDeviceWidget);
\endcode
- In the GWS6Channel.cpp, implement it. It could look like this:
\code
void GWS6Channel::PopulateDeviceWidget(GDeviceWidget* theDeviceWidget )
{
	QBoxLayout* pLay = new QHBoxLayout();
	theDeviceWidget->AddSubLayout(pLay);
	QDoubleSpinBox* pSpinBox = m_LastMeasure.ProvideNewParamSpinBox(theDeviceWidget);
	QLabel* nameLabel = m_LastMeasure.ProvideNewLabel(theDeviceWidget);
	pLay->addWidget(nameLabel);
	pLay->addWidget(pSpinBox);
	QCheckBox* pEnableMonitorBox = m_DoMonitor.ProvideNewParamCheckBox(theDeviceWidget, "Monitor");
	pLay->addWidget(pEnableMonitorBox);
}
\endcode

where m_LastMeasure and m_DoMonitor are members of the GWS6Channel class, and are respectively :
- holding the last measured value of the wavelength,
- specifying if the m_LastMeasure has to be updated when the WS6 API detects a change.
\code
private:
	....
	GParamDouble m_LastMeasure;
	GParamBool m_DoMonitor;
\endcode


\subsection ImplementGDeviceWS6Communication Communication from the parent GWavelengthMeterWS6

In order to update the wavelength parameter in the channel object you must declare a 
function that will be called by the parent GWavelengthMeterWS6. In the GWS6Channel class declaration:
\code
public:
	void IncomingUpdate(double newWaveLength);
\endcode
and the implementation could be something like:
\code
void GWS6Channel::IncomingUpdate( double newWaveLength )
{
	if(m_DoMonitor)
		m_LastMeasure = newWaveLength;
}
\endcode






\section ImplementGDeviceWS6AsParent Implementing the GWavelengthMeterWS6 class

\subsection ImplementGDeviceWS6AsParentConstructor Create the GWS6Channel objects in the constructor

in the constructor, create the channel objects. In order to clarify the code, it would be a good idea to define 
a function even if you now that it will be called only once. This function has to query the number of channels, using the WS6 API function:
\code
void GWavelengthMeterWS6::CreateSubDevicesChannels()
{
	// get the number of channels in the WS6
	int numberChan = GetChannelsCount(0);
	for(int nCh = 1; nCh <= numberChan; nCh++) {
		GWS6Channel* pChan = new GWS6Channel(UniqueIdentifierName() + QString("-ch") + QString::number(nCh), this);
		m_Channels.append(pChan);
	}
}
\endcode
where m_Channels is a member that will hold the ppointers to the 
channels objects created. Here is the declaration in the GWavelengthMeterWS6 class:
\code
private:
	....
	QList<GDevice*> m_Channels;
\endcode


\subsection ImplementGDeviceWS6AsParentShelf Put the sub-devices on the shelf

To show the channels objects on the lab shelf, you must re-implement the GDevice::SubDevices(). In the public section of the GWavelengthMeterWS6.h file:
\code
	//! Re-implemented
	QList<GDevice*> SubDevices() const {return m_Channels;}
\endcode
That should be it


\subsection ImplementGDeviceWS6UpdatingChannels Thread to update the channels

Measurement in the WS6 device is performed at undetermined times. In order to 
get updates at those time, there need to be a way to be informed when it happens. The API 
recommends to use callback functions, but some strange run time errors in the callback mechanism 
made us adopt the other recommended technique: continuously calling a function that check for changes. 
Since we don't want to take resource from the main program, we run that loop in a separate thread.

The GDevice class inherits the QThread class. This means that it is easy to run a separate 
thread by re-implementing the run() function. 
In the protected section of the GWavelengthMeterWS6 declaration:
\code
	void run();
\endcode
And the implementation:
\code
void GWavelengthMeterWS6::run()
{
	//change the priority of the thread to not take resources from the rest of the program
	setPriority(QThread::LowPriority);
	long whatChanged = 0;
	long intVal = 0;
	double dblVal = 0.0;
	// start looping
	m_ThreadWaitForEvents = true;
	while(m_ThreadWaitForEvents){ // keeps looping as long as m_ThreadWaitForEvents is true
		// get the first pending event in the WS6
		int ret = WaitForWLMEvent(whatChanged, intVal, dblVal);
		// in the case it is a channel with a wavelength changed we call the corresponding channel object update function.
		switch(whatChanged) {
		case cmiWavelength1:
			GWS6Channel* pChan = qobject_cast<GWS6Channel*>(m_Channels.at(0));
			if(pChan)
				pChan->IncomingUpdate(dblVal);
			break;
		}
	}
}
\endcode
where you can see the boolean m_ThreadWaitForEvents. It is declared in the private 
section of the GWavelengthMeterWS6 declaration:
\code
provate:
...
	bool m_ThreadWaitForEvents;
\endcode
Its role is to be turned off (false) by the main thread so that the WS6 updating loop would stop. 
For example, a function to turn on or off that loop could be called by the main thread, 
and would look like this:
\code
void GWavelengthMeterWS6::EnableWLMonitoring(bool doMonitor)
{
	if(doMonitor)
		start();
	else
		m_ThreadWaitForEvents = false;
}
\endcode

































*/
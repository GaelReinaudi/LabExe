/*! \file Tutorial-WS6-NewUserDeviceProject.h
* \brief A tutorial 
*
*/
/*!

\page NewDeviceInSolution Tutorial : Setting up a new project to control your own device from the LabExe

In this page we are going to go step by step through the process of making a new class in order to control 
your own hardware device. 

\section TocNewProject Table of content
- \ref AboutModularityNewDevice
- \ref CreateNewProjectHighFinesse
	- \ref CreateNewClassWS6
	- \ref FixClassWS6
	- \ref FixingLinkingLabExe
- \ref LinkSrYbExeToHighFinesse
	- \ref WS6DllExport
	- \ref WS6LinkInSrYbExe
	- \ref WS6LinkInSrYbExe







\note
The lab program we will consider here as an example is the SrYbExe program that was created for 
the Zelevinsky lab. SrYbExe is both the name of the derived class of GLabControlPanel, and the name of the visual studio solution 
that groups all the needed classes and libraries that we use for the SrYbExe program.

\note
LabExe is the name of the Framework, i.e. the group of classes writen to control an experiment. 

Also, the new device we will consider is the wavelength-meter WS6 from Highfinesse. 





\section AboutModularityNewDevice About modularity of LabExe
In order to keep the flexibility and modularity of the framework, the LabExe code (the framework code) 
has to never be aware of the existence of the WS6 device. Not doing so would make the framework useless 
for anyone not owning such a WS6 device. 
The SrYbExe program will (of course) be aware of the WS6 device.

For this, we are going to create a new library file that will make use of some base classes of LabExe. 
Then this library will be linked with the actual SrYbExe program. 






\section CreateNewProjectHighFinesse Create a new project for the device
right-click the SrYbExe solution and add a new project:
\image html NewDeviceProject-01.PNG

select a Qt Library and name the project "HighFinesseLabExe". Place the project folder in the \LabExe\UserDefinedDevice folder.
After clicking "OK", you click "Next", check the "GUI library" in the list of possible libraries, click "Finish".
\image html NewDeviceProject-02.PNG


\subsection CreateNewClassWS6 Create a class to control the wavelength meter WS6
Add a class to the HighFinesseLabExe project by right-clicking the project:
\image html NewDeviceProject-03.PNG
- go to Add then Class
- choose a QtClass and call it "GWavelengthMeterWS6"
- Set the Base class as GDevice. 
- un-check the lower case file names (this is just for respecting the naming convention of the LabExe)
- leave insert Q_OBJECT checked
\image html NewDeviceProject-04.PNG

The wizard then creates the GWavelengthMeterWS6.h file. You have to change the line:
\code
#include <GDevice>
\endcode
to the following in order to tell the compiler to include the correct file (by specifying GDevice in the wizard, it assumed it was a Qt base class, which would be accessed by using brackets):
\code
#include "device.h"
\endcode
and tell the compiler about the location of the LabExe framework source folder. For this, right-click on the project select "Properties" and go to "C/C++", "General" and add: 
\code
..\..\src;..\..\include;
\endcode
to the "Additional Include Directories":
\image html NewDeviceProject-05.PNG


\subsection FixClassWS6 Fix the WS6 class
If you try to compile, there should be the following error:
\code
error C2664: 'GDevice::GDevice(const GDevice &)' : cannot convert parameter 1 from 'QObject *' to 'const GDevice &'
\endcode
and this is because the GDevice class has one constructor with the following signature:
\code
//! Constructor. You have to provide a unique name to identify the physical device in the program
GDevice(QString uniqueIdentifierName, QObject *parent);
\endcode
To fix the class in order to be able to compile, you can follow this link:
- \ref ImplementGDeviceWS6 

which is located in the \ref NewDeviceImplementation,
After that, it should compile,...

or almost...

\subsection FixingLinkingLabExe Link the project to the LabExe library
You should now get the very typical linking error:
\code
error LNK2019: unresolved external symbol "__declspec(dllimport) public: __thiscall GDevice::GDevice(class QString,class QObject *)" etc................
\endcode
The compiler is telling you "I don't know where is the code of the GDevice class". Indeed, it knows 
about the header file but can't find the actual library that describes what a GDevice actually does.

You have to link the HighFinesseLabExe project to the LabExe dll. To do so, the most elegant way 
in Visual Studio is to tell the compiler that the HighFinesseLabExe project depends on the LabExe project. 
To do so, right-click on the project select "Project Dependencies..." and check the "labexe" box:
\image html NewDeviceProject-08.PNG

Doing so will both link to labexe.dll and force the compiler to first compile LabExe if any changed were made to it, then 
to compile HighFinesseLabExe.

Well, now it should build!

\section LinkSrYbExeToHighFinesse Link and use your new class in SrYbExe Program
\subsection WS6DllExport Exporting the WS6 class to the dll

So the project built a dll library in the ..\..\lib folder, but it is not much useful because there 
is nothing in the dll. In fact, you have to tell the compiler to put the WS6 class in the 
dll. To do that, you follow the procedure very well described in the Qt documentation "Making a library":
- You have to add the line :
\code
#include "highfinesselabexe_global.h"
\endcode
in the GWavelengthMeterWS6.h file. Now the top of your GWavelengthMeterWS6.h file looks like:
\code
#ifndef GWAVELENGTHMETERWS6_H
#define GWAVELENGTHMETERWS6_H
#include "highfinesselabexe_global.h"
#include "device.h"
\endcode
- What does the highfinesselabexe_global.h file is to define a macro to tell the compiler what to put in the 
library. You use that macro and change your class declaration from:
\code
class GWavelengthMeterWS6 : public GDevice
{.......
\endcode
to
\code
class HIGHFINESSELABEXE_EXPORT GWavelengthMeterWS6 : public GDevice
{.......
\endcode




\subsection WS6UseInSrYbExe Using in SrYbExe
Now you want to use your class in the SrYbExe program:
- First, in the srybexe.cpp file you should add the include:
\code
#include "../UserDefinedDevice/HighFinesseLabExe/GWavelengthMeterWS6.h"
\endcode
- Then you could use it, typically in the function SrYbExe::DevicesToShelf():
\code
QList<GDevice*> SrYbExe::DevicesToShelf() {
QList<GDevice*> listDevicesToShelf;
return listDevicesToShelf 
<< new GPhidgetManager("Phidget Manager", this) 
<< new GUeyeCameraManager("uEye-Camera-Manager", this) 
<< new GWavelengthMeterWS6("WS-200", this);
}
\endcode
In this code, the WS6 object is instantiated and is added to the list of devices to 
shelf so that you can use it in the gui of the program. 


This will not compile because the WS6 class is not yet implementing some of the functionality 
required by GDevice. The error looks like:
\code
error C2259: 'GWavelengthMeterWS6' : cannot instantiate abstract class
\endcode
To fix this error, you have to implement the virtual functions of the GDevice class. 
You should follow the \ref NewDeviceImplementation

\subsection WS6LinkInSrYbExe Linking in SrYbExe
Well even if it compiled, you would have a very nice linking error when trying to use the 
WS6 object in srybexe.cpp. This is because the SrYbExe knows about the declaration, but doesn't 
know about the actual code, which is in the library produced by the HighFinesseLabExe project.

- Let's have the compiler create the HighFinesseLabExe dll library in a place that is already 
being used by the SrYbEx.
To do so, right-click on the HighFinesseLabExe project select "Properties" and go 
to "Linker", "General", and change the following in the "Output File": 
\code
$(OutDir)\$(ProjectName).dll
\endcode
to
\code
$(ProjectDir)\..\..\lib\$(ProjectName).dll
\endcode
This will make the library in the folder lib of the LabExe folder.
\image html NewDeviceProject-07.PNG
- In the SrYbExe project, right-click on the project select "Project Dependencies..." and check the "HighFinesseLabExe" box:
\image html NewDeviceProject-09.PNG
Doing so will both link to HighFinesseLabExe.dll and force the compiler to first compile HighFinesseLabExe if any changed were made to it, then 
to compile SrYbExe.

Well, now your good to go !
but you still have to follow the \ref NewDeviceImplementation
*/
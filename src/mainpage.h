/*! \file mainpage.h
* \brief Definition of LabExe
*
*/
/*! \mainpage The LabExe framework

@authors Gael Reinaudi

\section TOC Table of content

\ref usingLabExe

\ref derivedDevices

\ref Example

\ref progDevices

@section usingLabExe Using the LabExe framework
Here is a typical code used to create the SrYbExe program.
\include SrYbExe\SrYbExe\main.cpp

Where SrYbExe is a class derived from the GLabControlPanel. Deriving the GLabControlPanel 
class to define your lab program consist only in implementing the pure virtual 
function GLabControlPanel::DevicesToShelf(). For example:
\code
QList<GDevice*> SrYbExe::DevicesToShelf()
{
	QList<GDevice*> listDevicesToShelf;
	return listDevicesToShelf	<< new GPhidgetManager("Phidget Manager", this) 
					<< new GPxiSystem("PXI system", this) 
					<< new GUeyeCameraManager("uEye-Camera-Manager", this);
}
\endcode

When implementing the SrYbExe::DevicesToShelf(), you need to create some GDevices objects. 
Each of those objects will represent a physical device in your system. In the case you have 
a hierarchy of devices, like 
- a PXI system that actually owns 
	- a PXI ai module that is actually made of a bunch of 
		-	PxiAiChannel, which represents the actual device with a BNC connector for inputing a signal
	- a PXI ao module that is actually made of a bunch of 
		-	PxiAoChannel, which represents the actual device with a BNC connector for outputing a signal

it is then good practice to only make the GPxiSystem object in the SrYbExe::DevicesToShelf().
- This GPxiSystem object should then take care of detecting and creating the modules objects. 
- Each GPxiModule object would take care of detecting and creating the specific devices object.

All those classes described in this example represent devices of your system and have to inherit the GDevice class.


@section derivedDevices Take control of your own devices from the LabExe: deriving your own device-class

If you want to control your own devices from the LabExe, you must create a class representation of the device. 
It means this class would, among other things, have to:
- implement the interaction with the device proprietary drivers,
- implement how the gui of the device will be looking like in the program
- optionally implement how to detect and create sub-devices, i.e. devices that are logically controlled by this one.

All those functionalities are prepared in the virtual class GDevice. 
Your device class has to inherit GDevice and implement the virtual functions in it.


\subsection Example Example and Tutorials
You want to control your wavelength meter WS6.
Your new class, e.g. GWavelengthMeterWS6, has to derive from the abstract class GDevice. 

\see \ref NewDeviceInSolution 
\see \ref NewDeviceImplementation 
\see \ref NewDeviceSubdevices 

@section progDevices Unleash the power of a user driven program: using and making user plugins

Quick questions: 
- Why the iPhone is the most powerful smartphone? (hint, it is not because of the electrinc components it uses) 
- Why Firefox has become the number one web browser in no time?

One answer:
- the plugins/apps, written by thousands of users for their specific needs, and later one used by millions of users as they need it.

The LabExe framework gives the users a structure to write their own plugins.

\see \ref NewDevicePluginInSolution







\page TutorialPage Tutorial
- \subpage NewDeviceInSolution
- \subpage NewDeviceImplementation
- \subpage NewDeviceSubdevices
- \subpage NewDevicePluginInSolution








\page UsedLibraries Framework and libraries used in LabExe
Leading text.

-Framework
- Qt is the Framework in which the LabExe is writen

-Additional libraries
- qwt is also used for some functionalities (GParamRange, plots?)
- loki, it implements some design pattern (like Factory, Singleton, ...).



\section sec An example section
This page contains the subsections \ref subsection1 and \ref subsection2.
For more info see page \ref page2.
\subsection subsection1 The first subsection
Text.
\subsection subsection2 The second subsection
More text.




\page exSyntax example of documetation syntax


THIS IS ONLY A TEST

@section intro Introduction
This package provides code templates for use by GLAST developers. 
All header files for external access are located in the templates directory,
as it is customary to put external public header files in the packageName 
directory. Header files that are not meant for external access reside in 
the src directory. Source files are located in the src directory. Files 
related to loading sharable libraries are located in the src/Dll directory.
There are 3 examples:

- User-Defined generic C++ class
-# templates/ClassTemplate.h
-# src/ClassTemplate.cxx
- User-Defined Gaudi Algorithm
-# src/ExampleAlg.cxx
-# src/Dll/templates_dll.cxx
-# src/Dll/templates_load.cxx
- User-Defined Gaudi Service
-# templates/IExampleSvc.h
-# templates/ExampleSvc.h
-# src/ExampleSvc.cxx
-# src/Dll/templates_dll.cxx
-# src/Dll/templates_load.cxx


Also note the existence of the following directories:
- cmt
-# Contains the requirements file
- doc
-# Contains the release.notes file


As you prepare to develop code for GLAST SAS, please be sure you are aware 
of our current
<A HREF="http://www-glast.slac.stanford.edu/software/CodeHowTo/codeStandards.html"> Coding Standards </A>


If using the code in this package as an example - please modify the comments
as appropriate for your own specific code.

<hr>
@section notes release.notes
release.notes
<hr>
@section requirements requirements
@verbinclude requirements
<hr> 
@todo Make GDeviceWidgetFactory
@todo For the filling of the memory by the sequence curves : 
- strategy patern: diffrent way to fill: 
* point by point, 
* curve by curve,
* fgaga




*/


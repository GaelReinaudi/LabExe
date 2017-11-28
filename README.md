https://vimeo.com/32183792

https://vimeo.com/31039111


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
- loki, it implements some design pattern (like Factory, Singleton, ...).



*/


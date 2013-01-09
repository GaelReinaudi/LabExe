/*! \file mainpage.h
* \brief Definition of LabExe
*
*/
/*! \mainpage LabExe API

\section TOC Table of content

- \ref WhyLabxe
- \ref TypicalScenario
- \ref OverviewLabExe
	- \ref Workbench
	- \ref HardDevice
	- \ref SoftDevice
- \ref userDevices
	- \ref checkPlugin
	- \ref writePlugin

- \ref ExamplesTutorial


@section WhyLabxe Why LabExe?

LabExe was imagined and created in order to overcome the huge drain on programming resources in labs and engineering environments. 

Researchers and engineers need to operate devices, treat acquired data, pictures, curves. 
For this, we have to use computer interfaces and other programs. Pre-existing programs are rarely flexible 
enough so that it gives us the ability to perform a task exactly how we imagined. So we happen to code our own 
computer programs.

However, science people are usually pretty bad programmer from a software engineer point of view. 
This means that code that is written by a scientist to perform a task will rarely be usable, and almost never 
modifiable by other colleague, or in a neighboring lab. A program coded from within a lab almost never 
resists the departure of the programmer or even a few month of not being used.

This is a terrible problem since we, the science community, keep coding over and over again the same 
program to achieve slightly different task than the previous one. 

People argue that some visual programming language bring the coding ability to anyone, removing the 
need to learn the syntax of a text-based language. This is true, but has nothing to do with the point. 
If anything a visual programming language will be the hardest to modify, extend, or adapt to a 
different lab environment. Programming well is always a difficult task and will always be. 

There is a need for a new picture in the context research and engineer programing, that will allow 
people to use and modify other's code.

LabExe is a solution that even brings us to the next step. A flexible program based on user-generated 
modules that operate and interact in a structure that removes all non-user specific dificulties from the table. 


@section TypicalScenario Does it sound familiar? or a typical scenario in a lab.
The postdoc just left the lab...

The program that controls the data acquisition device and sequencer were coded by him some time ago. 
It is a good software that does the job since it was tweaked specifically for this lab purpose. 
For month after the postdoc departure, the software runs flawlessly,... until:
- A new device is bought by the lab. Let's say it is a new camera. 
The camera has a software that can be used to control it and save pictures in the computer. 
But taken thousands of images that will be treated in the same way raise the perspective of having a 
dedicated program that automates the process of extracting data relevant to the scientist/engineer. 
The best way to do it is to integrate a piece of code relative to the camera in the existing data acquisition/sequencer program.
- A new functionality form the data acquisition/sequencer is needed. For example, saving the data in a 
new way, or giving the ability to automatically optimize some parameters of the sequencer.
- Someone needs to check by himself that the program is behaving normally in order to ensure the accuracy of 
his scientific results.

The burden of modifying the existing code is now falling on the next postdoc or PhD It turns out 
pretty quickly that it is impossible to modify the code so that it works compiles and give the scientist 
the assurance that it is doing accurately what it is supposed to do. Common problems will be:
- the old code was poorly organized/documented, 
- the old code heavily depends on a library that is no longer present on the computer, 
- the new camera's library is not compatible with the one used in the existing code, 
- the old code doesn't compile anymore on the new operating system, 
- converting the data provided by the camera to data usable by the old program looks like it is impossible, etc...

The PhD Starts coding a whole new data acquisition/sequencer program. It takes months. That time 
could have been used to do science if the integration of the camera code had been seamless, and using 
existing functionalities.
The new PhD's program is again tweaked for the purpose of the lab, reproducing all the mistakes 
that will lead to the same problem, 3 years from now. 

There is no programming language that will simplify the task, be it visual or text-based 
(visual programming will make it worse). 
What we need is a new framework.


@section OverviewLabExe Overview of LabExe

Upon starting, the program consists on a control panel which allows to create, load or delete "Workbenches".
\image html Overview-01.PNG 

@subsection Workbench Workbenches

In LabExe, a workbench is a virtual representation of what you could call a workbench in a laboratory. 
It is a space in which you can drag-and-drop devices, connect them in a way that suits 
your experiment, or measurement.

You can create as many of those workbenches as you want. They can share the same devices 
and be active at the same time. For example:
- a camera device can be part of a workbench that displays the raw received images,
- and also be part of another workbench that treats the images and displays relevant informations.
This would be typically useful when the user wants to periodically control the upstream flow of images, 
without clogging or disrupting his experiment. Showing and hiding the raw-image workbench 
gives an instant access to debugging informations that one need in a lab.

There are two main types of devices that are used in the lab:
- "Hard-devices" that control physical devices in the lab,
- "Soft-devices" that correspond to computing units.

@subsection HardDevice Hard-Devices

Hard devices are the virtual interface between the LabExe program and the actual physical devices in the laboratory. 
For example:
- a camera acquiring picture through the USB port of the computer is going to be controlled by one, 
and only one instance of its corresponding hard-device module in the LabExe program,  
- still the hard-device interface can appear on several workbenches at the same time, 
- but all of those displays will mirror each other and be consistent with the fact that the actual device 
is in a given state.

\image html Overview-HardDevice.PNG 


@subsection SoftDevice Soft-Devices

Soft-devices extend the abstraction of a device to a computing unit in the LabExe program. 
They have not connections with physical devices in the laboratory, but are merely programs 
that can be created in an unlimited number. 
For example:
- a plotting device is a soft-device that displays a plot of any variable of the LabExe program, 
- a text logger is a soft-device that saves any group of variable to the disk of the computer, 
- a numerical expression device applies user defined equations to variables and displays the result. 
A soft-device is part of the workbench it is used in and is usually not displayed in other workbenches. 
Still, the input or output variables of a soft-devices can be dragged-and-dropped to any workbench. 

\image html Overview-SoftDevice.PNG 



@section userDevices Control of your devices and use your coded routines from LabExe

@subsection checkPlugin Checking for existing plugins

There is a growing number of plugins available for the LabExe program and one of them might already 
working for your device. If you need a functionality that is missing from the plugin it is 
advisable to contact its author since he might want to expand the existing, user proofed plugin.

@subsection writePlugin Code your plugin

If you want to control your own devices in the LabExe program and there is no plugin available, 
you can use the plugin API provided to code your own plugin in a very efficient way:
- the key idea behind the LabExe framework is that you can concentrate in writing only the 
part of the code that actually interfaces your device's functionalities,... 
- without having to concentrate on the display, the settings being saved to disk or the drag-and-drop interaction with other devices. 
Those features are already coded in the LabExe program, provide a robust flexibility and makes it very powerful in a laboratory environment.


In order to make your device available to the LabExe, you must create a plugin that will provide 
a class representation of the device. 
It means this class would, among other things, have to:
- implement the interaction with the device proprietary drivers,
- implement how the gui of the device will be looking like in the program,
- optionally implement how to detect and create sub-devices, i.e. other devices that are logically controlled by this one.

All those functionalities are declared in the abstract class GDevice. 
Your device class has to inherit GDevice and implement the virtual functions in it.


\section ExamplesTutorial Examples and Tutorials
Here is a list of tutorials that introduce you to the 5-minute creation of a new plugin, starting from a working example.

\subsection ExampleSoftDevice Soft-Devices Plugins

- \subpage ExampleProject-LowPassFilterPlugin
- \subpage Tutorial-PluginWizard









\page UsedLibraries Framework and libraries used in LabExe
Leading text.

- Framework
	- Qt is the Framework in which the LabExe is writen

- Additional libraries
	- loki, it implements some design pattern (like Factory, Singleton, ...)
	- NLopt and EO for some optimization algorithms in certain version of LabExe






*/


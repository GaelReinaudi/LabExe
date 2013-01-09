/*! \file Tutorial-DevicePlugin-NewProgDeviceProject.h
* \brief A tutorial 
*
*/
/*!

\page NewDevicePluginInSolution Tutorial : Setting up a new project to make a user plugin

In this page we are going to go step by step through the process of making a new user plugin. 

A plugin is actually a special type of GDevice called a GProgDevice. This kind of device can be created 
dynamically in any number by dragging it from the e-Device shelf and dropping it on a workbench. 
We will thus derive a class from the GProgDevice class, 
and derive a class from the GDevicePlugin class in order to give it its plugin functionalities.

\section TocNewPluginProject Table of content
- \ref CreateNewProjectDevicePlugin
- \ref MakePluginDLL
	- \ref MakingThePlugin
	- \ref CopyingPluginInDevicePlugins
- \ref CreateNewClassPlotterDevice
	- \ref FixClassPlotter
	- \ref FixingLinkingLabExePlotter
	- \ref WS6LinkInSrYbExe




\note
The lab program we will consider here as an example is the SrYbExe program that was created for 
the Zelevinsky lab. 
SrYbExe is both the name of the derived class of GLabControlPanel, and the name of the visual studio solution 
that groups all the needed classes and libraries that we use for the SrYbExe program.

\note
LabExe is the name of the Framework, i.e. the group of classes writen to control an experiment. 

Also, the new plugin we will create is the GParamPlotter, which will give the ability of plotting a numerical value.










\section CreateNewProjectDevicePlugin Create a new project for the plugin
right-click the SrYbExe solution and add a new project:
\image html NewDevicePluginProject-01.PNG

select a Qt Library and name the project "ParamPlotterPLugin". Place the project folder in the \LabExe\Plugins folder.
\image html NewDevicePluginProject-02.PNG

After clicking "OK", click "Next", check the "GUI library", click "Next" and rename the 
main class of the project if you want. Here:
- we'll name the class GParamPlotterPlugin 
- and the files "paramplotterplugin.h" and "paramplotterplugin.cpp".

Click "Finish".

\section MakePluginDLL Making a dll that can be used as a device plugin 
In order to have the dll recognized by the program as being a device plugin:
- the class GParamPlotterPlugin has to inherit the GDevicePlugin class, 
- it has to be specified as being a plugin, 
- the dll must be copied in the lib/DevicePlugins directory.

\subsection MakingThePlugin Defining the project main class as being a plugin

To do so, in the "paramplotterplugin.cpp" file, change 
\code
GParamPlotterPlugin::GParamPlotterPlugin()
{
}
\endcode
to
\code
Q_EXPORT_PLUGIN2(ParamPlotterPlugin, GParamPlotterPlugin)
GParamPlotterPlugin::GParamPlotterPlugin(QObject *parent)
	: GDevicePlugin(parent)
{
	setObjectName("Param plotter");
}
\endcode

and in the "paramplotterplugin.h" file, change 
\code
class PARAMPLOTTERPLUGIN_EXPORT GParamPlotterPlugin
{
public:
	GParamPlotterPlugin();
...
\endcode
to
\code
#include "device.h"
#include "GParamPlotter.h"
class PARAMPLOTTERPLUGIN_EXPORT GParamPlotterPlugin : public GDevicePlugin
{
	Q_OBJECT
	Q_INTERFACES(GDevicePlugin)
	G_REGISTER_PLUGIN_PROG_DEVICE(GParamPlotter)

public:
	GParamPlotterPlugin(QObject *parent = 0);
...
\endcode
where GParamPlotter will be the name of the actual class of the plotter device (see bellow).

\subsection CopyingPluginInDevicePlugins Creating the plugin in the DevicePlugins folder

Right-click on the ParamPlotterPlugin project select "Properties" and go 
to "Linker", "General", and change the following in the "Output File": 
\code
$(OutDir)\$(ProjectName).dll
\endcode
to
\code
$(ProjectDir)\..\..\lib\DevicePlugins\$(ProjectName).dll
\endcode
\image html NewDevicePluginProject-05.PNG

\section CreateNewClassPlotterDevice Creating a GParamPlotter class that can plot a numerical value

Add a class to the ParamPlotterPLugin project by right-clicking the project:
\image html NewDevicePluginProject-03.PNG
- go to Add then Class
- choose a QtClass and call it "GParamPlotter"
- Set the Base class as GProgDevice. 
- un-check the lower case file names (this is just for respecting the naming convention of the LabExe)
- leave insert Q_OBJECT checked
\image html NewDevicePluginProject-04.PNG

The wizard then creates the GParamPlotter.h file. You have to change the line:
\code
#include <GProgDevice>
\endcode
to the following in order to tell the compiler to include the correct file (by specifying GProgDevice in the wizard, it assumed it was a Qt base class, which would be accessed by using brackets):
\code
#include "device.h"
\endcode
and tell the compiler about the location of the LabExe framework source folder. For this, right-click on the project select "Properties" and go to "C/C++", "General" and add: 
\code
..\..\src;..\..\include;
\endcode
to the "Additional Include Directories":
\image html NewDevicePluginProject-07.PNG


\subsection FixClassPlotter Fix the GParamPlotter class

If you try to compile, there should be the following error:
\code
error C2664: 'GProgDevice::GProgDevice(const GDevice &)' : cannot convert parameter 1 from 'QObject *' to 'const GProgDevice &'
\endcode
and this is because the GProgDevice class has one constructor with the following signature:
\code
GProgDevice(QObject *parent, QString uniqueIdentifierName = "");
\endcode
As a consequence (and this is just plain c++ rule) you should initialize the GProgDevice 
in the constructor of the GParamPlotter by changing the following in the GParamPlotter.cpp file:
\code
GParamPlotter::GParamPlotter(QObject *parent)
: GProgDevice(parent)
{...}
\endcode
to the following (don't forget to also update the signature of the constructor in the GParamPlotter.h file):
\code
GParamPlotter::GParamPlotter(QObject *parent, QString uniqueIdentifierName)
	: GProgDevice(parent, uniqueIdentifierName)
{...}
\endcode

Also, being derived from GProgDevice, which derives from the abstract class GDevice, some 
methods need to be implemented in GParamPlotter.
Fixing the GParamPlotter class is similar than what is done there:
- \ref ImplementGDeviceWS6 

After that, it should compile,...

or almost...

\subsection FixingLinkingLabExePlotter Link the project to the LabExe library

You should now get the very typical linking error:
\code
error LNK2019: unresolved external symbol "__declspec(dllimport) public: __thiscall GProgDevice::GProgDevice(class QString,class QObject *)" etc................
\endcode
The compiler is telling you "I don't know where is the code of the GProgDevice class". Indeed, it knows 
about the header file but can't find the actual library that describes what a GDevice actually does.

You have to link the ParamPlotterPlugin project to the LabExe dll. To do so, the most elegant way 
in Visual Studio is to tell the compiler that the ParamPlotterPlugin project depends on the LabExe project. 
To do so, right-click on the project select "Project Dependencies..." and check the "labexe" box:
\image html NewDevicePluginProject-06.PNG

Doing so will both link to labexe.dll and force the compiler to first compile LabExe if any changed were made to it, then 
to compile ParamPlotterPlugin.

Well, now it should build!


\subsection WS6LinkInSrYbExe Linking Using in SrYbExe

Since we just made a plugin, there is no linking to do. This is what a plugin is about: 
- the program doesn't have to now that it exist at all! 

The SrYbExe program should now show a GParamPlotter device in the e-Device shelf. 

Still, for convenience:
- In the SrYbExe project, right-click on the project select "Project Dependencies..." and check the "ParamPlotterPlugin" box:
\image html NewDevicePluginProject-09.PNG
Doing so will both force the compiler to allways first compile ParamPlotterPlugin if any changed were made to it.





*/
TEMPLATE = subdirs
#CONFIG += ordered

SUBDIRS += \
	../src/labexe.pro \
#	../../MapExe/src/LabExeGraphicsMap.pro \
	../src/LabExeImaging \
#	../src/LabExeSequencer \
	../src/LabExeOptimizing \
    ../../LabExe-UserPlugins/SoftwarePlugins/SoftwarePlugins.pro \
    ../../LabExe-UserPlugins/HardwarePlugins/HardwarePlugins.pro \
    ../Deployement/UserPluginWizard/UserPluginWizard.pro \
    SrYbExe/SrYbExe.pro

LabExeImaging.depends = labexe
LabExeOptimizing.depends = labexe
SoftwarePlugins.depends = labexe LabExeImaging
HardwarePlugins.depends = labexe LabExeImaging
SrYbExe.depends = labexe LabExeImaging LabExeOptimizing SoftwarePlugins HardwarePlugins #LabExeSequencer LabExeGraphicsMap


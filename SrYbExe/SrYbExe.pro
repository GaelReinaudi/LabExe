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
	SrYbExe/SrYbExe.pro

LabExeImaging.depends = labexe
LabExeOptimizing.depends = labexe
SrYbExe.depends = labexe LabExeImaging LabExeOptimizing #LabExeSequencer LabExeGraphicsMap


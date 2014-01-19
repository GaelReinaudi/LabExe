TEMPLATE = subdirs
#CONFIG += ordered

SUBDIRS += \
	../src/labexe.pro \
#	../../MapExe/src/LabExeGraphicsMap.pro \
	../src/LabExeImaging \
#	../src/LabExeSequencer \
	../src/LabExeOptimizing \
#	../../LabExe-UserPlugins/SoftwarePlugins/DevicePlugins.pro \
#	../../LabExe-UserPlugins/HardwarePlugins/HardwarePlugins.pro \
	SrYbExe/SrYbExe.pro

SrYbExe.depends = LabExeImaging LabExeOptimizing #LabExeSequencer LabExeGraphicsMap


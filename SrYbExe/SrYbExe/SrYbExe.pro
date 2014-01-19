TEMPLATE = app
DESTDIR = ../../bin

QT += core gui widgets testlib

INCLUDEPATH += \
	./../../../LabExe/src \
	./../../../LabExe/include \
	./GeneratedFiles \
	.

LIBS += \
	-L"./../../../LabExe/lib" \
	-L"./../../../LabExe/bin" \
#	-L"./../../../MapExe/lib" \
#	-L"./../../../MapExe/bin" \
	-L"."

CONFIG(debug, debug|release) {
	LIBS += -llabexe_D \
	-lLabExeImaging_D \
	-lLabExeOptimizing_D \
#	-lLabExeGraphicsMap_D \
#	-lLabExeSequencer_D
} else {
	LIBS += -llabexe \
	-lLabExeImaging \
	-lLabExeOptimizing \
#	-lLabExeGraphicsMap \
#	-lLabExeSequencer
}

MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./GeneratedFiles/Obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

include(SrYbExe.pri)

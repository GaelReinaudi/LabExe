TEMPLATE = app
DESTDIR = ../../bin
TARGET = WinLabExe

QT += core gui widgets

INCLUDEPATH += \
	./../../../LabExe/src \
	./../../../LabExe/include \
	./GeneratedFiles \
	.

LIBS += \
	-L"./../../../LabExe/lib" \
	-L"./../../../LabExe/bin" \
	-L"."

CONFIG(debug, debug|release) {
	LIBS += -llabexe_D \
	-lLabExeImaging_D \
	-lLabExeOptimizing_D \
} else {
	LIBS += -llabexe \
	-lLabExeImaging \
	-lLabExeOptimizing \
}

RC_FILE = SrYbExe.rc

MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./GeneratedFiles/Obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

include(SrYbExe.pri)

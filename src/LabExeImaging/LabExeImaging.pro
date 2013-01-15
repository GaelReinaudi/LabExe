TEMPLATE = lib
DESTDIR = ../../bin

QT += core gui widgets

DEFINES += LABEXEIMAGING_LIB

INCLUDEPATH += \
	./../../include \
	./../../src \
	./GeneratedFiles \
	.

LIBS += \
	-L"./../../lib" \
	-L"./../../bin" \
	-llabexe

MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./GeneratedFiles/Obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

include(LabExeImaging.pri)

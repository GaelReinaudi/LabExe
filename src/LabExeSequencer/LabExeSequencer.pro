TEMPLATE = lib
DESTDIR = ../../bin
TARGET = LabExeSequencer

QT += core gui widgets qml declarative

DEFINES += LABEXESEQUENCER_LIB

INCLUDEPATH += \
	./../../include \
	./../../src \
	./../../../MapExe/src \
	./GeneratedFiles \
	.

LIBS += \
	-L"./../../lib" \
	-L"./../../bin" \
	-L"./../../../MapExe/bin" \
	-llabexe \
	-lLabExeGraphicsMap

MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./GeneratedFiles/Obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

include(LabExeSequencer.pri)

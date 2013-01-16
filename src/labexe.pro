TEMPLATE = lib
DESTDIR = ../bin
TARGET = labexe

QT += core gui script widgets
INCLUDEPATH += \
	./../include \
    ./GeneratedFiles \
    . \
    ./Device \
    ./Param \
    ./ToolBox \
    ./TheLab

LIBS += \
	-L"./../lib" \
	-L"./../bin" \
	-L"."

MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./GeneratedFiles/Obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

DEFINES += LABEXE_LIB
include(labexe.pri)

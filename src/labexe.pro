TEMPLATE = lib
DESTDIR = ../bin
TARGET = labexe
QT += core gui script widgets
DEFINES += LABEXE_LIB
INCLUDEPATH += \
	./../include \
    ./GeneratedFiles \
    ./GeneratedFiles/Debug \
    . \
    ./Device \
    ./Param \
    ./ToolBox \
    ./TheLab
LIBS += \
	-L"./../lib" \
	-L"./../bin" \
	-L"."
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./GeneratedFiles/Obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(labexe.pri)

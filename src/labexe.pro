TEMPLATE = lib
DESTDIR = ../bin

CONFIG(debug, debug|release) {
#	mac: TARGET = $$join(TARGET,,,_debug)
#	win32: TARGET = $$join(TARGET,,,_D)
	TARGET = $$join(TARGET,,,_D)
}


QT += core gui widgets script
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

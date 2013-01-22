TEMPLATE = lib
DESTDIR = ../bin

#CONFIG(debug, debug|release) {
#    DLLDESTDIR += $$quote(../../../Jane/Jane/Build/debug)
#    message("Copying to Jane Debug Directory.")
#} else {
#    DLLDESTDIR += $$quote(../../../Jane/Jane/Build/release)
#    message("Copying to Jane Release Directory.")
#}
CONFIG(debug, debug|release) {
#	mac: TARGET = $$join(TARGET,,,_debug)
#	win32: TARGET = $$join(TARGET,,,_D)
	TARGET = $$join(TARGET,,,_D)
}


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

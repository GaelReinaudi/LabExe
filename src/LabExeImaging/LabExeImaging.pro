TEMPLATE = lib
DESTDIR = ../../bin
CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,_D)
}

QT += core gui widgets

INCLUDEPATH += \
	./../../include \
	./../../src \
	./GeneratedFiles \
	.

LIBS += \
	-L"./../../lib" \
	-L"./../../bin"

CONFIG(debug, debug|release) {
	LIBS += -llabexe_D
} else {
	LIBS += -llabexe
}

MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./GeneratedFiles/Obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

DEFINES += LABEXEIMAGING_LIB
include(LabExeImaging.pri)

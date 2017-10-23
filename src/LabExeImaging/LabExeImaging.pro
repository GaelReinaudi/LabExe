TEMPLATE = lib
CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,_D)
}

QT += core gui widgets
include($$PWD/../../common.pri)

DEFINES += LABEXEIMAGING_LIB

INCLUDEPATH += \
        ./../../include \
        ./../../src

CONFIG(debug, debug|release) {
	LIBS += -llabexe_D
} else {
	LIBS += -llabexe
}

include(LabExeImaging.pri)

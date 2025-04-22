TEMPLATE = lib
CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,_D)
}

QT += core gui widgets concurrent printsupport
include($$PWD/../../common.pri)

DEFINES += LABEXEOPTIMIZING_LIB

INCLUDEPATH += \
    ./../../include/eo \
    ./../../include \
    ./../../include/nlopt \
    ./../../src

LIBS += \
        -L"./../../lib/eo" \
        -L"./../../lib/nlopt"

CONFIG(debug, debug|release) {
    LIBS += -llabexe_D
} else {
    LIBS += -llabexe
}

win32 {
    LIBS += -llibeo -llibeoutils -llibes -llibga -llibcma -llibnlopt
} else {
    LIBS += -l:libeo.a -l:libeoutils.a -l:libes.a -l:libga.a -l:libcma.a -lnlopt
}

include(LabExeOptimizing.pri)

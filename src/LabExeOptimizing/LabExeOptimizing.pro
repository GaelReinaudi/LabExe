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
    ./../../src \
    /usr/local/include/paradiseo \
    /usr/include/nlopt

LIBS += \
        -L"./../../lib/eo" \
        -L"/usr/local/lib64" \
        -L"/usr/lib/x86_64-linux-gnu"

CONFIG(debug, debug|release) {
    LIBS += -llabexe_D \
        -leo_d -leoutils_d -les_d -lga_d -lcma_d -lmoeo_d \
        -lnlopt
} else {
	LIBS += -llabexe \
	-leo   -leoutils   -les   -lga   -lcma -lmoeo \
        -lnlopt
}

include(LabExeOptimizing.pri)

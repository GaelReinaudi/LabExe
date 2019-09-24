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
    LIBS += -llabexe_D \
        -leo_d -leoutils_d -les_d -lga_d -lcma_d \
        -lnlopt_d
} else {
	LIBS += -llabexe \
	-leo   -leoutils   -les   -lga   -lcma \
        -lnlopt
}

include(LabExeOptimizing.pri)

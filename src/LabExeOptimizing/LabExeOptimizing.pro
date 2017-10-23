TEMPLATE = lib
CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,_D)
}

QT += core gui widgets concurrent printsupport
include($$PWD/../../common.pri)

DEFINES += LABEXEOPTIMIZING_LIB

INCLUDEPATH += \
    /usr/include/paradiseo/eo \
    ./../../include/eo \
    ./../../include \
    ./../../src \
    ./GeneratedFiles \
    .

LIBS += \
	-L"./../../lib" \
	-L"./../../bin" \
        -L"./../../lib/eo"

CONFIG(debug, debug|release) {
    LIBS += -llabexe_D \
        -leo_d -leoutils_d -les_d -lga_d -lcma_d \
        -llibnlopt-0
} else {
	LIBS += -llabexe \
	-leo   -leoutils   -les   -lga   -lcma \
	-llibnlopt-0
}

include(LabExeOptimizing.pri)

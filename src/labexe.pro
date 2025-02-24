TEMPLATE = lib

CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,_D)
}

QT += core gui widgets core5compat qml
include($$PWD/../common.pri)

DEFINES += LABEXE_LIB

INCLUDEPATH += \
	./../include \
    ./GeneratedFiles \
    . \
    ./Device \
    ./Param \
    ./ToolBox \
    ./TheLab

include(labexe.pri)

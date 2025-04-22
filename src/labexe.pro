TEMPLATE = lib

CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,_D)
}

# Qt modules configuration
QT = core gui widgets script concurrent printsupport

# Remove any Qt 6 compatibility modules
QT -= core5compat

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

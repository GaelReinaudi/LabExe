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

# Add compiler flags for template handling
QMAKE_CXXFLAGS += -Zc:templateScope
QMAKE_CXXFLAGS += -permissive-

INCLUDEPATH += \
	./../include \
    ./GeneratedFiles \
    . \
    ./Device \
    ./Param \
    ./ToolBox \
    ./TheLab

include(labexe.pri)

CONFIG += c++17

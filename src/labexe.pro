TEMPLATE = lib

CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,_D)
}

QT += core gui widgets qml

# Add Qt6-specific modules conditionally
greaterThan(QT_MAJOR_VERSION, 5) {
    QT += core5compat
    message("Building with Qt6 compatibility mode")
} else {
    message("Building with native Qt5")
}

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

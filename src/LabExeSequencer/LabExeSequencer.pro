TEMPLATE = lib
DESTDIR = ../../bin
#TARGET = LabExeSequencer

QT += core gui widgets qml declarative

DEFINES += LABEXESEQUENCER_LIB

INCLUDEPATH += \
	./../../include \
	./../../src \
	./../../../MapExe/src \
	./GeneratedFiles \
	.

LIBS += \
	-L"./../../lib" \
	-L"./../../bin" \
	-L"./../../../MapExe/bin"

MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./GeneratedFiles/Obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles


CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,_D)
}
CONFIG(debug, debug|release) {
	LIBS += -llabexe_D \
	-lLabExeGraphicsMap_D
} else {
	LIBS += -llabexe \
	-lLabExeGraphicsMap
}

include(LabExeSequencer.pri)

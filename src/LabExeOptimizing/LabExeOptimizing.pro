TEMPLATE = lib
DESTDIR = ../../bin
TARGET = LabExeOptimizing

QT += core gui widgets concurrent

DEFINES += LABEXEOPTIMIZING_LIB

INCLUDEPATH += \
    ./../../include/eo \
    ./../../include \
    ./../../src \
    ./GeneratedFiles \
    .

LIBS += \
    -L"./../../lib" \
    -L"./../../bin" \
    -L"./../../lib/eo" \
    -llabexe \
    -leo_d \
    -leoutils_d \
    -les_d \
    -lga_d \
    -lcma_d \
    -llibnlopt-0

MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += ./GeneratedFiles/Obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

include(LabExeOptimizing.pri)

CONFIG += c++11

QT += concurrent

GIT_VERSION = $$system(git --git-dir $$PWD/core/.git --work-tree $$PWD describe --always --tags)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
DEFINES += USE_SPDLOG

DEBUGRELEASE = what
CONFIG(debug, debug|release) {
DEBUGRELEASE = debug
}
else {
DEBUGRELEASE = release
}

DEPEND_REPOS = $$PWD/../../dependRepos
SPDLOG_DIR = $$DEPEND_REPOS/spdlog/include
PARSER_DIR = $$DEPEND_REPOS/exprtk
ALG_LIB_DIR = $$DEPEND_REPOS/AlgLib/src

COMMON_DIR = $$PWD

INCLUDEPATH += $${SPDLOG_DIR}
INCLUDEPATH += $${ALG_LIB_DIR}

# to use ExprTK parser
INCLUDEPATH += $${PARSER_DIR}

#SOURCES += $$PWD/global.cpp
#HEADERS += $$PWD/core/log.h

BINDIR = $${COMMON_DIR}/../bin/$${DEBUGRELEASE}
LIBDIR = $${COMMON_DIR}/../lib/$${DEBUGRELEASE}
DESTDIR = $${BINDIR}

LIBS += -L$${BINDIR}
LIBS += -L$${LIBDIR}

MOC_DIR = $$PWD/../build/$${TARGET}/$${DEBUGRELEASE}
OBJECTS_DIR = $$PWD/../build/$${TARGET}/$${DEBUGRELEASE}
UI_DIR = $$PWD/../build/$${TARGET}/$${DEBUGRELEASE}

#unix|win32: LIBS += -ldbghelp

win32 {
QMAKE_CXXFLAGS += -D_WIN32_WINNT=0x600
QMAKE_CXXFLAGS += /bigobj
}
#QMAKE_CXXFLAGS += -fpermissive

DEFINES += CROSS_COMPILER_IF
LIBS += \
	-L$$PWD/lib \
	-L$${BINDIR} \
	-L"."



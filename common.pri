CONFIG += c++17

# Reset Qt modules first
QT += concurrent

lessThan(QT_MAJOR_VERSION, 6) {
    QT -= core5compat  # Ensure we're not using Qt6 compatibility module
}

GIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
DEFINES += USE_SPDLOG

DEBUGRELEASE = what
CONFIG(debug, debug|release) {
    DEBUGRELEASE = debug
}
else {
    DEBUGRELEASE = release
}

# Dependencies configuration
DEPEND_REPOS = $$PWD/../../dependRepos
SPDLOG_DIR = $$DEPEND_REPOS/spdlog/include
PARSER_DIR = $$DEPEND_REPOS/exprtk
ALG_LIB_DIR = $$DEPEND_REPOS/AlgLib/src

COMMON_DIR = $$PWD

INCLUDEPATH += $${SPDLOG_DIR}
INCLUDEPATH += $${ALG_LIB_DIR}
INCLUDEPATH += $${PARSER_DIR}  # ExprTK parser

# New build directory structure
BUILD_ROOT = $$PWD/../build/$${DEBUGRELEASE}

# Library and binary configuration
contains(TEMPLATE, lib) {
    contains(CONFIG, plugin) {
        DESTDIR = $${BUILD_ROOT}/bin/plugins/$$TARGET
    } else {
        DESTDIR = $${BUILD_ROOT}/lib
    }
} else {
    DESTDIR = $${BUILD_ROOT}/bin
}

# Make sure libraries are found
LIBS += -L$${BUILD_ROOT}/lib -L$${BUILD_ROOT}/bin -L$${BUILD_ROOT}/bin/plugins

# Generated files directories (moc, ui, objects)
MOC_DIR = $${BUILD_ROOT}/generated/moc
UI_DIR = $${BUILD_ROOT}/generated/ui
OBJECTS_DIR = $${BUILD_ROOT}/generated/obj
RCC_DIR = $${BUILD_ROOT}/generated/rcc

win32 {
    # Applies to *all* Windows toolchains
    QMAKE_CXXFLAGS += -D_WIN32_WINNT=0x0600

    # ---------- MinGW / g++ ----------
    win32-g++ {
        QMAKE_CXXFLAGS -= /bigobj
        QMAKE_CXXFLAGS += -Wa,-mbig-obj
    }

    # ---------- MSVC or clang‑cl ----------
    win32-msvc*|win32-clang-msvc {
        QMAKE_CXXFLAGS += /bigobj
        QMAKE_CXXFLAGS += -Zc:templateScope
        QMAKE_CXXFLAGS += -permissive-
    }
}

DEFINES += CROSS_COMPILER_IF
LIBS += -L"."

#unix|win32: LIBS += -ldbghelp

#SOURCES += $$PWD/global.cpp
#HEADERS += $$PWD/core/log.h

#QMAKE_CXXFLAGS += -fpermissive



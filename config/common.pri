
CONFIG -= qt
CONFIG += c++11
CONFIG += c++17

PRECOMPILED_HEADER = stable.h

INCLUDEPATH += .
INCLUDEPATH += ../common/include
INCLUDEPATH += ../common/include/corelib
INCLUDEPATH += ../common/include/guilib

THIRD_PATH = $(SolutionDir)3rd
INCLUDEPATH += $${THIRD_PATH}/include

QMAKE_LIBDIR += ../lib/$(Configuration)
QMAKE_LIBDIR += ../bin/$(Configuration)

DESTDIR = ../bin/$(Configuration)
OBJECTS_DIR = ../bin/objdir/$(ProjectName)/$(Configuration)

DEFINES += STORM_CORE_STATIC

QMAKE_CXXFLAGS += /await
QMAKE_CXXFLAGS += /std:c++latest

QMAKE_CFLAGS_RELEASE += /MT
QMAKE_CXXFLAGS_RELEASE += /MT
QMAKE_CFLAGS_DEBUG += /MTd
QMAKE_CXXFLAGS_DEBUG += /MTd
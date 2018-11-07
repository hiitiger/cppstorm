
TEMPLATE = app
DEPENDPATH += .
LIBS += corelib.lib
LIBS += applib.lib
LIBS += guilib.lib

include(../config/common.pri)

HEADERS += extest.h
HEADERS += more.h

SOURCES += main.cpp
SOURCES += more.cpp

HEADERS += $$files(common/*.h)
SOURCES += $$files(common/*.cpp)

HEADERS += $$files(cpp-go-chan/*.h)

HEADERS += $$files(bookmark/*.h)
SOURCES += $$files(bookmark/*.cpp)

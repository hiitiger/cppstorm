
TEMPLATE = app
DEPENDPATH += .
LIBS += corelib.lib
LIBS += applib.lib
LIBS += guilib.lib

include(../config/common.pri)

SOURCES += main.cpp
HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp)

HEADERS += $$files(common/*.h)
SOURCES += $$files(common/*.cpp)


HEADERS += $$files(bookmark/*.h)
SOURCES += $$files(bookmark/*.cpp)

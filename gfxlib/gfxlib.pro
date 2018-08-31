
TEMPLATE = lib
DEPENDPATH += .
LIBS += corelib.lib
LIBS += applib.lib
CONFIG += staticlib


include(../config/common.pri)


HEADERS += $$files(core/*.h)
SOURCES += $$files(core/*.cpp)

HEADERS += $$files(graphics/*.h)
SOURCES += $$files(graphics/*.cpp)


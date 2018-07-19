
TEMPLATE = lib
DEPENDPATH += .

LIBS += corelib.lib
LIBS += applib.lib

CONFIG += staticlib

include(../config/common.pri)

HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp)

HEADERS += $$files(core/*.h)
SOURCES += $$files(core/*.cpp)

HEADERS += $$files(tinygui/*.h)
SOURCES += $$files(tinygui/*.cpp)

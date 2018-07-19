
TEMPLATE = lib
DEPENDPATH += .

CONFIG += staticlib

include(../config/common.pri)

HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp)


HEADERS += $$files(gfxbasic/*.h)
SOURCES += $$files(gfxbasic/*.cpp)

HEADERS += $$files(app/*.h)
SOURCES += $$files(app/*.cpp)

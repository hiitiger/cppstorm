
TEMPLATE = lib
DEPENDPATH += .
LIBS += corelib.lib
LIBS += applib.lib
CONFIG += staticlib


include(../config/common.pri)

HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp)


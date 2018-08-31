
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

INCLUDEPATH += ./c11
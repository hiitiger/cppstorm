
TEMPLATE = app
DEPENDPATH += .
LIBS += corelib.lib
LIBS += applib.lib
LIBS += guilib.lib

include(../config/common.pri)

SOURCES += main.cpp
HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp)

INCLUDEPATH += ./c11
#-------------------------------------------------
#
#
#-------------------------------------------------


include (../visualsc.pri)

QT       += core gui opengl

TARGET = scgraphicsview
TEMPLATE = lib

scgraphicsview.depends = scdatamodel

$QMAKE_CLEAN += $$TARGET

DEFINES += SCGRAPHICSVIEW_LIBRARY


INCLUDEPATH += ../scdatamodel
win32:LIBS += $${LIB_PATH}/scdatamodel.dll
unix:LIBS += -L$${LIB_PATH} -lscdatamodel


SOURCES += scgraphicsview.cpp \ 
    stateboxgraphic.cpp \
    selectablelinesegmentgraphic.cpp \
    cornergrabber.cpp \
    selectableboxgraphic.cpp \
    selectabletextblock.cpp \
    textformattoolbar.cpp \
    texteditbox.cpp \
    buttongraphic.cpp


HEADERS += scgraphicsview.h\
        scgraphicsview_global.h \ 
    stateboxgraphic.h \
    selectablelinesegmentgraphic.h \
    cornergrabber.h \
    selectableboxgraphic.h \
    selectabletextblock.h \
    textformattoolbar.h \
    texteditbox.h \
    buttongraphic.h


OTHER_FILES += \
    COPYING.txt

RESOURCES += \
    resources/scgraphicsview.qrc

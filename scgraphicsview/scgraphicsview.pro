#-------------------------------------------------
#
#
#-------------------------------------------------


include (../visualsc.pri)

QT       += core gui

TARGET = scgraphicsview
TEMPLATE = lib

scgraphicsview.depends = scdatamodel sceditcontroller

DEFINES += SCGRAPHICSVIEW_LIBRARY

INCLUDEPATH += ../sceditcontroller


INCLUDEPATH += ../scdatamodel
win32:LIBS += $${LIB_PATH}/scdatamodel.dll
unix:LIBS += -L$${LIB_PATH} -lscdatamodel


SOURCES += scgraphicsview.cpp \ 
    stateboxgraphic.cpp \
    selectablelinesegmentgraphic.cpp \
    cornergrabber.cpp

HEADERS += scgraphicsview.h\
        scgraphicsview_global.h \ 
    stateboxgraphic.h \
    selectablelinesegmentgraphic.h \
    cornergrabber.h

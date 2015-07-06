#-------------------------------------------------
#
# Project created by QtCreator 2010-12-30T21:44:22
#
#-------------------------------------------------

include (../visualsc.pri)

CONFIG += link_prl

QT       += core gui widgets

TARGET = visualsc
TEMPLATE = app

QMAKE_CLEAN += visualsc

INCLUDEPATH += ../scdatamodel
win32:LIBS += $${LIB_PATH}/scdatamodel.lib
unix:LIBS += -L$${LIB_PATH} -lscdatamodel


INCLUDEPATH += ../scgraphicsview
win32:LIBS += $${LIB_PATH}/scgraphicsview.lib
unix:LIBS += -L$${LIB_PATH} -lscgraphicsview


INCLUDEPATH += ../scformview
win32:LIBS += $${LIB_PATH}/scformview.lib
unix:LIBS += -L$${LIB_PATH} -lscformview

SOURCES += \
    smproject.cpp \
    mainwindow.cpp \
    main.cpp

HEADERS  += \
    smproject.h \
    mainwindow.h

FORMS    += \
    mainwindow.ui

OTHER_FILES += \
    design.pptx \ 
    COPYING.txt

RESOURCES +=

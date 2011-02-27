#-------------------------------------------------
#
# Project created by QtCreator 2010-12-30T21:44:22
#
#-------------------------------------------------

include (../visualsc.pri)

CONFIG += link_prl

QT       += core gui

TARGET = visualsc
TEMPLATE = app

QMAKE_CLEAN += visualsc

INCLUDEPATH += ../scdatamodel
win32:LIBS += $${LIB_PATH}/scdatamodel.dll
unix:LIBS += -L$${LIB_PATH} -lscdatamodel

INCLUDEPATH += ../sceditcontroller
win32:LIBS += $${LIB_PATH}/sceditcontroller.dll
unix:LIBS += -L$${LIB_PATH} -lsceditcontroller

INCLUDEPATH += ../scgraphicsview
win32:LIBS += $${LIB_PATH}/scgraphicsview.dll
unix:LIBS += -L$${LIB_PATH} -lscgraphicsview

SOURCES += \
    smproject.cpp \
    projectdata.cpp \
    mainwindow.cpp \
    main.cpp \
    formeditorwindow.cpp

HEADERS  += \
    smproject.h \
    projectdata.h \
    mainwindow.h \
    formeditorwindow.h

FORMS    += \
    mainwindow.ui

OTHER_FILES += \
    design.pptx 

RESOURCES += \
    resources/formeditorwindow.qrc

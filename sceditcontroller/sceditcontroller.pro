#-------------------------------------------------
#
#
#-------------------------------------------------


include (../visualsc.pri)

QT       -= gui

TARGET = sceditcontroller 
TEMPLATE = lib


sceditcontroller.depends = scdatamodel

INCLUDEPATH += ../scdatamodel
win32:LIBS += $${LIB_PATH}/scdatamodel.dll
unix:LIBS += -L$${LIB_PATH} -lscdatamodel

DEFINES += SCEDITCONTROLLER_LIBRARY

SOURCES += sceditcontroller.cpp 

HEADERS += sceditcontroller.h\
        sceditcontroller_global.h 

#-------------------------------------------------
#
#
#-------------------------------------------------


include (../visualsc.pri)

QT       += core gui

TARGET = scformview
TEMPLATE = lib

scgraphicsview.depends = scdatamodel

$QMAKE_CLEAN += $$TARGET

DEFINES += SCGRAPHICSVIEW_LIBRARY


INCLUDEPATH += ../scdatamodel
win32:LIBS += $${LIB_PATH}/scdatamodel.dll
unix:LIBS += -L$${LIB_PATH} -lscdatamodel


OTHER_FILES += \
    COPYING.txt \
    resources/underline.png \
    resources/transitionbutton.bmp \
    resources/textpointer.png \
    resources/statebutton.bmp \
    resources/sendtoback.png \
    resources/pointer.png \
    resources/linepointer.png \
    resources/linecolor.png \
    resources/italic.png \
    resources/floodfill.png \
    resources/delete.png \
    resources/bringtofront.png \
    resources/bold.png

HEADERS += \
    scformview.h \
    customtreewidgetitem.h

SOURCES += \
    scformview.cpp \
    customtreewidgetitem.cpp

RESOURCES += \
    resources/scformview.qrc

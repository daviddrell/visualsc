#-------------------------------------------------
#
# Project created by QtCreator 2011-02-01T03:29:38
#
#-------------------------------------------------


include (../visualsc.pri)

QT       += gui
QT       += widgets

TARGET = scdatamodel
TEMPLATE = lib

QMAKE_CLEAN += $$TARGET

DEFINES += SCDATAMODEL_LIBRARY

SOURCES += scdatamodel.cpp \
    transitionattributes.cpp \
    stateattributes.cpp \
    sctransition.cpp \
    scstate.cpp \
    scxmlreader.cpp \
    iattribute.cpp \
    textblockattributes.cpp \
    sizeattribute.cpp \
    positionattribute.cpp \
    idattribute.cpp \
    scitem.cpp \
    textblock.cpp \
    codewriter.cpp \
    cwstate.cpp \
    cwtransition.cpp \
    cwstatemachine.cpp \
    sctransitionbranch.cpp \
    scforkedtransition.cpp \
    cwevent.cpp \
    cwaction.cpp

HEADERS += scdatamodel.h\
    SCDataModel_global.h \
    transitionattributes.h \
    stateattributes.h \
    sctransition.h \
    scstate.h \
    scxmlreader.h \
    iattribute.h \
    textblockattributes.h \
    sizeattribute.h \
    positionattribute.h \
    textblock.h \
    idattribute.h \
    scitem.h \
    codewriter.h \
    cwstate.h \
    cwtransition.h \
    cwstatemachine.h \
    sctransitionbranch.h \
    scforkedtransition.h \
    cwevent.h \
    cwaction.h

OTHER_FILES += \
    COPYING.txt

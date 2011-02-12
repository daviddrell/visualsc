#-------------------------------------------------
#
# Project created by QtCreator 2011-02-01T03:29:38
#
#-------------------------------------------------


include (../visualsc.pri)

QT       -= gui

TARGET = scdatamodel
TEMPLATE = lib

DEFINES += SCDATAMODEL_LIBRARY

SOURCES += scdatamodel.cpp \
    transitionattributes.cpp \
    stateattributes.cpp \
    sctransition.cpp \
    scstate.cpp \
    scxmlreader.cpp

HEADERS += scdatamodel.h\
        SCDataModel_global.h \
    transitionattributes.h \
    stateattributes.h \
    sctransition.h \
    scstate.h \
    scxmlreader.h


TEMPLATE= subdirs

include (visualsc.pri)

CONFIG += ordered

SUBDIRS = scdatamodel\
          sceditcontroller\
          scgraphicsview\
          app

app.depends = scdatamodel  scgraphicsview sceditcontroller

TARGET = app

OTHER_FILES += \
    visualsc/design.pptx 

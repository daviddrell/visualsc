
include (visualsc.pri)

TEMPLATE= subdirs

CONFIG += ordered

SUBDIRS = scdatamodel\
          sceditcontroller\
          scgraphicsview\
          app

app.depends = scdatamodel  scgraphicsview sceditcontroller

OTHER_FILES += \
    visualsc/design.pptx 

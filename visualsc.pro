
TEMPLATE= subdirs

include (visualsc.pri)

CONFIG += ordered

SUBDIRS = scdatamodel\
          scgraphicsview\
          app

app.depends = scdatamodel  scgraphicsview 

TARGET = app

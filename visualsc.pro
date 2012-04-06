
TEMPLATE= subdirs

include (visualsc.pri)

CONFIG += ordered

SUBDIRS = scdatamodel\
          scgraphicsview\
          scformview\
          app


app.depends = scdatamodel  scgraphicsview scformview

TARGET = app

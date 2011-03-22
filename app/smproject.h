#ifndef SMPROJECT_H
#define SMPROJECT_H

#include <QGraphicsView>
#include <QString>
#include "projectdata.h"
#include "scstate.h"
#include <QObject>
#include "scgraphicsview.h"
#include <QTimer>
#include "scdatamodel.h"



class SelectableLineSegmentGraphic;

class StateAttributes;

class SMProject : public QObject
{
    Q_OBJECT

public:
    SMProject(QWidget *parentWidget);
    ~SMProject();


    void close();

    void readInputFile(QString file);
    void save(QString file);
    QGraphicsView * getQGraphicsView();
    SCDataModel  * getDM();
signals:
    void error(QString err);
    void readInputFileCompleted (bool,QStringList );

private slots:
    void handleOpenCompleted (bool,QStringList );

private:

    // private data
    ProjectData     _pd;

    QString         _file;

    SCDataModel      _dm;
    SCGraphicsView   _graphicsView;



    private:

    // private methods


};

#endif // SMPROJECT_H

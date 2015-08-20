/*
    visualsc - Visual State Chart Editing for SCXML State Engines
    Copyright (C) 2011  David W. Drell (david@davidwdrell.net) and
    Contributing Authors:


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SMPROJECT_H
#define SMPROJECT_H

#include <QGraphicsView>
#include <QString>
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

    void initNewSM();

    void close();

    void readInputFile(QString file);
    void save(QString file);
    void exportToCode(QString file);
    QGraphicsView * getQGraphicsView();
    SCDataModel  * getDM();

    SCGraphicsView* getSCGraphicsView();

signals:
    void error(QString err);
    void readInputFileCompleted (bool,QStringList );

private slots:
    void handleOpenCompleted (bool,QStringList );

private:

    // private data

    QString         _file;
    SCDataModel      *_dm;
    SCGraphicsView   * _graphicsView;
    QWidget          * _parentWidget;



    private:

    // private methods


};

#endif // SMPROJECT_H

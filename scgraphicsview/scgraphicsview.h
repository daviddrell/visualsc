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

#ifndef SCGRAPHICSVIEW_H
#define SCGRAPHICSVIEW_H

#include "scgraphicsview_global.h"
#include "scdatamodel.h"
#include <QObject>
#include <QGraphicsView>
#include "stateattributes.h"
#include "stateboxgraphic.h"
#include "selectablelinesegmentgraphic.h"

#include <QMap>


class SCGRAPHICSVIEWSHARED_EXPORT SCGraphicsView : QWidget
{
      Q_OBJECT


public:
    SCGraphicsView(QWidget *parent, SCDataModel * dm);

    QGraphicsView * getQGraphicsView();

signals:

private slots:

    void handleNewState(SCState * newState);
    void handleNewTransition(SCTransition*);

private:

    // private data

    QGraphicsScene  _scene;
    QGraphicsView   _view;
    SCDataModel    *_dm;
    QMap<SCState*,StateBoxGraphic*> _mapStateToGraphic;


    //  private methods

    void increaseSizeOfAllAncestors (SCState * state);

    SCState * lookUpTargetState(QString target);


};

#endif // SCGRAPHICSVIEW_H

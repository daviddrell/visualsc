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
#include "keycontroller.h"
#include <QObject>
#include <QGraphicsView>
#include "stateattributes.h"
#include "stateboxgraphic.h"
#include "transitiongraphic.h"
#include "textformattoolbar.h"
#include "mousecontroller.h"
#include <QMap>

/**
  \defgroup GraphicsView

  The GraphicsView is the core of the state chart VisualSC program. It allows the user to graphical manipulate the chart.

  */


/**
  * \class SCGraphicsView
  * \brief The is the primary QWidget which holds the view, and the scene.
  * \ingroup GraphicsView
  */

class SCGRAPHICSVIEWSHARED_EXPORT SCGraphicsView : QWidget
{
      Q_OBJECT


public:
    SCGraphicsView(QWidget *parent, SCDataModel * dm);
    ~SCGraphicsView();

    QGraphicsView * getQGraphicsView();

signals:

private slots:

    void handleNewImportedMachine(SCState*);
    void handleRootMachineIsParallelChanged(StateString*);
    void handleNewRootMachine(SCState*);
    void handleAutoResize(StateBoxGraphic*);
    void handleNewState(SCState * newState);
    void handleNewTransition(SCTransition*);
    void handleStateDeleted(QObject *state);
    void handleTransitionDeleted(QObject* tr);
    void handleStateGraphicDeleted(QObject *sg);
    void handleTransitionGraphicDeleted(QObject *tg);
    void handleNewTextBlock(SCTransition*, QString);
    //void handleStatePositionChangedInDataModel(SCState* state, QPointF position);
    void handleStatePositionChangedInFormView(SCState* state, QPointF position);
    void handleStateSizeChangedInFormView(SCState* state, QPointF size);
    void handleMakeTransitionConnections(SCTransition*);
    void handleNewTransitionFormView(SCTransition* t);
    void handleChangedParent(SCState* state, SCState* newParent);
    void handleBringToFront(SCState* state);
    void handleSendToBack(SCState* state);

private:

    void connectState(SCState*, StateBoxGraphic*);
    void connectState(SCState*);
    void connectTransition(SCTransition*);

    qreal distance(QPointF a, QPointF b);
    // private data

    QGraphicsScene  *_scene;
    QGraphicsView   _view;
    SCDataModel    *_dm;
    QMap<SCState*,StateBoxGraphic*> _mapStateToGraphic;
    QMap<SCTransition*,TransitionGraphic *> _mapTransitionToGraphic;

    QHash<SCState*, StateBoxGraphic*> _hashStateToGraphic;
    QHash<SCTransition*, TransitionGraphic* > _hashTransitionToGraphic;

    KeyController *_keyController;
    MouseController* _mouseController;

    bool _rightAngleMode;

    //  private methods

    void increaseSizeOfAllAncestors (SCState * state);

    SCState * lookUpTargetState(QString target);
    StateBoxGraphic * lookUpTargetStateGraphic(QString stateId );
    void createGraph();

    bool eventFilter(QObject* o, QEvent* e);
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );///< allows the main object to be moved in the scene by capturing the mouse move events
   // virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );// [virtual protected]

};

#endif // SCGRAPHICSVIEW_H

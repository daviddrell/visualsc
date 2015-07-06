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

#include "scgraphicsview.h"
#include "sctransition.h"
#include "transitionattributes.h"
#include "selectablelinesegmentgraphic.h"
#include <QList>
#include <QDebug>
#include <QGLWidget>
#include <QLabel>
#include <QMessageBox>

SCGraphicsView::SCGraphicsView(QWidget *parentWidget, SCDataModel * dm) :
        QWidget (parentWidget),
        _scene(new QGraphicsScene(this)),
        _view(parentWidget),
        _dm(dm),
        _mapStateToGraphic()
       // KeyController
      //key controller class

{
    _dm->setScene( _scene);

    connect (_dm, SIGNAL(newStateSignal(SCState*)), this, SLOT(handleNewState(SCState*)));
    connect (_dm, SIGNAL(newTransitionSignal(SCTransition*)), this, SLOT(handleNewTransition(SCTransition*)));

    //using openGL

    _view.setViewport( new QGLWidget (QGLFormat(QGL::SampleBuffers) ));
    _view.setScene( _scene);
    _view.show();

    createGraph();

}


SCGraphicsView::~SCGraphicsView()
{
    delete _scene ;
}

void SCGraphicsView::createGraph()
{
    QList<SCState*> states;
    _dm->getAllStates(states);

    for(int s = 0; s < states.count(); s++)
    {
        handleNewState( states[s]);

        QList<SCTransition*> transitions;
        states[s]->getTransitions(transitions);

        for(int t = 0; t < transitions.count(); t ++)
        {
            handleNewTransition(transitions[t]);
        }
    }
}


QGraphicsView * SCGraphicsView::getQGraphicsView()
{
    return & _view;
}


void SCGraphicsView::increaseSizeOfAllAncestors (SCState * state)
{
    SCState * parentState = dynamic_cast<SCState *>(state->parent());

    if ( parentState )
    {
        StateBoxGraphic * parentGraphic =   _mapStateToGraphic[parentState];
        StateBoxGraphic * childGraphic =   _mapStateToGraphic[state];

        if ( parentGraphic )
        {
            // is the parent bigger than the new child?
            // make sure the parent is big enough to hold all the states

            QPointF parentSize = parentGraphic->getSize();
            QPointF childSize = childGraphic->getSize();
            if ( childSize.x() >= parentSize.x() )
            {
                parentSize.setX( parentSize.x() * 1.2 );
                parentGraphic->setSize( parentSize );
            }
            if ( childSize.y() >= parentSize.y() )
            {
                parentSize.setY( parentSize.y() * 1.2 );
                parentGraphic->setSize( parentSize );
            }
            increaseSizeOfAllAncestors (parentState);
        }
    }

}


StateBoxGraphic * SCGraphicsView::lookUpTargetStateGraphic(QString stateId )
{
    SCState * stateDM = lookUpTargetState(stateId);
    if ( stateDM == NULL ) return NULL;

    return _mapStateToGraphic[stateDM];

}


SCState * SCGraphicsView::lookUpTargetState(QString target )
{

    QList<SCState *> states;
    _dm->getAllStates(states);

    QList<SCState *>::iterator i;
    for (i = states.begin(); i != states.end(); ++i)
    {
        SCState *st = *i;
        StateAttributes * attr = dynamic_cast<StateAttributes * > (st->getAttributes());

        if ( attr->value("name") == NULL ) return NULL;

        if (attr->value("name")->asString() == target)
            return st;
    }

    return NULL;
}

void SCGraphicsView::handleTransitionGraphicDeleted(QObject *tg)
{

    TransitionGraphic* trans = (TransitionGraphic*) tg;

    QMapIterator<SCTransition*,TransitionGraphic*> i(_mapTransitionToGraphic);
    while (i.hasNext())
    {
        i.next();

        if ( i.value() == trans)
        {
            _mapTransitionToGraphic.remove(i.key());
        }
    }
}

void SCGraphicsView::handleTransitionDeleted(QObject* tr)
{
    // use a direct cast because dynamic_cast<SCTransition*> will not result in a
    // SCTransition because the SCTransition has already been destructed
    // and we are getting the destructor signal on the base QObject clas

    SCTransition * trans = (SCTransition *)tr;

    QMapIterator<SCTransition*,TransitionGraphic*> i(_mapTransitionToGraphic);
    while (i.hasNext())
    {
        i.next();

        if ( i.key() == trans)
        {
            delete i.value(); //delete the transition graphic associated with this transition
            _mapTransitionToGraphic.remove(i.key());
        }
    }

}

void SCGraphicsView::handleNewTransition (SCTransition *t)
{
    // get the parent state graphic
    SCState *parentState = dynamic_cast<SCState *>(t->parent());
    StateBoxGraphic * parentGraphic =   _mapStateToGraphic[parentState];
    if ( parentGraphic == NULL)
    {
        // no parent graphic means the user is adding a transition to the root machine
        QMessageBox msgBox;
        msgBox.setText("cannot add a transition from the root machine");
        msgBox.exec();
        return;
    }

    connect(t, SIGNAL(destroyed(QObject*)), this, SLOT(handleTransitionDeleted(QObject*)));

     // create a transition graphic
    TransitionGraphic * transGraphic  = 0;
    TransitionAttributes::TransitionPositionAttribute * pos =
            dynamic_cast<TransitionAttributes::TransitionPositionAttribute *> (  t->attributes.value("position"));
    QPointF position(0,0);
    if ( pos == 0 )
        qDebug()<< "pos returned null in SCGraphicsView::handleNewTransition";
    else
        position = pos->asQPointF();

    TransitionAttributes::TransitionStringAttribute *targetName = dynamic_cast<TransitionAttributes::TransitionStringAttribute *>(t->attributes.value("target"));
    StateBoxGraphic * targetGraphic  = lookUpTargetStateGraphic( targetName->asString() );
    transGraphic = new TransitionGraphic(parentGraphic, targetGraphic,  t );
    connect(transGraphic, SIGNAL(destroyed(QObject*)), this, SLOT(handleTransitionGraphicDeleted(QObject*)));
    _mapTransitionToGraphic.insert(t, transGraphic);
}

void SCGraphicsView::handleStateDeleted(QObject *state)
{
    // use a direct cast because dynamic_cast<State*> will not result in a
    // state because the state has already been destructed
    // and we are getting the destructor signal on the base QObject clas

    SCState * st = (SCState*)state;

    QMapIterator<SCState*,StateBoxGraphic*> i(_mapStateToGraphic);
    while (i.hasNext())
    {
        i.next();
        if ( i.key() == st)
        {
            delete i.value();// delete the graphic and all its children
            _mapStateToGraphic.remove(i.key());
        }
    }

}


void SCGraphicsView::handleStateGraphicDeleted(QObject *st)
{
    QMapIterator<SCState*,StateBoxGraphic*> i(_mapStateToGraphic);
    while (i.hasNext())
    {
        i.next();

        if ( i.value() == (StateBoxGraphic*)st)
            _mapStateToGraphic.remove(i.key());
    }
}


void SCGraphicsView::handleNewState (SCState *newState)
{

    connect(newState, SIGNAL(destroyed(QObject*)), this, SLOT(handleStateDeleted(QObject*)));

    StateString * type = dynamic_cast<StateString *> ( newState->attributes.value("type"));
    if ( type != 0 && (type->asString() == "machine"))
        return; // don't draw a picture for the top-level machine

    SCState * parentState = 0;
    StateBoxGraphic * parentGraphic =0;
    StateBoxGraphic * stateGraphic = 0;

    parentState = dynamic_cast<SCState *>(newState->parent());

    if (_mapStateToGraphic.contains(parentState))
    {
        parentGraphic =   _mapStateToGraphic[parentState];
    }

    // after the reference has been set through the construction
    // of the stateboxgraphic, the graphic and the model
    // will be linked internally. e.g. setting the size of the graphic
    // will set the size of the model and vice versa

    stateGraphic = new StateBoxGraphic( parentGraphic, newState);
    stateGraphic->setObjectName(newState->objectName());
    //stateGraphic->setPos(newState->attributes.value("position"));
    //stateGraphic->setSize(newState->attributes.value("size"));
    connect(stateGraphic, SIGNAL(destroyed(QObject*)), this, SLOT(handleStateGraphicDeleted(QObject*)));

    // quick look up of graphics from state model references

    _mapStateToGraphic.insert(newState, stateGraphic);



    // load size and position from state model
    PositionAttribute * position = dynamic_cast<PositionAttribute*> (newState->attributes.value("position"));
    stateGraphic->setPos(position->asPointF());
    //qDebug() << "pos : " << position->asString();

    SizeAttribute * size = dynamic_cast<SizeAttribute*> (newState->attributes.value("size"));
    stateGraphic->setSize(size->asPointF());


    if ( ! parentGraphic )
    {
        _scene->addItem(stateGraphic);
    }

    // make the parent bigger to hold this state
    // and since its bigger, make its parent bigger.....
    increaseSizeOfAllAncestors (newState);

}

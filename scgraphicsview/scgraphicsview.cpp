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
#include "keycontroller.h"
#include "selectableboxgraphic.h"
#include "mousecontroller.h"
#include <QList>
#include <QDebug>
#include <QGLWidget>
#include <QLabel>
#include <QMessageBox>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

SCGraphicsView::SCGraphicsView(QWidget *parentWidget, SCDataModel * dm) :
    QWidget (parentWidget),
    _scene(new QGraphicsScene(this)),
    _view(parentWidget),
    _dm(dm),
    _mapStateToGraphic(),
    _keyController(new KeyController()),
    _mouseController(new MouseController())


{
    _dm->setScene( _scene);
    // initialize key controller



    connect (_dm, SIGNAL(newStateSignal(SCState*)), this, SLOT(handleNewState(SCState*)));
    connect (_dm, SIGNAL(newTransitionSignal(SCTransition*)), this, SLOT(handleNewTransition(SCTransition*)));

    //connect(_dm, SIGNAL(newTextBlockSignal(SCTransition*,QString)), this, SLOT(handleNewTextBlock(SCTransition,QString)));

    //using openGL

    _view.setViewport( new QGLWidget (QGLFormat(QGL::SampleBuffers) ));
    _view.setScene( _scene);
    _view.show();

    createGraph();

    _scene->installEventFilter(this);
    //this->setMouseTracking(true);



}

SCGraphicsView::~SCGraphicsView()
{
    delete _scene ;
}

bool SCGraphicsView::eventFilter(QObject* o, QEvent * e)
{
    // scene captures key presses in KeyController Object
    // the key controller object is passed to the transition graphic and its segmented lines
    // segmented lines that are hovered will connect to the transition graphic's handle key event
    if(e->type()==QEvent::KeyPress)
    {
        //qWarning()<<"The bad guy which steals the keyevent is "<<o;
        //qWarning()<<"the focus item is "<< _scene->focusItem();
        //qDebug()<< "Key Press SCGraphicsView: " << _keyController->getLastKeyEvent();
        QKeyEvent *key = static_cast<QKeyEvent*>(e);
        _keyController->keyInput(key);
    }
    else if(e->type()==QEvent::GraphicsSceneMouseMove)  // track mouse movement in mouse controller
    {
        //qDebug()<<"mouse moved in event filter";
        QGraphicsSceneMouseEvent* qgs = static_cast<QGraphicsSceneMouseEvent*>(e);
        _mouseController->mouseInput(qgs);
        //_mouseController->printPos();
    }
    return false;
}

// for supporting moving the box across the scene
void SCGraphicsView::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    qDebug()<<"mouse moved in moveEvent";
}

/**
 * @brief SCGraphicsView::createGraph
 * add all states and transitions to the graphics view
 */
void SCGraphicsView::createGraph()
{
    QList<SCState*> states;
    _dm->getAllStates(states);

    // first load all states
    for(int s = 0; s < states.count(); s++)
    {
        handleNewState( states[s]);
    }

    // load all transitions
    for(int i = 0; i< states.count(); i++)
    {
        QList<SCTransition*> transitions;
        states[i]->getTransitions(transitions);

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

/**
 * @brief SCGraphicsView::lookUpTargetStateGraphic
 * @param stateId   string of the statebox's name
 * @return the statebox graphic whose name is is the passed parameter
 */
StateBoxGraphic * SCGraphicsView::lookUpTargetStateGraphic(QString stateId )
{
    SCState * stateDM = lookUpTargetState(stateId);
    if ( stateDM == NULL ) return NULL;


    //qDebug()<< "The target state graphic was found. "<< stateDM->objectName();
    StateBoxGraphic* ret = _mapStateToGraphic.find(stateDM).value();
    //qDebug() << "did we find anything? " << ret->objectName();

    return _mapStateToGraphic.find(stateDM).value();


}

/**
 * @brief SCGraphicsView::lookUpTargetState
 * @param target
 * @return
 *
 * returns the SCState from the data model by its name found in the "name" attribute
 */
SCState* SCGraphicsView::lookUpTargetState(QString target )
{

    QList<SCState *> states;
    _dm->getAllStates(states);

    QList<SCState *>::iterator i;
    for (i = states.begin(); i != states.end(); ++i)
    {
         SCState *st = *i;
        StateAttributes * attr = dynamic_cast<StateAttributes * > (st->getAttributes());

        //qDebug() << "Look up target states " << attr->value("name")->asString();


        if ( attr->value("name") == NULL ) return NULL;


        if(target.compare(attr->value("name")->asString()) == 0){
            //qDebug() << "target state found ! $$$ " << target << " vs " << attr->value("name")->asString();
            return st;
        }
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

/**
 * @brief SCGraphicsView::handleNewTransition
 * @param t
 *
 * Creation of a new transition graphic from the user creating a new transition or a transition being loaded in from the scxml file.
 */
void SCGraphicsView::handleNewTransition (SCTransition * t)
{
    // get the parent state graphic
    SCState *parentState = dynamic_cast<SCState *>(t->parent());
    StateBoxGraphic * parentGraphic =   _mapStateToGraphic[parentState];
    if ( parentGraphic == NULL )
    {
        // no parent graphic means the user is adding a transition to the root machine
        QMessageBox msgBox;
        msgBox.setText("cannot add a transition from the root machine");
        msgBox.exec();
        return;
    }

    // if the transition is deleted, then call the handleTransitionDeleted function in SCGraphicsView
    connect(t, SIGNAL(destroyed(QObject*)), this, SLOT(handleTransitionDeleted(QObject*)));


    TransitionAttributes::TransitionPositionAttribute * pos =
            dynamic_cast<TransitionAttributes::TransitionPositionAttribute *> (  t->attributes.value("position"));
    QPointF position(0,0);
    if ( pos == 0 )
        qDebug()<< "pos returned null in SCGraphicsView::handleNewTransition";
    else
        position = pos->asQPointF();

    TransitionAttributes::TransitionStringAttribute *targetName = dynamic_cast<TransitionAttributes::TransitionStringAttribute *>(t->attributes.value("target"));
    StateBoxGraphic * targetGraphic  = lookUpTargetStateGraphic( targetName->asString() );
 //   qDebug() << "target Graphic look up: " << targetName->asString();
 //   qDebug() << "target Graphic is set " << targetGraphic->objectName();

    // create a transition graphic
    TransitionGraphic * transGraphic = new TransitionGraphic(parentGraphic, targetGraphic,  t , _keyController,_mouseController);

    // when this transition graphic is deleted, call handleTransitionGraphicDeleted
    connect(transGraphic, SIGNAL(destroyed(QObject*)), this, SLOT(handleTransitionGraphicDeleted(QObject*)));

    // create the connection to automatically move anchor elbows when state graphics are moved.
    connect(parentGraphic, SIGNAL(stateBoxMoved(QPointF)), transGraphic, SLOT(handleParentStateGraphicMoved(QPointF)));
    connect(targetGraphic, SIGNAL(stateBoxMoved(QPointF)), transGraphic, SLOT(handleTargetStateGraphicMoved(QPointF)));



    // create the connect to automatically move anchor elbows when state graphics are moved.
    connect(parentGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleParentStateGraphicResized(QRectF, QRectF, int)));
    connect(targetGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleTargetStateGraphicResized(QRectF, QRectF, int)));

    // connect this state box's grand parents update anchors when they are resized
    StateBoxGraphic* grandParentGraphic = parentGraphic->parentItemAsStateBoxGraphic();
    while(grandParentGraphic)
    {
        connect(grandParentGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleGrandParentStateGraphicResized(QRectF, QRectF, int)));
        grandParentGraphic = grandParentGraphic->parentItemAsStateBoxGraphic();
    }

    // connect the target state box's grand parents to update the target anchors when they are resized
    StateBoxGraphic* grandParentTargetGraphic = targetGraphic->parentItemAsStateBoxGraphic();
    while(grandParentTargetGraphic)
    {

        connect(grandParentTargetGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleGrandParentTargetStateGraphicResized(QRectF, QRectF, int)));
        grandParentTargetGraphic = grandParentTargetGraphic->parentItemAsStateBoxGraphic();
    }


    // add the transitiongraphic to the map of transition graphics
    _mapTransitionToGraphic.insert(t, transGraphic);

    //targetGraphic->
}

/**
 * @brief SCGraphicsView::handleStateDeleted
 * @param state
 *
 * if the SCState is destroyed, delete it from the graphics view.
 */
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
            QList<QGraphicsItem*> children = i.value()->childItems();
            qDebug() << "deleting children for state "<<" with num children: " << children.count();
            for(int i = 0; i < children.count(); i++)
            {
                this->handleStateDeleted( (SCState*)(children.at(i)) );
            }

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


void SCGraphicsView::handleNewTextBlock(SCTransition* trans, QString text)
{
    SCTextBlock* textBlock = new SCTextBlock();
}

/**
 * @brief SCGraphicsView::handleNewState
 * @param newState
 *
 * connected to signal newStateSignal in the SCDataModel
 *
 * will add a state to the graphics view
 *
 */
void SCGraphicsView::handleNewState (SCState *newState)
{
    static int zVal = 0;
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

    // new states will be on top
    stateGraphic->setZValue(zVal++);

    stateGraphic->setObjectName(newState->objectName());
    //stateGraphic->setPos(newState->attributes.value("position"));
    //stateGraphic->setSize(newState->attributes.value("size"));

    // connect the state graphic's deconstructor to do removal protocol in the graphics view
    connect(stateGraphic, SIGNAL(destroyed(QObject*)), this, SLOT(handleStateGraphicDeleted(QObject*)));

    // quick look up of graphics from state model references

   // qDebug() << "inserting new state into mapstate to graphic : " << newState->objectName();
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

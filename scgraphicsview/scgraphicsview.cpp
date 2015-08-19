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
    _hashStateToGraphic(),
    _keyController(new KeyController()), // initialize key controller
    _mouseController(new MouseController()),
    _rightAngleMode(false)



{
    _dm->setScene(_scene);

    connect(_dm, SIGNAL(newStateSignal(SCState*)), this, SLOT(handleNewState(SCState*)));

    // the root machine needs to update its children if isParalle is changed
    connect(_dm, SIGNAL(newRootMachine(SCState*)), this, SLOT(handleNewRootMachine(SCState*)));

    // when the reader is done importing, signal the graphics view
    connect(_dm, SIGNAL(importedMachine(SCState*)), this, SLOT(handleNewImportedMachine(SCState*)));


    // reading transitions from scxml file
    connect(_dm, SIGNAL(newTransitionSignal(SCTransition*)), this, SLOT(handleNewTransition(SCTransition*)));
    connect(_dm, SIGNAL(transitionsReadyToConnect(SCTransition*)),this,SLOT(handleMakeTransitionConnections(SCTransition*)));

    // user inserts a new transition
    connect(_dm, SIGNAL(insertNewTransitionSignal(SCTransition*)), this, SLOT(handleNewTransitionFormView(SCTransition*)));


    //using openGL

//    _view.setViewport(new QWidget());
    _view.setViewport( new QGLWidget (QGLFormat(QGL::SampleBuffers) ));
    _view.setScene(_scene);
    _view.show();

    createGraph();

    _scene->installEventFilter(this);
}

SCGraphicsView::~SCGraphicsView()
{
    delete _scene;
}

/**
 * @brief SCGraphicsView::handleNewImportedMachine
 * @param state
 *
 * SLOT
 * connect in SCGraphicsView construcor
 *
 * when a state machine is imported fully, we call this to automatically resize the state machine to enclose all of its substates
 */
void SCGraphicsView::handleNewImportedMachine(SCState * importedMachine)
{
    StateBoxGraphic* sbg = _hashStateToGraphic.value(importedMachine);
    this->handleAutoResize(sbg);

    // the top level children of this state machine were using scene based coordinates.
    // now they get a parent state box graphic, which automatically maps their coordinates to this new parent state box graphic
    // we also must update the data model with the updated position
    QList<SCState*> directChildren = importedMachine->getStates();
    for(int i = 0; i < directChildren.size(); i++)
    {
        SCState* child = directChildren.at(i);
        StateBoxGraphic* childG = _hashStateToGraphic.value(child);

        QPointF machineMapped = childG->pos();
        child->setPosition(machineMapped);
    }
}

/**
 * @brief SCGraphicsView::handleMakeTransitionConnections
 *
 * SLOT
 * connect in SCGraphicsView
 * connect(_dm , SIGNAL(transitionsReadyToConnect()),this,SLOT(handleMakeTransitionConnections()));
 *
 * this is called by openFile in the datamodel after the reader adds states and transitions to the datamodel from reading a file
 * the transition connections may fail if they are attempted during the creation of the transition graphic because the target state may not have been loaded in when the transition was created.
 *
 * the solution to this problem is creating the connections after all states are made in the data model
 *
 *
 *
 */
void SCGraphicsView::handleMakeTransitionConnections(SCTransition* trans)
{
    qDebug() << "Made it into: SCGraphicsView::handleMakeTransitionConnections for transition:"  << trans->attributes.value("event")->asString();

    TransitionGraphic* transGraphic = _hashTransitionToGraphic.value(trans);

   // TransitionStringAttribute *targetName = dynamic_cast<TransitionStringAttribute *>(trans->attributes.value("target"));

    //SCState* parentState = trans->parentSCState();
    //SCState* targetState = lookUpTargetState(targetName->asString());
    SCState* targetState = trans->targetState();
    StateBoxGraphic * targetGraphic  = _hashStateToGraphic.value(targetState);



    //qDebug() << "targetGraphic: " << targetGraphic->objectName();

    // set the targetState for the transition
    trans->setTargetState(targetState);
    transGraphic->setTargetStateGraphic(targetGraphic);

    // add a transition reference for the target and source
    //targetState->addTransitionReference(trans, SCState::kTransitIn);
    //parentState->addTransitionReference(trans, SCState::kTransitOut);

    connectTransition(trans);
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
    return false; // propogate the event further
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

    // connect the top state
    connectState(_dm->getTopState());

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

/**
 * @brief SCGraphicsView::handleAutoResize
 * @param stateBoxGraphic
 *
 * SLOT
 * connect in connectState of SCGraphicsView
 *
 * this will automatically resize a state box graphic based on if its a state machine or state:
 * state machines will resize themselves to cover all of its children states.
 *
 * states will resize themselves to be the same size as a state in the same state machine, going by earliest added first order.
 */
void SCGraphicsView::handleAutoResize(StateBoxGraphic* stateBoxGraphic)
{
    QList<StateBoxGraphic*> sbgs;
//    stateBoxGraphic->getAllStates(sbgs);
    stateBoxGraphic->getStates(sbgs);
    //QPointF myPos = mapToHighestParent(pos());
    qreal x = 0;
    qreal y = 0;
    qreal newWidth = std::numeric_limits<double>::min();        // new dimensions of the state
    qreal newHeight = std::numeric_limits<double>::min();
    qreal minX = std::numeric_limits<double>::max();            // auto resize uses the smallest top/left distance to any child to determine the extra buffer zone on the bottom and right walls when resizing
    qreal minY = std::numeric_limits<double>::max();

    // if this is a state machine, then use all children to calculate the above variables
    for(int i = 0; i < sbgs.size(); i++)
    {
        StateBoxGraphic* sbg = sbgs.at(i);

        QPointF mts = stateBoxGraphic->mapFromItem(sbg->parentAsSelectableBoxGraphic(), sbg->pos());

        qreal outX = mts.x() + sbg->getSize().x();
        qreal outY = mts.y() + sbg->getSize().y();

        qDebug() << "outX: "<<outX << "\toutY:"<<outY;

        // we must make the new width include the outter most point
        if(x+newWidth < outX)
           newWidth = outX;

        // include outter most y point
        if(y+newHeight < outY)
           newHeight = outY;

        // determine the smallest buffer zone
        if(mts.x() < minX)
            minX = mts.x();

        // determine the smallest buffer zone
        if(mts.y() < minY)
            minY = mts.y();
    }

    // a state will have no children state box graphics
    bool onlyState = false;
    if(sbgs.size()==0)
    {
        onlyState = true;
        // must find another state box graphic that is in the same state machine on the same level.
        SCState* parentState = stateBoxGraphic->getStateModel()->parentAsSCState();
        QList<SCState*> states;
        parentState->getStates(states);
        for(int i = 0 ; i < states.size(); i++)
        {
            StateBoxGraphic* sbg = _hashStateToGraphic.value(states.at(i));
            if(sbg == stateBoxGraphic)
            {

            }
            else    // as soon as we find one that isn't the one being changed, we will mimic the size
            {
                newWidth = sbg->getSize().x();
                newHeight = sbg->getSize().y();
                break;
            }
        }
    }

    // only change the size if the size needs changing
    if(newWidth!=std::numeric_limits<double>::min() || std::numeric_limits<double>::min() != newHeight)
    {
        // if this is a state and not a state machine, we set the size to be the same as one of its sibling states
        if(onlyState)
        {
            QRectF oldBox = QRectF(stateBoxGraphic->pos().x(), stateBoxGraphic->pos().y(), stateBoxGraphic->getSize().x(), stateBoxGraphic->getSize().y());
            QRectF newBox = QRectF(stateBoxGraphic->pos().x(), stateBoxGraphic->pos().y(), newWidth, newHeight);
            stateBoxGraphic->setSize(QPointF(newWidth,newHeight));
            stateBoxGraphic->setCornerPositions();
            emit stateBoxGraphic->stateBoxResized(oldBox, newBox, 2);
        }
        else        // this is a state machine
        {
            // if children states are outside of the state machine from the top or left, then using minX and minY will have undesired results, so reposition all children to be inside of the state machine, then call this function again
            if(minX < 0 || minY < 0)
            {

                for(int i = 0; i < sbgs.size(); i++)
                {
                    StateBoxGraphic* sbg = sbgs.at(i);
                    sbg->setPosAndUpdateAnchors(sbg->pos()-QPointF(minX-25,minY-25));
                }

                // all children are inside of the state machine, so we can call this again
                this->handleAutoResize(stateBoxGraphic);
            }
            else    // all children are in the bottom right of the state machine's origin, so just resize the state machine
            {
                QRectF oldBox = QRectF(stateBoxGraphic->pos().x(), stateBoxGraphic->pos().y(), stateBoxGraphic->getSize().x(), stateBoxGraphic->getSize().y());
                newWidth+=minX;
                newHeight+=minY;
                QRectF newBox = QRectF(stateBoxGraphic->pos().x(), stateBoxGraphic->pos().y(), newWidth, newHeight);
                stateBoxGraphic->setSize(QPointF(newWidth,newHeight));
                stateBoxGraphic->setCornerPositions();
                emit stateBoxGraphic->stateBoxResized(oldBox, newBox, 2);
            }
        }
    }
}

/**
 * @brief SCGraphicsView::increaseSizeOfAllAncestors
 * @param state
 *
 * DEPRECATED FUNCTION
 *
 * this function needs to be changed to modify the data model -> emit changed -> updated graphics view
 *
 */
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

    qDebug()<< "The target state graphic was found. "<< stateDM->objectName();
    StateBoxGraphic* ret = _mapStateToGraphic.find(stateDM).value();
    qDebug() << "did we find anything? " << ret->objectName();

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
            _hashTransitionToGraphic.remove(i.key());
        }
    }
}

void SCGraphicsView::handleTransitionDeleted(QObject* tr)
{
    // use a direct cast because dynamic_cast<SCTransition*> will not result in a
    // SCTransition because the SCTransition has already been destructed
    // and we are getting the destructor signal on the base QObject clas

    SCTransition * trans = (SCTransition *)tr;

    qDebug () <<"SCGraphicsView::handleTransitionDeleted";


    TransitionGraphic* trGraph = _hashTransitionToGraphic.value(trans);

    delete trGraph;

    _mapTransitionToGraphic.remove(trans);
    _hashTransitionToGraphic.remove(trans);



/*
    QMapIterator<SCTransition*,TransitionGraphic*> i(_mapTransitionToGraphic);
    while (i.hasNext())
    {
        i.next();

        if ( i.key() == trans)
        {
            qDebug() << "deleting transition graphic for: " << i.key();
            delete i.value(); //delete the transition graphic associated with this transition
            _mapTransitionToGraphic.remove(i.key());
            _hashTransitionToGraphic.remove(i.key());
        }
    }
*/
}



/**
 * @brief SCGraphicsView::handleNewTransition
 * @param t
 *
 * Creation of a new transition graphic from a transition being loaded from the scxml file.
 * differs from the handleNewTransitionFormView in that this transition may not necessarily know its target state when it is created, so the connectTransition function to set up the connect()s is called after all states have been loaded
 */
void SCGraphicsView::handleNewTransition (SCTransition * t)
{
    SCState* parentState = t->parentSCState();
    StateBoxGraphic* parentGraphic = _hashStateToGraphic.value(parentState);

    // block creating a transition if this is the root machine
    if ( parentGraphic == NULL )
    {
        // no parent graphic means the user is adding a transition to the root machine
        QMessageBox msgBox;
        msgBox.setText("cannot add a transition from the root machine");
        msgBox.exec();
        return;
    }

    // create the transition graphic, stack its deconstructor, and create the link between the SCtransition to its graphic
    TransitionGraphic* transGraphic = new TransitionGraphic(parentGraphic, NULL, t, _keyController, _mouseController);
    //connect(t, SIGNAL(destroyed(QObject*)), this, SLOT(handleTransitionDeleted(QObject*)));
    //connect(transGraphic, SIGNAL(destroyed(QObject*)), this, SLOT(handleTransitionGraphicDeleted(QObject*)));

    // add the transitiongraphic to the map of transition graphics
    _mapTransitionToGraphic.insert(t, transGraphic);
    _hashTransitionToGraphic.insert(t, transGraphic);

    // this is done later after all states are added
//    // set the text position of the event attribute
//    PositionAttribute* textPos = (PositionAttribute*)t->getEventTextBlock()->attributes.value("position");
//    qDebug()<< "handle new transition text pos: " << textPos->asPointF();
//    transGraphic->setTextPos(textPos->asPointF());

    // wait to do connect calls

    qDebug()<<"SCGraphicsView::handleNewTransition inserted a new transition graphic with event name: " << t->attributes.value("event");
}

/**
 * @brief SCGraphicsView::handleNewTransitionFormView
 * @param t
 *
 * this creates a new transition graphic specifically when the formview insert new transition button is pressed
 * it differs from the scxml handleNewTransition in that this transition will already know its target state, so it can immediately proceed to setting its connections here.
 *
 */
void SCGraphicsView::handleNewTransitionFormView(SCTransition* t)
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

    TransitionPositionAttribute * pos =
            dynamic_cast<TransitionPositionAttribute *> (  t->attributes.value("position"));
    QPointF position(0,0);
    if ( pos == 0 )
        qDebug()<< "pos returned null in SCGraphicsView::handleNewTransition";
    else
        position = pos->asQPointF();

    TransitionStringAttribute *targetName = dynamic_cast<TransitionStringAttribute *>(t->attributes.value("target"));
    StateBoxGraphic * targetGraphic = lookUpTargetStateGraphic( targetName->asString() );

    // create a transition graphic
    TransitionGraphic * transGraphic = new TransitionGraphic(parentGraphic, targetGraphic,  t , _keyController,_mouseController);

    // add the transitiongraphic to the map of transition graphics
    _mapTransitionToGraphic.insert(t, transGraphic);
    _hashTransitionToGraphic.insert(t, transGraphic);


    // set the event text box position of this transition
    PositionAttribute* textPos = (PositionAttribute*)t->getEventTextBlock()->attributes.value("position");
    transGraphic->setTextPos(textPos->asPointF());


    connectTransition(t);

}


/**
 * @brief SCGraphicsView::handleStateDeleted
 * @param state
 *
 * SLOT
 * connect in SCGraphicsView
 * connect(state, SIGNAL(destroyed(QObject*)), this, SLOT(handleStateDeleted(QObject*)));

 *
 * if the SCState is destroyed, remove it and its children from the graphics view
 */
void SCGraphicsView::handleStateDeleted(QObject *state)
{
    // use a direct cast because dynamic_cast<State*> will not result in a
    // state because the state has already been destructed
    // and we are getting the destructor signal on the base QObject class

    SCState * st = (SCState*)state;
    qDebug() <<"SCGraphicsView::handleStateDeleted for state: " << st->objectName();

    QMapIterator<SCState*,StateBoxGraphic*> i(_mapStateToGraphic);
    while (i.hasNext())
    {
        i.next();
        if ( i.key() == st)
        {
            QList<QGraphicsItem*> children = i.value()->childItems();
            qDebug() << "deleting children for state "<<" with num children: " << children.count();
            for(int k = 0; k < children.count(); k++)
            {
                SCState* childState = dynamic_cast<SCState*>(children.at(k));
                if(childState)
                    this->handleStateDeleted( (SCState*)(children.at(k)) );
            }

            delete i.value();
                    ;// delete the graphic and all its children
            _mapStateToGraphic.remove(i.key());
        }
    }


    _hashStateToGraphic.remove(st);

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


/**
 * @brief SCGraphicsView::handleStatePositionChangedInFormView
 *
 * SLOT
 * connect in SCGraphicsView
 *
 * connect(SCState, positionChangedInFormView, scgraphicsview, handleStatePositionChangedInFormView)
 *
 * will update the state to new coordinates and update its anchors as well
 *
 *
 * when the form view property table is changed, the dat model will be changed and the graphics view will be changed.
 */
void SCGraphicsView::handleStatePositionChangedInFormView(SCState* state, QPointF position)
{
    qDebug() << "SCGraphicsView::handleStatePositionChangedInFormView";
    StateBoxGraphic* sbg = _hashStateToGraphic.value(state);
    //qDebug() << "sbg: "<< sbg->objectName();
    //sbg->setPos(position);
    sbg->setPosAndUpdateAnchors(position);
}

void SCGraphicsView::handleStateSizeChangedInFormView(SCState *state, QPointF size)
{
    qDebug() << "SCGraphicsView::handleStateSizeChangedInFormView";
    StateBoxGraphic* sbg = _hashStateToGraphic.value(state);
    sbg->setSizeAndUpdateAnchors(size);
}


/**
 * @brief SCGraphicsView::handleNewTextBlock
 * @param trans
 * @param text
 *
 * CURRENTLY NOT IN USE
 *
 */
void SCGraphicsView::handleNewTextBlock(SCTransition* trans, QString text)
{
    //SCTextBlock* textBlock = new SCTextBlock();
}

void SCGraphicsView::handleBringToFront(SCState* state)
{

    SCState* top = _dm->getTopState();
    QList<SCState*> children;
    top->getAllStates(children);
    qreal zMax = 0;
    for(int i = 0; i < children.size(); i++)
    {
        StateBoxGraphic* child = _hashStateToGraphic.value(children.at(i));
        if(zMax < child->zValue())
        {
            zMax = child->zValue();
        }
    }
    StateBoxGraphic* sbg = _hashStateToGraphic.value(state);
    sbg->setZValue(zMax+1);
}

void SCGraphicsView::handleSendToBack(SCState* state)
{
    SCState* top = _dm->getTopState();
    QList<SCState*> children;
    top->getAllStates(children);
    qreal zMin = std::numeric_limits<double>::max();
    for(int i = 0; i < children.size(); i++)
    {
        StateBoxGraphic* child = _hashStateToGraphic.value(children.at(i));
        if(zMin > child->zValue())
        {
            zMin = child->zValue();
        }
    }
    StateBoxGraphic* sbg = _hashStateToGraphic.value(state);
    sbg->setZValue(zMin-1);
}

/**
 * @brief SCGraphicsView::handleNewRootMachine
 * @param state
 *
 *
 */
void SCGraphicsView::handleNewRootMachine(SCState* state)
{
    this->connectState(state);
}

/**
 * @brief SCGraphicsView::handleRootMachineIsParallelChanged
 *
 * SLOT
 * connected specifically to the root machine's parallelState Attribtue
 * while other states will have this function in stateboxgraphic, since the root machine has no statebox graphic, update its children graphics will be handled here.
 *
 * is parallel state is the only attribute for the root machine that can affect something in the graphics view.
 */
void SCGraphicsView::handleRootMachineIsParallelChanged(StateString *)
{
    qDebug() << "SCGraphicsView::handleRootMachineIsParallelChanged";
    QList<SCState*> states;
    _dm->getTopState()->getStates(states);

    for(int i = 0 ; i < states.size();i ++)
    {
        StateBoxGraphic* sbg = _hashStateToGraphic.value(states.at(i));
        sbg->forceUpdate();
    }
}

/**
 * @brief SCGraphicsView::connectState
 *  this is called for the root machine.
 *  the only attribute change for root machines that causes a graphicsview change is toggling parallel state true/false
 */
void SCGraphicsView::connectState(SCState* state)
{
    StateString* ips = state->getStringAttr("parallelState");
    connect(ips, SIGNAL(changed(StateString*)), this, SLOT(handleRootMachineIsParallelChanged(StateString*)));
}

/**
 * @brief SCGraphicsView::connectState
 * @param state
 * @param stateGraphic
 *
 * sets the connections for the SCState and its graphic whenever a new state is created
 *
 */
void SCGraphicsView::connectState(SCState* state, StateBoxGraphic* stateGraphic)
{
//    qDebug() << "SCGraphicsView::connectState";
    connect(state, SIGNAL(markedForDeletion(QObject*)), this, SLOT(handleStateDeleted(QObject*)));
    connect(stateGraphic, SIGNAL(destroyed(QObject*)), this, SLOT(handleStateGraphicDeleted(QObject*)));

    connect(state, SIGNAL(changedParent(SCState*,SCState*)), this, SLOT(handleChangedParent(SCState*, SCState*)));
    connect(state, SIGNAL(bringToFront(SCState*)), this, SLOT(handleBringToFront(SCState*)));
    connect(state, SIGNAL(sendToBack(SCState*)), this, SLOT(handleSendToBack(SCState*)));

    // when the graphic is clicked also emit clicked for the state
    connect(stateGraphic, SIGNAL(clicked(SCState*)), state, SIGNAL(clicked(SCState*)));


    // set up data model changes to update the state box graphic
    StateName* name = state->getStateNameAttr();
    connect(name, SIGNAL(changed(StateName*)), stateGraphic, SLOT(handleAttributeChanged(StateName*)));

    SizeAttribute* size = state->getSizeAttr();
    connect(size, SIGNAL(changed(SizeAttribute*)), stateGraphic, SLOT(handleAttributeChanged(SizeAttribute*)));

    PositionAttribute* pos = state->getPosAttr();
    connect(pos, SIGNAL(changed(PositionAttribute*)), stateGraphic, SLOT(handleAttributeChanged(PositionAttribute*)));

    StateString* ips = state->getStringAttr("parallelState");
    connect(ips, SIGNAL(changed(StateString*)), stateGraphic, SLOT(handleIsParallelStateChanged(StateString*)));

    StateString* initialState = state->getStringAttr("initialState");
    connect(initialState, SIGNAL(changed(StateString*)), stateGraphic, SLOT(handleInitialStateChanged(StateString*)));

    StateString* finalState = state->getStringAttr("finalState");
    connect(finalState, SIGNAL(changed(StateString*)), stateGraphic, SLOT(handleFinalStateChanged(StateString*)));

    StateString* ena = state->getStringAttr("entryAction");
    connect(ena ,SIGNAL(changed(StateString*)), stateGraphic, SLOT(handleEntryActionChanged(StateString*)));

    StateString* exa = state->getStringAttr("exitAction");
    connect(exa , SIGNAL(changed(StateString*)), stateGraphic, SLOT(handleExitActionChanged(StateString*)));

    //connect(state, SIGNAL(positionChangedInFormView(SCState*,QPointF)), this, SLOT(handleStatePositionChangedInFormView(SCState*, QPointF)));
    //connect(state, SIGNAL(sizeChangedInFormView(SCState*,QPointF)), this, SLOT(handleStateSizeChangedInFormView(SCState*,QPointF)));



    // set up state box graphic changes to update the data model
    // handle when a state is double clicked
    connect(stateGraphic, SIGNAL(resizeState(StateBoxGraphic*)), this, SLOT(handleAutoResize(StateBoxGraphic*)));

    // handle a state name change from the graphics view
    connect(stateGraphic, SIGNAL(nameChanged(QString)), state, SLOT(setStateName(QString)));

    // handle a state entry action change
    connect(stateGraphic, SIGNAL(entryActionChanged(QString)), state, SLOT(handleEntryActionChanged(QString)));

    // handle state exit action change to update the data model
    connect(stateGraphic, SIGNAL(exitActionChanged(QString)), state, SLOT(handleExitActionChanged(QString)));
}


/**
 * @brief SCGraphicsView::connectTransition
 * @param trans
 *
 * calls all graphics related connects for transition graphics, their elbows, and the transition deconstructors
 *
 * additionally, repositions the anchors to their target states
 *
 */
void SCGraphicsView::connectTransition(SCTransition* trans)
{

    SCState* parentState = trans->parentSCState();
    SCState* targetState = trans->targetState();

    StateBoxGraphic* parentGraphic = _hashStateToGraphic.value(parentState);
    StateBoxGraphic* targetGraphic = _hashStateToGraphic.value(targetState);
    TransitionGraphic* transGraphic = _hashTransitionToGraphic.value(trans);

    // set the connects for the transition graphic
    connect(trans, SIGNAL(markedForDeletion(QObject*)), this, SLOT(handleTransitionDeleted(QObject*)));
    connect(transGraphic, SIGNAL(destroyed(QObject*)), this, SLOT(handleTransitionGraphicDeleted(QObject*)));

    // create the connection to automatically move anchor elbows when state graphics are moved.
    connect(parentGraphic, SIGNAL(stateBoxMoved(QPointF)), transGraphic, SLOT(handleParentStateGraphicMoved(QPointF)));
    connect(targetGraphic, SIGNAL(stateBoxMoved(QPointF)), transGraphic, SLOT(handleTargetStateGraphicMoved(QPointF)));

    // additionally snap the anchors when done moving the source and sink
    connect(parentGraphic, SIGNAL(stateBoxReleased()), transGraphic, SLOT(handleParentStateGraphicReleased()));
    connect(targetGraphic, SIGNAL(stateBoxReleased()), transGraphic, SLOT(handleTargetStateGraphicReleased()));

    // snap the anchors when done RESIZING
    connect(parentGraphic, SIGNAL(cornerReleased()), transGraphic, SLOT(handleParentStateGraphicResizedReleased()));
    connect(targetGraphic, SIGNAL(cornerReleased()), transGraphic, SLOT(handleTargetStateGraphicResizedReleased()));

    connect(transGraphic->getSourceAnchor(),SIGNAL(anchorMoved(QPointF)),parentGraphic,SLOT(handleTransitionLineStartMoved(QPointF)));  // state box will handle snapping the source elbow/anchor to its border instead of standard movement
    connect(transGraphic->getSinkAnchor(),SIGNAL(anchorMoved(QPointF)),targetGraphic,SLOT(handleTransitionLineEndMoved(QPointF)));  // state box will handle snapping the source elbow/anchor to its border instead of standard movement
    //qDebug() << "hooking anchor to state graphic: " << _targetStateGraphic->objectName();

    // do this to set closest wall from default
    emit transGraphic->getSourceAnchor()->anchorMoved(parentGraphic->mapToScene(transGraphic->getSourceAnchor()->pos()));
    emit transGraphic->getSinkAnchor()->anchorMoved(parentGraphic->mapToScene(transGraphic->getSinkAnchor()->pos()));

    // create the connect to automatically move anchor elbows when state graphics are moved.
    connect(parentGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleParentStateGraphicResized(QRectF, QRectF, int)));
    connect(targetGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleTargetStateGraphicResized(QRectF, QRectF, int)));

    // connect this state box's grand parents update anchors when they are resized
    StateBoxGraphic* grandParentGraphic = parentGraphic->parentItemAsStateBoxGraphic();
    while(grandParentGraphic)
    {
        connect(grandParentGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleGrandParentStateGraphicResized(QRectF, QRectF, int)));
        connect(grandParentGraphic, SIGNAL(stateBoxReleased()), transGraphic, SLOT(handleParentStateGraphicReleased()));

        grandParentGraphic = grandParentGraphic->parentItemAsStateBoxGraphic();
    }

    // connect the target state box's grand parents to update the target anchors when they are resized
    StateBoxGraphic* grandParentTargetGraphic = targetGraphic->parentItemAsStateBoxGraphic();
    while(grandParentTargetGraphic)
    {
        connect(grandParentTargetGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleGrandParentTargetStateGraphicResized(QRectF, QRectF, int)));
        grandParentTargetGraphic = grandParentTargetGraphic->parentItemAsStateBoxGraphic();
    }

    // connect the event name data model to the text block
    TransitionStringAttribute* eventName = trans->getTransStringAttr("event");
    connect(eventName, SIGNAL(changed(TransitionStringAttribute*)), transGraphic, SLOT(handleAttributeChanged(TransitionStringAttribute*)));

    // when the transition graphic emits clicked, also emit it for the sctransition
    connect(transGraphic, SIGNAL(clicked(SCTransition*)), trans, SIGNAL(clicked(SCTransition*)));
}

/**
 * @brief SCGraphicsView::handleChangedParent
 * @param state
 * @param newParent
 *
 * when the data model is about to change the parent of a state, first make sure that all the anchors are updated properly in the graphics view
 *
 */
void SCGraphicsView::handleChangedParent(SCState* state, SCState* newParent)
{
    StateBoxGraphic* stateGraphic = _hashStateToGraphic.value(state);
    StateBoxGraphic* newParentGraphic = _hashStateToGraphic.value(newParent);

    // first, keep track of all scene based positions of sink anchors, the only transition elbows to stay in place
    QList<QPointF*> tPoints;
    QList<SCTransition*> transitions;
    state->getAllTransitions(transitions);
    // these are the sink anchors that belong to out transitions of the state
    for(int i = 0; i < transitions.size(); i++)
    {
        SCTransition* trans = transitions.at(i);
        TransitionGraphic* transG = _hashTransitionToGraphic.value(trans);
        StateBoxGraphic* oldSourceState = _hashStateToGraphic.value(trans->parentSCState());
        tPoints.append(new QPointF(oldSourceState->mapToScene(transG->getSinkAnchor()->pos())));

    }

    QList<QPointF*> tPointsIn;
    QList<SCTransition*> transitionsIn;
    // these are the sink anchors that point to this state
    transitionsIn = state->getTransitionsIn();
    for(int i = 0; i < transitionsIn.size(); i++)
    {
        SCTransition* trans = transitionsIn.at(i);
        TransitionGraphic* transG = _hashTransitionToGraphic.value(trans);
        StateBoxGraphic* oldSourceState = _hashStateToGraphic.value(trans->parentSCState());
        tPointsIn.append(new QPointF(oldSourceState->mapToScene(transG->getSinkAnchor()->pos())));
    }

    // change the parent of the state box graphic
    stateGraphic->setParentItem(newParentGraphic);



    // The box will move on the screen because it is changing coordinate systems. so we also must update the anchor positions to snap to their targets once more
    // update all the state's anchors positions to snap to their targets
    for(int i = 0; i < transitions.size(); i++)
    {
        SCTransition* trans = transitions.at(i);
        TransitionGraphic* transG = _hashTransitionToGraphic.value(trans);
        emit transG->getSinkAnchor()->anchorMoved(*tPoints.at(i));
    }

    // update all the transitions in's sink anchors
    for(int i = 0; i < transitionsIn.size(); i++)
    {
        SCTransition* trans = transitionsIn.at(i);
        TransitionGraphic* transG = _hashTransitionToGraphic.value(trans);
        emit transG->getSinkAnchor()->anchorMoved(*tPointsIn.at(i));

    }




    qDeleteAll(tPoints.begin(), tPoints.end());
    qDeleteAll(tPointsIn.begin(), tPointsIn.end());
}

/**
 * @brief SCGraphicsView::handleNewState
 * @param newState
 *
 * generic add state for the graphics view:
 * called when the insert state button is pressed to create a new state graphic and when loading the xml states
 *
 * scformview::insertState -> scdatamodel::insertNewState, emit newStateSignal -> scgraphicsview::handleNewState
 * connected to signal newStateSignal in the SCDataModel
 *
 * will add a state to the graphics view
 *
 */
void SCGraphicsView::handleNewState (SCState *newState)
{
    qDebug () << "SCGraphicsView::handleNewState";
    static int zVal = 0;
    // SCState connects

    // connectState(newState);

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

    // create the stateboxgraphic
    stateGraphic = new StateBoxGraphic(parentGraphic, newState);

    //
    // if a parent state is a parallel state, then its children run in parallel.
    // this is show by the children being drawn with dotted lines. So inform
    // the children to watch the parent's isParallelAttribute
    //

    // new states will be on top
    stateGraphic->setZValue(zVal++);

    // set the name
    stateGraphic->setObjectName(newState->objectName());
    //stateGraphic->setPos(newState->attributes.value("position"));
    //stateGraphic->setSize(newState->attributes.value("size"));

    // connect the state graphic's deconstructor to do removal protocol in the graphics view
    connectState(newState,stateGraphic);
//connect(stateGraphic, SIGNAL(destroyed(QObject*)), this, SLOT(handleStateGraphicDeleted(QObject*)));

    // quick look up of graphics from state model references
    // link the SCState and StateBoxGraphic
    _mapStateToGraphic.insert(newState, stateGraphic);
    _hashStateToGraphic.insert(newState,stateGraphic);

    // load size and position from state model
    PositionAttribute * position = dynamic_cast<PositionAttribute*> (newState->attributes.value("position"));
    stateGraphic->setPos(position->asPointF());
    //qDebug() << "pos : " << position->asString();
    //SelectableTextBlock stateTextBlock = stateGraphic->TextItem;
    //PositionAttribute * tbPosition = dynamic_cast<PositionAttribute*> (stateTextBlock.att);

    SizeAttribute * size = dynamic_cast<SizeAttribute*> (newState->attributes.value("size"));
    stateGraphic->setSize(size->asPointF());
    if ( ! parentGraphic )
    {
        _scene->addItem(stateGraphic);
    }

    // make the parent bigger to hold this state
    // and since its bigger, make its parent bigger.....
   // increaseSizeOfAllAncestors (newState);

//    stateGraphic->getStateTitle()->resize();
//    stateGraphic->getStateTitle()->recenterText();

    // load the text blocks

    // reposition the text items after loading their text
    stateGraphic->updateTextBlocks();

}

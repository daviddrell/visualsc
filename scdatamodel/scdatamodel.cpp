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

#include "scdatamodel.h"
#include <QFile>
#include <QDebug>
#include <QMapIterator>
#include <QGraphicsScene>

SCDataModel::SCDataModel(QObject * parent) :
    QObject (parent),
    _reader(),
    _writer(0),
    _level(0),
    _topLevel(0),
    _currentState(NULL),
    _currentTransition(NULL),
    _topState(NULL),
    _scene(0)
{


}

/**
 * @brief SCDataModel::connectDataModel
 *
 * called by the smproject that holds a datamodel, will connect the scxml reader to the data model
 *
 */
void SCDataModel::connectDataModel()
{
    connect(&_reader, SIGNAL(makeANewState(StateAttributes*)), this, SLOT(handleMakeANewState(StateAttributes*)), Qt::DirectConnection);
    connect(&_reader, SIGNAL(enterStateElement()), this, SLOT(handleTransitDown()), Qt::DirectConnection);
    connect(&_reader, SIGNAL(leaveStateElement()), this, SLOT(handleTransitUp()), Qt::DirectConnection);
    connect(&_reader, SIGNAL(makeANewIDTextBlockElement(TextBlockAttributes*)), this, SLOT(handleMakeANewIDTextBlock(TextBlockAttributes*)), Qt::DirectConnection);
    connect(&_reader, SIGNAL(makeANewTransistion(TransitionAttributes*)), this, SLOT(handleMakeANewTransition(TransitionAttributes*)), Qt::DirectConnection);
    connect(&_reader, SIGNAL(leaveTransistionElement()), this, SLOT(handleLeaveTransitionElement()), Qt::DirectConnection);
    connect(&_reader, SIGNAL(makeANewTransistionPath(QString)), this, SLOT(handleMakeANewTransitionPath(QString)), Qt::DirectConnection);
    connect(&_reader, SIGNAL(makeANewTransitionTextBlockElement(TextBlockAttributes*)), this, SLOT(handleMakeANewEventTextBlock(TextBlockAttributes*)));

    // connects scxml reader to changing the state machine's name
    connect(&_reader, SIGNAL(changeStateMachineName(QString)), this, SLOT(handleStateMachineNameLoad(QString)));
}

SCDataModel * SCDataModel::singleton()
{
    static SCDataModel * instance=NULL;
    if ( instance == NULL)
        instance = new SCDataModel(NULL);

    return instance;
}

/**
 * @brief SCDataModel::handleStateMachineNameLoad
 * @param machineName
 *
 * SLOT
 * connect in SCDataModel
 * connect(&_reader, SIGNAL(changeStateMachineName(QString)), this, SLOT(handleStateMachineNameLoad(QString)));
 *
 * called when the scxml reader reads a scxml file and changes the datamodel state machine's name
 * addtionally signals the formview to update the root state machine there
 *
 *
 */
void SCDataModel::handleStateMachineNameLoad(QString machineName)
{
    // change the value in the data model and alert the formview that this happened to update the tree and property table
    _topState->attributes.value("name")->setValue(machineName);
    emit _topState->nameChangedInDataModel(_topState,machineName); // connected to SCFormView::handleItemNameChangedInDataModel()

}

/**
 * @brief SCDataModel::reset
 * will reset the datamodel, clearing all transitions and states bar the Root Machine State
 */
void SCDataModel::reset()
{
    _currentState = _topState;
    _currentTransition = NULL;
    _topLevel =_level = 0;
    _topState->setLevel(_level);
    {

        QList<SCState *> list;
        //list.append(_topState);
        _topState->getAllStates(list);



         // first delete transitions and disconnect them from the states
        /*for(int i =0; i < list.count(); i++)
        {
            SCState *st = list.at(i);
            st->removeTargetsTransitionIn();
            st->removeSourcesTransitionOut();
        }*/

        // delete highest level states, this will automatically delete all sub states
        QList<SCState*> directChildren;
        _topState->getStates(directChildren);
        for(int i = 0 ; i < directChildren.count(); i++)
        {
            SCState* st = directChildren.at(i);

                qDebug() << "removing state: " << st->attributes.value("name")->asString();
                st->deleteSafely();
        }
    }
    _transitions.clear();

    // reset the name of the state machine and alert the formview that this happened
    _topState->attributes.value("name")->setValue("State Machine");
    emit _topState->nameChangedInDataModel(_topState,"State Machine"); // connected to SCFormView::handleItemNameChangedInDataModel()

    qDebug() << "AFTER A RESET, YOU HAVE " << _transitions.count() << " TRANSITIONS LISTED IN _transitions";
}

void SCDataModel::logError(QString message)
{
    // TBD: put on a message list that can be displayed or or viewed in the application
    //  also consider adding to the syslog (or make an equivalent for windows
    qDebug()<<message;
}

void SCDataModel::logInfo(QString message)
{
    // TBD: put on a message list that can be displayed or or viewed in the application
    qDebug()<<message;
}

void SCDataModel::setScene(QGraphicsScene *scene)
{
    _scene = scene;
}

QGraphicsScene * SCDataModel::getScene()
{
    return _scene;
}

SCState * SCDataModel::getAsState(QObject*o)
{
    SCState * state = dynamic_cast<SCState*>(o);
    return state;
}

SCTransition * SCDataModel::getAsTransition(QObject*o)
{
    SCTransition * v = dynamic_cast<SCTransition*>(o);
    return v;
}

SCTextBlock * SCDataModel::getAsTextBlock(QObject*o)
{
    SCTextBlock * v = dynamic_cast<SCTextBlock*>(o);
    return v;
}

/**
 * @brief SCDataModel::save
 * @param fileName
 * @param errorMessage
 * @return
 *
 * Saves the datamodel into an scxml file
 *
 */
bool SCDataModel::save(QString fileName, QString& errorMessage)
{
    QFile file(fileName);

    if ( ! file.open(QIODevice::WriteOnly))
    {
        errorMessage = QString("error opening file for write");
        return false;
    }

    _writer = new QXmlStreamWriter(&file);
    _writer->setAutoFormatting(true);
    _writer->writeStartDocument();

    // the top level state should be of type 'machine', and is a special type of state element called 'scxml'
    StateString* type =  dynamic_cast<StateString*> ( _topState->attributes.value("type"));

    if ( type  && ( type->asString() == "machine"))
    {
        _writer->writeStartElement("scxml");
        _writer->writeAttribute("name", _topState->attributes.value("name")->asString());
        _writer->writeAttribute("xmlns", "http://www.w3.org/2005/07/scxml");
    }

    // recursively write out all the state elements
    QList<SCState*> topLevelKids;
    _topState->getStates(topLevelKids);
    for (int i = 0; i < topLevelKids.count(); i++ )
    {
        SCState  * state = topLevelKids.at(i);
        state->writeSCVXML( *_writer );
    }

    _writer->writeEndDocument();

    file.close();

    errorMessage = QString("sucess");
    return true;
}


QString SCDataModel::toClassFileName(QString className)
{
    QStringList qsl = className.split(" ");
    QString ret;
    for(int i = 0; i < qsl.size(); i++)
    {
        ret += qsl.at(i).toLower();
    }
    return ret;
}

QString SCDataModel::toClassName(QString className)
{
    QStringList qsl = className.split(" ");
    QString ret;
    QString word;
    for(int i = 0; i < qsl.size(); i++)
    {
        word = qsl.at(i);
        ret += word.at(0).toUpper() + word.mid(1,word.size()).toLower();
    }
    return ret;
}


bool SCDataModel::exportToCode(QString fileName, QString &errorMessage)
{
qDebug() << "time to save files " << fileName;

QString hFileName;
QString cppFileName;
#define FORCE_STATE_MACHINE_NAME
#ifndef FORCE_STATE_MACHINE_NAME
    // get the file name minus cpp and add h
    hFileName = fileName.mid(0,fileName.size()-3) + "h";

#endif

#ifdef FORCE_STATE_MACHINE_NAME
    QStringList qsl = fileName.split("/");
    QString smFileName;
    for(int i = 0; i < qsl.count() -1; i++)
    {
        smFileName+=qsl.at(i)+"/";
    }
    // Test State Machine -> ./teststatemachine.cpp
    QString classFileName = toClassFileName(_topState->attributes.value("name")->asString());
    cppFileName = smFileName+classFileName+".cpp";
    hFileName = smFileName+classFileName+".h";

    // Test State Machine -> TestStateMachine
    QString className = toClassName(_topState->attributes.value("name")->asString());
#endif

    CodeWriter cw(this->getTopState(), className, cppFileName, hFileName);
    QList<SCState *> list;
    //list.append(_topState);
    _topState->getAllStates(list);
    cw.setChildren(list);

    cw.createSignalsAndSlots();
    //cw.helloWorld();
    cw.writeHFile();
    cw.writeCppFile();


    return true;
}


/**


 * @brief SCDataModel::open
 * @param file
 *
 * called when the mainwindow.cpp QAction actionOpen fires
 *
 * will read from an scxml file and create a the datamodel
 *
 *
 *
 * DEPRECATED FUNCTION
 */
void SCDataModel::open(QString file)
{



    _reader.readFile(file);

    _reader.run();

    bool ok;
    QStringList messages;
    _reader.getReadResult(ok, messages);

    //qDebug() << "ok is " << ok;
    if ( ok )
    {
        // now for the second pass, connect all the transitions to the states in the state-path.
        // the state-path is all of the state's which a transition must transit through on the path to its taget state.
        //

        connectTransitionsToStatePath();
    }

    emit openCompleted ( ok,  messages );

}

/**
 * @brief SCDataModel::openFile
 * @param fileName
 *
 * called in MainWindow::handleFileOpenClick()
 * the datamodel is reset and then open file is called to reload the datamodel
 *
 * sets the filename for the reader and runs the reader
 *
 *
 *
 */
void SCDataModel::openFile(QString fileName)
{
    _reader.readFile(fileName);
    _reader.run();

    bool ok;
    QStringList messages;
    _reader.getReadResult(ok, messages);

    qDebug()<<"% ok message : " << ok;
    if(ok)
    {
       // set up the transition connections in the data model
       connectTransitionsToStatePath();

       // set up the transition connections in the graphics
       for(int i = 0; i < _transitions.count(); i++)
       {
           // alert the graphics view and formview that the transition is ready to set up its connections
           emit transitionsReadyToConnect(_transitions.at(i));


       }
    }

}




void SCDataModel::connectTransitionsToStatePath()
{
    QList<SCTransition *> tlist;

    _topState->getAllTransitions(tlist);
    qDebug () << "SCDataModel::connectTransitionsToStatePath() " <<tlist.count();
    for (int t = 0; t < tlist.count(); t++)
    {
        SCTransition* trans = tlist.at(t);
        QString targetStr = trans->attributes["target"]->asString();
        SCState * targetState = _topState->getStateByName(targetStr);
        qDebug() << "target String: " << targetStr << " targetState; " << targetState->objectName();
        if ( targetState )
        {
            qDebug() << "&&& HOOKING TRANSITION "+trans->attributes.value("event")->asString()+"TO TARGET! " << targetState->objectName();
            makeTransitionConnections(targetState, trans);
        }

    }
    qDebug() << "finished SCDataModel::connectTransitionsToStatePath";
}



void SCDataModel::makeTransitionConnections(SCState * targetState, SCTransition* trans)
{
    qDebug() << "SCDataModel::makeTransitionConnections";
    // work backwards from target state to its ancestors, from the transitions source state to its ancestors, until
    // both have a common ancestor

    // set the transition to point to its target
    trans->setTargetState(targetState);

    // add this transition to the target state's list of in transitions
    targetState->addTransitionReference(trans, SCState::kTransitIn);

    // add this transition to the source state's list of out transitions
    SCState * sourceState = dynamic_cast<SCState *> ( trans->parent());
    sourceState->addTransitionReference(trans, SCState::kTransitOut);
/*
    SCState *sourceTreeNode=NULL;
    SCState *targetTreeNode=NULL;

    int sourceLevel =  sourceState->getLevel();
    int targetLevel = targetState->getLevel();
*/
     // change behavior to not promote transition references to parent
    /*
   // example: targetStateLevel = 1, sourceStateLevel = 3
    while ( sourceLevel > targetLevel )
    {
        sourceTreeNode = sourceState->getParentState();
        sourceTreeNode->addTransitionReference(trans, SCState::kTransitOut);
        sourceLevel =  sourceTreeNode->getLevel();
    }

    // example: targetStateLevel = 3, sourceStateLevel = 1
     while ( targetLevel >  sourceLevel)
     {
         targetTreeNode = targetState->getParentState();
         targetTreeNode->addTransitionReference(trans, SCState::kTransitIn);
         targetLevel =  targetTreeNode->getLevel();
     }

     // now targetLevel and sourceLevel should match

    while ( sourceTreeNode != targetTreeNode )
    {
        targetTreeNode = targetState->getParentState();
        targetTreeNode->addTransitionReference(trans, SCState::kTransitOut);
        targetLevel =  targetTreeNode->getLevel();

        sourceTreeNode = sourceState->getParentState();
        sourceTreeNode->addTransitionReference(trans, SCState::kTransitOut);
        sourceLevel =  sourceTreeNode->getLevel();
    }
    */



}


void SCDataModel::getStates(QList<SCState *>& list)
{
    if ( _topState == NULL ) list = QList<SCState *>();

    return _topState->getStates(list);
}




/**
 * @brief SCDataModel::deleteItem
 * @param item
 * @return
 *
 * called by scformview when the delete item button is pressed
 *
 * will search through all states and transitions and deletes the parameter item
 *
 * returns true if item was found and deleted
 * returns false if item was not found
 *
 */




// TODO this can be optimized.
bool SCDataModel::deleteItem(QObject * item)
{
    SCState* state = dynamic_cast<SCState*>(item);
    SCTransition* trans = dynamic_cast<SCTransition*>(item);

    if(state)
    {
        qDebug() << "SCDataModel::deleteItem state: " << state->objectName() << " the top state is: " <<_topState->objectName();
        /*QList<SCState*> list;
        _topState->getAllStates(list);

        // unhook target state's in transitions
        state->removeTargetsTransitionIn();
        state->removeSourcesTransitionOut();

        QList<SCState*> childrenStates;

        state->getAllStates(childrenStates);
        SCState* child;
        for(int i = 0 ; i < childrenStates.count(); i++)
        {
            child = childrenStates.at(i);

        }*/

         //state->deleteLater();

        state->deleteSafely();
        return true;

    }
    else if(trans)
    {
        //delete trans;
        trans->deleteSafely();
        return true;
    }
    else // unexpected item
    {
        qDebug( )<< "ERROR: given an unexpected item to delete!";
        return false;
    }
}

/**
 * @brief SCDataModel::insertNewState
 * @param parent
 * @return
 *
 * Called from scformview::insertState(), when the new state button is pressed
 *
 * will add a new state that is a child of the parent parameter
 *
 * and will use newStateSignal to signal the scformview and scgraphcisview for their new state protocols
 *
 */
SCState* SCDataModel::insertNewState(SCState *parent)
{
    // SCState connects. 1 of 2 places. this is for any newly created state
    SCState * state = new SCState (parent);
    connectState(state);
    //
    //connect(state, SIGNAL(positionChangedInFormView(SCState*,QPointF)), );

    emit newStateSignal(state);

    return state;
}


SCState * SCDataModel::getTopState()
{
    return _topState;
}

QObject * SCDataModel::getItemByName(QString name)
{
    QObject *item=NULL;
    if ( (item = getStateByName(name)) != NULL ) return item;
    if ( (item = getTransitionByName(name))!= NULL) return item;

    return item;
}

SCState * SCDataModel::getStateByName(QString name)
{
    QList<SCState *> list;

    list.append(_topState);

    _topState->getAllStates(list);

    for(int i =0; i < list.count(); i++)
    {
        SCState *st = list.at(i);
        if ( st->attributes.value("name")->asString() == name)
            return st;
    }
    return NULL;
}


SCTransition * SCDataModel::getTransitionByName(QString name)
{
    QList<SCTransition *> list;


    _topState->getAllTransitions(list);

    for(int i =0; i < list.count(); i++)
    {
        SCTransition *tr = list.at(i);
        if ( tr->attributes.value("target")->asString() == name)
            return tr;
    }
    return NULL;
}


void SCDataModel::getAllStates(QList<SCState *>& list)
{
    return _topState->getAllStates(list);
}

void SCDataModel::handleTransitDown()
{
    _level ++;
    qDebug() << "down level = " + QString::number(_level);
}

void SCDataModel::handleTransitUp()
{
    _level --;

    qDebug() << "up level = " + QString::number(_level);

    if ( ! _currentState ) return;


    if (_currentState->parent())
        _currentState = dynamic_cast<SCState*> (_currentState->parent());
}


// TODO change "Root Machine" to name of file

/**
 * @brief SCDataModel::initializeEmptyStateMachine
 */

void SCDataModel::initializeEmptyStateMachine()
{

    if ( _topState != 0 )
    {
        _topState->deleteSafely();
        _topState = 0;
    }

    _topLevel =_level = 0;
    StateAttributes * stateAttributes = new StateAttributes(0,"stateAttributes");

    StateString *  sa = new StateString (0,"type","machine");
    stateAttributes->addItem( sa );

    StateString *nsp = new StateString(NULL,"xmlns","http://www.w3.org/2005/07/scxml");
    stateAttributes->addItem(nsp);

    StateName *nm = new StateName(NULL,"name", "State Machine");
    stateAttributes->addItem(nm);

    _topState = new SCState(true);
    _topState->attributes.setAttributes( *stateAttributes);
    _topState->setLevel(_level);


    _currentState  = _topState;

    // connect the top state
    connectState(_topState);

    //emit newStateSignal(_topState);

    qDebug() << "initialized empty state machine! " << "_currentState: " <<nm->asString()<<" on level: "<<_level;
    delete sa;

}

/**
 * @brief SCDataModel::handleMakeANewState
 * @param sa
 *
 *
 * SLOT
 *
 * connected to SCXMLReader
 * this function is only used when creating states from the xml file.
 *
 *
 * SIGNAL
 *
 * signals the graphicsview and form view
 *
 */
void SCDataModel::handleMakeANewState(StateAttributes*  sa)
{
    qDebug() << "SCDataModel::handleMakeANewState on level " << _level;
    SCState* state = NULL;
    state = new SCState(_currentState);
    state->attributes.setAttributes( *sa);
    state->setLevel(_level);


    /*PositionAttribute * position =dynamic_cast<PositionAttribute*> ((IAttribute*)state->attributes.value("position"));
    QPointF ps = position->asPointF();
    state->setPosition(ps);
*/
    QString name ;
    if (! sa->contains("name"))
    {
        // is this an initial state?
        if ( sa->value("type")->asString() == "initial")
        {
            name =     _currentState->objectName() + "_initial" ;
        }
        else if ( sa->value("type")->asString() == "final")
        {
            name =     _currentState->objectName() + "_final" ;
        }
        else
        {
            name = _currentState->objectName() + "_" + QString::number( _currentState->children().count() );
        }

        StateName *nm = new StateName(NULL,"name",name);
        state->attributes.addItem(nm);
    }
    else
    {
        name = sa->value("name")->asString();
    }

    state->setObjectName( name );
    state->setText(name);

    qDebug() << "adding state at level  :" + QString::number(_level) + ", name : " + name;

    // SCState connects. 1 of 2 places where the SCState connects are set up. this is for loading states through the xml
    connectState(state);

    _currentState  = state;


    // connected to scformview slot handleNewState
    // connected to scgraphicsview slot handleNewState
    emit newStateSignal(state);


    delete sa;


    if(false)
    {
    qDebug() <<" SCDataModel::handleMakeANewState " << _level;
    SCState * state = NULL;

    if ( _currentState == 0 )
    {

        // TODO: add scxml rules check - top state must be element 'scxml' and the reader should have assign an attribute type='machine'

        _topLevel = _level;
        state = new SCState(true);
        state->attributes.setAttributes( *sa);
        state->setLevel(_level);

        _topState = state;

        QString name;

        if ( ! sa->contains("name"))
        {
            name = "State Machine";

            StateName *nm = new StateName(NULL,"name",name);
            state->attributes.addItem(nm);

        }
        else
        {
            name = sa->value("name")->asString();
        }

        state->setObjectName( name);


        qDebug() << "adding new state at top level : " + state->attributes.value("name")->asString();
    }
    else if ( _level > _topLevel)
    {
        state = new SCState(_currentState);
        state->attributes.setAttributes( *sa);
        state->setLevel(_level);


        /*PositionAttribute * position =dynamic_cast<PositionAttribute*> ((IAttribute*)state->attributes.value("position"));
        QPointF ps = position->asPointF();
        state->setPosition(ps);
*/
        QString name ;
        if (! sa->contains("name"))
        {
            // is this an initial state?
            if ( sa->value("type")->asString() == "initial")
            {
                name =     _currentState->objectName() + "_initial" ;
            }
            else if ( sa->value("type")->asString() == "final")
            {
                name =     _currentState->objectName() + "_final" ;
            }
            else
            {
                name = _currentState->objectName() + "_" + QString::number( _currentState->children().count() );
            }

            StateName *nm = new StateName(NULL,"name",name);
            state->attributes.addItem(nm);
        }
        else
        {
            name = sa->value("name")->asString();
        }

        state->setObjectName( name );
        state->setText(name);

        qDebug() << "adding state at level  :" + QString::number(_level) + ", name : " + name;

        // SCState connects. 1 of 2 places where the SCState connects are set up. this is for loading states through the xml
        connectState(state);
    }



    _currentState  = state;


    // connected to scformview slot handleNewState
    // connected to scgraphicsview slot handleNewState
    emit newStateSignal(state);


    delete sa;
    }

}

/**
 * @brief SCDataModel::insertNewTextBlock
 * @param item
 * @param name
 * @return
 *
 * called by scformview::itemInsertTextBlock
 * handles adding a new textblock to either a transiton or state's list of textblocks
 *
 * then emits newTextBlockSignal for the scformview and scgraphics view protocol for new textblocks
 */
bool SCDataModel::insertNewTextBlock(SCItem* item, QString name)
{
    SCTransition* trans = dynamic_cast<SCTransition*> (item);
    SCState* state = dynamic_cast<SCState*>(item);

    if(state)
    {
      //  state->addTextBox(name, name);
    }
    else if(trans)
    {

        // add a new textblock to the datamodel
        //trans->addTextBlock(name, name);
        // TODO MONday

        //
        //emit newTextBlockSignal(trans, name);
        //emit newTextBox(trans, name);
    }
    else
    {
        qDebug() << "SCDataModel::insertNewTextBox ERROR given unexpected type";
        return false;
    }
    return true;
}


/**
 * @brief SCDataModel::insertNewProperty
 * @param item
 * @param propertyName
 * @return true if attempted to insert, false if highlighted item was not expected
 */
bool SCDataModel::insertNewProperty(SCItem* item, QString propertyName)
{
    SCTransition* trans = dynamic_cast<SCTransition*>(item);
    SCState* state = dynamic_cast<SCState*>(item);

    if(state)
    {
        qDebug() << "insert new state property: "<<propertyName;
        state->addAttribute(propertyName,"");
    }
    else if(trans)
    {
        qDebug() << "insert new transition property: " << propertyName;
        trans->addAttribute(propertyName,"");
    }
    else
    {
        qDebug() << "SCDataModel::insertNewProperty ERROR Given unexpected type: ";
        return false;
    }
    return true;
}

bool SCDataModel::deleteProperty(SCItem* item, QString propertyName)
{
    SCTransition* trans = dynamic_cast<SCTransition*>(item);
    SCState* state = dynamic_cast<SCState*>(item);


    if(state)
    {
        qDebug() << "deleting state property: " << propertyName;
        return state->removeAttribute(propertyName);

    }
    else if(trans)
    {
        qDebug() << "deleting transition property: " << propertyName;
        return trans->removeAttribute(propertyName);
    }
    else
    {
        qDebug() << "SCDataModel::deleteProperty ERROR given unexpected type.";
        return false;
    }
    return false;    // should not reach this

}

void SCDataModel::connectState(SCState* state)
{
    connect(state, SIGNAL(nameChangedInFormView(SCState*,QString)),     this, SLOT(handleStateNameChangedInFormView(SCState*,QString)));
    connect(state, SIGNAL(positionChangedInFormView(SCState*,QPointF)), this, SLOT(handleStatePositionChangedInFormView(SCState*,QPointF)));
    connect(state, SIGNAL(sizeChangedInFormView(SCState*,QPointF)),     this, SLOT(handleStateSizeChangedInFormView(SCState*,QPointF)));
}

void SCDataModel::connectTransition(SCTransition* trans)
{
    connect(trans, SIGNAL(eventChangedInFormView(SCTransition*,QString)), this, SLOT(handleEventNameChangedInFormView(SCTransition*,QString)));
}

void SCDataModel::handleEventNameChangedInFormView(SCTransition * trans, QString eventName)
{
    qDebug() << "SCDataModel::handleEventNameChangedInFormView";
    trans->setText(eventName);
}

void SCDataModel::handleEventSizeChangedInFormView(SCTransition *, QString)
{

}

void SCDataModel::handleEventPositionChangedInFormView(SCTransition *, QString)
{

}

void SCDataModel::handleStateSizeChangedInFormView(SCState *state, QPointF size)
{
    qDebug() << "SCDataModel::handleStateSizeChangeInFormView";
    state->setSize(size);
}

/**
 * @brief SCDataModel::handleStateNameChangedInFormView
 *
 * SLOT
 *
 * connect in SCDataModel
 * connect(SCState,SIGNAL(nameChangedInFormView(SCState*,QString)), SCDataModel,SLOT(handleStateNameChangedInFormView(SCState*,QString)));
 *
 *
 * called when the user types in the name in the property table
 * will update the data model's textbox with the new name
 * by calling SCState::setText, which will then emit a signal to change the graphics view text
 *
 *
 */
void SCDataModel::handleStateNameChangedInFormView(SCState* state, QString name)
{
    qDebug() << "SCDataModel::handleStateNameChangedInFormView";
    state->setText(name);
}

/**
 * @brief SCDataModel::handleStatePositionChangedInFormView
 * @param state
 * @param point
 *
 * SLOT
 * connect in SCDataModel
 * connect(SCState,SIGNAL(positionChangedInFormView(SCState*,QPointF)),SCDataModel,SLOT(handleStatePositionChangedInFormView(SCState*,QPointF)));
 *
 *
 * change the SCState's position
 *
 * this also will emit signal position changed in datamodel
 */
void SCDataModel::handleStatePositionChangedInFormView(SCState* state, QPointF point)
{
    qDebug() << "SCDataModel::handleStatePositionChangeInFormView";
    state->setPosition(point);
}


/**
 * @brief SCDataModel::insertNewTransition
 * @param source
 * @param target
 * @return
 *
 * called by SCFormView::insertTransition
 *
 * creates a new SCTransition object and then emits a newTransitionSignal
 * which will show the new transition in the tree view and in the graphics view
 *
 * also adds two connection references for the states and sets the target state for the transition
 *
 * signal
 *
 */
SCTransition* SCDataModel::insertNewTransition(SCState *source, SCState* target )
{
    if ( source == NULL)  return NULL;

    SCTransition * transition = new SCTransition(source);

    transition->setAttributeValue("target", target->attributes.value("name")->asString());
    transition->setAttributeValue("event", "event");
    transition->setObjectName("event");
    target->addTransitionReference(transition, SCState::kTransitIn);
    source->addTransitionReference(transition, SCState::kTransitOut);
    transition->setTargetState(target);

    connectTransition(transition);
    _transitions.append(transition);

    qDebug() << "@@@ adding transition out reference for state " << source->attributes.value("name")->asString();


    //connect(transition,SIGNAL((SCState*,QString)), this,SLOT(handleStateNameChangedInFormView(SCState*,QString)));
    //emit newTransitionSignal(transition);   // connected to scgraphics view's handleNewTransition and scformview's handle new transition
    //emit transitionsReadyToConnect(transition);
    emit formViewInsertNewTransitionSignal(transition);

    return transition;
}

/**
 * @brief SCDataModel::handleMakeANewTransition
 * @param ta
 *
 *
 * called by the scxmlreader
 * this function is used when adding transitions read from an scxml file
 *
 * SLOT
 * connect(scxmlreader, makeANewTransition, scdatamodel, handleMakeANewTransition)
 *
 */
void SCDataModel::handleMakeANewTransition(TransitionAttributes * ta)
{

    qDebug() << "handleMakeANewTransition, : "  + ta->value("target")->asString() << "\t" << ta->value("event")->asString();

    SCTransition * transition = new SCTransition(_currentState);

    if ( _currentState == 0)
        return;

    transition->attributes.setAttributes(*ta);
    transition->setText(transition->attributes.value("event")->asString());

    connectTransition(transition);

    _currentTransition = transition;
    _currentState->addTransistion(transition);

    _transitions.append(transition);

    delete ta;

    qDebug() << "leave handleMakeANewTransition, : "  ;





}
/*
void SCDataModel::handleMakeANewTransition(TransitionAttributes * ta, SCState* sourceState)
{
    SCTransition* trans = new SCTransition(sourceState);

    // load the transition attributes and set the transition's text box to the string in the event attribute
    trans->attributes.setAttributes(*ta);
    trans->setText(trans->attributes.value("event")->asString());

    // add this transition to the list of outgoing transition held by the state.
    sourceState->addTransistion(trans);
}
*/

void SCDataModel::handleLeaveTransitionElement()
{
    qDebug() << "handleLeaveTransitionElement ";

    emit newTransitionSignal(_currentTransition);

}

// TODO remove deprecated function. Transition properties will be part of the transition xml element and not under its own element
void SCDataModel::handleMakeANewTransitionProperty(const QString name)
{
    qDebug() << "handle make a new transition property " << name;

}
/**
 * @brief SCDataModel::handleMakeANewIDTextBlock
 * @param attributes

 SLOT
 connect in SCDataModel::open()
    connect(&_reader, SIGNAL(makeANewIDTextBlockElement(TextBlockAttributes*)), this, SLOT(handleMakeANewIDTextBlock(TextBlockAttributes*)), Qt::DirectConnection);




*/

/**
 * @brief SCDataModel::handleMakeANewIDTextBlock
 * @param attributes
 *
 * SLOT
 * connect in scdatamodel::open
 * connect(sxcmlreader, makeANewIDTextBlockElement, scdatamodel, handleMakeANewIdTextBlock
 *
 * called when the scxml reader is reading text block attributes for a state
 * this updates the corresponding state's SCTextBlock with the attributes
 *
 */
void SCDataModel::handleMakeANewIDTextBlock ( TextBlockAttributes *attributes)
{
    if  ( _currentState == NULL )
        return;

    SCTextBlock *textBlock = _currentState->getIDTextBlock();
    qDebug()<<" handleMakeANewIDTextBlock textBlock=" +QString::number((int)textBlock)+", current state= "+ _currentState->objectName();

    textBlock->attributes.setAttributes(*attributes);
    delete attributes;
}

/**
 * @brief SCDataModel::handleMakeANewEventTextBlock
 * @param attributes
 * SLOT
 * connect in scdatamodel
 * connect(scxmlreader, makeANewTransitionTextBlockElement, scdatamodel, handleMakeANewEventTextBlock)
 *
 * called when the scxml reader is reading textblock attributes for a transition
 * this updates the current transition's text block attributes
 *
 */
void SCDataModel::handleMakeANewEventTextBlock( TextBlockAttributes* attributes)
{
    qDebug () << "SCDataModel::handleMakeANewEventTextBlock";

    SCTextBlock *textBlock = _currentTransition->getEventTextBlock();
    textBlock->attributes.setAttributes(*attributes);
    delete attributes;
}

void SCDataModel::handleMakeANewTransitionPath (QString pathStr)
{
    TransitionAttributes::TransitionStringAttribute *targetAttr =  dynamic_cast<TransitionAttributes::TransitionStringAttribute *>( _currentTransition->attributes.value("target"));

    QString target = targetAttr->asString();

    qDebug() << "handleMakeANewTransitionPath (" + target + ") : " + pathStr ;

    if ( _currentState == 0)
        return;

    if ( _currentTransition == 0 ) return;

    TransitionAttributes::TransitionPathAttribute *path =
            dynamic_cast<TransitionAttributes::TransitionPathAttribute *>( _currentTransition->attributes.value("path"));

    path->setValue( pathStr );

    qDebug() << "interpreted path is " + path->asString();

    _currentTransition->attributes.addItem(path);

    qDebug() << "leave handleMakeANewTransitionPath : " ;

}



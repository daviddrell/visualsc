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
#include <QUuid>
#include <QEventLoop>
#include <QTime>
#include <QCoreApplication>

SCDataModel::SCDataModel(QObject * parent) :
    QObject (parent),
    _reader(),
    _writer(0),
    _level(0),
    _topLevel(0),
    _currentState(NULL),
    _currentTransition(NULL),
    _topState(NULL),
    _scene(0),
    _lastClickedItem(NULL)
{
    _reader.setDataModel(this);
    _settings = new QSettings(this);
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
    connect(&_reader, SIGNAL(changeStateMachineUid(QString)), this, SLOT(handleStateMachineUidLoad(QString)));
    connect(&_reader, SIGNAL(changeStateMachineAttribute(QString, QString)), this, SLOT(handleStateMachineAttributeLoad(QString,QString)));



}

/**
 * @brief SCDataModel::connectTransition
 * @param trans
 *
 * SCDataModel level connects for SCTransitions.
 * called when a transition is created.
 *
 */
void SCDataModel::connectTransition(SCTransition * trans)
{
    qDebug() << "scdatamodel::connecttransition";
    connect(trans->getTransStringAttr("event"), SIGNAL(changed(TransitionStringAttribute*)), this, SLOT(handleCheckEventCollision(TransitionStringAttribute*)));
    connect(trans,SIGNAL(markedForDeletion(QObject*)),this,SLOT(handleTransitionBeingDeleted(QObject*)));

    // when the transition is clicked, scdatamodel will do something
    connect(trans, SIGNAL(clicked(SCTransition*)), this, SLOT(handleItemClicked(SCTransition*)));

    // propogate clicked signal outside the datamodel
    connect(trans, SIGNAL(clicked(SCTransition*)), this, SIGNAL(itemClicked()));

    // propogate font attribute changes to the mainwindow combo boxes
    connect(trans->getEventTextBlock()->getFontFamilyAttr(), SIGNAL(changed(FontFamilyAttribute*)), this, SLOT(handleChangeProgramFont(FontFamilyAttribute*)));
    connect(trans->getEventTextBlock()->getFontSizeAttr(), SIGNAL(changed(FontSizeAttribute*)), this, SLOT(handleChangeProgramFont(FontSizeAttribute*)));
    connect(trans->getEventTextBlock()->getFontBoldAttr(), SIGNAL(changed(FontBoldAttribute*)), this, SLOT( handleChangeProgramFont(FontBoldAttribute*)));
}

/**
 * @brief SCDataModel::connectState
 * @param st
 *
 * called when a state is created.
 * calls scdatamodel scope connects.
 */
void SCDataModel::connectState(SCState * st)
{
    qDebug() << "scdm::connectState";
     // when the state is clicked, scdatamodel will do something
    connect(st, SIGNAL(clicked(SCState*)), this, SLOT(handleItemClicked(SCState*)));

    // propogate clicked signal outside the datamodel
    connect(st,SIGNAL(clicked(SCState*)), this, SIGNAL(itemClicked()));

    // propogate font attribute changes to the mainwindow combo boxes
    connect(st->getIDTextBlock()->getFontFamilyAttr(), SIGNAL(changed(FontFamilyAttribute*)), this, SLOT(handleChangeProgramFont(FontFamilyAttribute*)));
    connect(st->getIDTextBlock()->getFontSizeAttr(), SIGNAL(changed(FontSizeAttribute*)), this, SLOT(handleChangeProgramFont(FontSizeAttribute*)));
}

/**
 * @brief SCDataModel::handleChangeProgramFont
 * @param ffa
 *
 * SLOT
 *
 * connected to the sctextblock attributes of items
 * propogates attribute changes to change the program font in mainwindow
 *
 */
void SCDataModel::handleChangeProgramFont(FontFamilyAttribute * ffa)
{
    QFont font(ffa->asString(), 1);
    emit setProgramFont(&font);
}
void SCDataModel::handleChangeProgramFont(FontSizeAttribute * fsa)
{
    QFont font("", fsa->asInt());
    emit setProgramFont(&font);
}

void SCDataModel::handleChangeProgramFont(FontBoldAttribute * fba)
{
    QFont font;

    if(fba->asBool())
    {
        font=QFont("", 1, QFont::Bold);
    }
    else
    {
        font=QFont("",1);
    }
    emit setProgramFont(&font);
}


/**
 * @brief SCDataModel::handleItemClicked
 * @param st
 *
 * when the state is clicked, do stuff
 *
 */
void SCDataModel::handleItemClicked(SCState * st)
{
//    SCState* lastClickedSt = dynamic_cast<SCState*>(_lastClickedItem);

    if(_lastClickedItem==st)
        return;

    _lastClickedItem = st;

    QFont font(st->getIDTextBlock()->getFontFamilyAttr()->asString(), st->getIDTextBlock()->getFontSizeAttr()->asInt());
    emit setProgramFont(&font);
}

void SCDataModel::handleItemClicked(SCTransition * trans)
{
//    SCTransition* lastClickedTr = dynamic_cast<SCTransition*>(_lastClickedItem);

    if(_lastClickedItem==trans)
        return;

    _lastClickedItem = trans;


    QFont font(trans->getEventTextBlock()->getFontFamilyAttr()->asString(), trans->getEventTextBlock()->getFontSizeAttr()->asInt());
    emit setProgramFont(&font);
}

/**
 * @brief SCDataModel::handleCheckEventCollision
 * @param tsa
 *
 * SLOT
 *
 * when a transition name changes, this will check if there are more than one transitions with the same name
 * and message the user that the event slot will trigger the total number of transitions
 *
 *
 */
void SCDataModel::handleCheckEventCollision(TransitionStringAttribute * tsa)
{
    int count = 0;
    foreach(SCTransition* trans, _transitions)
    {
        if(trans->getEventName() == tsa->asString())
            count++;
    }
    qDebug() << "trans event name count: " << count;

    if(count > 1)
        emit message(("Event slot \"" + tsa->asString()+ "\" will trigger "+ QString::number(count)+" transitions"));

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
}

void SCDataModel::handleStateMachineUidLoad(QString uid)
{
    _topState->attributes.value("uid")->setValue(uid);
}

void SCDataModel::handleStateMachineAttributeLoad(QString key, QString value)
{
    _topState->getStringAttr(key)->setValue(value);
}


/**
 * @brief SCDataModel::reset
 * will reset the datamodel, clearing all transitions and states and make another root machine scstate
 */
void SCDataModel::reset()
{
    // reset reader variables
    _currentTransition = NULL;
    _transitions.clear();
    _topLevel =_level = 0;

    // make another top state and set the current state to it
    this->initializeEmptyStateMachine();

    // alert the formview and graphicsview that there is a new top state
    emit newRootMachine(_topState);
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
    _writer->setCodec("UTF-8");
    _writer->setAutoFormatting(true);
    _writer->writeStartDocument();

    // the top level state should be of type 'machine', and is a special type of state element called 'scxml'
    StateString* type =  dynamic_cast<StateString*> ( _topState->attributes.value("type"));

    if ( type  && ( type->asString() == "machine"))
    {
        _writer->writeStartElement("scxml");
//        _writer->writeAttribute("name", _topState->attributes.value("name")->asString());
//        _writer->writeAttribute("uid", _topState->getUid());
//        _writer->writeAttribute("xmlns", "http://www.w3.org/2005/07/scxml");

        // get the keys of the attributes
        QMapIterator<QString, IAttribute*> i(_topState->attributes);

        // for every attribute of this state, write into the scxml
        while(i.hasNext())
        {
            QString key = i.next().key();
            qDebug() << "writing " << key <<"...";
            _writer->writeAttribute(key, _topState->attributes.value(key)->asString());
        }
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


QString SCDataModel::getCFileName()
{
    QString name = _topState->getName();
    name.replace(" ","");
    return name.toLower()+".cpp";
}

bool SCDataModel::exportToCode(QString fileName, QString &errorMessage)
{
qDebug() << "time to save files " << fileName;

QString hFileName;
QString cppFileName;
QString className = toClassName(_topState->attributes.value("name")->asString());


//#define FORCE_STATE_MACHINE_NAME
#ifndef FORCE_STATE_MACHINE_NAME
    // get the file name minus cpp and add h
    cppFileName = fileName;
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

#endif

    CodeWriter cw(this->getTopState(), className, cppFileName, hFileName);

    //QList<SCState *> list;
    //_topState->getAllStates(list);
   // _topState->getStates(list);


//    QList<SCState* > all;
//    all.append(_topState);
//    _topState->getAllStates(all);


//    for(int i = 0; i < all.size();i++)
//    {
//        SCState* state = all.at(i);
//        if(state->isStateMachine())
//        {
//            cw.addStateMachine(state);
//        }
//    }

    // add the state machines to the codewriter
    QList<SCState*> stateMachines = this->getStateMachines();
    foreach(SCState* sm, stateMachines)
        cw.addStateMachine(sm);

    // define the signal and slot names
    cw.createStateMachines();

    // create the files
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
 * DEPRECATED FUNCTION replaced by openFile
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

        QList<SCTransition*> tlist;
        _topState->getAllTransitions(tlist);
       connectTransitionsToStatePath(tlist);
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
       QList<SCTransition*> tlist;
       _topState->getAllTransitions(tlist);
       connectTransitionsToStatePath(tlist);

       // set up the transition connections in the graphics
       for(int i = 0; i < _transitions.count(); i++)
       {
           // alert the graphics view and formview that the transition is ready to set up its connections
           emit transitionsReadyToConnect(_transitions.at(i));
       }
    }
}

/**
 * @brief SCDataModel::importFile
 * @param parent
 * @param fileName
 *
 *
 * called when inserting a whole scxml file into the parent state
 *
 */
void SCDataModel::importFile(SCState* parent,QString fileName)
{
    _importedTransitions.clear();
    _reader.readFile(fileName);

    // this function will emit signal importedMachine()
    SCState* importedMachine = _reader.importFile(parent);
    qDebug() << "imported Machine: " <<importedMachine->objectName();


    connectTransitionsToStatePath(_importedTransitions);
    for(int i = 0; i < _importedTransitions.count(); i++)
    {
        // alert the graphicsview and formview that the imported transitions are ready
        emit transitionsReadyToConnect(_importedTransitions.at(i));
    }
}



/**
 * @brief SCDataModel::connectTransitionsToStatePath
 *
 * Used when reading from an scxml file
 *
 * Once all states and transitions are loaded, the transitions' targets are set properly
 * this is done through a unique identifier given to each state, a QUuid is generated upon state creation
 * this is saved as a property in the scxml so that states with the same name can be diffentiated
 *
 */
void SCDataModel::connectTransitionsToStatePath(QList<SCTransition*> tlist)
{

    for (int t = 0; t < tlist.count(); t++)
    {
        // for every transition, load the uid of the state it targets and seach for the state
        SCTransition* trans = tlist.at(t);
        QString targetStr = trans->getUid();
        SCState* targetState = _topState->getStateByUid(targetStr);

        // if uid fails, then try the old method of searching by name
        if(!targetState)
        {
            qDebug() << "state uid was not found. searching by state name...";
            targetStr = trans->attributes["target"]->asString();
            targetState = _topState->getStateByName(targetStr);

            if(targetState && trans->getUid().isEmpty())
            {
                trans->setUid(targetState->getUid());
            }
        }

        // ensure that the target state exists
        if(targetState)    // the state was found, now connect the transition and state
        {
            //qDebug() << "&&& HOOKING TRANSITION "+trans->attributes.value("event")->asString()+"TO TARGET! " << targetState->objectName();
            makeTransitionConnections(targetState, trans);
        }

    }
}



void SCDataModel::makeTransitionConnections(SCState * targetState, SCTransition* trans)
{
    //qDebug() << "SCDataModel::makeTransitionConnections";

    // set the transition to point to its target
    trans->setTargetState(targetState);

    // add this transition to the target state's list of in transitions
    targetState->addTransitionReference(trans, SCState::kTransitIn);

    // add this transition to the source state's list of out transitions
    SCState * sourceState = trans->parentSCState();
    sourceState->addTransitionReference(trans, SCState::kTransitOut);
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
bool SCDataModel::deleteItem(QObject * item)
{
    SCState* state      = dynamic_cast<SCState*>(item);
    SCTransition* trans = dynamic_cast<SCTransition*>(item);
    SCTransitionBranch* br = dynamic_cast<SCTransitionBranch*>(item);

    if(state)
    {         
        state->deleteSafely();
        return true;
    }
    else if(trans)
    {    
        trans->deleteSafely();
        return true;
    }
    else if(br)
    {
        br->deleteSafely();
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
    SCState * state = new SCState (parent);
    connectState(state);
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

void SCDataModel::handleOpen(QString fileName)
{
    qDebug() << "SCDataModel::handleOpen";

    QList<SCState*> states;
    this->getAllStates(states);

    while(states.size()!=0)
    {
        // clear out the data model
        // _project->getDM()->reset();
        //qDebug() << "\n";
        for(int i = 0; i < states.size(); i++)
        {
            //states.at(i)->deleteLater();
            qDebug() << "state was not deleted: "<<states.at(i)->objectName();
        }
        states.clear();

        QTime dieTime= QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        this->getAllStates(states);
    }

    this->openFile(fileName);

}

void SCDataModel::handleReset()
{
    qDebug() << "SCDataModel::handleReset";
    this->reset();
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

    //emit newStateSignal(_topState);

    qDebug() << "initialized empty state machine! " << "_currentState: " <<nm->asString()<<" on level: "<<_level;
    delete sa;

}


SCState* SCDataModel::handleMakeANewState(SCState* parent,StateAttributes*  sa)
{
    qDebug() << "SCDataModel::handleMakeANewState on level " << parent->getLevel()+1;
    SCState* state = NULL;
    state = new SCState(parent);
    state->attributes.setAttributes( *sa);
    state->setLevel(parent->getLevel()+1);

    if(state->isInitial())
    {
        state->parentAsSCState()->setInitialState(state);
    }

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



    //_currentState  = state;

    connectState(state);
    // connected to scformview slot handleNewState
    // connected to scgraphicsview slot handleNewState
    emit newStateSignal(state);


    delete sa;

    return state;
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

    if(state->isInitial())
    {
        state->parentAsSCState()->setInitialState(state);
    }

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

    _currentState  = state;

    connectState(state);
    // connected to scformview slot handleNewState
    // connected to scgraphicsview slot handleNewState
    emit newStateSignal(state);


    delete sa;
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
    transition->setAttributeValue("uid", target->getUid());
    target->addTransitionReference(transition, SCState::kTransitIn);
    source->addTransitionReference(transition, SCState::kTransitOut);
    transition->setTargetState(target);

    _transitions.append(transition);        // add to the total list of transitions
    connectTransition(transition);
    qDebug() << "@@@ adding transition out reference for state " << source->attributes.value("name")->asString();

    emit insertNewTransitionSignal(transition);
    return transition;
}

SCForkedTransition* SCDataModel::insertNewTransition(QList<SCState *> states, SCState *target)
{
    SCForkedTransition* ft = new SCForkedTransition();
    ft->setTargetState(target);
    qDebug() << "created ForkedTransition: " << ft->attributes.value("forkUid")->asString();

    ft->setTargetState(target);

    foreach(SCState* st, states)
    {
        qDebug() << "addedState: " << st->objectName();
        SCTransitionBranch* tb = ft->addSourceState(st);

        tb->setAttributeValue("sourceUid",st->getUid());

        target->addTransitionReference(tb, SCState::kTransitIn);
        st->addTransitionReference(tb, SCState::kTransitOut);
    }

    //_transitions.append();

    emit this->insertNewTransitionSignal(ft);
    return ft;
}

/**
 * @brief SCDataModel::insertNewTransition
 * @param source
 * @param target
 * @param eventName
 * @param pathString
 * @return
 *
 * functions similarly to insertNewTransition with only source and target, but sets the event name and path
 *
 */
SCTransition* SCDataModel::insertNewTransition(SCState *source, SCState* target, QString eventName, QString pathString )
{
    if ( source == NULL)  return NULL;

    SCTransition * transition = new SCTransition(source);

    transition->setAttributeValue("target", target->attributes.value("name")->asString());
    transition->setAttributeValue("uid",target->getUid());
    transition->setAttributeValue("event", eventName);
    transition->setObjectName(eventName);
    target->addTransitionReference(transition, SCState::kTransitIn);
    source->addTransitionReference(transition, SCState::kTransitOut);
    transition->setTargetState(target);

    transition->setPathAttr(pathString);

    _transitions.append(transition);        // add to the total list of transitions
    connectTransition(transition);

    emit insertNewTransitionSignal(transition);
    return transition;
}


SCTransition* SCDataModel::handleMakeANewTransition(SCState* source, TransitionAttributes* ta)
{
    qDebug() << "handleMakeANewTransition, : "  + ta->value("target")->asString() << "\t" << ta->value("event")->asString();

    SCTransition * transition = new SCTransition(source);

    if ( source == 0)
        return NULL;

    transition->attributes.setAttributes(*ta);
    transition->setText(transition->attributes.value("event")->asString());

    TransitionPathAttribute *path = dynamic_cast<TransitionPathAttribute *>( transition->attributes.value("path"));

    // this will interpret the path values as points
    path->setValue( path->asString() );
    source->addTransistion(transition);
    _transitions.append(transition);
    connectTransition(transition);
    _importedTransitions.append(transition);    // add to the list of transitions that stil need connect


    delete ta;

    // alert the graphicsview and form view that there is a new transition
    emit this->newTransitionSignal(transition);

    return transition;

    qDebug() << "leave handleMakeANewTransition, : "  ;
}

void SCDataModel::handleTransitionBeingDeleted(QObject*o)
{
    SCTransition* t = dynamic_cast<SCTransition*>(o);
    if ( _transitions.contains(t)) _transitions.removeAll(t);
}

/**
 * @brief SCDataModel::handleMakeANewTransition
 * @param ta
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

    qDebug() << "do the dynamic cast on trans path";
    TransitionPathAttribute *path =
            dynamic_cast<TransitionPathAttribute *>( transition->attributes.value("path"));

    path->setValue( path->asString() );

    _currentTransition = transition;
    _currentState->addTransistion(transition);

    _transitions.append(transition);
    connectTransition(transition);

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

/**
 * @brief SCDataModel::handleLeaveTransitionElement
 * notifies the graphicsview and form view that there is a new transition element
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
    TransitionStringAttribute *targetAttr =  dynamic_cast<TransitionStringAttribute *>( _currentTransition->attributes.value("target"));

    QString target = targetAttr->asString();

    qDebug() << "handleMakeANewTransitionPath (" + target + ") : " + pathStr ;

    if ( _currentState == 0)
        return;

    if ( _currentTransition == 0 ) return;

    TransitionPathAttribute *path =
            dynamic_cast<TransitionPathAttribute *>( _currentTransition->attributes.value("path"));

    path->setValue( pathStr );

    qDebug() << "interpreted path is " + path->asString();

    _currentTransition->attributes.addItem(path);

    qDebug() << "leave handleMakeANewTransitionPath : " ;

}


/**
 * @brief SCDataModel::handleStateFontChanged
 * @param font
 *
 * change all states fonts to given Qfont
 */
void SCDataModel::handleStateFontChanged(QFont * font)
{
    QList<SCState*> states;
    _topState->getAllStates(states);

    foreach(SCState* st, states)
    {
        st->setFont(font->family());
        st->setFontSize(font->pointSize());
    }

}

/**
 * @brief SCDataModel::handleTransitionFontChanged
 * @param font
 *
 * change all transition fonts to given qfont
 */
void SCDataModel::handleTransitionFontChanged(QFont * font)
{
    QList<SCTransition*> transitions;
    _topState->getAllTransitions(transitions);

    foreach(SCTransition* trans, transitions)
    {
        trans->setFont(font->family());
        trans->setFontSize(font->pointSize());
    }

}



/**
 * @brief SCDataModel::checkDataModel
 *
 * checks data model for exporting correctness
 * called before export procedure (in mainwindow.cpp) to alert user if any errors will be produced in the generated code
 */
bool SCDataModel::checkDataModel()
{
    QString errorLog;

    // initial states check
    // ensures that user has an initial state for all non-parallel state machines
    QList<SCState*> stateMachines = this->getStateMachines();
    foreach(SCState* sm, stateMachines)
    {
        if(sm->isParallel())
        {
            QList<SCState*> initialStates;
            QList<SCState*> directChildren;
            sm->getStates(directChildren);
            foreach(SCState* child, directChildren)
            {
                if(child->isInitial())
                    initialStates.append(child);
            }

            if(initialStates.size()!=0)
            {
                //            errorLog.append("State");
                errorLog.append("State machine \""+sm->objectName()+"\" is parallel and should not have an initial state\n");
            }
        }
        else
        {
            // check number of initial states
            QList<SCState*> initialStates;
            QList<SCState*> directChildren;
            sm->getStates(directChildren);
            foreach(SCState* child, directChildren)
            {
                if(child->isInitial())
                    initialStates.append(child);
            }

            if(initialStates.size()==0)
            {
                //            errorLog.append("State");
                errorLog.append("State machine \""+sm->objectName()+"\" is missing an initial state\n");
            }
            else if(initialStates.size()>1)
            {
                errorLog.append("State machine \""+sm->objectName()+"\" has too many initial states ("+QString::number(initialStates.size())+")\n");
            }
        }
    }

    // if there were errors, then emit a message and return false
    if(!errorLog.isEmpty())
    {
        emit message(errorLog);
        return false;
    }

    // return true if there were no errors
    return true;
}

QList<SCState*> SCDataModel::getStateMachines()
{
    QList<SCState*> sm;
    QList<SCState*> allStates;

    // add the top state and add all of its children
    allStates.append(_topState);
    _topState->getAllStates(allStates);

    // add every statemachine to sm
    foreach(SCState* st, allStates)
        if(st->isStateMachine())
            sm.append(st);

    return sm;
}

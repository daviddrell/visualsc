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
     _topState(NULL),
    _reader(&_topState),
    _writer(0),
    _level(0),
    _topLevel(0),
    _currentState(NULL),
    _currentTransition(NULL),
    _scene(0)
{


}

SCDataModel * SCDataModel::singleton()
{
    static SCDataModel * instance=NULL;
    if ( instance == NULL)
        instance = new SCDataModel(NULL);

    return instance;
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
    StateAttributes::StateString* type =  dynamic_cast<StateAttributes::StateString*> ( _topState->attributes.value("type"));

    if ( type  && ( type->asString() == "machine"))
    {
        _writer->writeStartElement("scxml");
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

void SCDataModel::open(QString file)
{

    _reader.readFile(file);

    _reader.run();

    bool ok;
    QStringList messages;
    _reader.getReadResult(ok, messages);

    if ( ok )
    {
        // now for the second pass, connect all the transitions to the states in the state-path.
        // the state-path is all of the state's which a transition must transit through on the path to its taget state.
        //

        connectTransitionsToStatePath();
    }

    emit openCompleted ( ok,  messages );

}

void SCDataModel::connectTransitionsToStatePath()
{
    QList<SCTransition *> tlist;

    _topState->getAllTransitions(tlist);

    for (int t = 0; t < tlist.count(); t++)
    {
        SCTransition* trans = tlist.at(t);

        QString targetStr = trans->attributes["target"]->asString();;

        SCState * targetState = _topState->getStateByName(targetStr);

        if ( targetState )
        {
            makeTransitionConnections(targetState, trans);
        }
    }
}



void SCDataModel::makeTransitionConnections(SCState * targetState, SCTransition* trans)
{
    // work backwards from target state to its ancestors, from the transitions source state to its ancestors, until
    // both have a common ancestor

    targetState->addTransitionReference(trans, SCState::kDestination);

    SCState * sourceState = dynamic_cast<SCState *> ( trans->parent());

    SCState *sourceTreeNode=NULL;
    SCState *targetTreeNode=NULL;

    int sourceLevel =  sourceState->getLevel();
    int targetLevel = targetState->getLevel();

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
         targetTreeNode->addTransitionReference(trans, SCState::kTransitOut);
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
}


void SCDataModel::getStates(QList<SCState *>& list)
{
    if ( _topState == NULL ) list = QList<SCState *>();

    return _topState->getStates(list);
}

bool SCDataModel::deleteItem(QObject * item)
{

    QList<SCState *> list;

    list.append(_topState);

    _topState->getAllStates(list);

    for(int i =0; i < list.count(); i++)
    {
        SCState *st = list.at(i);
        if ( item == st )
        {
            int i = list.indexOf(st);
            list.removeAt(i);
            delete st;
            return true;
        }
    }

    QList<SCTransition *> tlist;

    _topState->getAllTransitions(tlist);

    for(int i =0; i < tlist.count(); i++)
    {
        SCTransition *tr = tlist.at(i);
        if ( tr == item)
        {
            int i = tlist.indexOf(tr);
            tlist.removeAt(i);
            delete tr;
            return true;
        }
    }

    return false;
}


SCState* SCDataModel::insertNewState(SCState *parent)
{
    SCState * state = new SCState (parent);

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
    if ( _topState == NULL )
        return;

    return _topState->getAllStates(list);
}



void SCDataModel::initializeEmptyStateMachine()
{
    if ( _topState != 0 )
    {
        delete _topState;
        _topState = 0;
    }

    _topLevel =_level = 0;
    StateAttributes * stateAttributes = new StateAttributes(0,"stateAttributes");

    StateAttributes::StateString *  sa = new StateAttributes::StateString (0,"type","machine");
    stateAttributes->addItem( sa );

    StateAttributes::StateString *nsp = new StateAttributes::StateString(NULL,"xmlns","http://www.w3.org/2005/07/scxml");
    stateAttributes->addItem(nsp);

    StateAttributes::StateName *nm = new StateAttributes::StateName(NULL,"name", "State Machine");
    stateAttributes->addItem(nm);

    _topState = new SCState(true);
    _topState->attributes.setAttributes( *stateAttributes);
    _topState->setLevel(_level);


    _currentState  = _topState;

    emit newStateSignal(_topState);


    delete sa;

}


SCTransition* SCDataModel::insertNewTransition(SCState *source, QString target )
{
    if ( source == NULL)  return NULL;

    SCTransition * transition = new SCTransition(source);

    transition->setAttributeValue("target", target);

    emit newTransitionSignal(transition);

    return transition;
}






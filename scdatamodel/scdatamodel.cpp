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
    StateString* type =  dynamic_cast<StateString*> ( _topState->attributes.value("type"));

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

    connect(&_reader, SIGNAL(makeANewState(StateAttributes*)), this, SLOT(handleMakeANewState(StateAttributes*)), Qt::DirectConnection);
    connect(&_reader, SIGNAL(enterStateElement()), this, SLOT(handleTransitDown()), Qt::DirectConnection);
    connect(&_reader, SIGNAL(leaveStateElement()), this, SLOT(handleTransitUp()), Qt::DirectConnection);

    connect(&_reader, SIGNAL(makeANewIDTextBlockElement(TextBlockAttributes*)), this, SLOT(handleMakeANewIDTextBlock(TextBlockAttributes*)), Qt::DirectConnection);

    connect(&_reader, SIGNAL(makeANewTransistion(TransitionAttributes*)), this, SLOT(handleMakeANewTransition(TransitionAttributes*)), Qt::DirectConnection);
    connect(&_reader, SIGNAL(leaveTransistionElement()), this, SLOT(handleLeaveTransitionElement()), Qt::DirectConnection);
    connect(&_reader, SIGNAL(makeANewTransistionPath(QString)), this, SLOT(handleMakeANewTransitionPath(QString)), Qt::DirectConnection);

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


void SCDataModel::initializeEmptyStateMachine()
{
    if ( _topState != 0 )
    {
        delete _topState;
        _topState = 0;
    }

    _topLevel =_level = 0;
    StateAttributes * stateAttributes = new StateAttributes(0,"stateAttributes");

    StateString *  sa = new StateString (0,"type","machine");
    stateAttributes->addItem( sa );

    StateString *nsp = new StateString(NULL,"xmlns","http://www.w3.org/2005/07/scxml");
    stateAttributes->addItem(nsp);

    StateName *nm = new StateName(NULL,"name", "Root Machine");
    stateAttributes->addItem(nm);

    _topState = new SCState(true);
    _topState->attributes.setAttributes( *stateAttributes);
    _topState->setLevel(_level);


    _currentState  = _topState;

    emit newStateSignal(_topState);


    delete sa;

}

void SCDataModel::handleMakeANewState(StateAttributes*  sa)
{
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

        qDebug() << "adding state at level  :" + QString::number(_level) + ", name : " + name;
    }



    _currentState  = state;

    emit newStateSignal(state);


    delete sa;

}

void SCDataModel::insertNewProperty(SCItem* item, QString propertyName)
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
    }

    //item->addAttribute(propertyName,"");

#ifdef USE_THE_STRING_INSTEAD
    qDebug() << "SCDataModel::insertNewProperty\titemType " << itemType;
    if(itemType.toLower() == "state")
    {
        qDebug() << "current state: "  << _currentState->objectName();
        _currentState->addAttribute(propertyName, "");
    }
    else if(itemType.toLower() == "transition")
    {

        _currentTransition->addAttribute(propertyName, "");
    }
    else
    {
        qDebug() << "SCDataModel::insertNewProperty ERROR Given unexpected type: " << propertyName;
    }
#endif
}

SCTransition* SCDataModel::insertNewTransition(SCState *source, QString target )
{
    if ( source == NULL)  return NULL;

    SCTransition * transition = new SCTransition(source);

    transition->setAttributeValue("target", target);

    emit newTransitionSignal(transition);   // connected to scgraphics view's handleNewTransition

    return transition;
}


void SCDataModel::handleMakeANewTransition(TransitionAttributes * ta)
{
    qDebug() << "handleMakeANewTransition, : "  + ta->value("target")->asString() << "\t" << ta->value("event")->asString();

    SCTransition * transition = new SCTransition(_currentState);

    if ( _currentState == 0)
        return;

    transition->attributes.setAttributes(*ta);

    _currentTransition = transition;
    _currentState->addTransistion(transition);



    delete ta;

    qDebug() << "leave handleMakeANewTransition, : "  ;

}

void SCDataModel::handleLeaveTransitionElement()
{
    qDebug() << "handleLeaveTransitionElement ";

    emit newTransitionSignal(_currentTransition);

}

void SCDataModel::handleMakeANewTransitionProperty(const QString name)
{
    qDebug() << "handle make a new transition property " << name;
}


void SCDataModel::handleMakeANewIDTextBlock ( TextBlockAttributes *attributes)
{
    if  ( _currentState == NULL )
        return;

    QString text = _currentState->attributes["name"]->asString();

    SCTextBlock *textBlock = _currentState->getIDTextBlock();
    qDebug()<<" handleMakeANewIDTextBlock textBlock=" +QString::number((int)textBlock)+", current state= "+ _currentState->objectName();

    textBlock->setText(text);

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



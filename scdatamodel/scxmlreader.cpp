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
#include "scxmlreader.h"
#include <QFile>
#include <iostream>
#include "scstate.h"
#include <QPoint>
#include <QString>
#include <QStringList>
#include <QDomDocument>
#include "sctransition.h"
#include "scstate.h"
#include <QDomNamedNodeMap>
#include <QDebug>

SCXMLReader::SCXMLReader(SCState **topState): QObject(NULL),
    _reader(),
    _file(),
    _resultMessages(),
    _currentState(NULL),
    _currentTransition(NULL),
    _error(false),
    _topState(topState)
{

}

void SCXMLReader::readFile(QString infile)
{
    _file = infile;
}


void SCXMLReader::run()
{

    _currentState = NULL;

    QDomDocument doc("mydocument");
    QFile file(_file);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();


    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
           readElement(&e,-1);
        }
        n = n.nextSibling();
    }


    emit done(_error, _resultMessages);
}

void SCXMLReader::readElement(QDomElement *e, int stateLevel)
{

    SCState * prevState = _currentState;

    if ( e->tagName()  == "scxml")
    {
        stateLevel++;

        _currentState = readState(e, kSTATE_TYPE_Machine, stateLevel);

        QDomNode n = e->firstChild();
        while(!n.isNull())
        {
            QDomElement ce = n.toElement(); // try to convert the node to an element.
            if(!ce.isNull())
            {
                readElement(&ce, stateLevel);
            }
            n = n.nextSibling();
        }

        _currentState = prevState;
    }
    else if ( e->tagName()  == "initial")
    {
        stateLevel++;

        _currentState = readState(e, kSTATE_TYPE_Initial, stateLevel);

        QDomNode n = e->firstChild();
        while(!n.isNull())
        {
            QDomElement ce = n.toElement(); // try to convert the node to an element.
            if(!ce.isNull())
            {
                readElement(&ce, stateLevel);
            }
            n = n.nextSibling();
        }

        _currentState = prevState;
    }
    else if ( e->tagName()  == "final")
    {
        stateLevel++;

        _currentState = readState(e, kSTATE_TYPE_Final, stateLevel);

        QDomNode n = e->firstChild();
        while(!n.isNull())
        {
            QDomElement ce = n.toElement(); // try to convert the node to an element.
            if(!ce.isNull())
            {
                readElement(&ce, stateLevel);
            }
            n = n.nextSibling();
        }

        _currentState = prevState;
    }
    else if ( e->tagName() == "state")
    {
        stateLevel++;

        _currentState = readState(e, kSTATE_TYPE_Normal, stateLevel);

        QDomNode n = e->firstChild();
        while(!n.isNull())
        {
            QDomElement ce = n.toElement(); // try to convert the node to an element.
            if(!ce.isNull())
            {
                readElement(&ce);
            }
            n = n.nextSibling();
        }

        _currentState = prevState;
    }
    else if ( e->tagName() == "transition")
    {
        readTransistion(e);

        QDomNode n = e->firstChild();
        while(!n.isNull())
        {
            QDomElement ce = n.toElement(); // try to convert the node to an element.
            if(!ce.isNull())
            {
                readElement(&ce, stateLevel);
            }
            n = n.nextSibling();
        }
    }
    else if ( e->tagName() == "textblock")
    {
        readIDTextBlockElement(e);
    }
    else if ( e->tagName() == "path")
    {
        readTransistionPath(e);
    }


}

void SCXMLReader::getReadResult(bool &success, QStringList& message)
{
    success  = _error;
    message = _resultMessages;
}


SCState* SCXMLReader::readState(QDomElement *e,STATE_TYPE stateType, int stateLevel)
{
    // make a parentless attribute container to hold all xml attributes.
    // this container will be passed to the instantiated state which will
    // then copy these attributes to its private container.

    // memory is allocated for the attributes on the heap. The slot will
    // delete it when its done.

    StateAttributes * stateAttributes = new StateAttributes(0,"stateAttributes");

    QString stateTypeStr;
    switch ( stateType)
    {
    case  kSTATE_TYPE_Normal:
        stateTypeStr = "normal";
        break;

    case  kSTATE_TYPE_Initial:
        stateTypeStr = "initial";
        break;

    case  kSTATE_TYPE_Final:
        stateTypeStr = "final";
        break;

    case  kSTATE_TYPE_Machine:
        stateTypeStr = "machine";        
        break;
    }

    StateAttributes::StateString *  sa = new StateAttributes::StateString (0,"type", stateTypeStr );
    stateAttributes->addItem( sa );

    QString idStr = e->attribute("id");
    QString sizeStr = e->attribute("size");
    QString positionStr = e->attribute("position");

    if ( ! idStr.isEmpty() )
    {
        qDebug()<<"reader in state id = " + idStr;
        StateAttributes::StateName *  sa = new StateAttributes::StateName (0,"name",idStr );
        stateAttributes->addItem( sa );
    }

    if ( ! sizeStr.isEmpty() )
    {
        SizeAttribute * sa = new SizeAttribute(NULL, "size", sizeStr);
        stateAttributes->addItem( sa );
    }

    if ( ! positionStr.isEmpty() )
    {
        PositionAttribute * sa  = new PositionAttribute(NULL, "position", positionStr);
        stateAttributes->addItem( sa );
    }


    return makeANewState( stateAttributes, stateLevel);

}


void SCXMLReader::readTransistion(QDomElement *e)
{

    QString eventStr = e->attribute("event");
    QString condStr = e->attribute("cond");
    QString targetStr = e->attribute("target");
    QString typeStr = e->attribute("type");

    TransitionAttributes * ta = new TransitionAttributes(0,"TransitionAttributes");

    TransitionAttributes::TransitionStringAttribute * event = new TransitionAttributes::TransitionStringAttribute(NULL,"event", eventStr);

    TransitionAttributes::TransitionStringAttribute * cond = new TransitionAttributes::TransitionStringAttribute (NULL,"cond", condStr);

    TransitionAttributes::TransitionStringAttribute * target = new TransitionAttributes::TransitionStringAttribute (NULL,"target", targetStr);

    TransitionAttributes::TransitionStringAttribute * ttype = new  TransitionAttributes::TransitionStringAttribute (NULL,"type", typeStr);

    ta->addItem(event);
    ta->addItem(cond);
    ta->addItem(target);
    ta->addItem(ttype);


    if ( ! typeStr.isEmpty() )
    {
        if (typeStr.toLower() == "internal")
        {   
            ta->value("type")->setValue("internal");
        }
        else
        {   
            ta->value("type")->setValue("external");
        }
    }


    SCTransition * transition = new SCTransition(_currentState);

    transition->attributes.setAttributes(*ta);

    _currentState->addTransistion(transition);

    _currentTransition = transition;

    delete ta;

}




void SCXMLReader::readTransistionPath(QDomElement * e)
{

    QString data = e->attribute("d");

    qDebug()<<"readTransistionPath = " + data;

    TransitionAttributes::TransitionStringAttribute *targetAttr =
            dynamic_cast<TransitionAttributes::TransitionStringAttribute *>( _currentTransition->attributes.value("target"));

    QString target = targetAttr->asString();

    qDebug() << "handleMakeANewTransitionPath (" + target + ") : " + data ;

    if ( _currentTransition == 0 )
        return;

    TransitionAttributes::TransitionPathAttribute *path =
            dynamic_cast<TransitionAttributes::TransitionPathAttribute *>( _currentTransition->attributes.value("path"));

    path->setValue( data );

    qDebug() << "interpreted path is " + path->asString();

    _currentTransition->attributes.addItem(path);

}



void SCXMLReader::readIDTextBlockElement(QDomElement * e)
{

    QDomNamedNodeMap attMap = e->attributes();

    TextBlockAttributes * tb = new TextBlockAttributes(0,"TextBlockAttributes");

    for (int i = 0; i < (int)attMap.length(); i++)
    {
        QDomNode node = attMap.item(i);
        QDomAttr attr = node.toAttr();
        if (attr.isNull())
            continue;
        GenericAttribute * a  = new GenericAttribute(NULL,attr.name(), attr.value());
        tb->addItem( a );
    }


    QString text = _currentState->attributes["name"]->asString();

    SCTextBlock *textBlock = _currentState->getIDTextBlock();

    textBlock->setText(text);

    textBlock->attributes.setAttributes(*tb);

    delete tb;
}




void SCXMLReader::readFinal()
{
    /*
    SCState * state = new SCState();

    QString name = _reader.attributes().value("id").toString();

    state->setName(name);

    _pd->states.append(state);
    */


}

void SCXMLReader::readOnEntry()
{
    /*
    SCState * state = new SCState();

    QString name = _reader.attributes().value("id").toString();

    state->setName(name);

    _pd->states.append(state);
    */


}

void SCXMLReader::readOnExit()
{
    /*
    SCState * state = new SCState();

    QString name = _reader.attributes().value("id").toString();

    state->setName(name);

    _pd->states.append(state);
    */


}




SCState * SCXMLReader::makeANewState(StateAttributes*  sa, int stateLevel)
{
    SCState * state = NULL;

    if ( _currentState == 0 )
    {

        // TODO: add scxml rules check - top state must be element 'scxml' and the reader should have assign an attribute type='machine'

        state = new SCState(true);
        state->attributes.setAttributes( *sa);
        state->setLevel(stateLevel);


        QString name;

        if ( ! sa->contains("name"))
        {
            name = "State Machine";

            StateAttributes::StateName *nm = new StateAttributes::StateName(NULL,"name",name);
            state->attributes.addItem(nm);
        }
        else
        {
            name = sa->value("name")->asString();
        }

        state->setObjectName( name);

        *_topState = state;

        qDebug() << "adding new state at top level : " + state->attributes.value("name")->asString();
    }
    else
    {


        state = new SCState(_currentState);
        state->attributes.setAttributes( *sa);
        state->setLevel(stateLevel);

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

            StateAttributes::StateName *nm = new StateAttributes::StateName(NULL,"name",name);
            state->attributes.addItem(nm);
        }
        else
        {
            name = sa->value("name")->asString();
        }

        state->setObjectName( name );

        qDebug() << "adding state at level  :" + QString::number(stateLevel) + ", name : " + name;

    }


    delete sa;

    return state;

}



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

#include "sctransition.h"
#include "scstate.h"

#include <QDebug>

SCXMLReader::SCXMLReader(): QThread(NULL), _reader(),_file(), _resultMessages()
{

}

void SCXMLReader::readFile(QString infile)
{
    _file = infile;
}


void SCXMLReader::run()
{
    bool error =false;

    QFile file(_file);

    if ( ! file.open(QFile::ReadOnly | QFile::Text))
    {
        _resultMessages   << QString("Error: Cannot read file ") << (_file)
                << QString(": ") << (file.errorString());

        error = true;
    }

    _reader.setDevice(&file);

    _reader.readNext();

    while ( ! _reader.atEnd())
    {
        if ( ! _reader.isStartElement())
            _reader.readNext();
        else
            readElement();
    }

    file.close();

    emit done(error, _resultMessages);
}


void SCXMLReader::readElement()
{
    bool enteredAStateElement = false;
    bool enteredATransistionElement = false;
    bool enteredATransistionPathElement = false;
    bool elementAtEnd = true;

    if (_reader.name() == "scxml")
    {
        emit enterStateElement();
        readState(kSTATE_TYPE_Machine);
        enteredAStateElement = true;
    }
    if (_reader.name() == "state")
    {
        emit enterStateElement();
        readState(kSTATE_TYPE_Normal);
        enteredAStateElement = true;
    }
    else if (_reader.name() == "initial")
    {
        emit enterStateElement();
        readState(kSTATE_TYPE_Initial);
        enteredAStateElement = true;
    }
    else if (_reader.name() == "final")
    {
        emit enterStateElement();
        readState(kSTATE_TYPE_Final);
        enteredAStateElement = true;
    }
    else if (_reader.name() == "transition")
    {
        emit enterTransistionElement();
        readTransistion();
        enteredATransistionElement = true;
    }
    else if (_reader.name() == "path")
    {
        emit enterTransitionPathElement();
        readTransistionPath();
        enteredATransistionPathElement = true;
    }
    else if (_reader.name() == "textblock")
    {
        qDebug() <<"reading TextBlockElement";
        readTextBlockElement();
        return; // textBlocks do not contain elements
    }
    else if ( _reader.name() == "onentry")
        readOnEntry();
    else if ( _reader.name() == "onexit")
        readOnExit();


    _reader.readNext();

    while ( ! _reader.atEnd())
    {
        if ( _reader.isEndElement()  )
        {
            qDebug()<<"end of Element";
            _reader.readNext();
            break; // hit end of this tree
        }

        if ( _reader.isStartElement())
            readElement();

        _reader.readNext();
    }

    if (enteredAStateElement )
    {
        qDebug()<<"leaving StateElement";
        emit leaveStateElement();
    }
    if (enteredATransistionElement ) {emit leaveTransistionElement();}
    if (enteredATransistionPathElement ) {emit leaveTransitionPathElement();}

}

void SCXMLReader::readState(STATE_TYPE stateType)
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


    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);

        // look for the Sate Attribute Name (id in SCXML)
        if ( XmlAttr.name() == "id")
        {
            qDebug()<<"reader in state id = " + XmlAttr.value().toString();
            StateAttributes::StateName *  sa = new StateAttributes::StateName (0,"name", XmlAttr.value().toString() );
            stateAttributes->addItem( sa );
        }
        else if ( XmlAttr.name() == "size" )
        {
            SizeAttribute * sa = new SizeAttribute(NULL, XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
        else if ( XmlAttr.name() == "position" )
        {
            PositionAttribute * sa  = new PositionAttribute(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
        else // unknown attribute
        {
            StateAttributes::StateString * sa  = new StateAttributes::StateString(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
    }

    emit makeANewState( stateAttributes);

}

void SCXMLReader::readTransistion()
{
    TransitionAttributes * ta = new TransitionAttributes(0,"TransitionAttributes");

    TransitionAttributes::TransitionStringAttribute * event = new TransitionAttributes::TransitionStringAttribute(NULL,"event", _reader.attributes().value("event").toString());

    TransitionAttributes::TransitionStringAttribute * cond = new TransitionAttributes::TransitionStringAttribute (NULL,"cond", _reader.attributes().value("cond").toString());

    TransitionAttributes::TransitionStringAttribute * target = new TransitionAttributes::TransitionStringAttribute (NULL,"target", _reader.attributes().value("target").toString());

    TransitionAttributes::TransitionStringAttribute * ttype = new  TransitionAttributes::TransitionStringAttribute (NULL,"type", _reader.attributes().value("internal").toString());

    ta->addItem(event);
    ta->addItem(cond);
    ta->addItem(target);
    ta->addItem(ttype);


    if ( ! _reader.attributes().value("type").isEmpty() )
    {
        if (_reader.attributes().value("type").toString().toLower() == "internal")
        {   
            ta->value("type")->setValue("internal");
        }
        else
        {   
            ta->value("type")->setValue("external");
        }
    }

    emit makeANewTransistion(ta);

}


void SCXMLReader::readTransistionPath()
{

    QString data =_reader.attributes().value("d").toString();

    emit makeANewTransistionPath(data);
}


void SCXMLReader::readTextBlockElement()
{

    TextBlockAttributes * tb = new TextBlockAttributes(0,"TextBlockAttributes");

    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);
        GenericAttribute * a  = new GenericAttribute(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
        tb->addItem( a );
    }

    QString data =_reader.readElementText();

    emit makeANewTextBlockElement(data, tb);
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



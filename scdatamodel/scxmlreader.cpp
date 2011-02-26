#include "scxmlreader.h"
#include <QFile>
#include <iostream>
#include "scstate.h"
#include <QPoint>
#include <QString>
#include <QStringList>

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


    emit enterElement();

    if (_reader.name() == "state")
    {
        emit enterStateElement();
        readState();
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
    else if ( _reader.name() == "final")
        readFinal();
    else if ( _reader.name() == "onentry")
        readOnEntry();
    else if ( _reader.name() == "onexit")
        readOnExit();

    _reader.readNext();

    while ( ! _reader.atEnd())
    {
        if ( _reader.isEndElement())
        {
            _reader.readNext();
            break; // hit end of this tree
        }
        if ( _reader.isStartElement())
            readElement();

        _reader.readNext();
    }

    emit leaveElement();

    if (enteredAStateElement )       {emit leaveStateElement();}
    if (enteredATransistionElement ) {emit leaveTransistionElement();}
    if (enteredATransistionPathElement ) {emit leaveTransitionPathElement();}

}

void SCXMLReader::readState()
{
    // make a parentless attribute container to hold all xml attributes.
    // this container will be passed to the instantiated state which will
    // then copy these attributes to its private container.

    // memory is allocated for the attributes on the heap. The slot will
    // delete it when its done.

    StateAttributes * stateAttributes = new StateAttributes(0,"stateAttributes");

    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);

        // look for the Sate Attribute Name (id in SCXML)
        if ( XmlAttr.name() == "id")
        {
            StateAttributes::StateName *  sa = new StateAttributes::StateName (0,"name", XmlAttr.value().toString() );
            stateAttributes->addItem( sa );
        }
        else if ( XmlAttr.name() == "size" )
        {
            StateAttributes::StateSize * sa = new StateAttributes::StateSize(NULL, XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
        else if ( XmlAttr.name() == "position" )
        {
            StateAttributes::StatePosition * sa  = new StateAttributes::StatePosition(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
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

    ta->value("event")->setValue(_reader.attributes().value("event").toString());
    ta->value("cond")->setValue(_reader.attributes().value("cond").toString());
    ta->value("target")->setValue(_reader.attributes().value("target").toString());
    ta->value("type")->setValue("internal");

    /*
     event
     cond
     target
     type
     */

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
    delete ta;
}

void SCXMLReader::readTransistionPath()
{

    QString data =_reader.attributes().value("d").toString();

    emit makeANewTransistionPath(data);
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

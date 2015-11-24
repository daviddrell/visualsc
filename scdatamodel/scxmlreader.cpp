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
#include "transitionattributes.h"
#include "sctransition.h"
#include "scstate.h"
#include <QDebug>
#include "scdatamodel.h"

SCXMLReader::SCXMLReader(): QObject(NULL), _reader(),_file(), _resultMessages(), _error(false)
{

}

SCXMLReader::SCXMLReader(SCDataModel* dm): QObject(NULL), _reader(),_file(), _resultMessages(), _error(false), _dm(dm)
{

}

void SCXMLReader::setDataModel(SCDataModel* dm)
{
    _dm = dm;
}

void SCXMLReader::readFile(QString infile)
{
    _file = infile;
}

SCState* SCXMLReader::importFile(SCState* parent)
{
    //_currentItem = parent;
    _currentState = parent;
    QFile file(_file);
    if ( ! file.open(QFile::ReadOnly | QFile::Text))
    {
        _resultMessages   << QString("Error: Cannot read file ") << (_file)
                << QString(": ") << (file.errorString());

        _error = true;
    }

    _reader.setDevice(&file);
    _reader.readNext();

    // while the reader hasn't ended, keep reading elements
    while ( !_reader.atEnd())
    {
        // look for a start element
        if (!_reader.isStartElement())
            _reader.readNext();

        else    // start element found for a state.
        {
            qDebug() << "importFile loop";
            importElement(_currentState);
        }
    }

    // close the file
    file.close();

    // alert the graphicsview that the import process was complete
    emit _dm->importedMachine(_importedMachine);

    // not currently conected to anything
    emit done(_error, _resultMessages);

    return _importedMachine;
}

/**
 * @brief SCXMLReader::importElement
 * @param parent
 *
 * will create a new state inside of parent and recusively add the state's children elements including states, transitions, and text blocks
 */
void SCXMLReader::importElement(SCState* parent)
{
    qDebug() << "SCXMLReader::importElement READING... "<< _reader.name() <<" importing into parent: "<< parent->objectName();

    bool readState = false;
    bool readTrans = false;
    bool readText = false;
    bool readTransText = false;

    if(_reader.name() == "scxml")
    {
        qDebug() <<"importing scxml";
        _currentItemType = ItemType::STATE;

        // keep track of the machine that is imported, this will be used later to signal the graphicsview
        _importedMachine = _currentState = importStateMachine(parent);
        readState = true;

    }
    else if(_reader.name() == "state")
    {
        qDebug() << "importing state";
        _currentItemType = ItemType::STATE;
        _currentState = importState(parent);
        readState = true;
    }
    else if(_reader.name() == "transition")
    {
        qDebug() << "importing tranistion";
        _currentItemType = ItemType::TRANSITION;
        _currentTransition = importTransition(parent);
        readTrans = true;
    }
    else if(_reader.name() == "textblock")
    {

        switch(_currentItemType)
        {
        case ItemType::STATE:
            readText = true;
            qDebug() <<"reading State TextBlockElement";
            importIDTextBlockElement(parent);
            break;


        case ItemType::TRANSITION:
            readTransText = true;
            qDebug() <<"reading Event TextBlockElement";
            importEventTextBlockElement(_currentTransition);
            break;


        default:
            break;
        }
    }


    if(readTransText)
        return;

    qDebug() << "CURRENT STATE IS: " << _currentState->objectName();


    // now we recursively call this for all of the elements that belong to this state.

    // if a state was read, then _currentState is set to that.
    // now read all the elements that belong to this state until we reach an end of element
    // read the next element
    _reader.readNext();

    // while there are sub elements to this, keep calling this function
    bool continueLoop = true;
    while (! _reader.atEnd()&&(continueLoop))
    {
        if ( _reader.isEndElement())
        {
            qDebug()<<"end of Element found... :" <<_reader.name() ;
            if(_reader.name() == "state")
            {
                qDebug() << "CURRENT STATE = PARENT. PARENT is: " << parent->getName()<<+"\tCURRENT is:"<<_currentState->getName();
                _currentState = _currentState->parentAsSCState();
            }
            _reader.readNext();
            continueLoop = false;


            break; // hit end of this tree
        }

        else if ( _reader.isStartElement()){
            qDebug() << "this element has another start element! calling import Element for this state!";
            importElement(_currentState);
        }
        _reader.readNext();
    }
    qDebug() << "left importElement while loop.";

        // when we leave this element, set the current state back one level before what it was originally
//    if(!readState)
//    {
//        //        qDebug() << "CURRENT STATE = PARENT. PARENT is: " << parent->getName()<<+"\tCURRENT is:" <<_currentState->getName();
//        if(_currentState)
//            _currentState = _currentState->parentAsSCState();
//        //            qDebug() << "NOW*: CURRENT STATE = PARENT. PARENT is: " << parent->getName()<<+"\tCURRENT is:" <<_currentState->getName();
//    }

}

SCTransition* SCXMLReader::importTransition(SCState* source)
{
    TransitionAttributes * ta = new TransitionAttributes(0,"TransitionAttributes");

    for(int i = 0; i < _reader.attributes().size();i++)
    {
        QString key =  _reader.attributes().at(i).name().toString();
        QString value = _reader.attributes().at(i).value().toString();
        qDebug() << "reader attributes: " << _reader.attributes().at(i).name() << ":"<<_reader.attributes().at(i).value();
        TransitionStringAttribute* tsa = new TransitionStringAttribute(NULL, key, value);
        ta->addItem(tsa);
    }

    return _dm->handleMakeANewTransition(source, ta);
}

SCState* SCXMLReader::importState(SCState* parent)
{
    StateAttributes * stateAttributes = new StateAttributes(0,"stateAttributes");
    //StateString *  sa = new StateString (0,"type", "normal");
    //stateAttributes->addItem( sa );

    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);
//        qDebug() << "reading attribute: " << XmlAttr.name();
        // look for the Sate Attribute Name (id in SCXML)
        if ( XmlAttr.name() == "size" )
        {
            SizeAttribute * sa = new SizeAttribute(NULL, XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
        else if ( XmlAttr.name() == "position" )
        {
            PositionAttribute * sa  = new PositionAttribute(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
        else    // string attribute
        {
            StateString * sa  = new StateString(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
    }

    return _dm->handleMakeANewState(parent, stateAttributes);
}

SCState* SCXMLReader::importStateMachine(SCState *parent)
{
    StateAttributes * stateAttributes = new StateAttributes(0,"stateAttributes");
    //StateString *  sa = new StateString (0,"type", "normal");
    //stateAttributes->addItem( sa );

    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);
        //qDebug() << "reading attribute: " << XmlAttr.name();
        // look for the Sate Attribute Name (id in SCXML)
        if ( XmlAttr.name() == "size" )
        {
            SizeAttribute * sa = new SizeAttribute(NULL, XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
        else if ( XmlAttr.name() == "position" )
        {
            PositionAttribute * sa  = new PositionAttribute(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
        else if(XmlAttr.name() == "type")
        {
            // change the type from "machine"
            StateString* sa = new StateString(NULL, XmlAttr.name().toString(), "imported machine");
            stateAttributes->addItem(sa);
        }
        else    // string attribute
        {
            StateString * sa  = new StateString(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
    }

    return _dm->handleMakeANewState(parent, stateAttributes);
}

/**
 * @brief SCXMLReader::run
 *
 * attempts to read the file in _file
 * runs with loading the data model with states, transitions, and textblocks until the last end element is hit
 *
 * uses handleMakeANewState in scdatamodel
 *
 *
 */
void SCXMLReader::run()
{

    QFile file(_file);

    if ( ! file.open(QFile::ReadOnly | QFile::Text))
    {
        _resultMessages   << QString("Error: Cannot read file ") << (_file)
                << QString(": ") << (file.errorString());

        _error = true;
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

    // not currently conected to anything
    emit done(_error, _resultMessages);
}



void SCXMLReader::getReadResult(bool &success, QStringList& message)
{
    success  = !(_error);
    message = _resultMessages;
}

/**
 * @brief SCXMLReader::readStateMachine
 * Used to change the state machine's name in the data model when reading from this file
 *
 *
 * SIGNAL
 * connect in SCDataModel::connectDataModel()
 *
 */
void SCXMLReader::readStateMachine()
{
    for(int i = 0 ; i < _reader.attributes().count() ; i++)
    {
        QXmlStreamAttribute xmlAttr = _reader.attributes().at(i);
        if(xmlAttr.name() == "name")
        {
            emit changeStateMachineName(xmlAttr.value().toString());
        }
        else if(xmlAttr.name() == "uid")
        {
            emit changeStateMachineUid(xmlAttr.value().toString());
        }
        else if(xmlAttr.name() == "size")
        {

        }
        else if(xmlAttr.name() == "position")
        {

        }
        else
        {
            emit changeStateMachineAttribute(xmlAttr.name().toString(), xmlAttr.value().toString());
        }
    }
}

/**
 * @brief SCXMLReader::readElement
 * recursively read the elements in the scxml file
 */
void SCXMLReader::readElement()
{
    bool enteredAStateElement = false;
    bool enteredATransistionElement = false;
    bool enteredATransistionPathElement = false;

    qDebug() << "READING... "<< _reader.name();
#if 0
    if (_reader.name() == "scxml")
    {
        qDebug() << "skipping scxml";
        //emit enterStateElement();           // increase the transit level
        readStateMachine();
        //readState(kSTATE_TYPE_Machine);   // disabled adding a state every time openFile happens
        enteredAStateElement = true;
    }
#endif
    if (_reader.name() == "state" || _reader.name() == "scxml")
    {
        _currentItemType = ItemType::STATE;
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
        _currentItemType = ItemType::TRANSITION;
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
        switch(_currentItemType)
        {
        case ItemType::STATE:
            qDebug() <<"reading State TextBlockElement";
            readIDTextBlockElement();

            break;


        case ItemType::TRANSITION:
            qDebug() <<"reading Event TextBlockElement";
            readEventTextBlockElement();
            break;


        default:
            break;
        }

        return; // textBlocks do not contain elements
    }
    else if ( _reader.name() == "onentry")
        readOnEntry();
    else if ( _reader.name() == "onexit")
        readOnExit();
    else
    {
        qDebug() << "not sure what to here " << _reader.name();
    }


    _reader.readNext();

    while ( ! _reader.atEnd())
    {
        if ( _reader.isEndElement()  )
        {
            qDebug()<<"end of Element";
            _reader.readNext();
            break; // hit end of this tree
        }

        if ( _reader.isStartElement()){
            qDebug() << "this element has another start element!";
            readElement();
        }

        _reader.readNext();
    }

    if (enteredAStateElement )
    {
        qDebug()<< "leaveStateElement";
        emit leaveStateElement();
    }
    if (enteredATransistionElement )
    {
        qDebug()<< "leaveTransistionElement";
        emit leaveTransistionElement();
    }
    if (enteredATransistionPathElement )
    {
        qDebug()<< "leaveTransitionPathElement";
        emit leaveTransitionPathElement();
    }

}



/**
 * @brief SCXMLReader::readState
 * @param stateType
 *
 * uses the scxml data for a state element to create a stateattribute item for a new SCState in the data model
 *
 * SIGNAL
 * makeANewState( stateAttributes);
 *
 * connect in SCDataModel
 * connect(&_reader, SIGNAL(makeANewState(StateAttributes*)), this, SLOT(handleMakeANewState(StateAttributes*)), Qt::DirectConnection);
 *
 *
 */
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

    StateString *  sa = new StateString (0,"type", stateTypeStr );
    stateAttributes->addItem( sa );


    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);
//        qDebug() << "reading attribute: " << XmlAttr.name();
        // look for the Sate Attribute Name (id in SCXML)
        if ( XmlAttr.name() == "id" || XmlAttr.name() == "name")
        {
            qDebug()<<"reader in state id = " + XmlAttr.value().toString();
            StateName *  sa = new StateName (0,"name", XmlAttr.value().toString() );
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
        else if( XmlAttr.name() == "isParallelState")   // retroactive change to make this parallelState instead
        {
            StateString * sa  = new StateString(NULL,"parallelState", XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
        else // unknown attribute, generalize it as a string attribute and add it
        {
            StateString * sa  = new StateString(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
            stateAttributes->addItem( sa );
        }
    }

    //  connect(&_reader, SIGNAL(makeANewState(StateAttributes*)), this, SLOT(handleMakeANewState(StateAttributes*)), Qt::DirectConnection);

    // signal connected to handleMakeANewState in scdatamodel
    emit makeANewState( stateAttributes);
}

/**
 * @brief SCXMLReader::readTransistion
 *
 * creates a new TransitionAttributes object and reads from the scxml
 *
 * then emits the makeANewTransitionSignal for the data model to create a new SCTransition
 *
 * SIGNAL
 * connect(&_reader, SIGNAL(makeANewTransistion(TransitionAttributes*)), this, SLOT(handleMakeANewTransition(TransitionAttributes*)), Qt::DirectConnection);
 *
 */
void SCXMLReader::readTransistion()
{
    TransitionAttributes * ta = new TransitionAttributes(0,"TransitionAttributes");

    for(int i = 0; i < _reader.attributes().size();i++)
    {
        QString key =  _reader.attributes().at(i).name().toString();
        QString value = _reader.attributes().at(i).value().toString();
        qDebug() << "reader attributes: " << _reader.attributes().at(i).name() << ":"<<_reader.attributes().at(i).value();
        TransitionStringAttribute* tsa = new TransitionStringAttribute(NULL, key, value);
        ta->addItem(tsa);
    }

    emit makeANewTransistion(ta);   // connected to handleMakeANewTransition in scdatamodel.cpp

}


void SCXMLReader::readTransistionPath()
{

    QString data =_reader.attributes().value("d").toString();

    qDebug()<<"readTransistionPath = " + data;

    emit makeANewTransistionPath(data);
}

void SCXMLReader::importIDTextBlockElement(SCState* state)
{
    TextBlockAttributes * tb = new TextBlockAttributes(0,"TextBlockAttributes");

    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);
        GenericAttribute * a  = new GenericAttribute(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
        tb->addItem( a );
    }

   _reader.readElementText(); //-- id-textblock gets it text data from the state ID

   state->getIDTextBlock()->attributes.setAttributes(*tb);
   delete tb;
    //emit makeANewIDTextBlockElement(tb);   // connected to handleMakeANewIDTextBlock in scdatamodel
}

void SCXMLReader::importEventTextBlockElement(SCTransition* parent)
{

    qDebug()<<"importEventTextBlockElement";

    TextBlockAttributes * tb = new TextBlockAttributes(0,"TextBlockAttributes");

    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);
        GenericAttribute * a  = new GenericAttribute(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
        tb->addItem( a );
        //qDebug() << "adding attribute: "<<a->asString();
    }

   _reader.readElementText(); //-- id-textblock gets it text data from the state ID

   parent->getEventTextBlock()->attributes.setAttributes(*tb);
   delete tb;
   // emit makeANewTransitionTextBlockElement(tb); //connected to handleMakeANewTransitionTextBlock in scdatamodel
    //emit makeANewIDTextBlockElement( tb);
}

/**
 * @brief SCXMLReader::readIDTextBlockElement
 *
 * SIGNAL
 *
 * connect in scdatamodel
 * connect(&_reader, SIGNAL(makeANewIDTextBlockElement(TextBlockAttributes*)), this, SLOT(handleMakeANewIDTextBlock(TextBlockAttributes*)), Qt::DirectConnection);
 */
void SCXMLReader::readIDTextBlockElement()
{

    qDebug()<<"readIDTextBlockElement";

    TextBlockAttributes * tb = new TextBlockAttributes(0,"TextBlockAttributes");

    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);
        GenericAttribute * a  = new GenericAttribute(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
        tb->addItem( a );
    }

   _reader.readElementText(); //-- id-textblock gets it text data from the state ID

    emit makeANewIDTextBlockElement(tb);   // connected to handleMakeANewIDTextBlock in scdatamodel
}

void SCXMLReader::readEventTextBlockElement()
{

    qDebug()<<"readEventTextBlockElement";

    TextBlockAttributes * tb = new TextBlockAttributes(0,"TextBlockAttributes");

    for (int i = 0; i < _reader.attributes().count(); i++)
    {
        QXmlStreamAttribute XmlAttr =_reader.attributes().at(i);
        GenericAttribute * a  = new GenericAttribute(NULL,XmlAttr.name().toString(), XmlAttr.value().toString());
        tb->addItem( a );
        //qDebug() << "adding attribute: "<<a->asString();
    }

   _reader.readElementText(); //-- id-textblock gets it text data from the state ID

    emit makeANewTransitionTextBlockElement(tb); //connected to handleMakeANewTransitionTextBlock in scdatamodel
    //emit makeANewIDTextBlockElement( tb);
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



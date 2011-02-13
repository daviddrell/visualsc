#include "scxmlreader.h"
#include <QFile>
#include <iostream>
#include "scstate.h"
#include "stateattributes.h"
#include "transitionattributes.h"
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
    StateAttributes * sa = new StateAttributes();

    sa->name =_reader.attributes().value("id").toString();

    if ( ! _reader.attributes().value("position").isEmpty() )
    {
        QStringList sl = _reader.attributes().value("position").toString().split(",");
        if ( ! sl.isEmpty() && sl.length() > 0)
        {
            int x = sl[0].toInt()   ;
            int y = sl[1].toInt()   ;
            sa->position =QPoint (x,y);
        }

        sa->hasBeenSized = true;
    }

    if ( ! _reader.attributes().value("size").isEmpty() )
    {
        QStringList sl = _reader.attributes().value("size").toString().split(",");
        if ( ! sl.isEmpty() && sl.length() > 0)
        {
            int x = sl[0].toInt()   ;
            int y = sl[1].toInt()   ;
            sa->size =QPoint (x,y);
        }

        sa->hasBeenSized = true;

    }


    emit makeANewState(sa);
}

void SCXMLReader::readTransistion()
{
    TransitionAttributes * ta = new TransitionAttributes();

    ta->event =_reader.attributes().value("event").toString();
    ta->cond =_reader.attributes().value("cond").toString();
    ta->target =_reader.attributes().value("target").toString();

    ta->type = TransitionAttributes::Internal; // default value

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
            ta->type =  TransitionAttributes::Internal;
        }
        else
        {
            ta->type = TransitionAttributes::External;
        }
    }

   emit makeANewTransistion(ta);
}

void SCXMLReader::readTransistionPath()
{
    TransitionPathAttributes * tp = new TransitionPathAttributes();

    QString data =_reader.attributes().value("d").toString();

//    <path d="m 100 100 L 300 100 L 200 300 "
//           fill="red" stroke="blue" stroke-width="3" />

    //    <path d="m 100 100  300 100  200 300 "
    //           fill="red" stroke="blue" stroke-width="3" />

   // parse the data string
    QStringList sl = data.split(" ");

    for (int i=0; i < sl.length(); i++ )
    {
        if ( sl[i].trimmed().isEmpty())
            sl.removeAt(i);
    }

    for (int i=0; i < sl.length(); )
    {
        if ( sl[i] == "m" ) i++;
        if ( sl[i] == "L" ) i++;

        if (  sl.length() > (i+1))
        {
            qreal x = sl[i].toDouble();
            qreal y = sl[i+1].toDouble();
            tp->pathPoints.append( QPointF(x,y));
            i += 2;
        }
        else
            break;
    }

   emit makeANewTransistionPath(tp);
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

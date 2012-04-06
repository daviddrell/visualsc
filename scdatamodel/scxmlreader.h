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

#ifndef SCXMLREADER_H
#define SCXMLREADER_H

#include <QString>
#include <QStringList>
#include "scstate.h"
#include <QObject>

#include "stateattributes.h"
#include "transitionattributes.h"
#include "textblock.h"

class StateData;
class QDomElement;

#include "scdatamodel_global.h"

class  SCDATAMODELSHARED_EXPORT  SCXMLReader : public QObject
{
    Q_OBJECT

public:
    SCXMLReader( SCState **topState);
    void readFile(QString infile);
    virtual void run();

    void getReadResult(bool &success, QStringList& message);

signals:
     void done(bool result, QStringList message);



private:

    enum STATE_TYPE{kSTATE_TYPE_Normal=0, kSTATE_TYPE_Initial, kSTATE_TYPE_Final, kSTATE_TYPE_Machine};

    void readIDTextBlockElement(QDomElement *);
    void readElement(QDomElement *, int stateLevel=-1);
    SCState* readState(QDomElement *, STATE_TYPE t= kSTATE_TYPE_Normal, int stateLevel=-1);
    void readTransistion(QDomElement *e);
    void readTransistionPath(QDomElement * e);
    void readFinal();
    void readOnEntry();
    void readOnExit();

    SCState* makeANewState(StateAttributes*, int stateLevel);
    void makeANewTransistion(TransitionAttributes*);


    //private data

    QXmlStreamReader   _reader;
    QString            _file;
    QStringList        _resultMessages;
    SCState           *_currentState;
    SCTransition      *_currentTransition;
    bool               _error;
    SCState           **_topState;
};

#endif // SCXMLREADER_H

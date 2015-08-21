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
#include <QXmlStreamReader>
#include <QObject>

#include "stateattributes.h"
#include "transitionattributes.h"
#include "textblock.h"
#include "sctransition.h"//
//#include "scdatamodel.h"


class SCDataModel;
class StateData;

#include "scdatamodel_global.h"

class  SCDATAMODELSHARED_EXPORT  SCXMLReader : public QObject
{
    Q_OBJECT

    enum ItemType{
        STATE,
        TRANSITION,
        TEXTBLOCK
    };

public:
    SCXMLReader();
    SCXMLReader(SCDataModel* dm);
    void readFile(QString infile);
    virtual void run();
    SCState* importFile(SCState* parent);
    void setDataModel(SCDataModel* dm);
    void getReadResult(bool &success, QStringList& message);

signals:
    void done(bool result, QStringList message);
    void makeANewState(StateAttributes*);
    void makeANewChildState(SCState* parent, StateAttributes*);
    void enterStateElement();
    void leaveStateElement();

    void changeStateMachineAttribute(QString, QString);
    void changeStateMachineName(QString);
    void changeStateMachineUid(QString);
    void enterTransistionElement();
    void leaveTransistionElement();
    void makeANewTransistion(TransitionAttributes*);

    void enterTransitionPathElement();
    void leaveTransitionPathElement();
    void makeANewTransistionPath(QString path);
    void makeANewIDTextBlockElement( TextBlockAttributes*);
    void makeANewTransitionTextBlockElement(TextBlockAttributes*);



private:

    enum STATE_TYPE{kSTATE_TYPE_Normal=0, kSTATE_TYPE_Initial, kSTATE_TYPE_Final, kSTATE_TYPE_Machine};

    QXmlStreamReader _reader;
    QString _file;
    QStringList _resultMessages;
    int _currentItemType;
    //SCItem* _currentItem;
    SCState* _currentState;
    SCState* _importedMachine;
    SCTransition* _currentTransition;

    void importIDTextBlockElement(SCState* parent);
    void importEventTextBlockElement(SCTransition* parent);
    void readIDTextBlockElement();
    void readEventTextBlockElement();
    void readElement();
    void importElement(SCState* parent);
    void readState(STATE_TYPE t= kSTATE_TYPE_Normal);
    SCTransition* importTransition(SCState* source);
    SCState* importState(SCState* parent);
    SCState* importStateMachine(SCState* parent);
    void readTransistion();
    void readTransistionPath();
    void readFinal();
    void readOnEntry();
    void readOnExit();
    void readStateMachine();

    SCDataModel* _dm;

    //private data
    bool _error;
};

#endif // SCXMLREADER_H

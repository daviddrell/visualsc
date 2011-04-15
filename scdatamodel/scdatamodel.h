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

#ifndef SCDATAMODEL_H
#define SCDATAMODEL_H

#include "scdatamodel_global.h"
#include "sctransition.h"
#include "scstate.h"
#include <QList>
#include "scxmlreader.h"
#include <QXmlStreamWriter>

class SCDATAMODELSHARED_EXPORT SCDataModel : public QObject
{
      Q_OBJECT


public:
    SCDataModel(QObject * parent);

    // direct interface
    void open(QString scxmlFile);

    bool save(QString scxmlFile, QString & message);
    void initializeEmptyStateMachine();

    void getStates(QList<SCState *>& list);
    void getAllStates(QList<SCState *>& list);
    SCState* getStateByName(QString name);
    SCTransition * getTransitionByName(QString name);
    QObject * getItemByName(QString name);
    SCState* getTopState( );

    /** \fn insertNewState
      * \brief Creates and returns a pointer to a new state inserted in the provided parent state.
      */
    SCState* insertNewState(SCState *parent);

    /**
      * \fn insertNewTransition
      * \param source - the SCState from which the transition leaves
      * \param event  -  the string which represents the event that causes the transition to occur
      */
    SCTransition* insertNewTransition(SCState *source, QString event );


signals:
    void openCompleted(bool sucess, QStringList message);
    void newStateSignal(SCState * newState);
    void newTransitionSignal(SCTransition * newTransition);

private slots:
    void handleReaderDone(bool sucess, QStringList message);
    void handleMakeANewState(StateAttributes*);
    void handleMakeANewTransition(TransitionAttributes*);
    void handleLeaveTransitionElement();
    void handleMakeANewTransitionPath(QString path);
    void handleTransitUp();
    void handleTransitDown();

private:
    SCXMLReader     _reader;
    QXmlStreamWriter *_writer;
    int              _level;
    int              _topLevel;
    SCState         *_currentState;
    SCTransition    *_currentTransition;
    SCState         *_topState;


//private methods




};

#endif // SCDATAMODEL_H

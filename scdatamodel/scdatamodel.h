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
#include <QTextStream>
#include <codewriter.h>

class QGraphicsScene;

/**
 \defgroup DataModel

 Elements of a state chart are states and state transitions. Each of these elements have attributes.
 In addition, states and transitions can have sub elements such as commentary text. Each sub element may
 have attributes.

 Examples of attributes are a state's id/name, state type (initial, final, etc), a state's graphical position, size.

 The data model holds a reference to the top-level state (class SCState). The top level state is also known
 as the state-machine. The top level state holds an attribute container
 for its attributes as well as a QList of substates and a QList of transitions which are sourced from that state.

 <b> Notes on State and Transition Identifiers</b>

  SCXML does not require a state have an id. But here we definitely need a consistent
     method of refering to a state. So 'name' is used as the definitive identifier for states in this model.
     When the model is saved back to an SCXML file, the name is pushed as the 'id' field. So all states in SCXML
     files produced by this program will end up with an id.

  In SCXML, Transitions are identified by their target states with the 'target' attribute.
     In order to genericise attribute handling, this program identifies transitions with the 'name' key (same as for state
     identifiers). Thus internally, all states and transitions have 'name's. When saved back to an SCXML file,
     the transition name is written as the 'target' attribute.


*/


/**
  \class SCDataModel
  \ingroup DataModel
  */

class SCDATAMODELSHARED_EXPORT SCDataModel : public QObject
{
      Q_OBJECT


public:



    void connectDataModel();
    void reset();
    static SCDataModel * singleton();

    // direct interface
    void openFile(QString scxmlFileName);
    void open(QString scxmlFile);

    bool save(QString scxmlFile, QString & message);
    bool exportToCode(QString cName, QString &message);

    void initializeEmptyStateMachine();

    bool deleteItem(QObject * item);

    void getStates(QList<SCState *>& list);
    void getAllStates(QList<SCState *>& list);
    SCState* getStateByName(QString name);
    SCTransition * getTransitionByName(QString name);
    QObject * getItemByName(QString name);
    SCState* getTopState( );
    void setScene(QGraphicsScene *scene);
    QGraphicsScene * getScene();

    static SCState *getAsState(QObject*);
    static SCTransition *getAsTransition(QObject*);
    static SCTextBlock *getAsTextBlock(QObject*);

    /** \fn insertNewState
      * \brief Creates and returns a pointer to a new state inserted in the provided parent state.
      */
    SCState* insertNewState(SCState *parent);

    /**
      * \fn insertNewTransition
      * \param source - the SCState from which the transition leaves
      * \param event  -  the string which represents the event that causes the transition to occur
      */
    //SCTransition* insertNewTransition(SCState *source, QString event );
    SCTransition* insertNewTransition(SCState *source, SCState* target);

    void logError(QString message);///< inform the user there was an error
    void logInfo(QString message);///< inform the user of anything that is not an error such as progress, etc

    bool insertNewProperty(SCItem* item, QString propertyName);
    bool deleteProperty(SCItem* item, QString propertyName);

    bool insertNewTextBlock(SCItem* item, QString name);


signals:
    void openCompleted(bool sucess, QStringList message);
    void newStateSignal(SCState * newState);
    void newTransitionSignal(SCTransition * newTransition);
    void newTextBlockSignal(SCTransition*, QString);
    void transitionsReadyToConnect(SCTransition*);
    void formViewInsertNewTransitionSignal(SCTransition*);


private slots:

    void handleMakeANewState(StateAttributes*);
    void handleMakeANewTransition(TransitionAttributes*);
    void handleLeaveTransitionElement();
    void handleMakeANewTransitionPath(QString path);
    void handleTransitUp();
    void handleTransitDown();
    void handleMakeANewIDTextBlock(TextBlockAttributes *attributes);
    void handleMakeANewEventTextBlock(TextBlockAttributes* attributes);
    void handleMakeANewTransitionProperty(const QString name);
    void handleStateNameChangedInFormView(SCState*, QString);
    void handleStatePositionChangedInFormView(SCState*, QPointF);
    void handleStateSizeChangedInFormView(SCState*, QPointF);

    void handleEventNameChangedInFormView(SCTransition*, QString);
    void handleEventPositionChangedInFormView(SCTransition*, QString);
    void handleEventSizeChangedInFormView(SCTransition*, QString);


private:


    void connectState(SCState*);
    void connectTransition(SCTransition*);

    SCXMLReader     _reader;
    QXmlStreamWriter *_writer;
    int              _level;
    int              _topLevel;
    SCState         *_currentState;
    SCTransition    *_currentTransition;
    SCState         *_topState;
    QGraphicsScene * _scene;
    //QList<SCState*> _states;
    QList<SCTransition*> _transitions;



//private methods
    SCDataModel(QObject * parent=NULL);
    void connectTransitionsToStatePath();
    void makeTransitionConnections(SCState * targetState, SCTransition* trans);
    //void deleteInTransitions(SCState* state);
};

#endif // SCDATAMODEL_H

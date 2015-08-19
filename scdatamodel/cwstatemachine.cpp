#include "cwstatemachine.h"

CWStateMachine::CWStateMachine(QString theStateName,QString theEntryRelaySlot, QString theExitRelaySlot, QString theEntryRelaySignal,QString theExitRelaySignal,QString comments):
    CWState( theStateName, theEntryRelaySlot,  theExitRelaySlot,  theEntryRelaySignal, theExitRelaySignal, comments)
{

}


CWStateMachine::CWStateMachine(SCState *state):
    CWState()
{

    // a CWStateMachine is a state with children states, so it has all the things a normal state would, but we also want to keep track of each of its states/statemachines
    _myState = state;


    QString stateName;          // _stateName                       state's name in camel case and preceeding "_"
    stateName =         "_" + toCamel(state->getName())+"_"+state->getUidFirstName();

    this->setParallel(state->isParallel());

    if(state->isParallel())
    {
        //QString finishedRelaySlot;  // Slot_StateFinished_stateName     A parallel StateMachine's private slot that is called when finished is emitted from all of its children states
        //QString finishedRelaySignal;// Signal_StateFinished_stateName   A parallel StateMachine's public signal that is emitted when finished is emitted from all of its children states

//        _finishedRelaySlot = "Slot_StateFinished"+stateName+"()";
//        _finishedRelaySignal = "Signal_StateFinished"+stateName+"()";
//        _parallel = true;
    }
    else
    {
        _parallel =false;
    }

    // set the function names for this State Machine
    _stateName = stateName;
    _readyRelaySignal = "Signal_StateReady"+stateName+"()";

    // set the comments variable
    _comments = state->attributes.value("comments")->asString();

    // find all transitions that belong to the state and have true for connectedToFinish
    QList<SCTransition*> transitions;
    state->getTransitions(transitions);

    for(int i = 0 ; i < transitions.size(); i++)
    {
        SCTransition* trans = transitions.at(i);
        if(trans->getAttributeValue("connectToFinished")=="true")
        {
            _finishedTransitions.append(trans);
        }
    }
}
CWStateMachine::~CWStateMachine()
{

}

bool CWStateMachine::isParallel()
{
    return _parallel;
}
void CWStateMachine::setParallel(bool parallel)
{
    _parallel = parallel;
}

void CWStateMachine::createSignalsAndSlots()
{
    // link the root machine's state name
    QString stateName;          // _stateName                       state's name in camel case and preceeding "_"
    QString entryRelaySlot;     // Slot_StateEntry_stateName        QState's private corresponding entry slot called when a transition leads to this state
    QString exitRelaySlot;      // Slot_StateExit_stateName         QState's private corresponding exit slot called when a transition exits this state
    QString entryRelaySignal;   // Signal_StateEntry_stateName      QState's public signal connected to its private entered() signal
    QString exitRelaySignal;    // Signal_StateExit_stateName       QState's public signal connected to its private exited() signal
    QString entryAction;        // EntryAction_eventName            signal that is emitted in the entry relay slot
    QString exitAction;         // ExitAction_eventName             signal that is emitted in the exit relay slot

    SCState* state;
    CWState* cwState;


    QList<SCTransition*> transitions;
    SCTransition* trans;
    QString eventName;
    QString relaySignal;
    QString targetName;
    CWTransition* cwTransition;
    QString comments;

    QList<SCState*> directChildren;
    _myState->getStates(directChildren);

    // for each child state, determine its QState name, its relay slot function name, and its relay slot signal name.
    // also for each of the out transitions of each child, determine the transition's event name and relay signal name
    for(int i = 0 ; i < directChildren.size();i++)
    {
        state = directChildren.at(i);
        stateName =         "_" + toCamel(state->getName())+"_"+state->getUidFirstName();

        entryRelaySignal =  "Signal_StateEntry"+stateName+"()";
        exitRelaySignal =   "Signal_StateExit"+stateName+"()";

        entryRelaySlot =    "Slot_StateEntry" + stateName+"()";
        exitRelaySlot =     "Slot_StateExit"+ stateName+"()";

        comments = state->attributes.value("comments")->asString();
        // create this codewriter state and link them using a QHash
        //cwState = new CWState(stateName,entryRelaySlot,exitRelaySlot,entryRelaySignal,exitRelaySignal,entryAction,exitAction);
        cwState = new CWState(stateName,entryRelaySlot,exitRelaySlot,entryRelaySignal,exitRelaySignal,comments);


        // add all entry actions
        entryAction = state->attributes.value("entryAction")->asString();
        if(!entryAction.isEmpty())
        {
            // delete any spaces, this will cause an error
            entryAction.replace(" ","");
            QStringList entries = entryAction.split(",");
            for(int i = 0; i < entries.size(); i++)
            {
                QString entry = "EntryAction_"+toCamel(entries.at(i))+"()";
                cwState->addEntryAction(entry);
            }
        }


        // add all exit actions
        exitAction = state->attributes.value("exitAction")->asString();
        if(!exitAction.isEmpty())
        {
            // delete any spaces, otherwise an assertion error is thrown
            exitAction.replace(" ","");
            QStringList exits = exitAction.split(",");
            for(int i = 0; i < exits.size(); i++)
            {
                QString exit = "ExitAction_" +toCamel(exits.at(i))+"()";
                cwState->addExitAction(exit);
            }
        }


        cwState->setState(state);   // give it a link back to its data model
        _states.append(cwState);


        // for each out transition, determine the transition's event name and relay signal name
        transitions.clear();
        state->getTransitions(transitions);
        for(int k = 0 ; k < transitions.size(); k++)
        {
            trans = transitions.at(k);
            eventName = "Event_"+toCamel(trans->getEventName())+"_"+trans->getUidFirstName()+"()";
            relaySignal = "Relay_"+eventName;
            targetName = "_"+toCamel(trans->targetState()->getName())+"_"+trans->targetState()->getUidFirstName();

            // create a new codewriter transition and link them with the state's Transition QHash
            cwTransition = new CWTransition(trans, eventName, relaySignal, targetName, trans->attributes.value("comments")->asString());
            cwState->insertTransition( cwTransition);
        }

        // determine the inital state
        if(state->isInitial())
        {
            qDebug() << "CWStateMachine setting initial state to: " << state->objectName();
            _initialState = state;
        }
    }
}


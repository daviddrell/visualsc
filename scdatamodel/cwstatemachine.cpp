#include "cwstatemachine.h"

//CWStateMachine::CWStateMachine(QString theStateName,QString theEntryRelaySlot, QString theExitRelaySlot, QString theEntryRelaySignal,QString theExitRelaySignal,QString comments):
//    CWState( theStateName, theEntryRelaySlot,  theExitRelaySlot,  theEntryRelaySignal, theExitRelaySignal, comments)
//{

//}


CWStateMachine::CWStateMachine(SCState *state,  QHash<SCState*, CWState*> &stateHash):
    CWState(),
    _stateHash(stateHash)
{

    _myState = state;
    // a CWStateMachine is a state with children states, so it has all the things a normal state would, but we also want to keep track of each of its states/statemachines

    QString stateName;          // _stateName                       state's name in camel case and preceeding "_"
    stateName =         "_" + toCamel(state->getName());

    this->setParallel(state->isParallel());
    _stateName = stateName;

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

QList<CWState*> CWStateMachine::getStates()
{
    return _states;
}

void CWStateMachine::createTransitions()
{
    for(int i = 0; i <_states.size(); i++)
    {
        createTransition(_states.at(i));
    }
}

void CWStateMachine::createTransition(CWState *state)
{
    // for each out transition, determine the transition's event name and relay signal name
    QList<SCTransition*> transitions;
    state->getState()->getTransitions(transitions);
    for(int k = 0 ; k < transitions.size(); k++)
    {
        SCTransition* trans = transitions.at(k);
        CWState* tar = _stateHash.value(trans->targetState());
        CWTransition *cwTransition = new CWTransition(trans, state->_stateName, tar->_stateName);
        state->insertTransition(cwTransition);
    }
}

/**
 * @brief CWStateMachine::createChildren
 *
 * creates CWState objects for each top level child of this state machine
 *
 */
void CWStateMachine::createChildren()
{
    QString stateName;          // _stateName                       state's name in camel case and preceeding "_"
    SCState* state;
    CWState* cwState;

    QList<SCState*> directChildren;
    _myState->getStates(directChildren);

    for(int i = 0 ; i < directChildren.size(); i++)
    {
        state = directChildren.at(i);
        stateName = "_" + toCamel(state->getName());
        cwState = new CWState(state, stateName);
        _states.append(cwState);
        _stateHash.insert(state, cwState);

        // set this state machine's initial state
        if(state->isInitial())
        {
            _initialState = state;
        }
    }
}

/**
 * @brief CWStateMachine::createSignalsAndSlots
 *
 * creates the signals and slots related to this state machine
 *
 */
void CWStateMachine::createSignalsAndSlots()
{

    CWState* cwState;
//    this->resolveCollisions(_states);
     _readyRelaySignal = "Signal_StateReady"+_stateName+"()";

     // set the comments variable
     _comments = _myState->attributes.value("comments")->asString();

    for(int i = 0 ; i < _states.size(); i++)
    {
        cwState = _states.at(i);
        cwState->createSignalsAndSlots();
    }
}


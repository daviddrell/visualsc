// This .cpp file was generated using VisualSC's Export Code Function

/*
ABOUT
the state machine class is modeled as a QStateMachine and encapsulates a hierarchical state machine by holding QStates for each state
every state has its own pair of private entry and exit slots AND pair of public entry and exit signals that are connected to the QState's
entered() and exited() signals (represents a private handler for each state's entry and exit operations AND public signals for entry and 
exit events visible to outside classes). These slots are empty by default. any text in entryAction or exitAction will have the
corresponding function emit a signal with the value found in these attributes.

 every state will
    connect its private entered()/exited() signals to the public pair of                                        StateEntry_stateName()          StateExit_stateName()
    connect its private entered()/exited() signals to private entry and exit slots                              Slot_StateEntry_stateName()     Slot_StateExit_stateName()
    *for any entryActions and exitActions attributes: have the private entry and exit slots emit a signal       EntryAction_entryActionValue()  ExitAction_exitActionValue()
    addTransition for each outgoing transition using private signals specific to each transition                Relay_Event_eventName()

every transition has its own public slot that emits a private (RELAY)signal named after the slot/transition (this signal is also what is 
registered when using addTransition to trigger transitions between the QStates), and represents an external event triggering a transition between states.
 every transition will
    have its own public slot    Event_eventName()
    emit a private signal       Relay_Event_eventName()

*/
#include "statesequencemachine.h"
#include <QDebug>

StateSequenceMachine::StateSequenceMachine(QObject* parent):
    QObject(parent),
    //////// State Machine: _stateSequenceMachine ////////
    _stateSequenceMachine(new QStateMachine(this)),
    _checkingAllStateSequences(new QState(QState::ParallelStates)),
    _failed(new QFinalState()),
    _success(new QFinalState()),

    //////// State Machine: _checkingAllStateSequences ////////
    _checkingCallSequence(new QState(_checkingAllStateSequences)),
    _checkingConnSequence(new QState(_checkingAllStateSequences)),
    _checkingSessionSeq(new QState(_checkingAllStateSequences)),

    //////// State Machine: _checkingCallSequence ////////
    _waiting_checkingCallSequence(new QState(_checkingCallSequence)),
    _completed_checkingCallSequence(new QFinalState(_checkingCallSequence)),

    //////// State Machine: _checkingConnSequence ////////
    _waiting_checkingConnSequence(new QState(_checkingConnSequence)),
    _completed_checkingConnSequence(new QFinalState(_checkingConnSequence)),

    //////// State Machine: _checkingSessionSeq ////////
    _waiting(new QState(_checkingSessionSeq)),
    _completed(new QFinalState(_checkingSessionSeq))

{
    //////// State Machine: _stateSequenceMachine ////////
    _stateSequenceMachine->addState(_checkingAllStateSequences);
    _stateSequenceMachine->setInitialState(_checkingAllStateSequences);
    _stateSequenceMachine->addState(_failed);
    _stateSequenceMachine->addState(_success);

    //    Add transitions for the QStates using the transitions' private relay signals
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event___checkingAllStatesCompleted_checkingAllStateSequences()), _success);
    _checkingAllStateSequences->addTransition(_checkingAllStateSequences, SIGNAL(finished()), _success);
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event___checkingCallSequenceFailed_checkingAllStateSequences()), _failed);
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event___checkingConnSequenceFailed_checkingAllStateSequences()), _failed);
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event___checkingSessionSeqFailed_checkingAllStateSequences()), _failed);

    //    Propogate the private QState signals to public signals
    connect(_stateSequenceMachine, SIGNAL(started()), this, SIGNAL(Signal_StateReady___stateSequenceMachine()));
    connect(_checkingAllStateSequences, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___checkingAllStateSequences()));
    connect(_checkingAllStateSequences, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___checkingAllStateSequences()));
    connect(_failed, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___failed()));
    connect(_failed, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___failed()));
    connect(_success, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___success()));
    connect(_success, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___success()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_checkingAllStateSequences, SIGNAL(entered()), this, SLOT(Slot_StateEntry___checkingAllStateSequences()));
    connect(_checkingAllStateSequences, SIGNAL(exited()), this, SLOT(Slot_StateExit___checkingAllStateSequences()));
    connect(_failed, SIGNAL(entered()), this, SLOT(Slot_StateEntry___failed()));
    connect(_failed, SIGNAL(exited()), this, SLOT(Slot_StateExit___failed()));
    connect(_success, SIGNAL(entered()), this, SLOT(Slot_StateEntry___success()));
    connect(_success, SIGNAL(exited()), this, SLOT(Slot_StateExit___success()));


    //////// State Machine: _checkingAllStateSequences ////////

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_checkingCallSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___checkingCallSequence()));
    connect(_checkingCallSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___checkingCallSequence()));
    connect(_checkingConnSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___checkingConnSequence()));
    connect(_checkingConnSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___checkingConnSequence()));
    connect(_checkingSessionSeq, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___checkingSessionSeq()));
    connect(_checkingSessionSeq, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___checkingSessionSeq()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_checkingCallSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___checkingCallSequence()));
    connect(_checkingCallSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___checkingCallSequence()));
    connect(_checkingConnSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___checkingConnSequence()));
    connect(_checkingConnSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___checkingConnSequence()));
    connect(_checkingSessionSeq, SIGNAL(entered()), this, SLOT(Slot_StateEntry___checkingSessionSeq()));
    connect(_checkingSessionSeq, SIGNAL(exited()), this, SLOT(Slot_StateExit___checkingSessionSeq()));


    //////// State Machine: _checkingCallSequence ////////
    _checkingCallSequence->setInitialState(_waiting_checkingCallSequence);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waiting_checkingCallSequence->addTransition(this, SIGNAL(Relay_Event___callSeqComplete_waiting_checkingCallSequence()), _completed_checkingCallSequence);

    //    Propogate the private QState signals to public signals
    connect(_waiting_checkingCallSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___waiting_checkingCallSequence()));
    connect(_waiting_checkingCallSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___waiting_checkingCallSequence()));
    connect(_completed_checkingCallSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___completed_checkingCallSequence()));
    connect(_completed_checkingCallSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___completed_checkingCallSequence()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting_checkingCallSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___waiting_checkingCallSequence()));
    connect(_waiting_checkingCallSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___waiting_checkingCallSequence()));
    connect(_completed_checkingCallSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___completed_checkingCallSequence()));
    connect(_completed_checkingCallSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___completed_checkingCallSequence()));


    //////// State Machine: _checkingConnSequence ////////
    _checkingConnSequence->setInitialState(_waiting_checkingConnSequence);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waiting_checkingConnSequence->addTransition(this, SIGNAL(Relay_Event___connSeqComplete_waiting_checkingConnSequence()), _completed_checkingConnSequence);

    //    Propogate the private QState signals to public signals
    connect(_waiting_checkingConnSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___waiting_checkingConnSequence()));
    connect(_waiting_checkingConnSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___waiting_checkingConnSequence()));
    connect(_completed_checkingConnSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___completed_checkingConnSequence()));
    connect(_completed_checkingConnSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___completed_checkingConnSequence()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting_checkingConnSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___waiting_checkingConnSequence()));
    connect(_waiting_checkingConnSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___waiting_checkingConnSequence()));
    connect(_completed_checkingConnSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___completed_checkingConnSequence()));
    connect(_completed_checkingConnSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___completed_checkingConnSequence()));


    //////// State Machine: _checkingSessionSeq ////////
    _checkingSessionSeq->setInitialState(_waiting);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waiting->addTransition(this, SIGNAL(Relay_Event___sessionSeqComplete_waiting()), _completed);

    //    Propogate the private QState signals to public signals
    connect(_waiting, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___waiting()));
    connect(_waiting, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___waiting()));
    connect(_completed, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___completed()));
    connect(_completed, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___completed()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting, SIGNAL(entered()), this, SLOT(Slot_StateEntry___waiting()));
    connect(_waiting, SIGNAL(exited()), this, SLOT(Slot_StateExit___waiting()));
    connect(_completed, SIGNAL(entered()), this, SLOT(Slot_StateEntry___completed()));
    connect(_completed, SIGNAL(exited()), this, SLOT(Slot_StateExit___completed()));


}

StateSequenceMachine::~StateSequenceMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//    Each State Machine Section shows all transitions between its direct children
    //////// State Machine: _stateSequenceMachine ////////
void StateSequenceMachine::Event_startMachine___stateSequenceMachine()
{
    _stateSequenceMachine->start();
}

void StateSequenceMachine::Event___checkingAllStatesCompleted_checkingAllStateSequences()
{
    emit Relay_Event___checkingAllStatesCompleted_checkingAllStateSequences();
}

void StateSequenceMachine::Event___checkingCallSequenceFailed_checkingAllStateSequences()
{
    emit Relay_Event___checkingCallSequenceFailed_checkingAllStateSequences();
}

void StateSequenceMachine::Event___checkingConnSequenceFailed_checkingAllStateSequences()
{
    emit Relay_Event___checkingConnSequenceFailed_checkingAllStateSequences();
}

void StateSequenceMachine::Event___checkingSessionSeqFailed_checkingAllStateSequences()
{
    emit Relay_Event___checkingSessionSeqFailed_checkingAllStateSequences();
}


    //////// State Machine: _checkingAllStateSequences ////////

    //////// State Machine: _checkingCallSequence ////////
void StateSequenceMachine::Event___callSeqComplete_waiting_checkingCallSequence()
{
    emit Relay_Event___callSeqComplete_waiting_checkingCallSequence();
}


    //////// State Machine: _checkingConnSequence ////////
void StateSequenceMachine::Event___connSeqComplete_waiting_checkingConnSequence()
{
    emit Relay_Event___connSeqComplete_waiting_checkingConnSequence();
}


    //////// State Machine: _checkingSessionSeq ////////
void StateSequenceMachine::Event___sessionSeqComplete_waiting()
{
    emit Relay_Event___sessionSeqComplete_waiting();
}


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _stateSequenceMachine ////////
void StateSequenceMachine::Slot_StateEntry___checkingAllStateSequences()
{

}

void StateSequenceMachine::Slot_StateExit___checkingAllStateSequences()
{

}

void StateSequenceMachine::Slot_StateEntry___failed()
{

}

void StateSequenceMachine::Slot_StateExit___failed()
{

}

void StateSequenceMachine::Slot_StateEntry___success()
{

}

void StateSequenceMachine::Slot_StateExit___success()
{

}


    //////// State Machine: _checkingAllStateSequences ////////
void StateSequenceMachine::Slot_StateEntry___checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateExit___checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateEntry___checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateExit___checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateEntry___checkingSessionSeq()
{

}

void StateSequenceMachine::Slot_StateExit___checkingSessionSeq()
{

}


    //////// State Machine: _checkingCallSequence ////////
void StateSequenceMachine::Slot_StateEntry___waiting_checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateExit___waiting_checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateEntry___completed_checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateExit___completed_checkingCallSequence()
{

}


    //////// State Machine: _checkingConnSequence ////////
void StateSequenceMachine::Slot_StateEntry___waiting_checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateExit___waiting_checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateEntry___completed_checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateExit___completed_checkingConnSequence()
{

}


    //////// State Machine: _checkingSessionSeq ////////
void StateSequenceMachine::Slot_StateEntry___waiting()
{

}

void StateSequenceMachine::Slot_StateExit___waiting()
{

}

void StateSequenceMachine::Slot_StateEntry___completed()
{

}

void StateSequenceMachine::Slot_StateExit___completed()
{

}



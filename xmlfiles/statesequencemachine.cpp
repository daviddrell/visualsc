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
    _waiting___checkingCallSequence(new QState(_checkingCallSequence)),
    _completed___checkingCallSequence(new QFinalState(_checkingCallSequence)),

    //////// State Machine: _checkingConnSequence ////////
    _waiting___checkingConnSequence(new QState(_checkingConnSequence)),
    _completed___checkingConnSequence(new QFinalState(_checkingConnSequence)),

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
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event_checkingAllStatesCompleted_914678f0()), _success_914678f0);
    _checkingAllStateSequences->addTransition(_checkingAllStateSequences, SIGNAL(finished()), _success_914678f0);
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event_checkingCallSequenceFailed_51ea86cc()), _failed_51ea86cc);
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event_checkingConnSequenceFailed_51ea86cc()), _failed_51ea86cc);
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event_checkingSessionSeqFailed_51ea86cc()), _failed_51ea86cc);

    //    Propogate the private QState signals to public signals
    connect(_stateSequenceMachine, SIGNAL(started()), this, SIGNAL(Signal_StateReady_stateSequenceMachine()));
    connect(_checkingAllStateSequences, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_checkingAllStateSequences()));
    connect(_checkingAllStateSequences, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_checkingAllStateSequences()));
    connect(_failed, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_failed()));
    connect(_failed, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_failed()));
    connect(_success, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_success()));
    connect(_success, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_success()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_checkingAllStateSequences, SIGNAL(entered()), this, SLOT(Slot_StateEntry_checkingAllStateSequences()));
    connect(_checkingAllStateSequences, SIGNAL(exited()), this, SLOT(Slot_StateExit_checkingAllStateSequences()));
    connect(_failed, SIGNAL(entered()), this, SLOT(Slot_StateEntry_failed()));
    connect(_failed, SIGNAL(exited()), this, SLOT(Slot_StateExit_failed()));
    connect(_success, SIGNAL(entered()), this, SLOT(Slot_StateEntry_success()));
    connect(_success, SIGNAL(exited()), this, SLOT(Slot_StateExit_success()));


    //////// State Machine: _checkingAllStateSequences ////////

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_checkingCallSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_checkingCallSequence()));
    connect(_checkingCallSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_checkingCallSequence()));
    connect(_checkingConnSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_checkingConnSequence()));
    connect(_checkingConnSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_checkingConnSequence()));
    connect(_checkingSessionSeq, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_checkingSessionSeq()));
    connect(_checkingSessionSeq, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_checkingSessionSeq()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_checkingCallSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry_checkingCallSequence()));
    connect(_checkingCallSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit_checkingCallSequence()));
    connect(_checkingConnSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry_checkingConnSequence()));
    connect(_checkingConnSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit_checkingConnSequence()));
    connect(_checkingSessionSeq, SIGNAL(entered()), this, SLOT(Slot_StateEntry_checkingSessionSeq()));
    connect(_checkingSessionSeq, SIGNAL(exited()), this, SLOT(Slot_StateExit_checkingSessionSeq()));


    //////// State Machine: _checkingCallSequence ////////
    _checkingCallSequence->setInitialState(_waiting___checkingCallSequence);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waiting___checkingCallSequence->addTransition(this, SIGNAL(Relay_Event_callSeqComplete_0b1b5af8()), _completed_0b1b5af8);

    //    Propogate the private QState signals to public signals
    connect(_waiting___checkingCallSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_waiting___checkingCallSequence()));
    connect(_waiting___checkingCallSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_waiting___checkingCallSequence()));
    connect(_completed___checkingCallSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_completed___checkingCallSequence()));
    connect(_completed___checkingCallSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_completed___checkingCallSequence()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting___checkingCallSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry_waiting___checkingCallSequence()));
    connect(_waiting___checkingCallSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit_waiting___checkingCallSequence()));
    connect(_completed___checkingCallSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry_completed___checkingCallSequence()));
    connect(_completed___checkingCallSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit_completed___checkingCallSequence()));


    //////// State Machine: _checkingConnSequence ////////
    _checkingConnSequence->setInitialState(_waiting___checkingConnSequence);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waiting___checkingConnSequence->addTransition(this, SIGNAL(Relay_Event_connSeqComplete_57b5d5f6()), _completed_57b5d5f6);

    //    Propogate the private QState signals to public signals
    connect(_waiting___checkingConnSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_waiting___checkingConnSequence()));
    connect(_waiting___checkingConnSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_waiting___checkingConnSequence()));
    connect(_completed___checkingConnSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_completed___checkingConnSequence()));
    connect(_completed___checkingConnSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_completed___checkingConnSequence()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting___checkingConnSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry_waiting___checkingConnSequence()));
    connect(_waiting___checkingConnSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit_waiting___checkingConnSequence()));
    connect(_completed___checkingConnSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry_completed___checkingConnSequence()));
    connect(_completed___checkingConnSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit_completed___checkingConnSequence()));


    //////// State Machine: _checkingSessionSeq ////////
    _checkingSessionSeq->setInitialState(_waiting);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waiting->addTransition(this, SIGNAL(Relay_Event_sessionSeqComplete_3967534e()), _completed_3967534e);

    //    Propogate the private QState signals to public signals
    connect(_waiting, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_waiting()));
    connect(_waiting, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_waiting()));
    connect(_completed, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_completed()));
    connect(_completed, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_completed()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting, SIGNAL(entered()), this, SLOT(Slot_StateEntry_waiting()));
    connect(_waiting, SIGNAL(exited()), this, SLOT(Slot_StateExit_waiting()));
    connect(_completed, SIGNAL(entered()), this, SLOT(Slot_StateEntry_completed()));
    connect(_completed, SIGNAL(exited()), this, SLOT(Slot_StateExit_completed()));


}

StateSequenceMachine::~StateSequenceMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//    Each State Machine Section shows all transitions between its direct children
    //////// State Machine: _stateSequenceMachine ////////
void StateSequenceMachine::Event_startMachine_stateSequenceMachine()
{
    _stateSequenceMachine->start();
}

void StateSequenceMachine::Event_checkingAllStatesCompleted_914678f0()
{
    emit Relay_Event_checkingAllStatesCompleted_914678f0();
}

void StateSequenceMachine::Event_checkingCallSequenceFailed_51ea86cc()
{
    emit Relay_Event_checkingCallSequenceFailed_51ea86cc();
}

void StateSequenceMachine::Event_checkingConnSequenceFailed_51ea86cc()
{
    emit Relay_Event_checkingConnSequenceFailed_51ea86cc();
}

void StateSequenceMachine::Event_checkingSessionSeqFailed_51ea86cc()
{
    emit Relay_Event_checkingSessionSeqFailed_51ea86cc();
}


    //////// State Machine: _checkingAllStateSequences ////////

    //////// State Machine: _checkingCallSequence ////////
void StateSequenceMachine::Event_callSeqComplete_0b1b5af8()
{
    emit Relay_Event_callSeqComplete_0b1b5af8();
}


    //////// State Machine: _checkingConnSequence ////////
void StateSequenceMachine::Event_connSeqComplete_57b5d5f6()
{
    emit Relay_Event_connSeqComplete_57b5d5f6();
}


    //////// State Machine: _checkingSessionSeq ////////
void StateSequenceMachine::Event_sessionSeqComplete_3967534e()
{
    emit Relay_Event_sessionSeqComplete_3967534e();
}


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _stateSequenceMachine ////////
void StateSequenceMachine::Slot_StateEntry_checkingAllStateSequences()
{

}

void StateSequenceMachine::Slot_StateExit_checkingAllStateSequences()
{

}

void StateSequenceMachine::Slot_StateEntry_failed()
{

}

void StateSequenceMachine::Slot_StateExit_failed()
{

}

void StateSequenceMachine::Slot_StateEntry_success()
{

}

void StateSequenceMachine::Slot_StateExit_success()
{

}


    //////// State Machine: _checkingAllStateSequences ////////
void StateSequenceMachine::Slot_StateEntry_checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateExit_checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateEntry_checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateExit_checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateEntry_checkingSessionSeq()
{

}

void StateSequenceMachine::Slot_StateExit_checkingSessionSeq()
{

}


    //////// State Machine: _checkingCallSequence ////////
void StateSequenceMachine::Slot_StateEntry_waiting___checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateExit_waiting___checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateEntry_completed___checkingCallSequence()
{

}

void StateSequenceMachine::Slot_StateExit_completed___checkingCallSequence()
{

}


    //////// State Machine: _checkingConnSequence ////////
void StateSequenceMachine::Slot_StateEntry_waiting___checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateExit_waiting___checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateEntry_completed___checkingConnSequence()
{

}

void StateSequenceMachine::Slot_StateExit_completed___checkingConnSequence()
{

}


    //////// State Machine: _checkingSessionSeq ////////
void StateSequenceMachine::Slot_StateEntry_waiting()
{

}

void StateSequenceMachine::Slot_StateExit_waiting()
{

}

void StateSequenceMachine::Slot_StateEntry_completed()
{

}

void StateSequenceMachine::Slot_StateExit_completed()
{

}



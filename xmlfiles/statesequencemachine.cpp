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
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////
    _checkingAllStateSequences_d74d8a42(new QStateMachine(this)),
    _checkingCallSequence_78faf58e(new QState()),
    _checkingConnSequence_926f34d4(new QState()),
    _checkingSessionSeq_9f53745d(new QState()),

    //////// State Machine: _checkingCallSequence_78faf58e ////////
    _checkingCallSequence_78faf58e(new QStateMachine(this)),
    _waiting_246b7dcb(new QState()),
    _completed_0b1b5af8(new QFinalState()),

    //////// State Machine: _checkingConnSequence_926f34d4 ////////
    _checkingConnSequence_926f34d4(new QStateMachine(this)),
    _waiting_c3267d7e(new QState()),
    _completed_57b5d5f6(new QFinalState()),

    //////// State Machine: _checkingSessionSeq_9f53745d ////////
    _checkingSessionSeq_9f53745d(new QStateMachine(this)),
    _waiting_ff5a583f(new QState()),
    _completed_3967534e(new QFinalState()),

    //////// State Machine: _stateSequenceMachine_d47570ed ////////
    _stateSequenceMachine_d47570ed(new QStateMachine(this)),
    _checkingAllStateSequences_d74d8a42(new QState()),
    _failed_51ea86cc(new QFinalState()),
    _success_914678f0(new QFinalState())

{
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////
    _checkingAllStateSequences_d74d8a42->addState(_checkingCallSequence_78faf58e);
    _checkingAllStateSequences_d74d8a42->addState(_checkingConnSequence_926f34d4);
    _checkingAllStateSequences_d74d8a42->addState(_checkingSessionSeq_9f53745d);

    //
    //    add transitions for the QStates using the transitions' private relay signals
    //

    //    Propogate the private QState signals to public signals
    connect(_checkingAllStateSequences_d74d8a42, SIGNAL(started()), this, SIGNAL(Signal_StateReady_checkingAllStateSequences_d74d8a42()));
    connect(_checkingCallSequence_78faf58e, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_checkingCallSequence_78faf58e()));
    connect(_checkingCallSequence_78faf58e, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_checkingCallSequence_78faf58e()));
    connect(_checkingConnSequence_926f34d4, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_checkingConnSequence_926f34d4()));
    connect(_checkingConnSequence_926f34d4, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_checkingConnSequence_926f34d4()));
    connect(_checkingSessionSeq_9f53745d, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_checkingSessionSeq_9f53745d()));
    connect(_checkingSessionSeq_9f53745d, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_checkingSessionSeq_9f53745d()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_checkingCallSequence_78faf58e, SIGNAL(entered()), this, SLOT(Slot_StateEntry_checkingCallSequence_78faf58e()));
    connect(_checkingCallSequence_78faf58e, SIGNAL(exited()), this, SLOT(Slot_StateExit_checkingCallSequence_78faf58e()));
    connect(_checkingConnSequence_926f34d4, SIGNAL(entered()), this, SLOT(Slot_StateEntry_checkingConnSequence_926f34d4()));
    connect(_checkingConnSequence_926f34d4, SIGNAL(exited()), this, SLOT(Slot_StateExit_checkingConnSequence_926f34d4()));
    connect(_checkingSessionSeq_9f53745d, SIGNAL(entered()), this, SLOT(Slot_StateEntry_checkingSessionSeq_9f53745d()));
    connect(_checkingSessionSeq_9f53745d, SIGNAL(exited()), this, SLOT(Slot_StateExit_checkingSessionSeq_9f53745d()));


    //////// State Machine: _checkingCallSequence_78faf58e ////////
    _checkingCallSequence_78faf58e->addState(_waiting_246b7dcb);
    _checkingCallSequence_78faf58e->setInitialState(_waiting_246b7dcb);
    _checkingCallSequence_78faf58e->addState(_completed_0b1b5af8);

    //
    //    add transitions for the QStates using the transitions' private relay signals
    //
    _waiting_246b7dcb->addTransition(this, SIGNAL(Relay_Event_callSeqComplete_0b1b5af8()),_completed_0b1b5af8);

    //    Propogate the private QState signals to public signals
    connect(_checkingCallSequence_78faf58e, SIGNAL(started()), this, SIGNAL(Signal_StateReady_checkingCallSequence_78faf58e()));
    connect(_waiting_246b7dcb, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_waiting_246b7dcb()));
    connect(_waiting_246b7dcb, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_waiting_246b7dcb()));
    connect(_completed_0b1b5af8, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_completed_0b1b5af8()));
    connect(_completed_0b1b5af8, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_completed_0b1b5af8()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting_246b7dcb, SIGNAL(entered()), this, SLOT(Slot_StateEntry_waiting_246b7dcb()));
    connect(_waiting_246b7dcb, SIGNAL(exited()), this, SLOT(Slot_StateExit_waiting_246b7dcb()));
    connect(_completed_0b1b5af8, SIGNAL(entered()), this, SLOT(Slot_StateEntry_completed_0b1b5af8()));
    connect(_completed_0b1b5af8, SIGNAL(exited()), this, SLOT(Slot_StateExit_completed_0b1b5af8()));


    //////// State Machine: _checkingConnSequence_926f34d4 ////////
    _checkingConnSequence_926f34d4->addState(_waiting_c3267d7e);
    _checkingConnSequence_926f34d4->setInitialState(_waiting_c3267d7e);
    _checkingConnSequence_926f34d4->addState(_completed_57b5d5f6);

    //
    //    add transitions for the QStates using the transitions' private relay signals
    //
    _waiting_c3267d7e->addTransition(this, SIGNAL(Relay_Event_connSeqComplete_57b5d5f6()),_completed_57b5d5f6);

    //    Propogate the private QState signals to public signals
    connect(_checkingConnSequence_926f34d4, SIGNAL(started()), this, SIGNAL(Signal_StateReady_checkingConnSequence_926f34d4()));
    connect(_waiting_c3267d7e, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_waiting_c3267d7e()));
    connect(_waiting_c3267d7e, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_waiting_c3267d7e()));
    connect(_completed_57b5d5f6, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_completed_57b5d5f6()));
    connect(_completed_57b5d5f6, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_completed_57b5d5f6()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting_c3267d7e, SIGNAL(entered()), this, SLOT(Slot_StateEntry_waiting_c3267d7e()));
    connect(_waiting_c3267d7e, SIGNAL(exited()), this, SLOT(Slot_StateExit_waiting_c3267d7e()));
    connect(_completed_57b5d5f6, SIGNAL(entered()), this, SLOT(Slot_StateEntry_completed_57b5d5f6()));
    connect(_completed_57b5d5f6, SIGNAL(exited()), this, SLOT(Slot_StateExit_completed_57b5d5f6()));


    //////// State Machine: _checkingSessionSeq_9f53745d ////////
    _checkingSessionSeq_9f53745d->addState(_waiting_ff5a583f);
    _checkingSessionSeq_9f53745d->setInitialState(_waiting_ff5a583f);
    _checkingSessionSeq_9f53745d->addState(_completed_3967534e);

    //
    //    add transitions for the QStates using the transitions' private relay signals
    //
    _waiting_ff5a583f->addTransition(this, SIGNAL(Relay_Event_sessionSeqComplete_3967534e()),_completed_3967534e);

    //    Propogate the private QState signals to public signals
    connect(_checkingSessionSeq_9f53745d, SIGNAL(started()), this, SIGNAL(Signal_StateReady_checkingSessionSeq_9f53745d()));
    connect(_waiting_ff5a583f, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_waiting_ff5a583f()));
    connect(_waiting_ff5a583f, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_waiting_ff5a583f()));
    connect(_completed_3967534e, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_completed_3967534e()));
    connect(_completed_3967534e, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_completed_3967534e()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting_ff5a583f, SIGNAL(entered()), this, SLOT(Slot_StateEntry_waiting_ff5a583f()));
    connect(_waiting_ff5a583f, SIGNAL(exited()), this, SLOT(Slot_StateExit_waiting_ff5a583f()));
    connect(_completed_3967534e, SIGNAL(entered()), this, SLOT(Slot_StateEntry_completed_3967534e()));
    connect(_completed_3967534e, SIGNAL(exited()), this, SLOT(Slot_StateExit_completed_3967534e()));


    //////// State Machine: _stateSequenceMachine_d47570ed ////////
    _stateSequenceMachine_d47570ed->addState(_checkingAllStateSequences_d74d8a42);
    _stateSequenceMachine_d47570ed->setInitialState(_checkingAllStateSequences_d74d8a42);
    _stateSequenceMachine_d47570ed->addState(_failed_51ea86cc);
    _stateSequenceMachine_d47570ed->addState(_success_914678f0);

    //
    //    add transitions for the QStates using the transitions' private relay signals
    //
    _checkingAllStateSequences_d74d8a42->addTransition(this, SIGNAL(Relay_Event_checkingAllStatesCompleted_914678f0()),_success_914678f0);
    _checkingAllStateSequences_d74d8a42->addTransition(this, SIGNAL(Relay_Event_checkingCallSequenceFailed_51ea86cc()),_failed_51ea86cc);
    _checkingAllStateSequences_d74d8a42->addTransition(this, SIGNAL(Relay_Event_checkingConnSequenceFailed_51ea86cc()),_failed_51ea86cc);
    _checkingAllStateSequences_d74d8a42->addTransition(this, SIGNAL(Relay_Event_checkingSessionSeqFailed_51ea86cc()),_failed_51ea86cc);

    //    Propogate the private QState signals to public signals
    connect(_stateSequenceMachine_d47570ed, SIGNAL(started()), this, SIGNAL(Signal_StateReady_stateSequenceMachine_d47570ed()));
    connect(_checkingAllStateSequences_d74d8a42, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_checkingAllStateSequences_d74d8a42()));
    connect(_checkingAllStateSequences_d74d8a42, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_checkingAllStateSequences_d74d8a42()));
    connect(_failed_51ea86cc, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_failed_51ea86cc()));
    connect(_failed_51ea86cc, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_failed_51ea86cc()));
    connect(_success_914678f0, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_success_914678f0()));
    connect(_success_914678f0, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_success_914678f0()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_checkingAllStateSequences_d74d8a42, SIGNAL(entered()), this, SLOT(Slot_StateEntry_checkingAllStateSequences_d74d8a42()));
    connect(_checkingAllStateSequences_d74d8a42, SIGNAL(exited()), this, SLOT(Slot_StateExit_checkingAllStateSequences_d74d8a42()));
    connect(_failed_51ea86cc, SIGNAL(entered()), this, SLOT(Slot_StateEntry_failed_51ea86cc()));
    connect(_failed_51ea86cc, SIGNAL(exited()), this, SLOT(Slot_StateExit_failed_51ea86cc()));
    connect(_success_914678f0, SIGNAL(entered()), this, SLOT(Slot_StateEntry_success_914678f0()));
    connect(_success_914678f0, SIGNAL(exited()), this, SLOT(Slot_StateExit_success_914678f0()));


}

StateSequenceMachine::~StateSequenceMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////
void StateSequenceMachine::Event_startMachine_checkingAllStateSequences_d74d8a42()
{
    _checkingAllStateSequences_d74d8a42->start();
}


    //////// State Machine: _checkingCallSequence_78faf58e ////////
void StateSequenceMachine::Event_startMachine_checkingCallSequence_78faf58e()
{
    _checkingCallSequence_78faf58e->start();
}

void StateSequenceMachine::Event_callSeqComplete_0b1b5af8()
{
    emit Relay_Event_callSeqComplete_0b1b5af8();
}


    //////// State Machine: _checkingConnSequence_926f34d4 ////////
void StateSequenceMachine::Event_startMachine_checkingConnSequence_926f34d4()
{
    _checkingConnSequence_926f34d4->start();
}

void StateSequenceMachine::Event_connSeqComplete_57b5d5f6()
{
    emit Relay_Event_connSeqComplete_57b5d5f6();
}


    //////// State Machine: _checkingSessionSeq_9f53745d ////////
void StateSequenceMachine::Event_startMachine_checkingSessionSeq_9f53745d()
{
    _checkingSessionSeq_9f53745d->start();
}

void StateSequenceMachine::Event_sessionSeqComplete_3967534e()
{
    emit Relay_Event_sessionSeqComplete_3967534e();
}


    //////// State Machine: _stateSequenceMachine_d47570ed ////////
void StateSequenceMachine::Event_startMachine_stateSequenceMachine_d47570ed()
{
    _stateSequenceMachine_d47570ed->start();
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


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////
void StateSequenceMachine::Slot_StateEntry_checkingCallSequence_78faf58e()
{

}

void StateSequenceMachine::Slot_StateExit_checkingCallSequence_78faf58e()
{

}

void StateSequenceMachine::Slot_StateEntry_checkingConnSequence_926f34d4()
{

}

void StateSequenceMachine::Slot_StateExit_checkingConnSequence_926f34d4()
{

}

void StateSequenceMachine::Slot_StateEntry_checkingSessionSeq_9f53745d()
{

}

void StateSequenceMachine::Slot_StateExit_checkingSessionSeq_9f53745d()
{

}


    //////// State Machine: _checkingCallSequence_78faf58e ////////
void StateSequenceMachine::Slot_StateEntry_waiting_246b7dcb()
{

}

void StateSequenceMachine::Slot_StateExit_waiting_246b7dcb()
{

}

void StateSequenceMachine::Slot_StateEntry_completed_0b1b5af8()
{

}

void StateSequenceMachine::Slot_StateExit_completed_0b1b5af8()
{

}


    //////// State Machine: _checkingConnSequence_926f34d4 ////////
void StateSequenceMachine::Slot_StateEntry_waiting_c3267d7e()
{

}

void StateSequenceMachine::Slot_StateExit_waiting_c3267d7e()
{

}

void StateSequenceMachine::Slot_StateEntry_completed_57b5d5f6()
{

}

void StateSequenceMachine::Slot_StateExit_completed_57b5d5f6()
{

}


    //////// State Machine: _checkingSessionSeq_9f53745d ////////
void StateSequenceMachine::Slot_StateEntry_waiting_ff5a583f()
{

}

void StateSequenceMachine::Slot_StateExit_waiting_ff5a583f()
{

}

void StateSequenceMachine::Slot_StateEntry_completed_3967534e()
{

}

void StateSequenceMachine::Slot_StateExit_completed_3967534e()
{

}


    //////// State Machine: _stateSequenceMachine_d47570ed ////////
void StateSequenceMachine::Slot_StateEntry_checkingAllStateSequences_d74d8a42()
{

}

void StateSequenceMachine::Slot_StateExit_checkingAllStateSequences_d74d8a42()
{

}

void StateSequenceMachine::Slot_StateEntry_failed_51ea86cc()
{

}

void StateSequenceMachine::Slot_StateExit_failed_51ea86cc()
{

}

void StateSequenceMachine::Slot_StateEntry_success_914678f0()
{

}

void StateSequenceMachine::Slot_StateExit_success_914678f0()
{

}



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
#include "statemachine.h"
#include <QDebug>

StateMachine::StateMachine(QObject* parent):
    QObject(parent),
    //////// State Machine: _stateMachine ////////
    _stateMachine(new QStateMachine(this)),
    _waitingForSystemReady(new QState()),
    _running(new QState()),
    _success(new QFinalState()),
    _failed(new QFinalState()),

    //////// State Machine: _running ////////
    _connecting(new QState(_running)),
    _connected(new QState(_running))

{
    //////// State Machine: _stateMachine ////////
    _stateMachine->addState(_waitingForSystemReady);
    _stateMachine->setInitialState(_waitingForSystemReady);
    _stateMachine->addState(_running);
    _stateMachine->addState(_success);
    _stateMachine->addState(_failed);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waitingForSystemReady->addTransition(this, SIGNAL(Relay_Event___systemReady_waitingForSystemReady()), _running);
    _running->addTransition(this, SIGNAL(Relay_Event___loopCountExceeded_running()), _success);
    _running->addTransition(this, SIGNAL(Relay_Event___equipmentFailed_running()), _failed);

    //    Propogate the private QState signals to public signals
    connect(_stateMachine, SIGNAL(started()), this, SIGNAL(Signal_StateReady___stateMachine()));
    connect(_waitingForSystemReady, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___waitingForSystemReady()));
    connect(_waitingForSystemReady, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___waitingForSystemReady()));
    connect(_running, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___running()));
    connect(_running, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___running()));
    connect(_success, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___success()));
    connect(_success, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___success()));
    connect(_failed, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___failed()));
    connect(_failed, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___failed()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waitingForSystemReady, SIGNAL(entered()), this, SLOT(Slot_StateEntry___waitingForSystemReady()));
    connect(_waitingForSystemReady, SIGNAL(exited()), this, SLOT(Slot_StateExit___waitingForSystemReady()));
    connect(_running, SIGNAL(entered()), this, SLOT(Slot_StateEntry___running()));
    connect(_running, SIGNAL(exited()), this, SLOT(Slot_StateExit___running()));
    connect(_success, SIGNAL(entered()), this, SLOT(Slot_StateEntry___success()));
    connect(_success, SIGNAL(exited()), this, SLOT(Slot_StateExit___success()));
    connect(_failed, SIGNAL(entered()), this, SLOT(Slot_StateEntry___failed()));
    connect(_failed, SIGNAL(exited()), this, SLOT(Slot_StateExit___failed()));


    //////// State Machine: _running ////////
    _running->setInitialState(_connecting);

    //    Add transitions for the QStates using the transitions' private relay signals
    _connecting->addTransition(this, SIGNAL(Relay_Event___connected_connecting()), _connected);
    _connecting->addTransition(this, SIGNAL(Relay_Event___dialFailed_connecting()), _failed);
    _connected->addTransition(this, SIGNAL(Relay_Event___disconnected_connected()), _connecting);
    _connected->addTransition(this, SIGNAL(Relay_Event___disconnectFailed_connected()), _failed);

    //    Propogate the private QState signals to public signals
    connect(_connecting, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___connecting()));
    connect(_connecting, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___connecting()));
    connect(_connected, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___connected()));
    connect(_connected, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___connected()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_connecting, SIGNAL(entered()), this, SLOT(Slot_StateEntry___connecting()));
    connect(_connecting, SIGNAL(exited()), this, SLOT(Slot_StateExit___connecting()));
    connect(_connected, SIGNAL(entered()), this, SLOT(Slot_StateEntry___connected()));
    connect(_connected, SIGNAL(exited()), this, SLOT(Slot_StateExit___connected()));


}

StateMachine::~StateMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//    Each State Machine Section shows all transitions between its direct children
    //////// State Machine: _stateMachine ////////
void StateMachine::Event_startMachine___stateMachine()
{
    _stateMachine->start();
}

void StateMachine::Event___systemReady_waitingForSystemReady()
{
    emit Relay_Event___systemReady_waitingForSystemReady();
}

void StateMachine::Event___loopCountExceeded_running()
{
    emit Relay_Event___loopCountExceeded_running();
}

void StateMachine::Event___equipmentFailed_running()
{
    emit Relay_Event___equipmentFailed_running();
}


    //////// State Machine: _running ////////
void StateMachine::Event___connected_connecting()
{
    emit Relay_Event___connected_connecting();
}

void StateMachine::Event___dialFailed_connecting()
{
    emit Relay_Event___dialFailed_connecting();
}

void StateMachine::Event___disconnected_connected()
{
    emit Relay_Event___disconnected_connected();
}

void StateMachine::Event___disconnectFailed_connected()
{
    emit Relay_Event___disconnectFailed_connected();
}


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _stateMachine ////////
void StateMachine::Slot_StateEntry___waitingForSystemReady()
{

}

void StateMachine::Slot_StateExit___waitingForSystemReady()
{

}

void StateMachine::Slot_StateEntry___running()
{

}

void StateMachine::Slot_StateExit___running()
{

}

void StateMachine::Slot_StateEntry___success()
{

}

void StateMachine::Slot_StateExit___success()
{

}

void StateMachine::Slot_StateEntry___failed()
{

}

void StateMachine::Slot_StateExit___failed()
{

}


    //////// State Machine: _running ////////
void StateMachine::Slot_StateEntry___connecting()
{
    emit EntryAction___dial();
}

void StateMachine::Slot_StateExit___connecting()
{
    emit ExitAction___incrementloopcounter();
}

void StateMachine::Slot_StateEntry___connected()
{
    emit EntryAction___hangup();
}

void StateMachine::Slot_StateExit___connected()
{

}



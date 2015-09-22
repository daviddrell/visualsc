// This .cpp file was generated using VisualSC's Export Code Function

/*
ABOUT
the state machine class is modeled as a QStateMachine and encapsulates a hierarchical state machine by holding QStates for each state
every state has its own pair of private entry and exit slots AND pair of public entry and exit signals that are connected to the QState's
entered() and exited() signals (represents a private handler for each state's entry and exit operations AND public signals for entry and 
exit events visible to outside classes). These slots are empty by default. any text in entryAction or exitAction will have the
corresponding function emit a signal with the value found in these attributes.

 every state will
    connect its private entered()/exited() signals to the public pair of                                        Signal_StateEntry___stateName()     Signal_StateExit___stateName()
    connect its private entered()/exited() signals to private entry and exit slots                              Slot_StateEntry___stateName()       Slot_StateExit___stateName()
    *for any entryActions and exitActions attributes: have the private entry and exit slots emit a signal       Action_entryAction()
    addTransition for each outgoing transition using private signals specific to each transition                Relay_Event___eventName()

every transition has its own public slot that emits a private (RELAY) signal named after the slot/transition (this signal is also what is 
registered when using addTransition to trigger transitions between the QStates), and represents an external event triggering a transition between states.
 every transition will
    have a public slot    Event_eventName()
    that will emit private signal(s) for all events sharing the same event name      Relay_Event___eventName()

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
    _looping(new QState(_running)),

    //////// State Machine: _looping ////////
    _connected(new QState(_looping)),
    _connecting(new QState(_looping)),
    _disconnected(new QState(_looping)),
    _registering(new QState(_looping)),
    _unregistered(new QState(_looping))

{
    //////// State Machine: _stateMachine ////////
    _stateMachine->addState(_waitingForSystemReady);
    _stateMachine->setInitialState(_waitingForSystemReady);
    _stateMachine->addState(_running);
    _stateMachine->addState(_success);
    _stateMachine->addState(_failed);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waitingForSystemReady->addTransition(this, SIGNAL(Relay_Event___systemReady_waitingForSystemReady()), _running);
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
    _running->setInitialState(_looping);

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_looping, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___looping()));
    connect(_looping, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___looping()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_looping, SIGNAL(entered()), this, SLOT(Slot_StateEntry___looping()));
    connect(_looping, SIGNAL(exited()), this, SLOT(Slot_StateExit___looping()));


    //////// State Machine: _looping ////////
    _looping->setInitialState(_registering);

    //    Add transitions for the QStates using the transitions' private relay signals
    _connected->addTransition(this, SIGNAL(Relay_Event___disconnected_connected()), _disconnected);
    _connected->addTransition(this, SIGNAL(Relay_Event___disconnectFailed_connected()), _failed);
    _connecting->addTransition(this, SIGNAL(Relay_Event___connected_connecting()), _connected);
    _connecting->addTransition(this, SIGNAL(Relay_Event___dialFailed_connecting()), _failed);
    _disconnected->addTransition(this, SIGNAL(Relay_Event___unregistered_disconnected()), _unregistered);
    _registering->addTransition(this, SIGNAL(Relay_Event___registered_registering()), _connecting);
    _unregistered->addTransition(this, SIGNAL(Relay_Event___loopCountExceeded_unregistered()), _success);
    _unregistered->addTransition(this, SIGNAL(Relay_Event___loopCountNotExceeded_unregistered()), _registering);

    //    Propogate the private QState signals to public signals
    connect(_connected, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___connected()));
    connect(_connected, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___connected()));
    connect(_connecting, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___connecting()));
    connect(_connecting, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___connecting()));
    connect(_disconnected, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___disconnected()));
    connect(_disconnected, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___disconnected()));
    connect(_registering, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___registering()));
    connect(_registering, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___registering()));
    connect(_unregistered, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___unregistered()));
    connect(_unregistered, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___unregistered()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_connected, SIGNAL(entered()), this, SLOT(Slot_StateEntry___connected()));
    connect(_connected, SIGNAL(exited()), this, SLOT(Slot_StateExit___connected()));
    connect(_connecting, SIGNAL(entered()), this, SLOT(Slot_StateEntry___connecting()));
    connect(_connecting, SIGNAL(exited()), this, SLOT(Slot_StateExit___connecting()));
    connect(_disconnected, SIGNAL(entered()), this, SLOT(Slot_StateEntry___disconnected()));
    connect(_disconnected, SIGNAL(exited()), this, SLOT(Slot_StateExit___disconnected()));
    connect(_registering, SIGNAL(entered()), this, SLOT(Slot_StateEntry___registering()));
    connect(_registering, SIGNAL(exited()), this, SLOT(Slot_StateExit___registering()));
    connect(_unregistered, SIGNAL(entered()), this, SLOT(Slot_StateEntry___unregistered()));
    connect(_unregistered, SIGNAL(exited()), this, SLOT(Slot_StateExit___unregistered()));


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

void StateMachine::Event___systemReady()
{
    emit Relay_Event___systemReady_waitingForSystemReady();
}

void StateMachine::Event___equipmentFailed()
{
    emit Relay_Event___equipmentFailed_running();
}


    //////// State Machine: _running ////////

    //////// State Machine: _looping ////////
void StateMachine::Event___disconnected()
{
    emit Relay_Event___disconnected_connected();
}

void StateMachine::Event___disconnectFailed()
{
    emit Relay_Event___disconnectFailed_connected();
}

void StateMachine::Event___connected()
{
    emit Relay_Event___connected_connecting();
}

void StateMachine::Event___dialFailed()
{
    emit Relay_Event___dialFailed_connecting();
}

void StateMachine::Event___unregistered()
{
    emit Relay_Event___unregistered_disconnected();
}

void StateMachine::Event___registered()
{
    emit Relay_Event___registered_registering();
}

void StateMachine::Event___loopCountExceeded()
{
    emit Relay_Event___loopCountExceeded_unregistered();
}

void StateMachine::Event___loopCountNotExceeded()
{
    emit Relay_Event___loopCountNotExceeded_unregistered();
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
void StateMachine::Slot_StateEntry___looping()
{

}

void StateMachine::Slot_StateExit___looping()
{

}


    //////// State Machine: _looping ////////
void StateMachine::Slot_StateEntry___connected()
{
    emit Action___hangup();
}

void StateMachine::Slot_StateExit___connected()
{

}

void StateMachine::Slot_StateEntry___connecting()
{
    emit Action___dial();
}

void StateMachine::Slot_StateExit___connecting()
{

}

void StateMachine::Slot_StateEntry___disconnected()
{
    emit Action___deregister();
}

void StateMachine::Slot_StateExit___disconnected()
{

}

void StateMachine::Slot_StateEntry___registering()
{
    emit Action___register();
}

void StateMachine::Slot_StateExit___registering()
{

}

void StateMachine::Slot_StateEntry___unregistered()
{
    emit Action___incrementloopcount();
}

void StateMachine::Slot_StateExit___unregistered()
{

}



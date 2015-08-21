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
    _a(new QState()),
    _b(new QState()),

    //////// State Machine: _a ////////
    _x_a(new QState(_a)),
    _y(new QFinalState(_a)),

    //////// State Machine: _b ////////
    _x_b(new QState(_b))

{
    //////// State Machine: _stateMachine ////////
    _stateMachine->addState(_a);
    _stateMachine->setInitialState(_a);
    _stateMachine->addState(_b);

    //    Add transitions for the QStates using the transitions' private relay signals
    _a->addTransition(this, SIGNAL(Relay_Event___event_a()), _b);
    _b->addTransition(this, SIGNAL(Relay_Event___sr_b()), _a);

    //    Propogate the private QState signals to public signals
    connect(_stateMachine, SIGNAL(started()), this, SIGNAL(Signal_StateReady___stateMachine()));
    connect(_a, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___a()));
    connect(_a, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___a()));
    connect(_b, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___b()));
    connect(_b, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___b()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_a, SIGNAL(entered()), this, SLOT(Slot_StateEntry___a()));
    connect(_a, SIGNAL(exited()), this, SLOT(Slot_StateExit___a()));
    connect(_b, SIGNAL(entered()), this, SLOT(Slot_StateEntry___b()));
    connect(_b, SIGNAL(exited()), this, SLOT(Slot_StateExit___b()));


    //////// State Machine: _a ////////
    _a->setInitialState(_x_a);

    //    Add transitions for the QStates using the transitions' private relay signals
    _x_a->addTransition(this, SIGNAL(Relay_Event___xToY_x_a()), _y);

    //    Propogate the private QState signals to public signals
    connect(_x_a, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___x_a()));
    connect(_x_a, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___x_a()));
    connect(_y, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___y()));
    connect(_y, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___y()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_x_a, SIGNAL(entered()), this, SLOT(Slot_StateEntry___x_a()));
    connect(_x_a, SIGNAL(exited()), this, SLOT(Slot_StateExit___x_a()));
    connect(_y, SIGNAL(entered()), this, SLOT(Slot_StateEntry___y()));
    connect(_y, SIGNAL(exited()), this, SLOT(Slot_StateExit___y()));


    //////// State Machine: _b ////////

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_x_b, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___x_b()));
    connect(_x_b, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___x_b()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_x_b, SIGNAL(entered()), this, SLOT(Slot_StateEntry___x_b()));
    connect(_x_b, SIGNAL(exited()), this, SLOT(Slot_StateExit___x_b()));


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

void StateMachine::Event___event_a()
{
    emit Relay_Event___event_a();
}

void StateMachine::Event___sr_b()
{
    emit Relay_Event___sr_b();
}


    //////// State Machine: _a ////////
void StateMachine::Event___xToY_x_a()
{
    emit Relay_Event___xToY_x_a();
}


    //////// State Machine: _b ////////

//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _stateMachine ////////
void StateMachine::Slot_StateEntry___a()
{

}

void StateMachine::Slot_StateExit___a()
{

}

void StateMachine::Slot_StateEntry___b()
{
    emit EntryAction___jump();
    emit EntryAction___sleep();
    emit EntryAction___dance();
}

void StateMachine::Slot_StateExit___b()
{
    emit ExitAction___fall();
    emit ExitAction___down();
    emit ExitAction___into();
}


    //////// State Machine: _a ////////
void StateMachine::Slot_StateEntry___x_a()
{

}

void StateMachine::Slot_StateExit___x_a()
{

}

void StateMachine::Slot_StateEntry___y()
{

}

void StateMachine::Slot_StateExit___y()
{

}


    //////// State Machine: _b ////////
void StateMachine::Slot_StateEntry___x_b()
{

}

void StateMachine::Slot_StateExit___x_b()
{

}



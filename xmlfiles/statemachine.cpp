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
    _x___a(new QState(_a)),
    _y(new QFinalState(_a)),

    //////// State Machine: _b ////////
    _x___b(new QState(_b))

{
    //////// State Machine: _stateMachine ////////
    _stateMachine->addState(_a);
    _stateMachine->setInitialState(_a);
    _stateMachine->addState(_b);

    //    Add transitions for the QStates using the transitions' private relay signals
    _a->addTransition(this, SIGNAL(Relay_Event_event___a()), _b);
    _b->addTransition(this, SIGNAL(Relay_Event_sr___b()), _a);

    //    Propogate the private QState signals to public signals
    connect(_stateMachine, SIGNAL(started()), this, SIGNAL(Signal_StateReady_stateMachine()));
    connect(_a, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_a()));
    connect(_a, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_a()));
    connect(_b, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_b()));
    connect(_b, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_b()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_a, SIGNAL(entered()), this, SLOT(Slot_StateEntry_a()));
    connect(_a, SIGNAL(exited()), this, SLOT(Slot_StateExit_a()));
    connect(_b, SIGNAL(entered()), this, SLOT(Slot_StateEntry_b()));
    connect(_b, SIGNAL(exited()), this, SLOT(Slot_StateExit_b()));


    //////// State Machine: _a ////////
    _a->setInitialState(_x___a);

    //    Add transitions for the QStates using the transitions' private relay signals
    _x___a->addTransition(this, SIGNAL(Relay_Event_xToY___x___a()), _y);

    //    Propogate the private QState signals to public signals
    connect(_x___a, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_x___a()));
    connect(_x___a, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_x___a()));
    connect(_y, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_y()));
    connect(_y, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_y()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_x___a, SIGNAL(entered()), this, SLOT(Slot_StateEntry_x___a()));
    connect(_x___a, SIGNAL(exited()), this, SLOT(Slot_StateExit_x___a()));
    connect(_y, SIGNAL(entered()), this, SLOT(Slot_StateEntry_y()));
    connect(_y, SIGNAL(exited()), this, SLOT(Slot_StateExit_y()));


    //////// State Machine: _b ////////

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_x___b, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_x___b()));
    connect(_x___b, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_x___b()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_x___b, SIGNAL(entered()), this, SLOT(Slot_StateEntry_x___b()));
    connect(_x___b, SIGNAL(exited()), this, SLOT(Slot_StateExit_x___b()));


}

StateMachine::~StateMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//    Each State Machine Section shows all transitions between its direct children
    //////// State Machine: _stateMachine ////////
void StateMachine::Event_startMachine_stateMachine()
{
    _stateMachine->start();
}

void StateMachine::Event_event___a()
{
    emit Relay_Event_event___a();
}

void StateMachine::Event_sr___b()
{
    emit Relay_Event_sr___b();
}


    //////// State Machine: _a ////////
void StateMachine::Event_xToY___x___a()
{
    emit Relay_Event_xToY___x___a();
}


    //////// State Machine: _b ////////

//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _stateMachine ////////
void StateMachine::Slot_StateEntry_a()
{

}

void StateMachine::Slot_StateExit_a()
{

}

void StateMachine::Slot_StateEntry_b()
{

}

void StateMachine::Slot_StateExit_b()
{

}


    //////// State Machine: _a ////////
void StateMachine::Slot_StateEntry_x___a()
{

}

void StateMachine::Slot_StateExit_x___a()
{

}

void StateMachine::Slot_StateEntry_y()
{

}

void StateMachine::Slot_StateExit_y()
{

}


    //////// State Machine: _b ////////
void StateMachine::Slot_StateEntry_x___b()
{

}

void StateMachine::Slot_StateExit_x___b()
{

}



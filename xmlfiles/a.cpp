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
#include "a.h"
#include <QDebug>

A::A(QObject* parent):
    QObject(parent),
    //////// State Machine: _a___3fa67e8e ////////
    _a___3fa67e8e(new QStateMachine(this)),
    _a___55ac6a18(new QState()),
    _a___ab81a686(new QState()),
    _a___c67f423a(new QState())

{
    //////// State Machine: _a___3fa67e8e ////////
    _a___3fa67e8e->addState(_a___55ac6a18);
    _a___3fa67e8e->addState(_a___ab81a686);
    _a___3fa67e8e->addState(_a___c67f423a);

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_a___3fa67e8e, SIGNAL(started()), this, SIGNAL(Signal_StateReady_a___3fa67e8e()));
    connect(_a___55ac6a18, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_a___55ac6a18()));
    connect(_a___55ac6a18, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_a___55ac6a18()));
    connect(_a___ab81a686, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_a___ab81a686()));
    connect(_a___ab81a686, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_a___ab81a686()));
    connect(_a___c67f423a, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_a___c67f423a()));
    connect(_a___c67f423a, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_a___c67f423a()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_a___55ac6a18, SIGNAL(entered()), this, SLOT(Slot_StateEntry_a___55ac6a18()));
    connect(_a___55ac6a18, SIGNAL(exited()), this, SLOT(Slot_StateExit_a___55ac6a18()));
    connect(_a___ab81a686, SIGNAL(entered()), this, SLOT(Slot_StateEntry_a___ab81a686()));
    connect(_a___ab81a686, SIGNAL(exited()), this, SLOT(Slot_StateExit_a___ab81a686()));
    connect(_a___c67f423a, SIGNAL(entered()), this, SLOT(Slot_StateEntry_a___c67f423a()));
    connect(_a___c67f423a, SIGNAL(exited()), this, SLOT(Slot_StateExit_a___c67f423a()));


}

A::~A()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//    Each State Machine Section shows all transitions between its direct children
    //////// State Machine: _a___3fa67e8e ////////
void A::Event_startMachine_a___3fa67e8e()
{
    _a___3fa67e8e->start();
}


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _a___3fa67e8e ////////
void A::Slot_StateEntry_a___55ac6a18()
{

}

void A::Slot_StateExit_a___55ac6a18()
{

}

void A::Slot_StateEntry_a___ab81a686()
{

}

void A::Slot_StateExit_a___ab81a686()
{

}

void A::Slot_StateEntry_a___c67f423a()
{

}

void A::Slot_StateExit_a___c67f423a()
{

}



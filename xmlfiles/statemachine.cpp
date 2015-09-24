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
    _stateMachine_1(new QState()),
    _stateMachine_2(new QState()),
    _stateMachine_3(new QState())

{
    //////// State Machine: _stateMachine ////////
    _stateMachine->addState(_stateMachine_1);
    _stateMachine->addState(_stateMachine_2);
    _stateMachine->addState(_stateMachine_3);
    _stateMachine->setInitialState(_stateMachine_3);

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_stateMachine, SIGNAL(started()), this, SIGNAL(Signal_StateReady___stateMachine()));
    connect(_stateMachine_1, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___stateMachine_1()));
    connect(_stateMachine_1, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___stateMachine_1()));
    connect(_stateMachine_2, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___stateMachine_2()));
    connect(_stateMachine_2, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___stateMachine_2()));
    connect(_stateMachine_3, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___stateMachine_3()));
    connect(_stateMachine_3, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___stateMachine_3()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_stateMachine_1, SIGNAL(entered()), this, SLOT(Slot_StateEntry___stateMachine_1()));
    connect(_stateMachine_1, SIGNAL(exited()), this, SLOT(Slot_StateExit___stateMachine_1()));
    connect(_stateMachine_2, SIGNAL(entered()), this, SLOT(Slot_StateEntry___stateMachine_2()));
    connect(_stateMachine_2, SIGNAL(exited()), this, SLOT(Slot_StateExit___stateMachine_2()));
    connect(_stateMachine_3, SIGNAL(entered()), this, SLOT(Slot_StateEntry___stateMachine_3()));
    connect(_stateMachine_3, SIGNAL(exited()), this, SLOT(Slot_StateExit___stateMachine_3()));


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


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _stateMachine ////////
void StateMachine::Slot_StateEntry___stateMachine_1()
{

}

void StateMachine::Slot_StateExit___stateMachine_1()
{

}

void StateMachine::Slot_StateEntry___stateMachine_2()
{

}

void StateMachine::Slot_StateExit___stateMachine_2()
{

}

void StateMachine::Slot_StateEntry___stateMachine_3()
{
    emit Action___wowDoCoolStuff();
    emit Action___increidblePeformance();
    emit Action___and();
    emit Action___handling();
}

void StateMachine::Slot_StateExit___stateMachine_3()
{
    emit Action___wrastleMania();
    emit Action___awwowow();
}



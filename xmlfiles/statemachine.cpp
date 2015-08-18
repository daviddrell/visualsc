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
    //////// State Machine: _stateMachine_dcf5b3ed ////////
    _stateMachine_dcf5b3ed(new QStateMachine(this)),
    _stateMachineA_2c066f77(new QState()),
    _stateMachineB_ad0fe279(new QState(QState::ParallelStates)),

    //////// State Machine: _stateMachineA_2c066f77 ////////
    _ax_0a93998e(new QState(_stateMachineA_2c066f77)),
    _ay_07d9c974(new QState(_stateMachineA_2c066f77)),
    _az_abbca67c(new QFinalState(_stateMachineA_2c066f77)),

    //////// State Machine: _stateMachineB_ad0fe279 ////////
    _x_d409769e(new QState(_stateMachineB_ad0fe279)),
    _y_46a3e410(new QState(_stateMachineB_ad0fe279)),

    //////// State Machine: _x_d409769e ////////
    _1_3e580603(new QState(_x_d409769e)),
    _2_b6d62949(new QState(_x_d409769e)),

    //////// State Machine: _y_46a3e410 ////////
    _3_926f7882(new QState(_y_46a3e410)),
    _4_de6acfb3(new QState(_y_46a3e410))

{
    //////// State Machine: _stateMachine_dcf5b3ed ////////
    _stateMachine_dcf5b3ed->addState(_stateMachineA_2c066f77);
    _stateMachine_dcf5b3ed->setInitialState(_stateMachineA_2c066f77);
    _stateMachine_dcf5b3ed->addState(_stateMachineB_ad0fe279);

    //    Add transitions for the QStates using the transitions' private relay signals
    _stateMachineA_2c066f77->addTransition(this, SIGNAL(Relay_Event_special_ad0fe279()), _stateMachineB_ad0fe279);
    _stateMachineA_2c066f77->addTransition(_stateMachineA_2c066f77, SIGNAL(finished()), _stateMachineB_ad0fe279);

    //    Propogate the private QState signals to public signals
    connect(_stateMachine_dcf5b3ed, SIGNAL(started()), this, SIGNAL(Signal_StateReady_stateMachine_dcf5b3ed()));
    connect(_stateMachineA_2c066f77, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_stateMachineA_2c066f77()));
    connect(_stateMachineA_2c066f77, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_stateMachineA_2c066f77()));
    connect(_stateMachineB_ad0fe279, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_stateMachineB_ad0fe279()));
    connect(_stateMachineB_ad0fe279, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_stateMachineB_ad0fe279()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_stateMachineA_2c066f77, SIGNAL(entered()), this, SLOT(Slot_StateEntry_stateMachineA_2c066f77()));
    connect(_stateMachineA_2c066f77, SIGNAL(exited()), this, SLOT(Slot_StateExit_stateMachineA_2c066f77()));
    connect(_stateMachineB_ad0fe279, SIGNAL(entered()), this, SLOT(Slot_StateEntry_stateMachineB_ad0fe279()));
    connect(_stateMachineB_ad0fe279, SIGNAL(exited()), this, SLOT(Slot_StateExit_stateMachineB_ad0fe279()));


    //////// State Machine: _stateMachineA_2c066f77 ////////
    _stateMachineA_2c066f77->setInitialState(_ax_0a93998e);

    //    Add transitions for the QStates using the transitions' private relay signals
    _ax_0a93998e->addTransition(this, SIGNAL(Relay_Event_event_07d9c974()), _ay_07d9c974);
    _ay_07d9c974->addTransition(this, SIGNAL(Relay_Event_event_abbca67c()), _az_abbca67c);

    //    Propogate the private QState signals to public signals
    connect(_ax_0a93998e, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_ax_0a93998e()));
    connect(_ax_0a93998e, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_ax_0a93998e()));
    connect(_ay_07d9c974, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_ay_07d9c974()));
    connect(_ay_07d9c974, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_ay_07d9c974()));
    connect(_az_abbca67c, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_az_abbca67c()));
    connect(_az_abbca67c, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_az_abbca67c()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_ax_0a93998e, SIGNAL(entered()), this, SLOT(Slot_StateEntry_ax_0a93998e()));
    connect(_ax_0a93998e, SIGNAL(exited()), this, SLOT(Slot_StateExit_ax_0a93998e()));
    connect(_ay_07d9c974, SIGNAL(entered()), this, SLOT(Slot_StateEntry_ay_07d9c974()));
    connect(_ay_07d9c974, SIGNAL(exited()), this, SLOT(Slot_StateExit_ay_07d9c974()));
    connect(_az_abbca67c, SIGNAL(entered()), this, SLOT(Slot_StateEntry_az_abbca67c()));
    connect(_az_abbca67c, SIGNAL(exited()), this, SLOT(Slot_StateExit_az_abbca67c()));


    //////// State Machine: _stateMachineB_ad0fe279 ////////

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_x_d409769e, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_x_d409769e()));
    connect(_x_d409769e, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_x_d409769e()));
    connect(_y_46a3e410, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_y_46a3e410()));
    connect(_y_46a3e410, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_y_46a3e410()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_x_d409769e, SIGNAL(entered()), this, SLOT(Slot_StateEntry_x_d409769e()));
    connect(_x_d409769e, SIGNAL(exited()), this, SLOT(Slot_StateExit_x_d409769e()));
    connect(_y_46a3e410, SIGNAL(entered()), this, SLOT(Slot_StateEntry_y_46a3e410()));
    connect(_y_46a3e410, SIGNAL(exited()), this, SLOT(Slot_StateExit_y_46a3e410()));


    //////// State Machine: _x_d409769e ////////
    _x_d409769e->setInitialState(_1_3e580603);

    //    Add transitions for the QStates using the transitions' private relay signals
    _1_3e580603->addTransition(this, SIGNAL(Relay_Event_event_b6d62949()), _2_b6d62949);
    _2_b6d62949->addTransition(this, SIGNAL(Relay_Event_event_3e580603()), _1_3e580603);

    //    Propogate the private QState signals to public signals
    connect(_1_3e580603, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_1_3e580603()));
    connect(_1_3e580603, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_1_3e580603()));
    connect(_2_b6d62949, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_2_b6d62949()));
    connect(_2_b6d62949, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_2_b6d62949()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_1_3e580603, SIGNAL(entered()), this, SLOT(Slot_StateEntry_1_3e580603()));
    connect(_1_3e580603, SIGNAL(exited()), this, SLOT(Slot_StateExit_1_3e580603()));
    connect(_2_b6d62949, SIGNAL(entered()), this, SLOT(Slot_StateEntry_2_b6d62949()));
    connect(_2_b6d62949, SIGNAL(exited()), this, SLOT(Slot_StateExit_2_b6d62949()));


    //////// State Machine: _y_46a3e410 ////////
    _y_46a3e410->setInitialState(_3_926f7882);

    //    Add transitions for the QStates using the transitions' private relay signals
    _3_926f7882->addTransition(this, SIGNAL(Relay_Event_event_de6acfb3()), _4_de6acfb3);
    _4_de6acfb3->addTransition(this, SIGNAL(Relay_Event_event_926f7882()), _3_926f7882);

    //    Propogate the private QState signals to public signals
    connect(_3_926f7882, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_3_926f7882()));
    connect(_3_926f7882, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_3_926f7882()));
    connect(_4_de6acfb3, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_4_de6acfb3()));
    connect(_4_de6acfb3, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_4_de6acfb3()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_3_926f7882, SIGNAL(entered()), this, SLOT(Slot_StateEntry_3_926f7882()));
    connect(_3_926f7882, SIGNAL(exited()), this, SLOT(Slot_StateExit_3_926f7882()));
    connect(_4_de6acfb3, SIGNAL(entered()), this, SLOT(Slot_StateEntry_4_de6acfb3()));
    connect(_4_de6acfb3, SIGNAL(exited()), this, SLOT(Slot_StateExit_4_de6acfb3()));


}

StateMachine::~StateMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//    Each State Machine Section shows all transitions between its direct children
    //////// State Machine: _stateMachine_dcf5b3ed ////////
void StateMachine::Event_startMachine_stateMachine_dcf5b3ed()
{
    _stateMachine_dcf5b3ed->start();
}

void StateMachine::Event_special_ad0fe279()
{
    emit Relay_Event_special_ad0fe279();
}


    //////// State Machine: _stateMachineA_2c066f77 ////////
void StateMachine::Event_event_07d9c974()
{
    emit Relay_Event_event_07d9c974();
}

void StateMachine::Event_event_abbca67c()
{
    emit Relay_Event_event_abbca67c();
}


    //////// State Machine: _stateMachineB_ad0fe279 ////////

    //////// State Machine: _x_d409769e ////////
void StateMachine::Event_event_b6d62949()
{
    emit Relay_Event_event_b6d62949();
}

void StateMachine::Event_event_3e580603()
{
    emit Relay_Event_event_3e580603();
}


    //////// State Machine: _y_46a3e410 ////////
void StateMachine::Event_event_de6acfb3()
{
    emit Relay_Event_event_de6acfb3();
}

void StateMachine::Event_event_926f7882()
{
    emit Relay_Event_event_926f7882();
}


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _stateMachine_dcf5b3ed ////////
void StateMachine::Slot_StateEntry_stateMachineA_2c066f77()
{

}

void StateMachine::Slot_StateExit_stateMachineA_2c066f77()
{

}

void StateMachine::Slot_StateEntry_stateMachineB_ad0fe279()
{

}

void StateMachine::Slot_StateExit_stateMachineB_ad0fe279()
{

}


    //////// State Machine: _stateMachineA_2c066f77 ////////
void StateMachine::Slot_StateEntry_ax_0a93998e()
{
    emit EntryAction_jump();
    emit EntryAction_dance();
    emit EntryAction_fly();
}

void StateMachine::Slot_StateExit_ax_0a93998e()
{

}

void StateMachine::Slot_StateEntry_ay_07d9c974()
{

}

void StateMachine::Slot_StateExit_ay_07d9c974()
{

}

void StateMachine::Slot_StateEntry_az_abbca67c()
{

}

void StateMachine::Slot_StateExit_az_abbca67c()
{

}


    //////// State Machine: _stateMachineB_ad0fe279 ////////
void StateMachine::Slot_StateEntry_x_d409769e()
{

}

void StateMachine::Slot_StateExit_x_d409769e()
{
    emit ExitAction_fall();
    emit ExitAction_stop();
    emit ExitAction_lay();
}

void StateMachine::Slot_StateEntry_y_46a3e410()
{

}

void StateMachine::Slot_StateExit_y_46a3e410()
{

}


    //////// State Machine: _x_d409769e ////////
void StateMachine::Slot_StateEntry_1_3e580603()
{

}

void StateMachine::Slot_StateExit_1_3e580603()
{

}

void StateMachine::Slot_StateEntry_2_b6d62949()
{

}

void StateMachine::Slot_StateExit_2_b6d62949()
{

}


    //////// State Machine: _y_46a3e410 ////////
void StateMachine::Slot_StateEntry_3_926f7882()
{

}

void StateMachine::Slot_StateExit_3_926f7882()
{

}

void StateMachine::Slot_StateEntry_4_de6acfb3()
{

}

void StateMachine::Slot_StateExit_4_de6acfb3()
{

}



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
#include "testmanagerstatemachine.h"
#include <QDebug>

TestManagerStateMachine::TestManagerStateMachine(QObject* parent):
    QObject(parent),
    //////// State Machine: _testManagerStateMachine_0c016b77 ////////
    _testManagerStateMachine_0c016b77(new QStateMachine(this)),
    _idle_1aefb2b3(new QState()),
    _downloadingAVC_faa33767(new QState()),
    _unzipAVC_21a6ba6b(new QState()),
    _runningTests_7a81f0a9(new QState()),
    _completed_680d4ac1(new QState())

{
    //////// State Machine: _testManagerStateMachine_0c016b77 ////////
    _testManagerStateMachine_0c016b77->addState(_idle_1aefb2b3);
    _testManagerStateMachine_0c016b77->setInitialState(_idle_1aefb2b3);
    _testManagerStateMachine_0c016b77->addState(_downloadingAVC_faa33767);
    _testManagerStateMachine_0c016b77->addState(_unzipAVC_21a6ba6b);
    _testManagerStateMachine_0c016b77->addState(_runningTests_7a81f0a9);
    _testManagerStateMachine_0c016b77->addState(_completed_680d4ac1);

    //
    //    add transitions for the QStates using the transitions' private relay signals
    //
    _idle_1aefb2b3->addTransition(this, SIGNAL(Relay_Event_start_faa33767()),_downloadingAVC_faa33767);
    _downloadingAVC_faa33767->addTransition(this, SIGNAL(Relay_Event_sHA1IsDifferent_21a6ba6b()),_unzipAVC_21a6ba6b);
    _downloadingAVC_faa33767->addTransition(this, SIGNAL(Relay_Event_downloadFailed_680d4ac1()),_completed_680d4ac1);
    _downloadingAVC_faa33767->addTransition(this, SIGNAL(Relay_Event_sHA1IsSame_680d4ac1()),_completed_680d4ac1);
    _unzipAVC_21a6ba6b->addTransition(this, SIGNAL(Relay_Event_unzipSuccess_7a81f0a9()),_runningTests_7a81f0a9);
    _unzipAVC_21a6ba6b->addTransition(this, SIGNAL(Relay_Event_unzipFail_680d4ac1()),_completed_680d4ac1);
    _runningTests_7a81f0a9->addTransition(this, SIGNAL(Relay_Event_testsCompleted_680d4ac1()),_completed_680d4ac1);
    _completed_680d4ac1->addTransition(this, SIGNAL(Relay_Event_pollTimerPopped_faa33767()),_downloadingAVC_faa33767);

    //    Propogate the private QState signals to public signals
    connect(_testManagerStateMachine_0c016b77, SIGNAL(started()), this, SIGNAL(Signal_StateReady_testManagerStateMachine_0c016b77()));
    connect(_idle_1aefb2b3, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_idle_1aefb2b3()));
    connect(_idle_1aefb2b3, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_idle_1aefb2b3()));
    connect(_downloadingAVC_faa33767, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_downloadingAVC_faa33767()));
    connect(_downloadingAVC_faa33767, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_downloadingAVC_faa33767()));
    connect(_unzipAVC_21a6ba6b, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_unzipAVC_21a6ba6b()));
    connect(_unzipAVC_21a6ba6b, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_unzipAVC_21a6ba6b()));
    connect(_runningTests_7a81f0a9, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_runningTests_7a81f0a9()));
    connect(_runningTests_7a81f0a9, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_runningTests_7a81f0a9()));
    connect(_completed_680d4ac1, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_completed_680d4ac1()));
    connect(_completed_680d4ac1, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_completed_680d4ac1()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_idle_1aefb2b3, SIGNAL(entered()), this, SLOT(Slot_StateEntry_idle_1aefb2b3()));
    connect(_idle_1aefb2b3, SIGNAL(exited()), this, SLOT(Slot_StateExit_idle_1aefb2b3()));
    connect(_downloadingAVC_faa33767, SIGNAL(entered()), this, SLOT(Slot_StateEntry_downloadingAVC_faa33767()));
    connect(_downloadingAVC_faa33767, SIGNAL(exited()), this, SLOT(Slot_StateExit_downloadingAVC_faa33767()));
    connect(_unzipAVC_21a6ba6b, SIGNAL(entered()), this, SLOT(Slot_StateEntry_unzipAVC_21a6ba6b()));
    connect(_unzipAVC_21a6ba6b, SIGNAL(exited()), this, SLOT(Slot_StateExit_unzipAVC_21a6ba6b()));
    connect(_runningTests_7a81f0a9, SIGNAL(entered()), this, SLOT(Slot_StateEntry_runningTests_7a81f0a9()));
    connect(_runningTests_7a81f0a9, SIGNAL(exited()), this, SLOT(Slot_StateExit_runningTests_7a81f0a9()));
    connect(_completed_680d4ac1, SIGNAL(entered()), this, SLOT(Slot_StateEntry_completed_680d4ac1()));
    connect(_completed_680d4ac1, SIGNAL(exited()), this, SLOT(Slot_StateExit_completed_680d4ac1()));


}

TestManagerStateMachine::~TestManagerStateMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//
    //////// State Machine: _testManagerStateMachine_0c016b77 ////////
void TestManagerStateMachine::Event_startMachine_testManagerStateMachine_0c016b77()
{
    _testManagerStateMachine_0c016b77->start();
}

void TestManagerStateMachine::Event_start_faa33767()
{
    emit Relay_Event_start_faa33767();
}

void TestManagerStateMachine::Event_sHA1IsDifferent_21a6ba6b()
{
    emit Relay_Event_sHA1IsDifferent_21a6ba6b();
}

void TestManagerStateMachine::Event_downloadFailed_680d4ac1()
{
    emit Relay_Event_downloadFailed_680d4ac1();
}

void TestManagerStateMachine::Event_sHA1IsSame_680d4ac1()
{
    emit Relay_Event_sHA1IsSame_680d4ac1();
}

void TestManagerStateMachine::Event_unzipSuccess_7a81f0a9()
{
    emit Relay_Event_unzipSuccess_7a81f0a9();
}

void TestManagerStateMachine::Event_unzipFail_680d4ac1()
{
    emit Relay_Event_unzipFail_680d4ac1();
}

void TestManagerStateMachine::Event_testsCompleted_680d4ac1()
{
    emit Relay_Event_testsCompleted_680d4ac1();
}

void TestManagerStateMachine::Event_pollTimerPopped_faa33767()
{
    emit Relay_Event_pollTimerPopped_faa33767();
}


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _testManagerStateMachine_0c016b77 ////////
void TestManagerStateMachine::Slot_StateEntry_idle_1aefb2b3()
{

}

void TestManagerStateMachine::Slot_StateExit_idle_1aefb2b3()
{

}

void TestManagerStateMachine::Slot_StateEntry_downloadingAVC_faa33767()
{
    emit EntryAction_startDownload();
}

void TestManagerStateMachine::Slot_StateExit_downloadingAVC_faa33767()
{

}

void TestManagerStateMachine::Slot_StateEntry_unzipAVC_21a6ba6b()
{
    emit EntryAction_startUnzip();
}

void TestManagerStateMachine::Slot_StateExit_unzipAVC_21a6ba6b()
{

}

void TestManagerStateMachine::Slot_StateEntry_runningTests_7a81f0a9()
{
    emit EntryAction_startTests();
}

void TestManagerStateMachine::Slot_StateExit_runningTests_7a81f0a9()
{

}

void TestManagerStateMachine::Slot_StateEntry_completed_680d4ac1()
{

}

void TestManagerStateMachine::Slot_StateExit_completed_680d4ac1()
{

}



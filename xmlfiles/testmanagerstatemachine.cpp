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
    _testManagerStateMachine(new QStateMachine(this)),
    _idle(new QState()),
    _downloadingAVC(new QState()),
    _unzipAVC(new QState()),
    _runningTests(new QState()),
    _completed(new QState())
{
    _testManagerStateMachine->addState(_idle);
    _testManagerStateMachine->addState(_downloadingAVC);
    _testManagerStateMachine->addState(_unzipAVC);
    _testManagerStateMachine->addState(_runningTests);
    _testManagerStateMachine->addState(_completed);
    _testManagerStateMachine->setInitialState(_idle);

    //
    //    add transitions for the QStates using the transitions' private relay signals
    //
    _idle->addTransition(this,SIGNAL(Relay_Event_start()),_downloadingAVC);
    _downloadingAVC->addTransition(this,SIGNAL(Relay_Event_sHA1IsDifferent()),_unzipAVC);
    _downloadingAVC->addTransition(this,SIGNAL(Relay_Event_downloadFailed()),_completed);
    _downloadingAVC->addTransition(this,SIGNAL(Relay_Event_sHA1IsSame()),_completed);
    _unzipAVC->addTransition(this,SIGNAL(Relay_Event_unzipSuccess()),_runningTests);
    _unzipAVC->addTransition(this,SIGNAL(Relay_Event_unzipFail()),_completed);
    _runningTests->addTransition(this,SIGNAL(Relay_Event_testsCompleted()),_completed);
    _completed->addTransition(this,SIGNAL(Relay_Event_pollTimerPopped()),_downloadingAVC);

    //    Propogate the private QState signals to public signals
    connect(_testManagerStateMachine, SIGNAL(started()), this, SIGNAL(Signal_StateReady_testManagerStateMachine());
    connect(_idle, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_idle()));
    connect(_idle, SIGNAL(exited()), this, SIGNAL(Signal_StateEntry_idle()));
    connect(_downloadingAVC, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_downloadingAVC()));
    connect(_downloadingAVC, SIGNAL(exited()), this, SIGNAL(Signal_StateEntry_downloadingAVC()));
    connect(_unzipAVC, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_unzipAVC()));
    connect(_unzipAVC, SIGNAL(exited()), this, SIGNAL(Signal_StateEntry_unzipAVC()));
    connect(_runningTests, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_runningTests()));
    connect(_runningTests, SIGNAL(exited()), this, SIGNAL(Signal_StateEntry_runningTests()));
    connect(_completed, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_completed()));
    connect(_completed, SIGNAL(exited()), this, SIGNAL(Signal_StateEntry_completed()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_idle, SIGNAL(entered()), this, SLOT(Slot_StateEntry_idle()));
    connect(_idle, SIGNAL(exited()), this, SLOT(Slot_StateExit_idle()));
    connect(_downloadingAVC, SIGNAL(entered()), this, SLOT(Slot_StateEntry_downloadingAVC()));
    connect(_downloadingAVC, SIGNAL(exited()), this, SLOT(Slot_StateExit_downloadingAVC()));
    connect(_unzipAVC, SIGNAL(entered()), this, SLOT(Slot_StateEntry_unzipAVC()));
    connect(_unzipAVC, SIGNAL(exited()), this, SLOT(Slot_StateExit_unzipAVC()));
    connect(_runningTests, SIGNAL(entered()), this, SLOT(Slot_StateEntry_runningTests()));
    connect(_runningTests, SIGNAL(exited()), this, SLOT(Slot_StateExit_runningTests()));
    connect(_completed, SIGNAL(entered()), this, SLOT(Slot_StateEntry_completed()));
    connect(_completed, SIGNAL(exited()), this, SLOT(Slot_StateExit_completed()));
}

TestManagerStateMachine::~TestManagerStateMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//
void TestManagerStateMachine::Event_startMachine_testManagerStateMachine()
{
    _testManagerStateMachine->start();
}

void TestManagerStateMachine::Event_start()
{
    emit Relay_Event_start();
}

void TestManagerStateMachine::Event_sHA1IsDifferent()
{
    emit Relay_Event_sHA1IsDifferent();
}

void TestManagerStateMachine::Event_downloadFailed()
{
    emit Relay_Event_downloadFailed();
}

void TestManagerStateMachine::Event_sHA1IsSame()
{
    emit Relay_Event_sHA1IsSame();
}

void TestManagerStateMachine::Event_unzipSuccess()
{
    emit Relay_Event_unzipSuccess();
}

void TestManagerStateMachine::Event_unzipFail()
{
    emit Relay_Event_unzipFail();
}

void TestManagerStateMachine::Event_testsCompleted()
{
    emit Relay_Event_testsCompleted();
}

void TestManagerStateMachine::Event_pollTimerPopped()
{
    emit Relay_Event_pollTimerPopped();
}

//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
void TestManagerStateMachine::Slot_StateEntry_idle()
{
}

void TestManagerStateMachine::Slot_StateExit_idle()
{

}

void TestManagerStateMachine::Slot_StateEntry_downloadingAVC()
{
    emit EntryAction_startDownload();
}

void TestManagerStateMachine::Slot_StateExit_downloadingAVC()
{

}

void TestManagerStateMachine::Slot_StateEntry_unzipAVC()
{
    emit EntryAction_startUnzip();
}

void TestManagerStateMachine::Slot_StateExit_unzipAVC()
{

}

void TestManagerStateMachine::Slot_StateEntry_runningTests()
{
    emit EntryAction_startTests();
}

void TestManagerStateMachine::Slot_StateExit_runningTests()
{

}

void TestManagerStateMachine::Slot_StateEntry_completed()
{
}

void TestManagerStateMachine::Slot_StateExit_completed()
{

}


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
    //////// State Machine: _testManagerStateMachine_97b850af ////////
    _testManagerStateMachine_97b850af(new QStateMachine(this)),
    _idle_c9dcfd20(new QState()),
    _downloadingAVC_fa1bce49(new QState()),
    _runningTests_c9e4b34a(new QState()),
    _completed_850da190(new QState()),
    _updateFirmwareAllUnits_53416b9a(new QState(QState::ParallelStates)),

    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////
    _updatingGSUnits_52c831de(new QState(_updateFirmwareAllUnits_53416b9a)),
    _updatingWincomm_7778a7d0(new QState(_updateFirmwareAllUnits_53416b9a)),

    //////// State Machine: _updatingGSUnits_52c831de ////////
    _updateInProgress_6d5ca4f3(new QState(_updatingGSUnits_52c831de)),
    _done_48c3bcdd(new QFinalState(_updatingGSUnits_52c831de)),

    //////// State Machine: _updatingWincomm_7778a7d0 ////////
    _unzipInProgress_9f1f6d0f(new QState(_updatingWincomm_7778a7d0)),
    _done_f93692c9(new QFinalState(_updatingWincomm_7778a7d0))

{
    //////// State Machine: _testManagerStateMachine_97b850af ////////
    _testManagerStateMachine_97b850af->addState(_idle_c9dcfd20);
    _testManagerStateMachine_97b850af->setInitialState(_idle_c9dcfd20);
    _testManagerStateMachine_97b850af->addState(_downloadingAVC_fa1bce49);
    _testManagerStateMachine_97b850af->addState(_runningTests_c9e4b34a);
    _testManagerStateMachine_97b850af->addState(_completed_850da190);
    _testManagerStateMachine_97b850af->addState(_updateFirmwareAllUnits_53416b9a);

    //    Add transitions for the QStates using the transitions' private relay signals
    _idle_c9dcfd20->addTransition(this, SIGNAL(Relay_Event_start_fa1bce49()), _downloadingAVC_fa1bce49);
    _downloadingAVC_fa1bce49->addTransition(this, SIGNAL(Relay_Event_downloadFailed_850da190()), _completed_850da190);
    _downloadingAVC_fa1bce49->addTransition(this, SIGNAL(Relay_Event_sHA1IsSame_850da190()), _completed_850da190);
    _downloadingAVC_fa1bce49->addTransition(this, SIGNAL(Relay_Event_sHA1IsDifferent_53416b9a()), _updateFirmwareAllUnits_53416b9a);
    _runningTests_c9e4b34a->addTransition(this, SIGNAL(Relay_Event_testsCompleted_850da190()), _completed_850da190);
    _completed_850da190->addTransition(this, SIGNAL(Relay_Event_pollTimerPopped_fa1bce49()), _downloadingAVC_fa1bce49);
    _updateFirmwareAllUnits_53416b9a->addTransition(this, SIGNAL(Relay_Event_updateFailure_850da190()), _completed_850da190);
    _updateFirmwareAllUnits_53416b9a->addTransition(this, SIGNAL(Relay_Event_updateFirmwareAllUnitsFinished_c9e4b34a()), _runningTests_c9e4b34a);
    _updateFirmwareAllUnits_53416b9a->addTransition(_updateFirmwareAllUnits_53416b9a, SIGNAL(finished()), _runningTests_c9e4b34a);

    //    Propogate the private QState signals to public signals
    connect(_testManagerStateMachine_97b850af, SIGNAL(started()), this, SIGNAL(Signal_StateReady_testManagerStateMachine_97b850af()));
    connect(_idle_c9dcfd20, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_idle_c9dcfd20()));
    connect(_idle_c9dcfd20, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_idle_c9dcfd20()));
    connect(_downloadingAVC_fa1bce49, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_downloadingAVC_fa1bce49()));
    connect(_downloadingAVC_fa1bce49, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_downloadingAVC_fa1bce49()));
    connect(_runningTests_c9e4b34a, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_runningTests_c9e4b34a()));
    connect(_runningTests_c9e4b34a, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_runningTests_c9e4b34a()));
    connect(_completed_850da190, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_completed_850da190()));
    connect(_completed_850da190, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_completed_850da190()));
    connect(_updateFirmwareAllUnits_53416b9a, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_updateFirmwareAllUnits_53416b9a()));
    connect(_updateFirmwareAllUnits_53416b9a, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_updateFirmwareAllUnits_53416b9a()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_idle_c9dcfd20, SIGNAL(entered()), this, SLOT(Slot_StateEntry_idle_c9dcfd20()));
    connect(_idle_c9dcfd20, SIGNAL(exited()), this, SLOT(Slot_StateExit_idle_c9dcfd20()));
    connect(_downloadingAVC_fa1bce49, SIGNAL(entered()), this, SLOT(Slot_StateEntry_downloadingAVC_fa1bce49()));
    connect(_downloadingAVC_fa1bce49, SIGNAL(exited()), this, SLOT(Slot_StateExit_downloadingAVC_fa1bce49()));
    connect(_runningTests_c9e4b34a, SIGNAL(entered()), this, SLOT(Slot_StateEntry_runningTests_c9e4b34a()));
    connect(_runningTests_c9e4b34a, SIGNAL(exited()), this, SLOT(Slot_StateExit_runningTests_c9e4b34a()));
    connect(_completed_850da190, SIGNAL(entered()), this, SLOT(Slot_StateEntry_completed_850da190()));
    connect(_completed_850da190, SIGNAL(exited()), this, SLOT(Slot_StateExit_completed_850da190()));
    connect(_updateFirmwareAllUnits_53416b9a, SIGNAL(entered()), this, SLOT(Slot_StateEntry_updateFirmwareAllUnits_53416b9a()));
    connect(_updateFirmwareAllUnits_53416b9a, SIGNAL(exited()), this, SLOT(Slot_StateExit_updateFirmwareAllUnits_53416b9a()));


    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_updateFirmwareAllUnits_53416b9a, SIGNAL(started()), this, SIGNAL(Signal_StateReady_updateFirmwareAllUnits_53416b9a()));
    connect(_updatingGSUnits_52c831de, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_updatingGSUnits_52c831de()));
    connect(_updatingGSUnits_52c831de, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_updatingGSUnits_52c831de()));
    connect(_updatingWincomm_7778a7d0, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_updatingWincomm_7778a7d0()));
    connect(_updatingWincomm_7778a7d0, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_updatingWincomm_7778a7d0()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_updatingGSUnits_52c831de, SIGNAL(entered()), this, SLOT(Slot_StateEntry_updatingGSUnits_52c831de()));
    connect(_updatingGSUnits_52c831de, SIGNAL(exited()), this, SLOT(Slot_StateExit_updatingGSUnits_52c831de()));
    connect(_updatingWincomm_7778a7d0, SIGNAL(entered()), this, SLOT(Slot_StateEntry_updatingWincomm_7778a7d0()));
    connect(_updatingWincomm_7778a7d0, SIGNAL(exited()), this, SLOT(Slot_StateExit_updatingWincomm_7778a7d0()));


    //////// State Machine: _updatingGSUnits_52c831de ////////
    _updatingGSUnits_52c831de->setInitialState(_updateInProgress_6d5ca4f3);

    //    Add transitions for the QStates using the transitions' private relay signals
    _updateInProgress_6d5ca4f3->addTransition(this, SIGNAL(Relay_Event_gSUpdateSuccess_48c3bcdd()), _done_48c3bcdd);

    //    Propogate the private QState signals to public signals
    connect(_updatingGSUnits_52c831de, SIGNAL(started()), this, SIGNAL(Signal_StateReady_updatingGSUnits_52c831de()));
    connect(_updateInProgress_6d5ca4f3, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_updateInProgress_6d5ca4f3()));
    connect(_updateInProgress_6d5ca4f3, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_updateInProgress_6d5ca4f3()));
    connect(_done_48c3bcdd, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_done_48c3bcdd()));
    connect(_done_48c3bcdd, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_done_48c3bcdd()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_updateInProgress_6d5ca4f3, SIGNAL(entered()), this, SLOT(Slot_StateEntry_updateInProgress_6d5ca4f3()));
    connect(_updateInProgress_6d5ca4f3, SIGNAL(exited()), this, SLOT(Slot_StateExit_updateInProgress_6d5ca4f3()));
    connect(_done_48c3bcdd, SIGNAL(entered()), this, SLOT(Slot_StateEntry_done_48c3bcdd()));
    connect(_done_48c3bcdd, SIGNAL(exited()), this, SLOT(Slot_StateExit_done_48c3bcdd()));


    //////// State Machine: _updatingWincomm_7778a7d0 ////////
    _updatingWincomm_7778a7d0->setInitialState(_unzipInProgress_9f1f6d0f);

    //    Add transitions for the QStates using the transitions' private relay signals
    _unzipInProgress_9f1f6d0f->addTransition(this, SIGNAL(Relay_Event_upzipSuccess_f93692c9()), _done_f93692c9);

    //    Propogate the private QState signals to public signals
    connect(_updatingWincomm_7778a7d0, SIGNAL(started()), this, SIGNAL(Signal_StateReady_updatingWincomm_7778a7d0()));
    connect(_unzipInProgress_9f1f6d0f, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_unzipInProgress_9f1f6d0f()));
    connect(_unzipInProgress_9f1f6d0f, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_unzipInProgress_9f1f6d0f()));
    connect(_done_f93692c9, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry_done_f93692c9()));
    connect(_done_f93692c9, SIGNAL(exited()), this, SIGNAL(Signal_StateExit_done_f93692c9()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_unzipInProgress_9f1f6d0f, SIGNAL(entered()), this, SLOT(Slot_StateEntry_unzipInProgress_9f1f6d0f()));
    connect(_unzipInProgress_9f1f6d0f, SIGNAL(exited()), this, SLOT(Slot_StateExit_unzipInProgress_9f1f6d0f()));
    connect(_done_f93692c9, SIGNAL(entered()), this, SLOT(Slot_StateEntry_done_f93692c9()));
    connect(_done_f93692c9, SIGNAL(exited()), this, SLOT(Slot_StateExit_done_f93692c9()));


}

TestManagerStateMachine::~TestManagerStateMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//    Each State Machine Section shows all transitions between its direct children
    //////// State Machine: _testManagerStateMachine_97b850af ////////
void TestManagerStateMachine::Event_startMachine_testManagerStateMachine_97b850af()
{
    _testManagerStateMachine_97b850af->start();
}

void TestManagerStateMachine::Event_start_fa1bce49()
{
    emit Relay_Event_start_fa1bce49();
}

void TestManagerStateMachine::Event_downloadFailed_850da190()
{
    emit Relay_Event_downloadFailed_850da190();
}

void TestManagerStateMachine::Event_sHA1IsSame_850da190()
{
    emit Relay_Event_sHA1IsSame_850da190();
}

void TestManagerStateMachine::Event_sHA1IsDifferent_53416b9a()
{
    emit Relay_Event_sHA1IsDifferent_53416b9a();
}

void TestManagerStateMachine::Event_testsCompleted_850da190()
{
    emit Relay_Event_testsCompleted_850da190();
}

void TestManagerStateMachine::Event_pollTimerPopped_fa1bce49()
{
    emit Relay_Event_pollTimerPopped_fa1bce49();
}

void TestManagerStateMachine::Event_updateFailure_850da190()
{
    emit Relay_Event_updateFailure_850da190();
}

void TestManagerStateMachine::Event_updateFirmwareAllUnitsFinished_c9e4b34a()
{
    emit Relay_Event_updateFirmwareAllUnitsFinished_c9e4b34a();
}


    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////

    //////// State Machine: _updatingGSUnits_52c831de ////////
void TestManagerStateMachine::Event_gSUpdateSuccess_48c3bcdd()
{
    emit Relay_Event_gSUpdateSuccess_48c3bcdd();
}


    //////// State Machine: _updatingWincomm_7778a7d0 ////////
void TestManagerStateMachine::Event_upzipSuccess_f93692c9()
{
    emit Relay_Event_upzipSuccess_f93692c9();
}


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _testManagerStateMachine_97b850af ////////
void TestManagerStateMachine::Slot_StateEntry_idle_c9dcfd20()
{

}

void TestManagerStateMachine::Slot_StateExit_idle_c9dcfd20()
{

}

void TestManagerStateMachine::Slot_StateEntry_downloadingAVC_fa1bce49()
{
    emit EntryAction_startDownload();
}

void TestManagerStateMachine::Slot_StateExit_downloadingAVC_fa1bce49()
{

}

void TestManagerStateMachine::Slot_StateEntry_runningTests_c9e4b34a()
{
    emit EntryAction_startTests();
}

void TestManagerStateMachine::Slot_StateExit_runningTests_c9e4b34a()
{

}

void TestManagerStateMachine::Slot_StateEntry_completed_850da190()
{

}

void TestManagerStateMachine::Slot_StateExit_completed_850da190()
{

}

void TestManagerStateMachine::Slot_StateEntry_updateFirmwareAllUnits_53416b9a()
{

}

void TestManagerStateMachine::Slot_StateExit_updateFirmwareAllUnits_53416b9a()
{

}


    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////
void TestManagerStateMachine::Slot_StateEntry_updatingGSUnits_52c831de()
{

}

void TestManagerStateMachine::Slot_StateExit_updatingGSUnits_52c831de()
{

}

void TestManagerStateMachine::Slot_StateEntry_updatingWincomm_7778a7d0()
{

}

void TestManagerStateMachine::Slot_StateExit_updatingWincomm_7778a7d0()
{

}


    //////// State Machine: _updatingGSUnits_52c831de ////////
void TestManagerStateMachine::Slot_StateEntry_updateInProgress_6d5ca4f3()
{
    emit EntryAction_startGSUpdate();
}

void TestManagerStateMachine::Slot_StateExit_updateInProgress_6d5ca4f3()
{

}

void TestManagerStateMachine::Slot_StateEntry_done_48c3bcdd()
{

}

void TestManagerStateMachine::Slot_StateExit_done_48c3bcdd()
{

}


    //////// State Machine: _updatingWincomm_7778a7d0 ////////
void TestManagerStateMachine::Slot_StateEntry_unzipInProgress_9f1f6d0f()
{
    emit EntryAction_startUnzip();
}

void TestManagerStateMachine::Slot_StateExit_unzipInProgress_9f1f6d0f()
{

}

void TestManagerStateMachine::Slot_StateEntry_done_f93692c9()
{

}

void TestManagerStateMachine::Slot_StateExit_done_f93692c9()
{

}



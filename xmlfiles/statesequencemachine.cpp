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
#include "testmanagerstatemachine.h"
#include <QDebug>

TestManagerStateMachine::TestManagerStateMachine(QObject* parent):
    QObject(parent),
    //////// State Machine: _testManagerStateMachine ////////
    _testManagerStateMachine(new QStateMachine(this)),
    _idle(new QState()),
    _downloadingAVC(new QState()),
    _runningTests(new QState()),
    _completed(new QState()),
    _updateFirmwareAllUnits(new QState(QState::ParallelStates)),

    //////// State Machine: _updateFirmwareAllUnits ////////
    _updatingGSUnits(new QState(_updateFirmwareAllUnits)),
    _updatingWincomm(new QState(_updateFirmwareAllUnits)),

    //////// State Machine: _updatingGSUnits ////////
    _updateInProgress(new QState(_updatingGSUnits)),
    _done_updatingGSUnits(new QFinalState(_updatingGSUnits)),

    //////// State Machine: _updatingWincomm ////////
    _unzipInProgress(new QState(_updatingWincomm)),
    _done_updatingWincomm(new QFinalState(_updatingWincomm))

{
    //////// State Machine: _testManagerStateMachine ////////
    _testManagerStateMachine->addState(_idle);
    _testManagerStateMachine->setInitialState(_idle);
    _testManagerStateMachine->addState(_downloadingAVC);
    _testManagerStateMachine->addState(_runningTests);
    _testManagerStateMachine->addState(_completed);
    _testManagerStateMachine->addState(_updateFirmwareAllUnits);

    //    Add transitions for the QStates using the transitions' private relay signals
    _idle->addTransition(this, SIGNAL(Relay_Event___start_idle()), _downloadingAVC);
    _downloadingAVC->addTransition(this, SIGNAL(Relay_Event___downloadFailed_downloadingAVC()), _completed);
    _downloadingAVC->addTransition(this, SIGNAL(Relay_Event___sHA1IsSame_downloadingAVC()), _completed);
    _downloadingAVC->addTransition(this, SIGNAL(Relay_Event___sHA1IsDifferent_downloadingAVC()), _updateFirmwareAllUnits);
    _runningTests->addTransition(this, SIGNAL(Relay_Event___testsCompleted_runningTests()), _completed);
    _completed->addTransition(this, SIGNAL(Relay_Event___pollTimerPopped_completed()), _downloadingAVC);
    _updateFirmwareAllUnits->addTransition(this, SIGNAL(Relay_Event___updateFailure_updateFirmwareAllUnits()), _completed);
    _updateFirmwareAllUnits->addTransition(this, SIGNAL(Relay_Event___updateFirmwareAllUnitsFinished_updateFirmwareAllUnits()), _runningTests);
    _updateFirmwareAllUnits->addTransition(_updateFirmwareAllUnits, SIGNAL(finished()), _runningTests);

    //    Propogate the private QState signals to public signals
    connect(_testManagerStateMachine, SIGNAL(started()), this, SIGNAL(Signal_StateReady___testManagerStateMachine()));
    connect(_idle, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___idle()));
    connect(_idle, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___idle()));
    connect(_downloadingAVC, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___downloadingAVC()));
    connect(_downloadingAVC, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___downloadingAVC()));
    connect(_runningTests, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___runningTests()));
    connect(_runningTests, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___runningTests()));
    connect(_completed, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___completed()));
    connect(_completed, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___completed()));
    connect(_updateFirmwareAllUnits, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___updateFirmwareAllUnits()));
    connect(_updateFirmwareAllUnits, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___updateFirmwareAllUnits()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_idle, SIGNAL(entered()), this, SLOT(Slot_StateEntry___idle()));
    connect(_idle, SIGNAL(exited()), this, SLOT(Slot_StateExit___idle()));
    connect(_downloadingAVC, SIGNAL(entered()), this, SLOT(Slot_StateEntry___downloadingAVC()));
    connect(_downloadingAVC, SIGNAL(exited()), this, SLOT(Slot_StateExit___downloadingAVC()));
    connect(_runningTests, SIGNAL(entered()), this, SLOT(Slot_StateEntry___runningTests()));
    connect(_runningTests, SIGNAL(exited()), this, SLOT(Slot_StateExit___runningTests()));
    connect(_completed, SIGNAL(entered()), this, SLOT(Slot_StateEntry___completed()));
    connect(_completed, SIGNAL(exited()), this, SLOT(Slot_StateExit___completed()));
    connect(_updateFirmwareAllUnits, SIGNAL(entered()), this, SLOT(Slot_StateEntry___updateFirmwareAllUnits()));
    connect(_updateFirmwareAllUnits, SIGNAL(exited()), this, SLOT(Slot_StateExit___updateFirmwareAllUnits()));


    //////// State Machine: _updateFirmwareAllUnits ////////

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_updatingGSUnits, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___updatingGSUnits()));
    connect(_updatingGSUnits, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___updatingGSUnits()));
    connect(_updatingWincomm, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___updatingWincomm()));
    connect(_updatingWincomm, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___updatingWincomm()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_updatingGSUnits, SIGNAL(entered()), this, SLOT(Slot_StateEntry___updatingGSUnits()));
    connect(_updatingGSUnits, SIGNAL(exited()), this, SLOT(Slot_StateExit___updatingGSUnits()));
    connect(_updatingWincomm, SIGNAL(entered()), this, SLOT(Slot_StateEntry___updatingWincomm()));
    connect(_updatingWincomm, SIGNAL(exited()), this, SLOT(Slot_StateExit___updatingWincomm()));


    //////// State Machine: _updatingGSUnits ////////
    _updatingGSUnits->setInitialState(_updateInProgress);

    //    Add transitions for the QStates using the transitions' private relay signals
    _updateInProgress->addTransition(this, SIGNAL(Relay_Event___gSUpdateSuccess_updateInProgress()), _done_updatingGSUnits);

    //    Propogate the private QState signals to public signals
    connect(_updateInProgress, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___updateInProgress()));
    connect(_updateInProgress, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___updateInProgress()));
    connect(_done_updatingGSUnits, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___done_updatingGSUnits()));
    connect(_done_updatingGSUnits, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___done_updatingGSUnits()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_updateInProgress, SIGNAL(entered()), this, SLOT(Slot_StateEntry___updateInProgress()));
    connect(_updateInProgress, SIGNAL(exited()), this, SLOT(Slot_StateExit___updateInProgress()));
    connect(_done_updatingGSUnits, SIGNAL(entered()), this, SLOT(Slot_StateEntry___done_updatingGSUnits()));
    connect(_done_updatingGSUnits, SIGNAL(exited()), this, SLOT(Slot_StateExit___done_updatingGSUnits()));


    //////// State Machine: _updatingWincomm ////////
    _updatingWincomm->setInitialState(_unzipInProgress);

    //    Add transitions for the QStates using the transitions' private relay signals
    _unzipInProgress->addTransition(this, SIGNAL(Relay_Event___upzipSuccess_unzipInProgress()), _done_updatingWincomm);

    //    Propogate the private QState signals to public signals
    connect(_unzipInProgress, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___unzipInProgress()));
    connect(_unzipInProgress, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___unzipInProgress()));
    connect(_done_updatingWincomm, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___done_updatingWincomm()));
    connect(_done_updatingWincomm, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___done_updatingWincomm()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_unzipInProgress, SIGNAL(entered()), this, SLOT(Slot_StateEntry___unzipInProgress()));
    connect(_unzipInProgress, SIGNAL(exited()), this, SLOT(Slot_StateExit___unzipInProgress()));
    connect(_done_updatingWincomm, SIGNAL(entered()), this, SLOT(Slot_StateEntry___done_updatingWincomm()));
    connect(_done_updatingWincomm, SIGNAL(exited()), this, SLOT(Slot_StateExit___done_updatingWincomm()));


}

TestManagerStateMachine::~TestManagerStateMachine()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//    Each State Machine Section shows all transitions between its direct children
    //////// State Machine: _testManagerStateMachine ////////
void TestManagerStateMachine::Event_startMachine___testManagerStateMachine()
{
    _testManagerStateMachine->start();
}

void TestManagerStateMachine::Event___start()
{
    emit Relay_Event___start_idle();
}

void TestManagerStateMachine::Event___downloadFailed()
{
    emit Relay_Event___downloadFailed_downloadingAVC();
}

void TestManagerStateMachine::Event___sHA1IsSame()
{
    emit Relay_Event___sHA1IsSame_downloadingAVC();
}

void TestManagerStateMachine::Event___sHA1IsDifferent()
{
    emit Relay_Event___sHA1IsDifferent_downloadingAVC();
}

void TestManagerStateMachine::Event___testsCompleted()
{
    emit Relay_Event___testsCompleted_runningTests();
}

void TestManagerStateMachine::Event___pollTimerPopped()
{
    emit Relay_Event___pollTimerPopped_completed();
}

void TestManagerStateMachine::Event___updateFailure()
{
    emit Relay_Event___updateFailure_updateFirmwareAllUnits();
}

void TestManagerStateMachine::Event___updateFirmwareAllUnitsFinished()
{
    emit Relay_Event___updateFirmwareAllUnitsFinished_updateFirmwareAllUnits();
}


    //////// State Machine: _updateFirmwareAllUnits ////////

    //////// State Machine: _updatingGSUnits ////////
void TestManagerStateMachine::Event___gSUpdateSuccess()
{
    emit Relay_Event___gSUpdateSuccess_updateInProgress();
}


    //////// State Machine: _updatingWincomm ////////
void TestManagerStateMachine::Event___upzipSuccess()
{
    emit Relay_Event___upzipSuccess_unzipInProgress();
}


//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _testManagerStateMachine ////////
void TestManagerStateMachine::Slot_StateEntry___idle()
{

}

void TestManagerStateMachine::Slot_StateExit___idle()
{

}

void TestManagerStateMachine::Slot_StateEntry___downloadingAVC()
{
    emit Action___startdownload();
}

void TestManagerStateMachine::Slot_StateExit___downloadingAVC()
{

}

void TestManagerStateMachine::Slot_StateEntry___runningTests()
{
    emit Action___starttests();
}

void TestManagerStateMachine::Slot_StateExit___runningTests()
{

}

void TestManagerStateMachine::Slot_StateEntry___completed()
{

}

void TestManagerStateMachine::Slot_StateExit___completed()
{

}

void TestManagerStateMachine::Slot_StateEntry___updateFirmwareAllUnits()
{

}

void TestManagerStateMachine::Slot_StateExit___updateFirmwareAllUnits()
{

}


    //////// State Machine: _updateFirmwareAllUnits ////////
void TestManagerStateMachine::Slot_StateEntry___updatingGSUnits()
{

}

void TestManagerStateMachine::Slot_StateExit___updatingGSUnits()
{

}

void TestManagerStateMachine::Slot_StateEntry___updatingWincomm()
{

}

void TestManagerStateMachine::Slot_StateExit___updatingWincomm()
{

}


    //////// State Machine: _updatingGSUnits ////////
void TestManagerStateMachine::Slot_StateEntry___updateInProgress()
{
    emit Action___startGSupdate();
}

void TestManagerStateMachine::Slot_StateExit___updateInProgress()
{

}

void TestManagerStateMachine::Slot_StateEntry___done_updatingGSUnits()
{

}

void TestManagerStateMachine::Slot_StateExit___done_updatingGSUnits()
{

}


    //////// State Machine: _updatingWincomm ////////
void TestManagerStateMachine::Slot_StateEntry___unzipInProgress()
{
    emit Action___startunzip();
}

void TestManagerStateMachine::Slot_StateExit___unzipInProgress()
{

}

void TestManagerStateMachine::Slot_StateEntry___done_updatingWincomm()
{

}

void TestManagerStateMachine::Slot_StateExit___done_updatingWincomm()
{

}



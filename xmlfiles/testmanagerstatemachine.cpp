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
    //////// State Machine: _testManagerStateMachine ////////
    _testManagerStateMachine(new QStateMachine(this)),
    _idle(new QState()),
    _downloadingAVC(new QState()),
    _runningTests(new QState()),
    _completed_testManagerStateMachine(new QState()),
    _updateFirmwareAllUnits(new QState(QState::ParallelStates)),
    _stateSequenceMachine(new QState()),

    //////// State Machine: _updateFirmwareAllUnits ////////
    _updatingGSUnits(new QState(_updateFirmwareAllUnits)),
    _updatingWincomm(new QState(_updateFirmwareAllUnits)),

    //////// State Machine: _updatingGSUnits ////////
    _updateInProgress(new QState(_updatingGSUnits)),
    _done_updatingGSUnits(new QFinalState(_updatingGSUnits)),

    //////// State Machine: _updatingWincomm ////////
    _unzipInProgress(new QState(_updatingWincomm)),
    _done_updatingWincomm(new QFinalState(_updatingWincomm)),

    //////// State Machine: _stateSequenceMachine ////////
    _checkingAllStateSequences(new QState(QState::ParallelStates, _stateSequenceMachine)),
    _failed(new QFinalState(_stateSequenceMachine)),
    _success(new QFinalState(_stateSequenceMachine)),

    //////// State Machine: _checkingAllStateSequences ////////
    _checkingCallSequence(new QState(_checkingAllStateSequences)),
    _checkingConnSequence(new QState(_checkingAllStateSequences)),
    _checkingSessionSeq(new QState(_checkingAllStateSequences)),

    //////// State Machine: _checkingCallSequence ////////
    _waiting_checkingCallSequence(new QState(_checkingCallSequence)),
    _completed_checkingCallSequence(new QFinalState(_checkingCallSequence)),

    //////// State Machine: _checkingConnSequence ////////
    _waiting_checkingConnSequence(new QState(_checkingConnSequence)),
    _completed_checkingConnSequence(new QFinalState(_checkingConnSequence)),

    //////// State Machine: _checkingSessionSeq ////////
    _waiting(new QState(_checkingSessionSeq)),
    _completed_checkingSessionSeq(new QFinalState(_checkingSessionSeq))

{
    //////// State Machine: _testManagerStateMachine ////////
    _testManagerStateMachine->addState(_idle);
    _testManagerStateMachine->setInitialState(_idle);
    _testManagerStateMachine->addState(_downloadingAVC);
    _testManagerStateMachine->addState(_runningTests);
    _testManagerStateMachine->addState(_completed_testManagerStateMachine);
    _testManagerStateMachine->addState(_updateFirmwareAllUnits);
    _testManagerStateMachine->addState(_stateSequenceMachine);

    //    Add transitions for the QStates using the transitions' private relay signals
    _idle->addTransition(this, SIGNAL(Relay_Event___start_idle()), _downloadingAVC);
    _downloadingAVC->addTransition(this, SIGNAL(Relay_Event___downloadFailed_downloadingAVC()), _completed_testManagerStateMachine);
    _downloadingAVC->addTransition(this, SIGNAL(Relay_Event___sHA1IsDifferent_downloadingAVC()), _updateFirmwareAllUnits);
    _downloadingAVC->addTransition(this, SIGNAL(Relay_Event___sHA1IsSame_downloadingAVC()), _stateSequenceMachine);
    _runningTests->addTransition(this, SIGNAL(Relay_Event___testsCompleted_runningTests()), _completed_testManagerStateMachine);
    _completed_testManagerStateMachine->addTransition(this, SIGNAL(Relay_Event___pollTimerPopped_completed_testManagerStateMachine()), _downloadingAVC);
    _updateFirmwareAllUnits->addTransition(this, SIGNAL(Relay_Event___updateFailure_updateFirmwareAllUnits()), _completed_testManagerStateMachine);
    _updateFirmwareAllUnits->addTransition(this, SIGNAL(Relay_Event___updateFirmwareAllUnitsFinished_updateFirmwareAllUnits()), _runningTests);
    _updateFirmwareAllUnits->addTransition(_updateFirmwareAllUnits, SIGNAL(finished()), _runningTests);
    _stateSequenceMachine->addTransition(this, SIGNAL(Relay_Event___event_stateSequenceMachine()), _completed_testManagerStateMachine);

    //    Propogate the private QState signals to public signals
    connect(_testManagerStateMachine, SIGNAL(started()), this, SIGNAL(Signal_StateReady___testManagerStateMachine()));
    connect(_idle, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___idle()));
    connect(_idle, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___idle()));
    connect(_downloadingAVC, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___downloadingAVC()));
    connect(_downloadingAVC, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___downloadingAVC()));
    connect(_runningTests, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___runningTests()));
    connect(_runningTests, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___runningTests()));
    connect(_completed_testManagerStateMachine, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___completed_testManagerStateMachine()));
    connect(_completed_testManagerStateMachine, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___completed_testManagerStateMachine()));
    connect(_updateFirmwareAllUnits, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___updateFirmwareAllUnits()));
    connect(_updateFirmwareAllUnits, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___updateFirmwareAllUnits()));
    connect(_stateSequenceMachine, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___stateSequenceMachine()));
    connect(_stateSequenceMachine, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___stateSequenceMachine()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_idle, SIGNAL(entered()), this, SLOT(Slot_StateEntry___idle()));
    connect(_idle, SIGNAL(exited()), this, SLOT(Slot_StateExit___idle()));
    connect(_downloadingAVC, SIGNAL(entered()), this, SLOT(Slot_StateEntry___downloadingAVC()));
    connect(_downloadingAVC, SIGNAL(exited()), this, SLOT(Slot_StateExit___downloadingAVC()));
    connect(_runningTests, SIGNAL(entered()), this, SLOT(Slot_StateEntry___runningTests()));
    connect(_runningTests, SIGNAL(exited()), this, SLOT(Slot_StateExit___runningTests()));
    connect(_completed_testManagerStateMachine, SIGNAL(entered()), this, SLOT(Slot_StateEntry___completed_testManagerStateMachine()));
    connect(_completed_testManagerStateMachine, SIGNAL(exited()), this, SLOT(Slot_StateExit___completed_testManagerStateMachine()));
    connect(_updateFirmwareAllUnits, SIGNAL(entered()), this, SLOT(Slot_StateEntry___updateFirmwareAllUnits()));
    connect(_updateFirmwareAllUnits, SIGNAL(exited()), this, SLOT(Slot_StateExit___updateFirmwareAllUnits()));
    connect(_stateSequenceMachine, SIGNAL(entered()), this, SLOT(Slot_StateEntry___stateSequenceMachine()));
    connect(_stateSequenceMachine, SIGNAL(exited()), this, SLOT(Slot_StateExit___stateSequenceMachine()));


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


    //////// State Machine: _stateSequenceMachine ////////
    _stateSequenceMachine->setInitialState(_checkingAllStateSequences);

    //    Add transitions for the QStates using the transitions' private relay signals
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event___checkingAllStatesCompleted_checkingAllStateSequences()), _success);
    _checkingAllStateSequences->addTransition(_checkingAllStateSequences, SIGNAL(finished()), _success);
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event___checkingCallSequenceFailed_checkingAllStateSequences()), _failed);
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event___checkingConnSequenceFailed_checkingAllStateSequences()), _failed);
    _checkingAllStateSequences->addTransition(this, SIGNAL(Relay_Event___checkingSessionSeqFailed_checkingAllStateSequences()), _failed);

    //    Propogate the private QState signals to public signals
    connect(_checkingAllStateSequences, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___checkingAllStateSequences()));
    connect(_checkingAllStateSequences, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___checkingAllStateSequences()));
    connect(_failed, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___failed()));
    connect(_failed, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___failed()));
    connect(_success, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___success()));
    connect(_success, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___success()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_checkingAllStateSequences, SIGNAL(entered()), this, SLOT(Slot_StateEntry___checkingAllStateSequences()));
    connect(_checkingAllStateSequences, SIGNAL(exited()), this, SLOT(Slot_StateExit___checkingAllStateSequences()));
    connect(_failed, SIGNAL(entered()), this, SLOT(Slot_StateEntry___failed()));
    connect(_failed, SIGNAL(exited()), this, SLOT(Slot_StateExit___failed()));
    connect(_success, SIGNAL(entered()), this, SLOT(Slot_StateEntry___success()));
    connect(_success, SIGNAL(exited()), this, SLOT(Slot_StateExit___success()));


    //////// State Machine: _checkingAllStateSequences ////////

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_checkingCallSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___checkingCallSequence()));
    connect(_checkingCallSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___checkingCallSequence()));
    connect(_checkingConnSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___checkingConnSequence()));
    connect(_checkingConnSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___checkingConnSequence()));
    connect(_checkingSessionSeq, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___checkingSessionSeq()));
    connect(_checkingSessionSeq, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___checkingSessionSeq()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_checkingCallSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___checkingCallSequence()));
    connect(_checkingCallSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___checkingCallSequence()));
    connect(_checkingConnSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___checkingConnSequence()));
    connect(_checkingConnSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___checkingConnSequence()));
    connect(_checkingSessionSeq, SIGNAL(entered()), this, SLOT(Slot_StateEntry___checkingSessionSeq()));
    connect(_checkingSessionSeq, SIGNAL(exited()), this, SLOT(Slot_StateExit___checkingSessionSeq()));


    //////// State Machine: _checkingCallSequence ////////
    _checkingCallSequence->setInitialState(_waiting_checkingCallSequence);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waiting_checkingCallSequence->addTransition(this, SIGNAL(Relay_Event___callSeqComplete_waiting_checkingCallSequence()), _completed_checkingCallSequence);

    //    Propogate the private QState signals to public signals
    connect(_waiting_checkingCallSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___waiting_checkingCallSequence()));
    connect(_waiting_checkingCallSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___waiting_checkingCallSequence()));
    connect(_completed_checkingCallSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___completed_checkingCallSequence()));
    connect(_completed_checkingCallSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___completed_checkingCallSequence()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting_checkingCallSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___waiting_checkingCallSequence()));
    connect(_waiting_checkingCallSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___waiting_checkingCallSequence()));
    connect(_completed_checkingCallSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___completed_checkingCallSequence()));
    connect(_completed_checkingCallSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___completed_checkingCallSequence()));


    //////// State Machine: _checkingConnSequence ////////
    _checkingConnSequence->setInitialState(_waiting_checkingConnSequence);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waiting_checkingConnSequence->addTransition(this, SIGNAL(Relay_Event___connSeqComplete_waiting_checkingConnSequence()), _completed_checkingConnSequence);

    //    Propogate the private QState signals to public signals
    connect(_waiting_checkingConnSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___waiting_checkingConnSequence()));
    connect(_waiting_checkingConnSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___waiting_checkingConnSequence()));
    connect(_completed_checkingConnSequence, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___completed_checkingConnSequence()));
    connect(_completed_checkingConnSequence, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___completed_checkingConnSequence()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting_checkingConnSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___waiting_checkingConnSequence()));
    connect(_waiting_checkingConnSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___waiting_checkingConnSequence()));
    connect(_completed_checkingConnSequence, SIGNAL(entered()), this, SLOT(Slot_StateEntry___completed_checkingConnSequence()));
    connect(_completed_checkingConnSequence, SIGNAL(exited()), this, SLOT(Slot_StateExit___completed_checkingConnSequence()));


    //////// State Machine: _checkingSessionSeq ////////
    _checkingSessionSeq->setInitialState(_waiting);

    //    Add transitions for the QStates using the transitions' private relay signals
    _waiting->addTransition(this, SIGNAL(Relay_Event___sessionSeqComplete_waiting()), _completed_checkingSessionSeq);

    //    Propogate the private QState signals to public signals
    connect(_waiting, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___waiting()));
    connect(_waiting, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___waiting()));
    connect(_completed_checkingSessionSeq, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___completed_checkingSessionSeq()));
    connect(_completed_checkingSessionSeq, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___completed_checkingSessionSeq()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_waiting, SIGNAL(entered()), this, SLOT(Slot_StateEntry___waiting()));
    connect(_waiting, SIGNAL(exited()), this, SLOT(Slot_StateExit___waiting()));
    connect(_completed_checkingSessionSeq, SIGNAL(entered()), this, SLOT(Slot_StateEntry___completed_checkingSessionSeq()));
    connect(_completed_checkingSessionSeq, SIGNAL(exited()), this, SLOT(Slot_StateExit___completed_checkingSessionSeq()));


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

void TestManagerStateMachine::Event___start_idle()
{
    emit Relay_Event___start_idle();
}

void TestManagerStateMachine::Event___downloadFailed_downloadingAVC()
{
    emit Relay_Event___downloadFailed_downloadingAVC();
}

void TestManagerStateMachine::Event___sHA1IsDifferent_downloadingAVC()
{
    emit Relay_Event___sHA1IsDifferent_downloadingAVC();
}

void TestManagerStateMachine::Event___sHA1IsSame_downloadingAVC()
{
    emit Relay_Event___sHA1IsSame_downloadingAVC();
}

void TestManagerStateMachine::Event___testsCompleted_runningTests()
{
    emit Relay_Event___testsCompleted_runningTests();
}

void TestManagerStateMachine::Event___pollTimerPopped_completed_testManagerStateMachine()
{
    emit Relay_Event___pollTimerPopped_completed_testManagerStateMachine();
}

void TestManagerStateMachine::Event___updateFailure_updateFirmwareAllUnits()
{
    emit Relay_Event___updateFailure_updateFirmwareAllUnits();
}

void TestManagerStateMachine::Event___updateFirmwareAllUnitsFinished_updateFirmwareAllUnits()
{
    emit Relay_Event___updateFirmwareAllUnitsFinished_updateFirmwareAllUnits();
}

void TestManagerStateMachine::Event___event_stateSequenceMachine()
{
    emit Relay_Event___event_stateSequenceMachine();
}


    //////// State Machine: _updateFirmwareAllUnits ////////

    //////// State Machine: _updatingGSUnits ////////
void TestManagerStateMachine::Event___gSUpdateSuccess_updateInProgress()
{
    emit Relay_Event___gSUpdateSuccess_updateInProgress();
}


    //////// State Machine: _updatingWincomm ////////
void TestManagerStateMachine::Event___upzipSuccess_unzipInProgress()
{
    emit Relay_Event___upzipSuccess_unzipInProgress();
}


    //////// State Machine: _stateSequenceMachine ////////
void TestManagerStateMachine::Event___checkingAllStatesCompleted_checkingAllStateSequences()
{
    emit Relay_Event___checkingAllStatesCompleted_checkingAllStateSequences();
}

void TestManagerStateMachine::Event___checkingCallSequenceFailed_checkingAllStateSequences()
{
    emit Relay_Event___checkingCallSequenceFailed_checkingAllStateSequences();
}

void TestManagerStateMachine::Event___checkingConnSequenceFailed_checkingAllStateSequences()
{
    emit Relay_Event___checkingConnSequenceFailed_checkingAllStateSequences();
}

void TestManagerStateMachine::Event___checkingSessionSeqFailed_checkingAllStateSequences()
{
    emit Relay_Event___checkingSessionSeqFailed_checkingAllStateSequences();
}


    //////// State Machine: _checkingAllStateSequences ////////

    //////// State Machine: _checkingCallSequence ////////
void TestManagerStateMachine::Event___callSeqComplete_waiting_checkingCallSequence()
{
    emit Relay_Event___callSeqComplete_waiting_checkingCallSequence();
}


    //////// State Machine: _checkingConnSequence ////////
void TestManagerStateMachine::Event___connSeqComplete_waiting_checkingConnSequence()
{
    emit Relay_Event___connSeqComplete_waiting_checkingConnSequence();
}


    //////// State Machine: _checkingSessionSeq ////////
void TestManagerStateMachine::Event___sessionSeqComplete_waiting()
{
    emit Relay_Event___sessionSeqComplete_waiting();
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
    emit EntryAction___startdownload();
}

void TestManagerStateMachine::Slot_StateExit___downloadingAVC()
{

}

void TestManagerStateMachine::Slot_StateEntry___runningTests()
{
    emit EntryAction___starttests();
}

void TestManagerStateMachine::Slot_StateExit___runningTests()
{

}

void TestManagerStateMachine::Slot_StateEntry___completed_testManagerStateMachine()
{

}

void TestManagerStateMachine::Slot_StateExit___completed_testManagerStateMachine()
{

}

void TestManagerStateMachine::Slot_StateEntry___updateFirmwareAllUnits()
{

}

void TestManagerStateMachine::Slot_StateExit___updateFirmwareAllUnits()
{

}

void TestManagerStateMachine::Slot_StateEntry___stateSequenceMachine()
{

}

void TestManagerStateMachine::Slot_StateExit___stateSequenceMachine()
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
    emit EntryAction___startGSupdate();
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
    emit EntryAction___startunzip();
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


    //////// State Machine: _stateSequenceMachine ////////
void TestManagerStateMachine::Slot_StateEntry___checkingAllStateSequences()
{

}

void TestManagerStateMachine::Slot_StateExit___checkingAllStateSequences()
{

}

void TestManagerStateMachine::Slot_StateEntry___failed()
{

}

void TestManagerStateMachine::Slot_StateExit___failed()
{

}

void TestManagerStateMachine::Slot_StateEntry___success()
{

}

void TestManagerStateMachine::Slot_StateExit___success()
{

}


    //////// State Machine: _checkingAllStateSequences ////////
void TestManagerStateMachine::Slot_StateEntry___checkingCallSequence()
{

}

void TestManagerStateMachine::Slot_StateExit___checkingCallSequence()
{

}

void TestManagerStateMachine::Slot_StateEntry___checkingConnSequence()
{

}

void TestManagerStateMachine::Slot_StateExit___checkingConnSequence()
{

}

void TestManagerStateMachine::Slot_StateEntry___checkingSessionSeq()
{

}

void TestManagerStateMachine::Slot_StateExit___checkingSessionSeq()
{

}


    //////// State Machine: _checkingCallSequence ////////
void TestManagerStateMachine::Slot_StateEntry___waiting_checkingCallSequence()
{

}

void TestManagerStateMachine::Slot_StateExit___waiting_checkingCallSequence()
{

}

void TestManagerStateMachine::Slot_StateEntry___completed_checkingCallSequence()
{

}

void TestManagerStateMachine::Slot_StateExit___completed_checkingCallSequence()
{

}


    //////// State Machine: _checkingConnSequence ////////
void TestManagerStateMachine::Slot_StateEntry___waiting_checkingConnSequence()
{

}

void TestManagerStateMachine::Slot_StateExit___waiting_checkingConnSequence()
{

}

void TestManagerStateMachine::Slot_StateEntry___completed_checkingConnSequence()
{

}

void TestManagerStateMachine::Slot_StateExit___completed_checkingConnSequence()
{

}


    //////// State Machine: _checkingSessionSeq ////////
void TestManagerStateMachine::Slot_StateEntry___waiting()
{

}

void TestManagerStateMachine::Slot_StateExit___waiting()
{

}

void TestManagerStateMachine::Slot_StateEntry___completed_checkingSessionSeq()
{

}

void TestManagerStateMachine::Slot_StateExit___completed_checkingSessionSeq()
{

}



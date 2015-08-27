// This header file was generated using VisualSC's Export Code Function

// Visual Statechart is an open source project. Visit us at https://github.com/daviddrell/visualsc
#ifndef TESTMANAGERSTATEMACHINE_H
#define TESTMANAGERSTATEMACHINE_H

#include <QObject>
#include <QStateMachine>
#include <QFinalState>

class TestManagerStateMachine : public QObject
{
    Q_OBJECT

    //////////////////
    //
    //    Public interface
    //
    //    Connect to public slots to drive events into
    //    the state machine
    //
    //    Connect to the public signals to do work
    //    and monitor state changes
    //
    //////////////////

public:
    TestManagerStateMachine(QObject*parent=NULL);
    ~TestManagerStateMachine();

    //
    //    INPUT EVENT SLOTS
    //
    //    Call these slots to signal event inputs to the machine
    //


public slots:
    //////// State Machine: _testManagerStateMachine ////////
    void Event_startMachine___testManagerStateMachine();
    void Event___start_idle();
    void Event___downloadFailed_downloadingAVC();
    void Event___sHA1IsDifferent_downloadingAVC();
    void Event___sHA1IsSame_downloadingAVC();
    void Event___testsCompleted_runningTests();
    void Event___pollTimerPopped_completed_testManagerStateMachine();
    void Event___updateFailure_updateFirmwareAllUnits();
    void Event___updateFirmwareAllUnitsFinished_updateFirmwareAllUnits();
    void Event___event_stateSequenceMachine();

    //////// State Machine: _updateFirmwareAllUnits ////////

    //////// State Machine: _updatingGSUnits ////////
    void Event___gSUpdateSuccess_updateInProgress();

    //////// State Machine: _updatingWincomm ////////
    void Event___upzipSuccess_unzipInProgress();

    //////// State Machine: _stateSequenceMachine ////////
    void Event___checkingAllStatesCompleted_checkingAllStateSequences();
    void Event___checkingCallSequenceFailed_checkingAllStateSequences();
    void Event___checkingConnSequenceFailed_checkingAllStateSequences();
    void Event___checkingSessionSeqFailed_checkingAllStateSequences();

    //////// State Machine: _checkingAllStateSequences ////////

    //////// State Machine: _checkingCallSequence ////////
    void Event___callSeqComplete_waiting_checkingCallSequence();

    //////// State Machine: _checkingConnSequence ////////
    void Event___connSeqComplete_waiting_checkingConnSequence();

    //////// State Machine: _checkingSessionSeq ////////
    void Event___sessionSeqComplete_waiting();


signals:
    //
    //    OUTPUT ACTION SIGNALS
    //
    //    Connect to these signals to a slot corresponding to a entryAction or exitAction
    //
    //////// State Machine: _testManagerStateMachine ////////
    void EntryAction___startdownload();
    void EntryAction___starttests();

    //////// State Machine: _updateFirmwareAllUnits ////////

    //////// State Machine: _updatingGSUnits ////////
    void EntryAction___startGSupdate();

    //////// State Machine: _updatingWincomm ////////
    void EntryAction___startunzip();

    //////// State Machine: _stateSequenceMachine ////////

    //////// State Machine: _checkingAllStateSequences ////////

    //////// State Machine: _checkingCallSequence ////////

    //////// State Machine: _checkingConnSequence ////////

    //////// State Machine: _checkingSessionSeq ////////


    //
    //    STATE CHANGE MONITOR SIGNALS
    //
    //    Connect to these signals to monitor state transitions
    //
    //////// State Machine: _testManagerStateMachine ////////
    void Signal_StateReady___testManagerStateMachine();
    void Signal_StateEntry___idle();
    void Signal_StateExit___idle();
    void Signal_StateEntry___downloadingAVC();
    void Signal_StateExit___downloadingAVC();
    void Signal_StateEntry___runningTests();
    void Signal_StateExit___runningTests();
    void Signal_StateEntry___completed_testManagerStateMachine();
    void Signal_StateExit___completed_testManagerStateMachine();
    void Signal_StateEntry___updateFirmwareAllUnits();
    void Signal_StateExit___updateFirmwareAllUnits();
    void Signal_StateEntry___stateSequenceMachine();
    void Signal_StateExit___stateSequenceMachine();

    //////// State Machine: _updateFirmwareAllUnits ////////
    void Signal_StateEntry___updatingGSUnits();
    void Signal_StateExit___updatingGSUnits();
    void Signal_StateEntry___updatingWincomm();
    void Signal_StateExit___updatingWincomm();

    //////// State Machine: _updatingGSUnits ////////
    void Signal_StateEntry___updateInProgress();
    void Signal_StateExit___updateInProgress();
    void Signal_StateEntry___done_updatingGSUnits();
    void Signal_StateExit___done_updatingGSUnits();

    //////// State Machine: _updatingWincomm ////////
    void Signal_StateEntry___unzipInProgress();
    void Signal_StateExit___unzipInProgress();
    void Signal_StateEntry___done_updatingWincomm();
    void Signal_StateExit___done_updatingWincomm();

    //////// State Machine: _stateSequenceMachine ////////
    void Signal_StateEntry___checkingAllStateSequences();
    void Signal_StateExit___checkingAllStateSequences();
    void Signal_StateEntry___failed();
    void Signal_StateExit___failed();
    void Signal_StateEntry___success();
    void Signal_StateExit___success();

    //////// State Machine: _checkingAllStateSequences ////////
    void Signal_StateEntry___checkingCallSequence();
    void Signal_StateExit___checkingCallSequence();
    void Signal_StateEntry___checkingConnSequence();
    void Signal_StateExit___checkingConnSequence();
    void Signal_StateEntry___checkingSessionSeq();
    void Signal_StateExit___checkingSessionSeq();

    //////// State Machine: _checkingCallSequence ////////
    void Signal_StateEntry___waiting_checkingCallSequence();
    void Signal_StateExit___waiting_checkingCallSequence();
    void Signal_StateEntry___completed_checkingCallSequence();
    void Signal_StateExit___completed_checkingCallSequence();

    //////// State Machine: _checkingConnSequence ////////
    void Signal_StateEntry___waiting_checkingConnSequence();
    void Signal_StateExit___waiting_checkingConnSequence();
    void Signal_StateEntry___completed_checkingConnSequence();
    void Signal_StateExit___completed_checkingConnSequence();

    //////// State Machine: _checkingSessionSeq ////////
    void Signal_StateEntry___waiting();
    void Signal_StateExit___waiting();
    void Signal_StateEntry___completed_checkingSessionSeq();
    void Signal_StateExit___completed_checkingSessionSeq();


    //////////////////
    //
    //    Private below here
    //
    //////////////////


    /*
     *  A consequence of OOB encapsulation is that this class does know about its users, and so cannot listen
     * to their signals. Therefore this class exposes public slots that users can call. These slots
     * are then translated into private signals that the private state machine can listen to.
     *
     * For outbound signals, I choose to keep the statemachine objects private so that means I need
     * to translate private state machine signals into public signals.
     *
     * This all leads to a lot of confusion and possible name collisions. Therefore its better for this
     * file to be autogenerated from the visual state chart editor.
     *
     */


private:


private slots:
    // The Entry/Exit Slots that belong to QStates
    //////// State Machine: _testManagerStateMachine ////////
    void Slot_StateEntry___idle();
    void Slot_StateExit___idle();
    void Slot_StateEntry___downloadingAVC();
    void Slot_StateExit___downloadingAVC();
    void Slot_StateEntry___runningTests();
    void Slot_StateExit___runningTests();
    void Slot_StateEntry___completed_testManagerStateMachine();
    void Slot_StateExit___completed_testManagerStateMachine();
    void Slot_StateEntry___updateFirmwareAllUnits();
    void Slot_StateExit___updateFirmwareAllUnits();
    void Slot_StateEntry___stateSequenceMachine();
    void Slot_StateExit___stateSequenceMachine();

    //////// State Machine: _updateFirmwareAllUnits ////////
    void Slot_StateEntry___updatingGSUnits();
    void Slot_StateExit___updatingGSUnits();
    void Slot_StateEntry___updatingWincomm();
    void Slot_StateExit___updatingWincomm();

    //////// State Machine: _updatingGSUnits ////////
    void Slot_StateEntry___updateInProgress();
    void Slot_StateExit___updateInProgress();
    void Slot_StateEntry___done_updatingGSUnits();
    void Slot_StateExit___done_updatingGSUnits();

    //////// State Machine: _updatingWincomm ////////
    void Slot_StateEntry___unzipInProgress();
    void Slot_StateExit___unzipInProgress();
    void Slot_StateEntry___done_updatingWincomm();
    void Slot_StateExit___done_updatingWincomm();

    //////// State Machine: _stateSequenceMachine ////////
    void Slot_StateEntry___checkingAllStateSequences();
    void Slot_StateExit___checkingAllStateSequences();
    void Slot_StateEntry___failed();
    void Slot_StateExit___failed();
    void Slot_StateEntry___success();
    void Slot_StateExit___success();

    //////// State Machine: _checkingAllStateSequences ////////
    void Slot_StateEntry___checkingCallSequence();
    void Slot_StateExit___checkingCallSequence();
    void Slot_StateEntry___checkingConnSequence();
    void Slot_StateExit___checkingConnSequence();
    void Slot_StateEntry___checkingSessionSeq();
    void Slot_StateExit___checkingSessionSeq();

    //////// State Machine: _checkingCallSequence ////////
    void Slot_StateEntry___waiting_checkingCallSequence();
    void Slot_StateExit___waiting_checkingCallSequence();
    void Slot_StateEntry___completed_checkingCallSequence();
    void Slot_StateExit___completed_checkingCallSequence();

    //////// State Machine: _checkingConnSequence ////////
    void Slot_StateEntry___waiting_checkingConnSequence();
    void Slot_StateExit___waiting_checkingConnSequence();
    void Slot_StateEntry___completed_checkingConnSequence();
    void Slot_StateExit___completed_checkingConnSequence();

    //////// State Machine: _checkingSessionSeq ////////
    void Slot_StateEntry___waiting();
    void Slot_StateExit___waiting();
    void Slot_StateEntry___completed_checkingSessionSeq();
    void Slot_StateExit___completed_checkingSessionSeq();


signals:
    // A Transition/Event slot's corresponding signal emitted in the slot
    //////// State Machine: _testManagerStateMachine ////////
    void Relay_Event___start_idle();
    void Relay_Event___downloadFailed_downloadingAVC();
    void Relay_Event___sHA1IsDifferent_downloadingAVC();
    void Relay_Event___sHA1IsSame_downloadingAVC();
    void Relay_Event___testsCompleted_runningTests();
    void Relay_Event___pollTimerPopped_completed_testManagerStateMachine();
    void Relay_Event___updateFailure_updateFirmwareAllUnits();
    void Relay_Event___updateFirmwareAllUnitsFinished_updateFirmwareAllUnits();
    void Relay_Event___event_stateSequenceMachine();

    //////// State Machine: _updateFirmwareAllUnits ////////

    //////// State Machine: _updatingGSUnits ////////
    void Relay_Event___gSUpdateSuccess_updateInProgress();

    //////// State Machine: _updatingWincomm ////////
    void Relay_Event___upzipSuccess_unzipInProgress();

    //////// State Machine: _stateSequenceMachine ////////
    void Relay_Event___checkingAllStatesCompleted_checkingAllStateSequences();
    void Relay_Event___checkingCallSequenceFailed_checkingAllStateSequences();
    void Relay_Event___checkingConnSequenceFailed_checkingAllStateSequences();
    void Relay_Event___checkingSessionSeqFailed_checkingAllStateSequences();

    //////// State Machine: _checkingAllStateSequences ////////

    //////// State Machine: _checkingCallSequence ////////
    void Relay_Event___callSeqComplete_waiting_checkingCallSequence();

    //////// State Machine: _checkingConnSequence ////////
    void Relay_Event___connSeqComplete_waiting_checkingConnSequence();

    //////// State Machine: _checkingSessionSeq ////////
    void Relay_Event___sessionSeqComplete_waiting();


private:
    //////// State Machine: _testManagerStateMachine ////////
    QStateMachine*    _testManagerStateMachine;
    QState*    _idle;
    QState*    _downloadingAVC;    // incredibel sfajlsdfasdf
    QState*    _runningTests;
    QState*    _completed_testManagerStateMachine;
    QState*    _updateFirmwareAllUnits;
    QState*    _stateSequenceMachine;

    //////// State Machine: _updateFirmwareAllUnits ////////
    // child QState declared elsewhere _updateFirmwareAllUnits
    QState*    _updatingGSUnits;
    QState*    _updatingWincomm;

    //////// State Machine: _updatingGSUnits ////////
    // child QState declared elsewhere _updatingGSUnits
    QState*    _updateInProgress;
    QFinalState*    _done_updatingGSUnits;    // wowwoowowo

    //////// State Machine: _updatingWincomm ////////
    // child QState declared elsewhere _updatingWincomm
    QState*    _unzipInProgress;
    QFinalState*    _done_updatingWincomm;

    //////// State Machine: _stateSequenceMachine ////////
    // child QState declared elsewhere _stateSequenceMachine
    QState*    _checkingAllStateSequences;
    QFinalState*    _failed;
    QFinalState*    _success;

    //////// State Machine: _checkingAllStateSequences ////////
    // child QState declared elsewhere _checkingAllStateSequences
    QState*    _checkingCallSequence;
    QState*    _checkingConnSequence;
    QState*    _checkingSessionSeq;

    //////// State Machine: _checkingCallSequence ////////
    // child QState declared elsewhere _checkingCallSequence
    QState*    _waiting_checkingCallSequence;
    QFinalState*    _completed_checkingCallSequence;

    //////// State Machine: _checkingConnSequence ////////
    // child QState declared elsewhere _checkingConnSequence
    QState*    _waiting_checkingConnSequence;
    QFinalState*    _completed_checkingConnSequence;

    //////// State Machine: _checkingSessionSeq ////////
    // child QState declared elsewhere _checkingSessionSeq
    QState*    _waiting;
    QFinalState*    _completed_checkingSessionSeq;


};

#endif // TESTMANAGERSTATEMACHINE_H

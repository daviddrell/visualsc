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
    //////// State Machine: _testManagerStateMachine_97b850af ////////
    void Event_startMachine_testManagerStateMachine_97b850af();
    void Event_start_fa1bce49();
    void Event_downloadFailed_850da190();
    void Event_sHA1IsSame_850da190();
    void Event_sHA1IsDifferent_53416b9a();
    void Event_testsCompleted_850da190();
    void Event_pollTimerPopped_fa1bce49();
    void Event_updateFailure_850da190();
    void Event_updateFirmwareAllUnitsFinished_c9e4b34a();

    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////

    //////// State Machine: _updatingGSUnits_52c831de ////////
    void Event_gSUpdateSuccess_48c3bcdd();

    //////// State Machine: _updatingWincomm_7778a7d0 ////////
    void Event_upzipSuccess_f93692c9();


signals:
    //
    //    OUTPUT ACTION SIGNALS
    //
    //    Connect to these signals to a slot corresponding to a entryAction or exitAction
    //
    //////// State Machine: _testManagerStateMachine_97b850af ////////
    void EntryAction_startDownload();
    void EntryAction_doADance();
    void EntryAction_jump();
    void EntryAction_slide();
    void EntryAction_okay();
    void ExitAction_wow();
    void ExitAction_no();
    void ExitAction_suchWow();
    void ExitAction_amazing();
    void EntryAction_startTests();

    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////

    //////// State Machine: _updatingGSUnits_52c831de ////////
    void EntryAction_startGSUpdate();

    //////// State Machine: _updatingWincomm_7778a7d0 ////////
    void EntryAction_startUnzip();


    //
    //    STATE CHANGE MONITOR SIGNALS
    //
    //    Connect to these signals to monitor state transitions
    //
    //////// State Machine: _testManagerStateMachine_97b850af ////////
    void Signal_StateReady_testManagerStateMachine_97b850af();
    void Signal_StateEntry_idle_c9dcfd20();
    void Signal_StateExit_idle_c9dcfd20();
    void Signal_StateEntry_downloadingAVC_fa1bce49();
    void Signal_StateExit_downloadingAVC_fa1bce49();
    void Signal_StateEntry_runningTests_c9e4b34a();
    void Signal_StateExit_runningTests_c9e4b34a();
    void Signal_StateEntry_completed_850da190();
    void Signal_StateExit_completed_850da190();
    void Signal_StateEntry_updateFirmwareAllUnits_53416b9a();
    void Signal_StateExit_updateFirmwareAllUnits_53416b9a();

    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////
    void Signal_StateReady_updateFirmwareAllUnits_53416b9a();
    void Signal_StateEntry_updatingGSUnits_52c831de();
    void Signal_StateExit_updatingGSUnits_52c831de();
    void Signal_StateEntry_updatingWincomm_7778a7d0();
    void Signal_StateExit_updatingWincomm_7778a7d0();

    //////// State Machine: _updatingGSUnits_52c831de ////////
    void Signal_StateReady_updatingGSUnits_52c831de();
    void Signal_StateEntry_updateInProgress_6d5ca4f3();
    void Signal_StateExit_updateInProgress_6d5ca4f3();
    void Signal_StateEntry_done_48c3bcdd();
    void Signal_StateExit_done_48c3bcdd();

    //////// State Machine: _updatingWincomm_7778a7d0 ////////
    void Signal_StateReady_updatingWincomm_7778a7d0();
    void Signal_StateEntry_unzipInProgress_9f1f6d0f();
    void Signal_StateExit_unzipInProgress_9f1f6d0f();
    void Signal_StateEntry_done_f93692c9();
    void Signal_StateExit_done_f93692c9();


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
    //////// State Machine: _testManagerStateMachine_97b850af ////////
    void Slot_StateEntry_idle_c9dcfd20();
    void Slot_StateExit_idle_c9dcfd20();
    void Slot_StateEntry_downloadingAVC_fa1bce49();
    void Slot_StateExit_downloadingAVC_fa1bce49();
    void Slot_StateEntry_runningTests_c9e4b34a();
    void Slot_StateExit_runningTests_c9e4b34a();
    void Slot_StateEntry_completed_850da190();
    void Slot_StateExit_completed_850da190();
    void Slot_StateEntry_updateFirmwareAllUnits_53416b9a();
    void Slot_StateExit_updateFirmwareAllUnits_53416b9a();

    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////
    void Slot_StateEntry_updatingGSUnits_52c831de();
    void Slot_StateExit_updatingGSUnits_52c831de();
    void Slot_StateEntry_updatingWincomm_7778a7d0();
    void Slot_StateExit_updatingWincomm_7778a7d0();

    //////// State Machine: _updatingGSUnits_52c831de ////////
    void Slot_StateEntry_updateInProgress_6d5ca4f3();
    void Slot_StateExit_updateInProgress_6d5ca4f3();
    void Slot_StateEntry_done_48c3bcdd();
    void Slot_StateExit_done_48c3bcdd();

    //////// State Machine: _updatingWincomm_7778a7d0 ////////
    void Slot_StateEntry_unzipInProgress_9f1f6d0f();
    void Slot_StateExit_unzipInProgress_9f1f6d0f();
    void Slot_StateEntry_done_f93692c9();
    void Slot_StateExit_done_f93692c9();


signals:
    // A Transition/Event slot's corresponding signal emitted in the slot
    //////// State Machine: _testManagerStateMachine_97b850af ////////
    void Relay_Event_start_fa1bce49();
    void Relay_Event_downloadFailed_850da190();
    void Relay_Event_sHA1IsSame_850da190();
    void Relay_Event_sHA1IsDifferent_53416b9a();
    void Relay_Event_testsCompleted_850da190();
    void Relay_Event_pollTimerPopped_fa1bce49();
    void Relay_Event_updateFailure_850da190();
    void Relay_Event_updateFirmwareAllUnitsFinished_c9e4b34a();

    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////

    //////// State Machine: _updatingGSUnits_52c831de ////////
    void Relay_Event_gSUpdateSuccess_48c3bcdd();

    //////// State Machine: _updatingWincomm_7778a7d0 ////////
    void Relay_Event_upzipSuccess_f93692c9();


private:
    //////// State Machine: _testManagerStateMachine_97b850af ////////
    QStateMachine*    _testManagerStateMachine_97b850af;
    QState*    _idle_c9dcfd20;
    QState*    _downloadingAVC_fa1bce49;
    QState*    _runningTests_c9e4b34a;
    QState*    _completed_850da190;
    QState*    _updateFirmwareAllUnits_53416b9a;

    //////// State Machine: _updateFirmwareAllUnits_53416b9a ////////
    // child QState declared elsewhere _updateFirmwareAllUnits_53416b9a
    QState*    _updatingGSUnits_52c831de;
    QState*    _updatingWincomm_7778a7d0;

    //////// State Machine: _updatingGSUnits_52c831de ////////
    // child QState declared elsewhere _updatingGSUnits_52c831de
    QState*    _updateInProgress_6d5ca4f3;
    QFinalState*    _done_48c3bcdd;

    //////// State Machine: _updatingWincomm_7778a7d0 ////////
    // child QState declared elsewhere _updatingWincomm_7778a7d0
    QState*    _unzipInProgress_9f1f6d0f;
    QFinalState*    _done_f93692c9;


};

#endif // TESTMANAGERSTATEMACHINE_H

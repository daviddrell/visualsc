// This header file was generated using VisualSC's Export Code Function

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
    void Event_startMachine_testManagerStateMachine();
    void Event_start();
    void Event_sHA1IsDifferent();
    void Event_downloadFailed();
    void Event_sHA1IsSame();
    void Event_unzipSuccess();
    void Event_unzipFail();
    void Event_testsCompleted();
    void Event_pollTimerPopped();

signals:
    //
    //    OUTPUT ACTION SIGNALS
    //
    //    Connect to these signals to a slot corresponding to a entryAction or exitAction
    //
    void EntryAction_startDownload();
    void EntryAction_startUnzip();
    void EntryAction_startTests();

    //
    //    STATE CHANGE MONITOR SIGNALS
    //
    //    Connect to these signals to monitor state transitions
    //
    void Signal_StateReady_testManagerStateMachine();
    void Signal_StateEntry_idle();
    void Signal_StateExit_idle();
    void Signal_StateEntry_downloadingAVC();
    void Signal_StateExit_downloadingAVC();
    void Signal_StateEntry_unzipAVC();
    void Signal_StateExit_unzipAVC();
    void Signal_StateEntry_runningTests();
    void Signal_StateExit_runningTests();
    void Signal_StateEntry_completed();
    void Signal_StateExit_completed();

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
    void Slot_StateEntry_idle();
    void Slot_StateExit_idle();
    void Slot_StateEntry_downloadingAVC();
    void Slot_StateExit_downloadingAVC();
    void Slot_StateEntry_unzipAVC();
    void Slot_StateExit_unzipAVC();
    void Slot_StateEntry_runningTests();
    void Slot_StateExit_runningTests();
    void Slot_StateEntry_completed();
    void Slot_StateExit_completed();

signals:
    // A Transiton/Event slot's corresponding signal emitted in the slot
    void Relay_Event_start();
    void Relay_Event_sHA1IsDifferent();
    void Relay_Event_downloadFailed();
    void Relay_Event_sHA1IsSame();
    void Relay_Event_unzipSuccess();
    void Relay_Event_unzipFail();
    void Relay_Event_testsCompleted();
    void Relay_Event_pollTimerPopped();

private:
    QStateMachine*    _testManagerStateMachine;
    QState*    _idle;
    QState*    _downloadingAVC;
    QState*    _unzipAVC;
    QState*    _runningTests;
    QState*    _completed;

};

#endif // TESTMANAGERSTATEMACHINE_H
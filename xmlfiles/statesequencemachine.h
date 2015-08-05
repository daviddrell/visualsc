// This header file was generated using VisualSC's Export Code Function

#ifndef STATESEQUENCEMACHINE_H
#define STATESEQUENCEMACHINE_H

#include <QObject>
#include <QStateMachine>
#include <QFinalState>

class StateSequenceMachine : public QObject
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
    StateSequenceMachine(QObject*parent=NULL);
    ~StateSequenceMachine();

    //
    //    INPUT EVENT SLOTS
    //
    //    Call these slots to signal event inputs to the machine
    //


public slots:
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////
    void Event_startMachine_checkingAllStateSequences_d74d8a42();

    //////// State Machine: _checkingCallSequence_78faf58e ////////
    void Event_startMachine_checkingCallSequence_78faf58e();
    void Event_callSeqComplete_0b1b5af8();

    //////// State Machine: _checkingConnSequence_926f34d4 ////////
    void Event_startMachine_checkingConnSequence_926f34d4();
    void Event_connSeqComplete_57b5d5f6();

    //////// State Machine: _checkingSessionSeq_9f53745d ////////
    void Event_startMachine_checkingSessionSeq_9f53745d();
    void Event_sessionSeqComplete_3967534e();

    //////// State Machine: _stateSequenceMachine_ba72ef13 ////////
    void Event_startMachine_stateSequenceMachine_ba72ef13();
    void Event_checkingAllStatesCompleted_914678f0();
    void Event_checkingCallSequenceFailed_51ea86cc();
    void Event_checkingConnSequenceFailed_51ea86cc();
    void Event_checkingSessionSeqFailed_51ea86cc();


signals:
    //
    //    OUTPUT ACTION SIGNALS
    //
    //    Connect to these signals to a slot corresponding to a entryAction or exitAction
    //
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////

    //////// State Machine: _checkingCallSequence_78faf58e ////////

    //////// State Machine: _checkingConnSequence_926f34d4 ////////

    //////// State Machine: _checkingSessionSeq_9f53745d ////////

    //////// State Machine: _stateSequenceMachine_ba72ef13 ////////


    //
    //    STATE CHANGE MONITOR SIGNALS
    //
    //    Connect to these signals to monitor state transitions
    //
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////
    void Signal_StateReady_checkingAllStateSequences_d74d8a42();
    void Signal_StateEntry_checkingCallSequence_78faf58e();
    void Signal_StateExit_checkingCallSequence_78faf58e();
    void Signal_StateEntry_checkingConnSequence_926f34d4();
    void Signal_StateExit_checkingConnSequence_926f34d4();
    void Signal_StateEntry_checkingSessionSeq_9f53745d();
    void Signal_StateExit_checkingSessionSeq_9f53745d();

    //////// State Machine: _checkingCallSequence_78faf58e ////////
    void Signal_StateReady_checkingCallSequence_78faf58e();
    void Signal_StateEntry_waiting_246b7dcb();
    void Signal_StateExit_waiting_246b7dcb();
    void Signal_StateEntry_completed_0b1b5af8();
    void Signal_StateExit_completed_0b1b5af8();

    //////// State Machine: _checkingConnSequence_926f34d4 ////////
    void Signal_StateReady_checkingConnSequence_926f34d4();
    void Signal_StateEntry_waiting_c3267d7e();
    void Signal_StateExit_waiting_c3267d7e();
    void Signal_StateEntry_completed_57b5d5f6();
    void Signal_StateExit_completed_57b5d5f6();

    //////// State Machine: _checkingSessionSeq_9f53745d ////////
    void Signal_StateReady_checkingSessionSeq_9f53745d();
    void Signal_StateEntry_waiting_ff5a583f();
    void Signal_StateExit_waiting_ff5a583f();
    void Signal_StateEntry_completed_3967534e();
    void Signal_StateExit_completed_3967534e();

    //////// State Machine: _stateSequenceMachine_ba72ef13 ////////
    void Signal_StateReady_stateSequenceMachine_ba72ef13();
    void Signal_StateEntry_checkingAllStateSequences_d74d8a42();
    void Signal_StateExit_checkingAllStateSequences_d74d8a42();
    void Signal_StateEntry_failed_51ea86cc();
    void Signal_StateExit_failed_51ea86cc();
    void Signal_StateEntry_success_914678f0();
    void Signal_StateExit_success_914678f0();


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
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////
    void Slot_StateEntry_checkingCallSequence_78faf58e();
    void Slot_StateExit_checkingCallSequence_78faf58e();
    void Slot_StateEntry_checkingConnSequence_926f34d4();
    void Slot_StateExit_checkingConnSequence_926f34d4();
    void Slot_StateEntry_checkingSessionSeq_9f53745d();
    void Slot_StateExit_checkingSessionSeq_9f53745d();

    //////// State Machine: _checkingCallSequence_78faf58e ////////
    void Slot_StateEntry_waiting_246b7dcb();
    void Slot_StateExit_waiting_246b7dcb();
    void Slot_StateEntry_completed_0b1b5af8();
    void Slot_StateExit_completed_0b1b5af8();

    //////// State Machine: _checkingConnSequence_926f34d4 ////////
    void Slot_StateEntry_waiting_c3267d7e();
    void Slot_StateExit_waiting_c3267d7e();
    void Slot_StateEntry_completed_57b5d5f6();
    void Slot_StateExit_completed_57b5d5f6();

    //////// State Machine: _checkingSessionSeq_9f53745d ////////
    void Slot_StateEntry_waiting_ff5a583f();
    void Slot_StateExit_waiting_ff5a583f();
    void Slot_StateEntry_completed_3967534e();
    void Slot_StateExit_completed_3967534e();

    //////// State Machine: _stateSequenceMachine_ba72ef13 ////////
    void Slot_StateEntry_checkingAllStateSequences_d74d8a42();
    void Slot_StateExit_checkingAllStateSequences_d74d8a42();
    void Slot_StateEntry_failed_51ea86cc();
    void Slot_StateExit_failed_51ea86cc();
    void Slot_StateEntry_success_914678f0();
    void Slot_StateExit_success_914678f0();


signals:
    // A Transition/Event slot's corresponding signal emitted in the slot
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////

    //////// State Machine: _checkingCallSequence_78faf58e ////////
    void Relay_Event_callSeqComplete_0b1b5af8();

    //////// State Machine: _checkingConnSequence_926f34d4 ////////
    void Relay_Event_connSeqComplete_57b5d5f6();

    //////// State Machine: _checkingSessionSeq_9f53745d ////////
    void Relay_Event_sessionSeqComplete_3967534e();

    //////// State Machine: _stateSequenceMachine_ba72ef13 ////////
    void Relay_Event_checkingAllStatesCompleted_914678f0();
    void Relay_Event_checkingCallSequenceFailed_51ea86cc();
    void Relay_Event_checkingConnSequenceFailed_51ea86cc();
    void Relay_Event_checkingSessionSeqFailed_51ea86cc();


private:
    //////// State Machine: _checkingAllStateSequences_d74d8a42 ////////
    QStateMachine*    _checkingAllStateSequences_d74d8a42;
    QState*    _checkingCallSequence_78faf58e;
    QState*    _checkingConnSequence_926f34d4;
    QState*    _checkingSessionSeq_9f53745d;

    //////// State Machine: _checkingCallSequence_78faf58e ////////
    QStateMachine*    _checkingCallSequence_78faf58e;
    QState*    _waiting_246b7dcb;
    QFinalState*    _completed_0b1b5af8;

    //////// State Machine: _checkingConnSequence_926f34d4 ////////
    QStateMachine*    _checkingConnSequence_926f34d4;
    QState*    _waiting_c3267d7e;
    QFinalState*    _completed_57b5d5f6;

    //////// State Machine: _checkingSessionSeq_9f53745d ////////
    QStateMachine*    _checkingSessionSeq_9f53745d;
    QState*    _waiting_ff5a583f;
    QFinalState*    _completed_3967534e;

    //////// State Machine: _stateSequenceMachine_ba72ef13 ////////
    QStateMachine*    _stateSequenceMachine_ba72ef13;
    QState*    _checkingAllStateSequences_d74d8a42;
    QFinalState*    _failed_51ea86cc;
    QFinalState*    _success_914678f0;


};

#endif // STATESEQUENCEMACHINE_H

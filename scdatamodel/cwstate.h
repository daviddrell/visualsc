#ifndef CWSTATE_H
#define CWSTATE_H
#include <QString>
#include "scstate.h"
#include "cwtransition.h"
#include "sctransition.h"
#include <QDebug>

/*
 the state machine class is modeled as a QStateMachine and encapsulates a hierarchical state machine by holding QStates for each state

 every state has its own pair of private entry and exit slots AND pair of public entry and exit signals that are connected to the QState's entered() and exited() signals (represents a private handler for each state's entry and exit operations AND public signals for entry and exit events visible to outside classes). These slots are empty by default. any text in entryAction or exitAction will have the corresponding function emit a signal with the value found in these attributes.

 every state will
        connect its private entered()/exited() signals to the public pair of                                        StateEntry_stateName()          StateExit_stateName()
        connect its private entered()/exited() signals to private entry and exit slots                              Slot_StateEntry_stateName()     Slot_StateExit_stateName()
        *for any entryActions and exitActions attributes: have the private entry and exit slots emit a signal       EntryAction_entryActionValue()  ExitAction_exitActionValue()
        addTransition for each outgoing transition using private signals specific to each transition                Relay_Event_eventName()



 every transition has its own public slot that emits a private (RELAY)signal named after the slot/transition (this signal is also what is registered when using addTransition to trigger transitions between the QStates), and represents an external event triggering a transition between states.

 every transition will
        have its own public slot    Event_eventName();
        emit a private signal       Relay_Event_eventName();

*/
class CWState
{
public:
    CWState(QString theStateName,QString theEntryRelaySlot, QString theExitRelaySlot, QString theEntryRelaySignal,QString theExitRelaySignal,QString theEntryAction, QString theExitAction);
    CWState();
    ~CWState();

    QList<CWTransition*> getTransitions();

    QString _stateName;          // _stateName                       state's name in camel case and preceeding "_"
    QString _entryRelaySlot;     // Slot_StateEntry_stateName        QState's private corresponding entry slot called when a transition leads to this state
    QString _exitRelaySlot;      // Slot_StateExit_stateName         QState's private corresponding exit slot called when a transition exits this state
    QString _entryRelaySignal;   // Signal_StateEntry_stateName      QState's public signal connected to its private entered() signal
    QString _exitRelaySignal;    // Signal_StateExit_stateName       QState's public signal connected to its private exited() signal
    QString _entryAction;        // EntryAction_eventName            signal that is emitted in the entry relay slot
    QString _exitAction;         // ExitAction_eventName             signal that is emitted in the exit relay slot

    QString _readyRelaySignal;   // Signal_StateReady_stateName      signal reserved for the inital state of this machine

    QString _finishedRelaySlot;
    QString _finishedRelaySignal;   // Signal_StateFinished_stateName public signal that is connected to a private finished signal. used for parallel states.

    QString toCamel(QString);

    void insertTransition(CWTransition* transCW);

    void setState(SCState*);
    SCState* getState();

protected:

    // transitions that belong to this state
    //QHash<SCTransition*, CWTransition*> _transitions;
    QList<CWTransition*> _transitions;
    SCState* _myState;
};

#endif // CWSTATE_H

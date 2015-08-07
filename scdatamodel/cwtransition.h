#ifndef CWTRANSITION_H
#define CWTRANSITION_H

#include <QString>
#include "sctransition.h"

// a transition in the code writer will have its own slot labeled Event_eventName()
// this slot will also emit a signal labeled RelaySlot_Event_eventName() to alert anyone connected to this event


/*
 the state machine class is modeled as a QStateMachine and encapsulates a hierarchical state machine by holding QStates for each state

 every state has its own pair of private entry and exit slots AND pair of public entry and exit signals that are connected to the QState's entered() and exited() signals (represents a private handler for each state's entry and exit operations AND public signals for entry and exit events visible to outside classes). These slots are empty by default. any text in entryAction or exitAction will have the corresponding function emit a signal with the value found in these attributes.

 every state will
        connect its private entered()/exited() signals to the public pair of                            StateEntry_stateName()          StateExit_stateName()
        connect its private entered()/exited() signals to private entry and exit slots                  Slot_StateEntry_stateName()     Slot_StateExit_stateName()
        addTransition for each outgoing transition using private signals specific to each transition    Relay_Event_eventName()



 every transition has its own public slot that emits a private (RELAY)signal named after the slot/transition (this signal is also what is registered when using addTransition to trigger transitions between the QStates), and represents an external event triggering a transition between states.

 every transition will
        have its own public slot    Event_eventName();
        emit a private signal       Relay_Event_eventName();

*/

class CWTransition
{
public:

    SCTransition* getTransition();

    CWTransition(SCTransition* trans,QString theEventName, QString theRelaySignalName, QString targetName);
    ~CWTransition();

    QString _eventName;      // Event_eventName              public slot that transition calls, the "event" attribute value in a transition
    QString _relaySignal;    // RelaySlot_Event_eventName    the signal corresponding to the transiton's slot

    QString _targetName;

    SCTransition* _transModel;
};

#endif // CWTRANSITION_H

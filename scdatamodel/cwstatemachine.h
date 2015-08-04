#ifndef CWSTATEMACHINE_H
#define CWSTATEMACHINE_H

#include "cwstate.h"
#include "cwtransition.h"

class CWStateMachine : public CWState
{
public:
    CWStateMachine(QString theStateName,QString theEntryRelaySlot, QString theExitRelaySlot, QString theEntryRelaySignal,QString theExitRelaySignal,QString theEntryAction, QString theExitAction);
    CWStateMachine(SCState* state);
    CWStateMachine(SCState* state, bool isRoot);
    ~CWStateMachine();


    void createSignalsAndSlots();

    //QList<CWState*> _children;
    //QList<SCState*> _children;

    // direct descendants of this state
    //QHash<SCState*, CWState*> _states;
    QList<CWState*>     _states;

    //SCState*    _myState;
    SCState*    _initialState;
    bool _parallel;

    bool isParallel();
    void setParallel(bool);
};

#endif // CWSTATEMACHINE_H

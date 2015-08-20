#ifndef CWSTATEMACHINE_H
#define CWSTATEMACHINE_H

#include "cwstate.h"
#include "cwtransition.h"

class CWStateMachine : public CWState
{
public:
//    CWStateMachine(QString theStateName,QString theEntryRelaySlot, QString theExitRelaySlot, QString theEntryRelaySignal,QString theExitRelaySignal,QString comments);
    CWStateMachine(SCState* state, QHash<SCState*, CWState*> &stateHash);
    ~CWStateMachine();
    void createSignalsAndSlots();

    // direct descendants of this state
    QList<CWState*>     _states;

    //SCState*    _myState;
    SCState*    _initialState;
    bool _parallel;

    bool isParallel();
    void setParallel(bool);

    QList<SCTransition*> _finishedTransitions;

    void createChildren();
    QList<CWState*> getStates();

    QHash<SCState*, CWState*> _stateHash;

    void createTransition(CWState* state);
    void createTransitions();
private:


};

#endif // CWSTATEMACHINE_H

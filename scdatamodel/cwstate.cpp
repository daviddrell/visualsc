#include "cwstate.h"

CWState::CWState(QString theStateName,QString theEntryRelaySlot, QString theExitRelaySlot, QString theEntryRelaySignal,QString theExitRelaySignal,QString theEntryAction, QString theExitAction)
{
    stateName = theStateName;
    entryRelaySlot = theEntryRelaySlot;
    exitRelaySlot = theExitRelaySlot;
    entryRelaySignal = theEntryRelaySignal;
    exitRelaySignal = theExitRelaySignal;
    entryAction = theEntryAction;
    exitAction = theExitAction;
}

CWState::~CWState()
{

}


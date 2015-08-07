#include "cwtransition.h"

CWTransition::CWTransition(SCTransition* transModel,QString theEventName, QString theRelaySignalName, QString targetName)
{
    _eventName = theEventName;
    _relaySignal = theRelaySignalName;
    _targetName = targetName;
    _transModel = transModel;
}

CWTransition::~CWTransition()
{

}

SCTransition* CWTransition::getTransition()
{
    return _transModel;
}


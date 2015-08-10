#include "cwtransition.h"

CWTransition::CWTransition(SCTransition* transModel,QString theEventName, QString theRelaySignalName, QString targetName, QString comments)
{
    _eventName = theEventName;
    _relaySignal = theRelaySignalName;
    _targetName = targetName;
    _transModel = transModel;
    _comments = comments;
}

CWTransition::~CWTransition()
{

}

SCTransition* CWTransition::getTransition()
{
    return _transModel;
}


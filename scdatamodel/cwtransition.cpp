#include "cwtransition.h"

CWTransition::CWTransition(QString theEventName, QString theRelaySignalName, QString targetName)
{
    _eventName = theEventName;
    _relaySignal = theRelaySignalName;
    _targetName = targetName;
}

CWTransition::~CWTransition()
{

}


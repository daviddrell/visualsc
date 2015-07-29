#include "cwtransition.h"

CWTransition::CWTransition(QString theEventName, QString theRelaySignalName)
{
    eventName = theEventName;
    relaySignal = theRelaySignalName;
}

CWTransition::~CWTransition()
{

}


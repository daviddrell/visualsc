#include "cwevent.h"

CWEvent::CWEvent()
{

}

CWEvent::~CWEvent()
{

}

void CWEvent::handleNewTransition(CWTransition * cwt)
{
    transitions.append(cwt);
}


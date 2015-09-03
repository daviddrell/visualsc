#include "cwaction.h"

CWAction::CWAction()
{

}

CWAction::~CWAction()
{

}

void CWAction::handleNewAction(QString actionName)
{
    actions.append(actionName);
}

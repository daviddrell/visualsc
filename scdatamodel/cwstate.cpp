#include "cwstate.h"

CWState::CWState(QString theStateName,QString theEntryRelaySlot, QString theExitRelaySlot, QString theEntryRelaySignal,QString theExitRelaySignal,QString theEntryAction, QString theExitAction):
    _myState(NULL)
{
    _stateName = theStateName;
    _entryRelaySlot = theEntryRelaySlot;
    _exitRelaySlot = theExitRelaySlot;
    _entryRelaySignal = theEntryRelaySignal;
    _exitRelaySignal = theExitRelaySignal;
    _entryAction = theEntryAction;
    _exitAction = theExitAction;
}

CWState::CWState():
    _myState(NULL)
{

}

CWState::~CWState()
{

}

SCState* CWState::getState()
{
    return _myState;
}

void CWState::setState(SCState * st)
{
    _myState = st;
}

void CWState::insertTransition( CWTransition *transCW)
{
    _transitions.append(transCW);
}

QList<CWTransition*> CWState::getTransitions()
{
    return _transitions;
}


/**
 * @brief CodeWriter::toCamel
 * @param text
 * @return
 * converts any space separated string of words and returns a camel case string
 */
QString CWState::toCamel(QString text)
{
    QStringList qls = text.split(" ");
    //if(qls.size()==1)
      //  return text;

    QString ret;
    QString part;
    QChar firstLetter;

    part = qls.at(0);
    firstLetter = part.at(0);

#ifdef FORCE_CAMEL_FIRST_WORD_LOWERCASE
    ret+=part.toLower();
#endif
#ifndef FORCE_CAMEL_FIRST_WORD_LOWERCASE
    ret+= firstLetter.toLower();
    ret+= part.mid(1,part.size());
#endif
    for(int i = 1 ; i < qls.size(); i++)
    {
        part = qls.at(i);
        firstLetter = part.at(0);

        ret+= firstLetter.toUpper();
        ret+= part.mid(1,part.size());
    }
    return ret;
}

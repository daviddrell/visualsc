#include "cwstate.h"

#define UNDERSCORES         "__"

CWState::CWState( SCState* state, QString stateName):
    _myState(state),
    _stateName(stateName),
    _parentNameLevel(0)
{

}

CWState::CWState(QString theStateName,QString theEntryRelaySlot, QString theExitRelaySlot, QString theEntryRelaySignal,QString theExitRelaySignal, QString comments):
    _myState(NULL)
{
    _stateName = theStateName;
    _entryRelaySlot = theEntryRelaySlot;
    _exitRelaySlot = theExitRelaySlot;
    _entryRelaySignal = theEntryRelaySignal;
    _exitRelaySignal = theExitRelaySignal;
//    _entryAction = theEntryAction;
//    _exitAction = theExitAction;
    _comments = comments;
    _parentNameLevel = 0;
}

CWState::CWState():
    _myState(NULL),
    _parentNameLevel(0)
{

}

CWState::~CWState()
{

}

SCState* CWState::getState()
{
    return _myState;
}

void CWState::addEntryAction(QString entryAction)
{
    _entryActions.append(entryAction);
}

void CWState::addExitAction(QString exitAction)
{
    _exitActions.append(exitAction);
}

void CWState::createSignalsAndSlots()
{
    _comments = _myState->attributes.value("comments")->asString();

    _entryRelaySignal =  "Signal_StateEntry__"+_stateName+"()";
    _exitRelaySignal =   "Signal_StateExit__"+_stateName+"()";

    _entryRelaySlot =    "Slot_StateEntry__" + _stateName+"()";
    _exitRelaySlot =     "Slot_StateExit__"+ _stateName+"()";



    // add all entry actions
    QString entryAction = _myState->getStringAttr("entryAction")->asString();
    if(!entryAction.isEmpty())
    {
        // delete any spaces, this will cause an error
        entryAction.replace(" ","");
        QStringList entries = entryAction.split(",");
        for(int i = 0; i < entries.size(); i++)
        {
            QString entry = "Action___"+toCamel(entries.at(i))+"()";
            this->addEntryAction(entry);
        }
    }


    // add all exit actions
    QString exitAction = _myState->getStringAttr("exitAction")->asString();
    if(!exitAction.isEmpty())
    {
        // delete any spaces, otherwise an assertion error is thrown
        exitAction.replace(" ","");
        QStringList exits = exitAction.split(",");
        for(int i = 0; i < exits.size(); i++)
        {
            QString exit = "Action___"+toCamel(exits.at(i))+"()";
            this->addExitAction(exit);
        }
    }
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

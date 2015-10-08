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

    // get all things separated by n spaces "," then k spaces where n and k are 0, 1, 2, 3, ... spaces
    QRegExp sep("\\s*,\\s*");

    // add all entry actions
    QString entryAction = _myState->getStringAttr("entryAction")->asString();
    if(!entryAction.isEmpty())
    {
        // delete any spaces, otherwise there will be an error
        QStringList entries = entryAction.split(sep);
        for(int i = 0; i < entries.size(); i++)
        {
            if(entries.at(i).isEmpty())
                continue;

            QString entry = QString("Action___"+toCamel(entries.at(i))+"()");
            this->addEntryAction(entry);
        }
    }


    // add all exit actions
    QString exitAction = _myState->getStringAttr("exitAction")->asString();
    if(!exitAction.isEmpty())
    {
        // delete any spaces, otherwise an assertion error is thrown
        QStringList exits = exitAction.split(sep);
        for(int i = 0; i < exits.size(); i++)
        {
            if(exits.at(i).isEmpty())
                continue;

            QString exit = QString("Action___"+toCamel(exits.at(i))+"()");
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
    // get all words separted by 1+n spaces, n = 0, 1, 2, 3, ...
    QRegExp sep("\\s+");
    QStringList qls = text.split(sep);

    QString ret;
    QString part;
    QChar firstLetter;

    // find the starting word, where the word is not an empty string
    int start = 0;
    for(int i = 0 ; i < qls.size(); i++)
    {
        if(!qls.at(i).isEmpty())
        {
            start = i;
            break;
        }
    }

    // the first letter of the first word will be lowercase
    part = qls.at(start);
    firstLetter = part.at(0);


    ret+= firstLetter.toLower();
    ret+= part.mid(1,part.size());


    // now, for every word, capitialize the first letter
    for(int i = start+1 ; i < qls.size(); i++)
    {
        // check if there was trailing spaces
        if(qls.at(i).isEmpty())
            continue;

        part = qls.at(i);
        firstLetter = part.at(0);

        ret+= firstLetter.toUpper();
        ret+= part.mid(1,part.size());
    }
    return ret;
}

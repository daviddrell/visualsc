#include "cwtransition.h"

#define UNDERSCORES_PARENT  "__"
#define UNDERSCORES         "___"

CWTransition::CWTransition(SCTransition *trans, QString sourceName, QString targetName):
    _transModel(trans),
    _targetName(targetName),
    _eventName("Event___"+toCamel(trans->getEventName())+""+sourceName+"()"),
    _comments(trans->getTransStringAttr("comments")->asString()),
    _relaySignal("Relay_"+_eventName)
{

}

CWTransition::~CWTransition()
{

}

SCTransition* CWTransition::getTransition()
{
    return _transModel;
}

QString CWTransition::toCamel(QString text)
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

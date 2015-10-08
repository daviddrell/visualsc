#include "cwtransition.h"

#define UNDERSCORES_PARENT  "__"
#define UNDERSCORES         "___"

CWTransition::CWTransition(SCTransition *trans, QString sourceName, QString targetName):
    _transModel(trans),
    _targetName(targetName),
//    _eventName("Event___"+toCamel(trans->getEventName())+""+sourceName+"()"),
    _eventName("Event___"+toCamel(trans->getEventName())+"()"),
    _comments(trans->getTransStringAttr("comments")->asString()),
    _relaySignal("Relay_Event___"+toCamel(trans->getEventName())+""+sourceName+"()")
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

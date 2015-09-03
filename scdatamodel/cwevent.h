#ifndef CWEVENT_H
#define CWEVENT_H

#include "sctransition.h"
#include "scstate.h"
#include "cwtransition.h"
#include "cwstate.h"

class CWEvent : public QObject
{
    Q_OBJECT

public:
    CWEvent();
    ~CWEvent();

    QList<CWTransition*> transitions;

public slots:
    void handleNewTransition(CWTransition*);


};

#endif // CWEVENT_H

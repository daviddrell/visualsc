#ifndef CWACTION_H
#define CWACTION_H

#include "cwtransition.h"

class CWAction : public QObject
{
    Q_OBJECT

public:
    CWAction();
    ~CWAction();

    QList<QString> actions;
public slots:
    void handleNewAction(QString);


};

#endif // CWACTION_H

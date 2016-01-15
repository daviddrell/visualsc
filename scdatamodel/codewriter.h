#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <QTextStream>
#include <QFile>
#include <scstate.h>
#include <sctransition.h>
#include "cwstate.h"
#include "cwtransition.h"
#include "cwstatemachine.h"

class CodeWriter : QObject
{
    Q_OBJECT
public:
    CodeWriter(SCState*,QString,QString, QString);
    ~CodeWriter();


    virtual bool writeHFile();
    virtual bool writeCppFile();

    void addStateMachine(SCState*);
    void setRootMachine(SCState*);
    virtual void createStateMachines();
    void connectStateMachine(CWStateMachine*);

public slots:
    void handleNewRelayEvent(CWTransition*);

protected:


    virtual void cWriteConstructor();
    virtual void cWriteDeconstructor();
    void cWriteEventSlots();
    void cWriteEntryExitSlots();

    void hWriteEventSlots();
    void hWriteActionSignals();
    void hWriteStateChangeSignals();
    void hWriteEventRelaySignals();
    void hWriteActionRelaySlots();
    virtual void hWriteStates();

    void resolveCollisions(QList<SCState*>);
    void resolveCollision(SCState* one, SCState* two, int parentLevel );
    SCState *getParentAtLevel(SCState* s, int parentLevel);

    void resolveCollisions(QList<CWState*>);
    void resolveCollision(CWState* one, CWState* two);

    QString toCamel(QString, bool leaveFirstLetterCap=false);
    void hPrint(QString);
    void hPrintln(QString);
    void hPrintln(QString, int);
    void cPrint(QString);
    void cPrintln(QString);
    void cPrintln(QString, int);

    QString className;
    QString stateMachineName;
    QFile cFile;
    QFile hFile;
    QTextStream cOut;
    QTextStream hOut;

    SCState* _rootMachine;
    QList<SCState*> _machines;

    QHash<SCState*, CWState*> _stateHash;
    QHash<SCState*, CWStateMachine*> _machineHash;

    QHash<QString, QList<QString>*> _eventRelaySignals;

    QHash<QString, int> _actionDeclare;
    QHash<QString, int> _eventDeclare;
    QHash<QString, int> _eventDefine;

    bool isActionDeclared(QString);
    bool isEventDeclared(QString);
    bool isEventDefined(QString);

};

#endif // CODEWRITER_H

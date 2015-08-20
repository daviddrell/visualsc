#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <QTextStream>
#include <QFile>
#include <scstate.h>
#include <sctransition.h>
#include "cwstate.h"
#include "cwtransition.h"
#include "cwstatemachine.h"

class CodeWriter
{
public:
    CodeWriter(SCState*,QString,QString, QString);
    ~CodeWriter();


    bool writeHFile();
    bool writeCppFile();

    void addStateMachine(SCState*);
    void setRootMachine(SCState*);
    //void addState(SCState*);
    //void setChildren(QList<SCState*>);
  //  void createSignalsAndSlots();
    void createStateMachines();

private:



    void cWriteConstructor();
    void cWriteDeconstructor();
    void cWriteEventSlots();
    void cWriteEntryExitSlots();

    void hWriteEventSlots();
    void hWriteActionSignals();
    void hWriteStateChangeSignals();
    void hWriteEventRelaySignals();
    void hWriteActionRelaySlots();
    void hWriteStates();

    void resolveCollisions(QList<CWState*>);
    void resolveCollision(CWState* one, CWState* two);
//    void resolveCollisionsInsideStateMachines(QList<CWStateMachine*>);
//    void resolveCollisionsBetweenStateMachines(QList<CWStateMachine*>);

    QString toCamel(QString);
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
    //SCState* _initialState;

    //QList<SCState*> _children;
    //zQList<SCState*> _childrenMachines;
    QList<SCState*> _machines;

    QHash<SCState*, CWState*> _stateHash;
    QHash<SCState*, CWStateMachine*> _machineHash;

    //QHash<SCTransition*, CWTransition*> _transitions;



};

#endif // CODEWRITER_H

#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <QTextStream>
#include <QFile>
#include <scstate.h>
#include <sctransition.h>

class CodeWriter
{
public:
    CodeWriter(SCState*,QString,QString, QString);
    ~CodeWriter();

    bool helloWorld();
    bool writeHFile();
    bool writeCppFile();

    void addStateMachine(SCState*);
    void setRootMachine(SCState*);
    void addState(SCState*);
    void setChildren(QList<SCState*>);
    void createSignalsAndSlots();

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

    QString toCamel(QString);
    void hPrint(QString);
    void hPrintln(QString);
    void hPrintln(QString, int);
    void cPrint(QString);
    void cPrintln(QString);
    void cPrintln(QString, int);

    QString className;
    QFile cFile;
    QFile hFile;
    QTextStream cOut;
    QTextStream hOut;

    SCState* _rootMachine;
    QList<SCState*> _children;
    QList<SCState*> _childrenMachines;

    //QHash<SCState*, QHash<QString, QString>*> _sAndS;
    QHash<SCState*, QString> _stateToName;
    QHash<SCTransition*, QString> _transitionToRelaySignal;
    QHash<SCTransition*, QString> _transitionToEvent;
    QHash<SCState*, QString> _stateToEntryRelaySlot;
    QHash<SCState*, QString> _stateToExitRelaySlot;
    QHash<SCState*, QString> _stateToEntryAction;
    QHash<SCState*, QString> _stateToExitAction;
    //QHash<SCTransition*, QString> _transitionTo
};

#endif // CODEWRITER_H

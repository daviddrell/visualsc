#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <QTextStream>
#include <QFile>
#include <scstate.h>
#include <sctransition.h>
#include <cwstate.h>
#include <cwtransition.h>

class CodeWriter
{
public:
    CodeWriter(SCState*,QString,QString, QString);
    ~CodeWriter();


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
    QString stateMachineName;
    QFile cFile;
    QFile hFile;
    QTextStream cOut;
    QTextStream hOut;

    SCState* _rootMachine;
    SCState* _initialState;

    QList<SCState*> _children;
    QList<SCState*> _childrenMachines;

    QHash<SCState*, CWState*> _states;
    QHash<SCTransition*, CWTransition*> _transitions;



};

#endif // CODEWRITER_H

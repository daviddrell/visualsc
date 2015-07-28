#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <QTextStream>
#include <QFile>
#include <scstate.h>
#include <sctransition.h>

class CodeWriter
{
public:
    CodeWriter(SCState*,QString, QString);
    ~CodeWriter();

    bool helloWorld();
    bool writeHFile();
    bool writeCppFile();

    void addStateMachine(SCState*);
    void setRootMachine(SCState*);
    void addState(SCState*);
    void setChildren(QList<SCState*>);

private:

    void hWriteEventSlots();
    QString toCamelCase(QString);
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
};

#endif // CODEWRITER_H

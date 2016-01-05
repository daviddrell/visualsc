#ifndef CODEWRITTERPSTATE_H
#define CODEWRITTERPSTATE_H
#include "codewriter.h"

class CodeWriterPState : CodeWriter
{
public:
    CodeWriterPState(SCState* rootMachine, QString classNameString,QString cFileName, QString hFileName);
    ~CodeWriterPState();


    virtual bool writeHFile();
    virtual bool writeCppFile();
    virtual void createStateMachines();

protected:
    virtual void cWriteConstructor();
    virtual void cWriteDeconstructor();
    virtual void hWriteStates();
    void cWriteStateInitialization(SCState *s);
    void createCppBody();
    QStringList getEntryActions(SCState *s);
    QStringList getExitActions(SCState *s);
    QString createEventEnum();
    QString createActionEnum();
    QString createStateEnum();

    QString _rootStateClassName;
    QMap<QString,QString>  _eventNums; // key= string user types into the visual editor, value = enum version of the string
    QMap<QString,QString>  _actionNums; // key= string user types into the visual editor, value = enum version of the string
    QMap<QString,QString>  _stateNums; // key= state name string user types into the visual editor, value = enum version of the string
};

#endif // CODEWRITTERPSTATE_H

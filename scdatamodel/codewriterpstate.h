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

};

#endif // CODEWRITTERPSTATE_H

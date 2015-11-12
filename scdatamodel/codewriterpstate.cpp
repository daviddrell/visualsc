#include "codewriterpstate.h"
#include "scdatamodel.h"

CodeWriterPState::CodeWriterPState(SCState* rootMachine, QString classNameString,QString cFileName, QString hFileName) :
    CodeWriter(rootMachine, classNameString, cFileName, hFileName)
{

}

CodeWriterPState::~CodeWriterPState()
{

}

void CodeWriterPState::createStateMachines()
{


}

void CodeWriterPState::cWriteStateInitialization(SCState *s)
{
    QString isFinal = s->isFinal() ? "true" : "false";
    QString isInitial = s->isInitial() ? "true" : "false";
    QString isParallel = s->isParallel() ? "true" : "false";

    cPrintln(toCamel(s->objectName() +"->setIsInitialState(" + isInitial + ");"),1);
    cPrintln(toCamel(s->objectName() +"->setIsFinalState(" + isFinal + ");"),1);
    cPrintln(toCamel(s->objectName() +"->setIsParallel(" + isParallel + ");"),1);

    QList<SCTransition*> transitions;
    s->getTransitions(transitions);
    for(int i=0; i < transitions.count(); i++)
    {
        SCTransition* t = transitions[i];
        SCState* target = t->targetState();
        QString tName = t->objectName();
        cPrintln(toCamel(s->objectName()) +"->addTransition(" + toCamel(target->objectName()) + ", \""+tName+"\");",1);
    }

    QList<SCState*> states = s->getStates();
    for(int i = 0 ; i < states.count(); i++)
    {
        SCState *c = states[i];
        cPrintln(toCamel(s->objectName() +"->addChildState(" + toCamel(c->objectName()) + ");"),1);
    }

    for(int i = 0 ; i < states.count(); i++)
    {
        SCState *c = states[i];
        cWriteStateInitialization(c);
    }
    cPrintln("");
}

void CodeWriterPState::cWriteDeconstructor()
{
}

void CodeWriterPState::cWriteConstructor()
{
    if(!cFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "ERROR writing to .cpp file!";
        return ;
    }

    SCDataModel * dm = SCDataModel::singleton();
    SCState * rootState = dm->getTopState();
    if  ( rootState == 0 )
    {
        return;
    }

    cPrintln( className +"::" +className +"()" );
    cPrintln( "{" );
    cPrintln( toCamel( rootState->objectName()) +" = new PState(\""+toCamel(rootState->objectName())+"\");",1);

    for(int i = 0 ; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        cPrintln( this->toCamel( machine->objectName()) +" = new PState(\""+toCamel(machine->objectName())+"\");",1);
    }
    cPrintln( "" );


    cPrintln( toCamel( rootState->objectName()) +"->setIsRoot(true);" ,1);

    QList<SCState*> states =rootState->getStates();
    for(int i = 0 ; i < states.count(); i++)
    {
        SCState *s = states[i];
        cWriteStateInitialization(s);
    }

    cPrintln(toCamel( rootState->objectName())+"->addCallBackToAllDescendants(eStateEntered, (CallbackFuncPtrT)stateEnteredStatic, this, kSameTaskCallback);",1);
    cPrintln(toCamel( rootState->objectName())+"->addCallBackToAllDescendants(eStateExited, (CallbackFuncPtrT)stateExitedStatic, this, kSameTaskCallback);",1);
    cPrintln(toCamel( rootState->objectName())+"->addCallBackToAllDescendants(eStateFinished, (CallbackFuncPtrT)stateFinishedStatic, this, kSameTaskCallback);",1);

    cPrintln( "}\n" );

}

bool CodeWriterPState::writeCppFile()
{
    // intro
    cPrintln("/* This .cpp file was generated using VisualSC's Export Code Function\n");
    cPrintln("\n");
    cPrintln("The state machine class is modeled as a PState machine and encapsulates a hierarchical state machine by holding PStates for each state");
    cPrintln("*/\n");

    // include statements
    cPrintln("#include \""+className.toLower()+".h\" \n");

    // write the constructor
    this->cWriteConstructor();

    // write the deconstructor
    this->cWriteDeconstructor();


    return true;
}

bool CodeWriterPState::writeHFile()
{
    if(!hFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "ERROR writing to .h file!";
        return false;
    }

    hPrintln("// This header file was generated using VisualSC's Export Code Function\n");
    hPrintln("// Visual Statechart is an open source project. Visit us at https://github.com/daviddrell/visualsc");
    hPrintln("#ifndef "+ className.toUpper() + "_H\n#define "+className.toUpper()+"_H\n");
    hPrintln("#include \"pstate.h\"");
    hPrintln("using std::string;\n");
    hPrintln("class "+className);
    hPrintln("{");
    hPrintln("//////////////////",1);
    hPrintln("//",1);
    hPrintln("//    Public interface",1);
    hPrintln("//",1);
    hPrintln("//    Call start() to start the state machine",1);
    hPrintln("//",1);
    hPrintln("//    Call inputEvent() to drive events into the state machine",1);
    hPrintln("//",1);
    hPrintln("//    To take action, inherit from this class and override handleAction()",1);
    hPrintln("//",1);
    hPrintln("//    To monitor state changes, inherit from this class and override handleStateEntered() or handleStateExited()",1);
    hPrintln("//",1);
    hPrintln("//////////////////\n",1);

    // public
    hPrintln("public:\n");
    hPrintln(className+"();",1);
    hPrintln("~"+className+"();\n",1);
    hPrintln("\n",1);
    hPrintln("void start();",1);
    hPrintln("void inputEvent(string evt);",1);
    hPrintln("virtual void handleAction(string action);",1);
    hPrintln("virtual void handleStateEntered(string state);",1);
    hPrintln("virtual void handleStateExited(string state);\n\n",1);

    // private
    hPrintln("private:\n");
    hPrintln("static void stateEnteredStatic(void *p, UInt32 funcIndex, void *userDefined, PStateEnteredArgsT *pArgs);",1);
    hPrintln("static void stateExitedStatic(void *p, UInt32 funcIndex, void *userDefined, PStateExitedArgsT *pArgs);",1);
    hPrintln("static void stateFinishedStatic(void *p, UInt32 funcIndex, void *userDefined, PStateFinishedArgsT *pArgs);",1);

    hPrintln("void stateEntered(PStateEnteredArgsT *pArgs);",1);
    hPrintln("void stateExited(PStateExitedArgsT  *pArgs);",1);
    hPrintln("void stateFinished(PStateFinishedArgsT  *pArgs);\n\n",1);


    //write the statemachine states
    this->hWriteStates();

    // end if define
    hPrintln("\n};\n\n#endif // "+className.toUpper()+"_H");

    // true if no complications
    return true;

}

/**
 * @brief CodeWriterPState::hWriteStates
 *
 * .h write of PStates
 *
 */
void CodeWriterPState::hWriteStates()
{
    SCDataModel * dm = SCDataModel::singleton();

    SCState * rootState = dm->getTopState();
    QList<SCState*> allchildStates;
    dm->getAllStates(allchildStates);
    hPrintln("PState*    "+ this->toCamel( rootState->objectName()) +";",1);
    for(int i = 0 ; i < allchildStates.count(); i++)
    {
        SCState* machine = allchildStates.at(i);
        hPrintln("PState*    "+ this->toCamel( machine->objectName()) +";",1);
    }
    hPrintln("\n");
}


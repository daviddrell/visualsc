#include "codewriterpstate.h"
#include "datamodellist.h"

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

void CodeWriterPState::createCppBody()
{
    // this function just creates the rest of the cpp implemenation

    //cPrintln( className +"::~" +className +"()" );
    cPrintln("void "+className+ "::start()",0);
    cPrintln("{",0);
    cPrintln(_rootStateClassName+"->startSM();",1);
    cPrintln("}\n",0);

    cPrintln("void "+className+ "::inputEvent(int evt)",0);
    cPrintln("{",0);
    cPrintln(_rootStateClassName+"->signalEvent(evt);",1);
    cPrintln("}\n",0);


    cPrintln("void "+className+ "::stateEnteredStatic(void *, UInt32, void *userDefined, PStateCallbackArgsT *pArgs)",0);
    cPrintln("{",0);
    cPrintln(className+"* s= ("+className+"*) userDefined;",1);
    cPrintln("s->stateEntered(pArgs);",1);
    cPrintln("}\n",0);


    cPrintln("void "+className+ "::stateEntered(PStateCallbackArgsT *pArgs)",0);
    cPrintln("{",0);
    cPrintln("handleStateEntered(pArgs->s->GetModT()->name);",1);
    cPrintln("for (int i = 0; i < pArgs->actionCount; i++)",1);
    cPrintln("{",1);
    cPrintln("handleAction(pArgs->actions[i]);",2);
    cPrintln("}",1);
    cPrintln("}\n",0);



    cPrintln("void "+className+ "::stateExitedStatic(void *, UInt32, void *userDefined, PStateCallbackArgsT *pArgs)",0);
    cPrintln("{",0);
    cPrintln(className+"* s= ("+className+"*) userDefined;" ,1);
    cPrintln("s->stateExited(pArgs);",1);
    cPrintln("}\n",0);


    cPrintln("void "+className+ "::stateExited(PStateCallbackArgsT  *pArgs)",0);
    cPrintln("{",0);
    cPrintln("handleStateExited(pArgs->s->GetModT()->name);",1);
    cPrintln("for (int i = 0; i < pArgs->actionCount; i++)",1);
    cPrintln("{",1);
    cPrintln("handleAction(pArgs->actions[i]);",2);
    cPrintln("}",1);
    cPrintln("}\n",0);


    cPrintln("void "+className+ "::stateFinishedStatic(void *p, UInt32 funcIndex, void *userDefined, PStateCallbackArgsT *pArgs)",0);
    cPrintln("{",0);
    cPrintln(className+"* s= ("+className+"*) userDefined;" ,1);
    cPrintln("s->stateFinished(pArgs);",1);
    cPrintln("}\n",0);


    cPrintln("void "+className+ "::stateFinished(PStateCallbackArgsT  *pArgs)",0);
    cPrintln("{",0);
    cPrintln("for (int i = 0; i < pArgs->actionCount; i++)",1);
    cPrintln("{",1);
    cPrintln("handleAction(pArgs->actions[i]);",2);
    cPrintln("}",1);
    cPrintln("}\n",0);


    cPrintln("std::string SessionModelStateMachineSM::actionCodeToString(int action)");
    cPrintln("{");
    cPrintln("switch (action)",1);
    cPrintln("{",1);

    QMap<QString,QString>::iterator i;
    for( i =  _actionNums.begin(); i != _actionNums.end();i++)
    {
        cPrintln("case "+ i.value()+":",1);
        cPrintln("return \""+ i.key() +"\";",2);
        cPrintln("break;",2);
    }

    cPrintln("default:",1);
    cPrintln("return \"action not found in table\";",2);
    cPrintln("break;",2);
    cPrintln("}",1);
    cPrintln("}");
}




QStringList CodeWriterPState::getExitActions(SCState *s)
{
    QStringList actions;
    QString ActionStr = s->getStringAttr("exitAction")->asString();
    if(!ActionStr.isEmpty())
    {
        // delete any spaces, otherwise there will be an error
        QStringList entries = ActionStr.split(',');
        for(int i = 0; i < entries.count(); i++)
        {
            if(entries.at(i).isEmpty())
                continue;
            actions.append(entries.at(i).trimmed());
        }
    }
    return actions;
}

QStringList CodeWriterPState::getEntryActions(SCState *s)
{
    QStringList actions;
    QString ActionStr = s->getStringAttr("entryAction")->asString();
    if(!ActionStr.isEmpty())
    {
        // delete any spaces, otherwise there will be an error
        QStringList entries = ActionStr.split(',');
        for(int i = 0; i < entries.count(); i++)
        {
            if(entries.at(i).isEmpty())
                continue;
            actions.append(entries.at(i).trimmed());
        }
    }
    return actions;
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
        cPrintln(toCamel(s->objectName()) +"->addTransition(" + toCamel(target->objectName()) + ","+ _eventNums[tName]+");",1);
    }

    QStringList entryActions = getEntryActions(s);
    for(int a = 0 ; a < entryActions.count(); a++)
    {
        cPrintln(toCamel(s->objectName()) +"->addEnterAction(" + _actionNums[ entryActions[a]] + ");",1);
    }

    QStringList exitActions = getExitActions(s);
    for(int a = 0 ; a < exitActions.count(); a++)
    {
        cPrintln(toCamel(s->objectName()) +"->addExitAction(" + _actionNums[exitActions[a]] + ");",1);
    }


    //
    // add child states
    //
    QList<SCState*> states = s->getStates();
    for(int i = 0 ; i < states.count(); i++)
    {
        SCState *c = states[i];
        cPrintln(toCamel(s->objectName()) +"->addChildState(" + toCamel(c->objectName() + ");"),1);
    }
    cPrintln("");

    for(int i = 0 ; i < states.count(); i++)
    {
        SCState *c = states[i];
        cWriteStateInitialization(c);
    }

}

void CodeWriterPState::cWriteDeconstructor()
{
    cPrintln( className +"::~" +className +"()" );
    cPrintln( "{\n}" );

}

void CodeWriterPState::cWriteConstructor()
{
    if(!cFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "ERROR writing to .cpp file!";
        return ;
    }

    SCDataModel * dm = DataModelList::singleton()->root();
    SCState * rootState = dm->getTopState();
    if  ( rootState == 0 )
    {
        return;
    }

    _rootStateClassName =  toCamel( rootState->objectName());

    cPrintln( className +"::" +className +"()" );
    cPrintln( "{" );
    cPrintln( _rootStateClassName +" = new PState(\""+toCamel(rootState->objectName())+"\");",1);


    //
    // add all:   stateInstances = new PState()
    //
    for(int i = 0 ; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        cPrintln( this->toCamel( machine->objectName()) +" = new PState(\""+toCamel(machine->objectName())+"\");",1);
    }
    cPrintln( "" );

    //
    // initialize the root state
    //
    cPrintln( toCamel( rootState->objectName()) +"->setIsRoot(true);" ,1);
    cWriteStateInitialization(rootState);

    //
    // add the call backs
    //

    cPrintln(toCamel( rootState->objectName())+"->addCallBackToAllDescendants(eStateEntered, (CallbackFuncPtrT)stateEnteredStatic, this, kSameTaskCallback);",1);
    cPrintln(toCamel( rootState->objectName())+"->addCallBackToAllDescendants(eStateExited, (CallbackFuncPtrT)stateExitedStatic, this, kSameTaskCallback);",1);
    cPrintln(toCamel( rootState->objectName())+"->addCallBackToAllDescendants(eStateFinished, (CallbackFuncPtrT)stateFinishedStatic, this, kSameTaskCallback);",1);

    cPrintln( "}\n" );

}

bool CodeWriterPState::writeCppFile()
{
    // intro
    cPrintln("/*\n    This .cpp file was generated using VisualSC's Export Code Function.");
    cPrintln("    The state machine class is modeled as a PState machine and encapsulates a hierarchical state machine by holding PStates for each state.\n");
    cPrintln("*/\n");

    // include statements
    cPrintln("#include \""+className.toLower()+".h\" \n");

    // write the constructor
    this->cWriteConstructor();

    // write the deconstructor
    this->cWriteDeconstructor();

    this->createCppBody();

    return true;
}

QString CodeWriterPState::createEventEnum()
{
    QString enumStr;
    enumStr.append("enum " + className +"_EVENTS\n    {\n");

    SCDataModel * dm =  DataModelList::singleton()->root();
    QList<SCState*> allchildStates;
    dm->getAllStates(allchildStates);

    for(int i = 0 ; i < allchildStates.count(); i++)
    {
        SCState* s = allchildStates.at(i);
        QList<SCTransition*> transitions;
        s->getTransitions(transitions);
        for(int i=0; i < transitions.count(); i++)
        {
            SCTransition* t = transitions[i];
            QString tName = t->objectName();
            if ( ! _eventNums.contains(tName))
            {
                QString eventStr = toCamel("k "+tName) + "_EVENT";
                _eventNums.insert(tName,eventStr);
                enumStr.append( "        "+ eventStr +",\n");
            }
        }
    }
    enumStr.append("    };\n");
    return enumStr;
}

QString CodeWriterPState::createActionEnum()
{
    QString enumStr;
    enumStr.append("enum " + className +"_ACTIONS\n    {\n");

    SCDataModel * dm =  DataModelList::singleton()->root();
    QList<SCState*> allchildStates;
    dm->getAllStates(allchildStates);

    for(int i = 0 ; i < allchildStates.count(); i++)
    {
        SCState* s = allchildStates.at(i);
        QStringList actions = getEntryActions(s);

        foreach(QString action, actions)
        {
            if ( ! _actionNums.contains(action))
            {
                QString actionStr =toCamel("k "+action)+"_ACTION";
                _actionNums.insert(action,actionStr);
                enumStr.append( "        "+ actionStr +",\n");
            }
        }
        actions.clear();
        actions = getExitActions(s);

        foreach(QString action, actions)
        {
            if ( ! _actionNums.contains(action))
            {
                QString actionStr =toCamel("k "+action)+"_ACTION";
                _actionNums.insert(action,actionStr);
                enumStr.append( "        "+ actionStr +",\n");
            }
        }
    }
    enumStr.append("    };\n");
    return enumStr;

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
    hPrintln("void inputEvent(int evt);",1);
    hPrintln("virtual void handleAction(int action)=0;",1);
    hPrintln("virtual void handleStateEntered(string state)=0;",1);
    hPrintln("virtual void handleStateExited(string state)=0;\n\n",1);

    QString eventsEnum = createEventEnum();
    hPrintln(eventsEnum,1);

    QString actionsEnum = createActionEnum();
    hPrintln(actionsEnum,1);

    hPrintln("std::string actionCodeToString(int action);\n",1);

    // private
    hPrintln("private:\n");
    hPrintln("static void stateEnteredStatic(void *p, UInt32 funcIndex, void *userDefined, PStateCallbackArgsT *pArgs);",1);
    hPrintln("static void stateExitedStatic(void *p, UInt32 funcIndex, void *userDefined, PStateCallbackArgsT *pArgs);",1);
    hPrintln("static void stateFinishedStatic(void *p, UInt32 funcIndex, void *userDefined, PStateCallbackArgsT *pArgs);",1);

    hPrintln("void stateEntered(PStateCallbackArgsT *pArgs);",1);
    hPrintln("void stateExited(PStateCallbackArgsT  *pArgs);",1);
    hPrintln("void stateFinished(PStateCallbackArgsT  *pArgs);\n\n",1);


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
    SCDataModel * dm =  DataModelList::singleton()->root();
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


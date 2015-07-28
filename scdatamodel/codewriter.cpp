#include "codewriter.h"
#include <QDebug>

CodeWriter::CodeWriter(SCState* rootMachine, QString classNameString,QString cFileName, QString hFileName):
    cFile(cFileName),
    hFile(hFileName),
    cOut(&cFile),
    hOut(&hFile),
    _rootMachine(rootMachine),
    className(classNameString)
{
    //_sAndS = new QHash<SCState*, QHash<QString, QString> *>();
    //_stateToName = new QHash<SCState*, QString>();
    //QStringList qsl = cFileName.split("/");
   //QString fName = qsl.at(qsl.size()-1);
    //className = fName.mid(0,fName.size()-4);    // get the class name out of the entire file name

    //className = classNameString;

    qDebug() << "the class name is " << className;
   // cOut = new QTextStream(&cFile);
  //  hOut = new QTextStream(&hFile);
}

void CodeWriter::createSignalsAndSlots()
{
    // link the root machine's state name
    _stateToName.insert(_rootMachine, "_" + toCamel(_rootMachine->attributes.value("name")->asString()));

    SCState* state;
    QList<SCTransition*> transitions;
    SCTransition* trans;
    //QHash<QString,QString>* ssht;
    QString eventName;
    QString entryAction;
    QString exitAction;
    QString stateName;
    for(int i = 0; i < _children.size(); i++)
    {
        // link the state to its name
        state = _children.at(i);
        stateName = toCamel("_"+toCamel(state->attributes.value("name")->asString()));
        _stateToName.insert(state, stateName);

        transitions.clear();
        //ssht = new QHash<QString,QString>();
        //_sAndS.insert(state, ssht);
        state->getTransitions(transitions);

        // add transitions and relay signals for them
        for(int k = 0 ; k < transitions.size(); k++)
        {
            trans = transitions.at(k);
            eventName = "Event_"+toCamel(trans->attributes.value("event")->asString())+"()";
            //ssht->insert("slot",eventName);
            //ssht->insert("relayslot","RelaySlot_"+eventName);
            _transitionToEvent.insert(trans, eventName);
            _transitionToRelaySignal.insert(trans,"RelaySlot_"+eventName );

        }


        // add entry/exit relay slots
        entryAction = state->attributes.value("entryAction")->asString();
        if(!entryAction.isEmpty())
        {
            _stateToEntryAction.insert(state,"EntryAction_"+toCamel(entryAction)+"()");
        }


        exitAction = state->attributes.value("exitAction")->asString();
        if(!exitAction.isEmpty())
        {
            _stateToExitAction.insert(state, "ExitAction_" +toCamel(exitAction)+"()");
        }

        _stateToEntryRelaySlot.insert(state, "RelaySlot_StateEntry"+_stateToName.value(state));
        _stateToExitRelaySlot.insert(state, "RelaySlot_StateExit"+_stateToName.value(state));

        /*
        entryAction = state->attributes.value("entryAction")->asString();
        if(!entryAction.isEmpty())
        {
            ssht->insert("signal","EntryAction_"+toCamel(entryAction)+"()");
            ssht->insert("relaysignal","RelaySignal_EntryAction_"+toCamel(entryAction)+"()");

        }


        exitAction = state->attributes.value("exitAction")->asString();
        if(!exitAction.isEmpty())
        {
            ssht->insert("signal","EntryAction_"+toCamel(exitAction)+"()");
            ssht->insert("relaysignal","RelaySignal_EntryAction_"+toCamel(exitAction)+"()");

        }


        // add default entry/exit actions
        ssht->insert("signal", "StateEntry_"+stateName+"()");
        ssht->insert("signal","StateExit_"+stateName+"()");
        ssht->insert("relaysignal", "RelaySignal_StateEntry_"+stateName+"()");
        ssht->insert("relaysignal","RelaySignal_StateExit_"+stateName+"()");
        */

    }
}

bool CodeWriter::helloWorld()
{
    if(!cFile.open(QIODevice::WriteOnly|QIODevice::Text))
        return false;

    if(!hFile.open(QIODevice::WriteOnly|QIODevice::Text))
        return false;

    cOut << "//This is a cpp file\n\nvoid helloWorld()\n{\n\n}\n";

    hOut << "//This is an h file\n\nvoid helloWorld()\n{\n\n}\n";

    return true;
}

void CodeWriter::addState(SCState* state)
{
    _children.append(state);
}

void CodeWriter::addStateMachine(SCState* state)
{
    _childrenMachines.append(state);
}

void CodeWriter::setRootMachine(SCState* state)
{
    _rootMachine = state;
}

void CodeWriter::setChildren(QList<SCState*> states)
{
    _children = states;
}

void CodeWriter::cWriteDeconstructor()
{
    cPrintln(className+"::~"+className+"()");
    cPrintln("{\n");
    cPrintln("}");
}

void CodeWriter::cWriteConstructor()
{
    QString stateMachineName = "_"+toCamel(_rootMachine->attributes.value("name")->asString());
    QList<QString> childrenNames;
    for(int i = 0; i < _children.size();i++)
    {
        childrenNames.append("_"+toCamel(_children.at(i)->attributes.value("name")->asString()));
    }


    cPrintln(className+"::"+className+"(QObject* parent):");
    cPrintln("QObject(parent),",1);
    cPrintln(stateMachineName+"(new QStateMachine(this)),",1);
    for(int i = 0 ; i < childrenNames.size()-1; i++)
    {
        cPrintln(childrenNames.at(i)+"(new QState()),",1);
    }
    cPrintln(childrenNames.at(childrenNames.size()-1)+"(new QState())",1);
    cPrintln("{");

    for(int i = 0 ; i < childrenNames.size(); i++)
    {
        cPrintln(stateMachineName+"->addState("+childrenNames.at(i)+");",1);
    }

    cPrintln("\n//",1);
    cPrintln("//    connect state transitions",1);
    cPrintln("//",1);

    QString childName;
    QList<SCTransition*> transitions;
    SCTransition* trans;
    SCState* state;
    for(int i = 0 ; i < _children.size(); i++)
    {
        state = _children.at(i);
        childName = "_"+toCamel(state->attributes.value("name")->asString());
        transitions.clear();
        state->getTransitions(transitions);

        // write add transitions for every transition
        for(int k = 0; k < transitions.size();k++)
        {
            trans = transitions.at(k);
            cPrintln(_stateToName.value(state)+"->addTransition(this,SIGNAL("+_transitionToRelaySignal.value(trans)+"),"+_stateToName.value( trans->targetState())+");",1);
        }

        // connect state entered/exit signals to the StateEntry/StateExit signals



    }

// RELAY the state's enter/exit signal to the public signal
    cPrintln("\n//    relay the private SM signals to the public facing signals",1);

    for(int i = 0 ; i < _children.size(); i++)
    {
        state = _children.at(i);
        childName = "_"+toCamel(state->attributes.value("name")->asString());
        cPrintln("connect("+_stateToName.value(state)+", SIGNAL(entered()), this, SIGNAL(StateEntry"+_stateToName.value(state)+"()));",1);
        cPrintln("connect("+_stateToName.value(state)+", SIGNAL(exited()), this, SIGNAL(StateExit"+_stateToName.value(state)+"()));",1);


    }



    // connect(state, SIGNAL(entered))


// Connect the private QState entry/exit signals to their own private entry/exit slots
    cPrintln("\n//    private state transition handlers to take actions",1);

    for(int i = 0; i < _children.size(); i++)
    {
        state = _children.at(i);
        cPrintln("connect("+_stateToName.value(state)+", SIGNAL(entered()), this, SLOT(StateEntry"+_stateToEntryRelaySlot.value(state)+");",1);
        cPrintln("connect("+_stateToName.value(state)+", SIGNAL(exited()), this, SLOT(StateExit"+_stateToExitRelaySlot.value(state)+");",1);

    }

    cPrintln("}\n");
}

/**
 * @brief CodeWriter::cWriteEventSlots
 *
 * writes the event slot for every transition
 * a transition will perform whatever is in its event slot written here
 * additionally, each event slot will emit a relaysignal for that event
 *
 */
void CodeWriter::cWriteEventSlots()
{

    cPrintln("//");
    cPrintln("//    these functions connect external Event slots to internal signals to drive the inputs to the state machine");
    cPrintln("//");

    SCState* state;
    SCTransition* trans;
    QList<SCTransition*> transitions;
    for(int i = 0; i < _children.size(); i++)
    {
        state = _children.at(i);
        transitions.clear();
        state->getTransitions(transitions);
        for(int k = 0; k < transitions.size(); k++)
        {
            trans = transitions.at(k);
            cPrintln("void "+className+"::"+_transitionToEvent.value(trans));
            cPrintln("{");
            cPrintln("emit "+_transitionToRelaySignal.value(trans)+";",1);
            cPrintln("}\n");
        }
    }
}

void CodeWriter::cWriteEntryExitSlots()
{
    cPrintln("//");
    cPrintln("//    these slots register the state entry/exits to generate event signals for any given entry or exit events");
    cPrintln("//");

    QString actionSignal;
    SCState* state;
    SCTransition* trans;
    QList<SCTransition*> transitions;
    for(int i = 0; i < _children.size(); i++)
    {
        state = _children.at(i);
        cPrintln("void "+className+"::"+_stateToEntryRelaySlot.value(state)+"()");
        cPrintln("{");
        actionSignal = _stateToEntryAction.value(state);
        if(!actionSignal.isEmpty())
        {
            cPrintln("emit "+actionSignal+";",1);
        }
        cPrintln("}");


        cPrintln("void "+className+"::"+_stateToExitRelaySlot.value(state)+"()");
        cPrintln("{");
        actionSignal = _stateToExitAction.value(state);
        if(!actionSignal.isEmpty())
        {
            cPrintln("emit "+actionSignal+";",1);
        }
        else
        {
            cPrintln("");
        }
        cPrintln("}\n");

    }
}

bool CodeWriter::writeCppFile()
{
    if(!cFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "ERROR writing to .cpp file!";
        return false;
    }

    /*
    // write a code generated message
    cOut << "// This .cpp file was generated using VisualSC's Export Code Function\n\n";

    // write include statement
    cOut << "#include \"" << className << ".h\"\n\n";

    // write constructor
    cOut << className <<"::"<<className<<"()\n{\n\n}\n";

    // write deconstructor
    cOut << className <<"::~"<<className<<"()\n{\n\n}\n";
*/

// intro
    cPrintln("// This .cpp file was generated using VisualSC's Export Code Function\n");
    cPrintln("#include \""+className.toLower()+".h\"");
    cPrintln("#include <QDebug>\n");
    this->cWriteConstructor();
    this->cWriteDeconstructor();
    this->cWriteEventSlots();
    this->cWriteEntryExitSlots();


    // true if no complications
    return true;
}

bool CodeWriter::writeHFile()
{
    if(!hFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "ERROR writing to .h file!";
        return false;
    }


// intro
    hPrintln("// This header file was generated using VisualSC's Export Code Function\n");
    hPrintln("#ifndef "+ className.toUpper() + "_H\n#define "+className.toUpper()+"_H\n");
    hPrintln("#include <QObject>");
    hPrintln("#include <QStateMachine>");
    hPrintln("#include <QFinalState>\n");
    hPrintln("class "+className+" : public QObject");
    hPrintln("{");
    hPrintln("Q_OBJECT\n",1);
    hPrintln("//////////////////",1);
    hPrintln("//",1);
    hPrintln("//    Public interface",1);
    hPrintln("//",1);
    hPrintln("//    Connect to public slots to drive events into",1);
    hPrintln("//    the state machine",1);
    hPrintln("//",1);
    hPrintln("//    Connect to the public signals to do work",1);
    hPrintln("//    and monitor state changes",1);
    hPrintln("//",1);
    hPrintln("//////////////////\n",1);


// public
    hPrintln("public:");
    hPrintln(className+"(QObject*parent=NULL);",1);
    hPrintln("~"+className+"();\n",1);
    hPrintln("//",1);
    hPrintln("//    INPUT EVENT SLOTS",1);
    hPrintln("//",1);
    hPrintln("//    Call these slots to signal event inputs to the machine",1);
    hPrintln("//\n",1);

// public slots
    hPrintln("\npublic slots:");
    this->hWriteEventSlots();

// signals
    hPrintln("\nsignals:\n");
    hPrintln("//",1);
    hPrintln("//    OUTPUT ACTION SIGNALS",1);
    hPrintln("//",1);
    hPrintln("//    Connect to these signals to take actions",1);
    hPrintln("//",1);

    this->hWriteActionSignals();

    hPrintln("\n//",1);
    hPrintln("//    STATE CHANGE MONITOR SIGNALS",1);
    hPrintln("//",1);
    hPrintln("//    Connect to these signals to monitor state transitions",1);
    hPrintln("//",1);


    this->hWriteStateChangeSignals();

// private alert
    hPrintln("\n//////////////////",1);
    hPrintln("//",1);
    hPrintln("//    Private below here",1);
    hPrintln("//",1);
    hPrintln("//////////////////\n",1);

// paragraph
    hPrintln("\n/*",1);
    hPrintln(" *  A consequence of OOB encapsulation is that this class does know about its users, and so cannot listen",1);
    hPrintln(" * to their signals. Therefore this class exposes public slots that users can call. These slots",1);
    hPrintln(" * are then translated into private signals that the private state machine can listen to.",1);
    hPrintln(" *",1);
    hPrintln(" * For outbound signals, I choose to keep the statemachine objects private so that means I need",1);
    hPrintln(" * to translate private state machine signals into public signals.",1);
    hPrintln(" *",1);
    hPrintln(" * This all leads to a lot of confusion and possible name collisions. Therefore its better for this",1);
    hPrintln(" * file to be autogenerated from the visual state chart editor.",1);
    hPrintln(" *",1);
    hPrintln(" */\n",1);


    hPrintln("\nprivate:\n");

// private slots
    hPrintln("\nprivate slots:");
    hPrintln("// The Entry/Exit Slots that belong to QStates",1);

    this->hWriteActionRelaySlots();


// signals
    hPrintln("\nsignals:");
    hPrintln("// A Transiton/Event slot's corresponding signal emitted in the slot",1);
    this->hWriteEventRelaySignals();


// private members
    hPrintln("\nprivate:");


    //write the statemachine and states
    this->hWriteStates();

// end if define
    hPrintln("\n};\n\n#endif // "+className.toUpper()+"_H");

    // true if no complications
    return true;

}




/**
 * @brief CodeWriter::toCamel
 * @param text
 * @return
 * converts any space separated string of words and returns a camel case string
 */
QString CodeWriter::toCamel(QString text)
{
    QStringList qls = text.split(" ");
    //if(qls.size()==1)
      //  return text;

    QString ret;
    QString part;
    QChar firstLetter;

    part = qls.at(0);
    firstLetter = part.at(0);

#ifdef FORCE_CAMEL_FIRST_WORD_LOWERCASE
    ret+=part.toLower();
#endif
#ifndef FORCE_CAMEL_FIRST_WORD_LOWERCASE
    ret+= firstLetter.toLower();
    ret+= part.mid(1,part.size());
#endif
    for(int i = 1 ; i < qls.size(); i++)
    {
        part = qls.at(i);
        firstLetter = part.at(0);

        ret+= firstLetter.toUpper();
        ret+= part.mid(1,part.size());
    }
    return ret;
}

/**
 * @brief CodeWriter::hWriteEventSlots
 *
 * events correspond to a state transitioning
 * go through all out transitions of every state, and add a public slot for every event
 *
 */
void CodeWriter::hWriteEventSlots()
{
    SCState* state;
    SCTransition* trans;
    QList<SCTransition*> transitions;
    for(int i = 0 ; i < _children.count();i++)
    {
        // if we go through all out transitions, all transitions will be covered.
        state = _children.at(i);
        transitions.clear();
        state->getTransitions(transitions);
        for(int k = 0; k < transitions.count(); k++)
        {
            trans = transitions.at(k);
            hPrintln("void Event_"+toCamel(trans->attributes.value("event")->asString())+"();",1);
        }
    }
}

/**
 * @brief CodeWriter::hWriteActionSignals
 *
 * these signals correspond to non empty, custom entry events to every state
 *
 */
void CodeWriter::hWriteActionSignals()
{
    SCState* state;
    QString entryAction;
    QString exitAction;
    for(int i = 0 ; i < _children.count(); i++)
    {
        state = _children.at(i);
        entryAction = state->attributes.value("entryAction")->asString();

        if(entryAction!="")
        {
            hPrintln("void EntryAction_"+toCamel(entryAction)+"();",1);
        }

        exitAction = state->attributes.value("exitAction")->asString();

        if(exitAction!="")
        {
            hPrintln("void ExitAction_"+toCamel(exitAction)+"();",1);
        }
    }
}

/**
 * @brief CodeWriter::hWriteStateChangeSignals
 *
 * every state is given a set of entry and exit signals by default
 *
 */
void CodeWriter::hWriteStateChangeSignals()
{
    SCState* state;
    QString stateName;
    for(int i = 0; i < _children.count(); i++)
    {
        state = _children.at(i);
        stateName = toCamel(state->attributes.value("name")->asString());

        hPrintln("void StateEntry_"+stateName+"();",1);
        hPrintln("void StateExit_"+stateName+"();",1);
    }
}

/**
 * @brief CodeWriter::hWriteRelaySlots
 * a signal to relay to the state machine that the event slot occurred
 */
void CodeWriter::hWriteEventRelaySignals()
{
    SCState* state;
    SCTransition* trans;
    QList<SCTransition*> transitions;
    for(int i = 0 ; i < _children.count();i++)
    {
        // if we go through all out transitions, all transitions will be covered.
        state = _children.at(i);
        transitions.clear();
        state->getTransitions(transitions);
        for(int k = 0; k < transitions.count(); k++)
        {
            trans = transitions.at(k);
            hPrintln("void RelaySignal_Event_"+toCamel(trans->attributes.value("event")->asString())+"();",1);
        }
    }
}

/**
 * @brief CodeWriter::hWriteRelaySignals
 *
 * Private state machine slot for the entry events of states
 *
 */
void CodeWriter::hWriteActionRelaySlots()
{
    SCState* state;
    //QString entryAction;
    //QString exitAction;
    QString stateName;
    for(int i = 0 ; i < _children.count(); i++)
    {
        state = _children.at(i);
        /*
        entryAction = state->attributes.value("entryAction")->asString();

        if(entryAction!="")
        {
            hPrintln("void RelaySlot_EntryAction_"+toCamel(entryAction)+"();",1);
        }

        exitAction = state->attributes.value("exitAction")->asString();

        if(entryAction!="")
        {
            hPrintln("void RelaySlot_ExitAction_"+toCamel(entryAction)+"();",1);
        }*/

        stateName = toCamel(state->attributes.value("name")->asString());

        hPrintln("void RelaySlot_StateEntry_"+stateName+"();",1);
        hPrintln("void RelaySlot_StateExit_"+stateName+"();",1);
    }
}

void CodeWriter::hWriteStates()
{
    if(_rootMachine)
    {
        hPrintln("QStateMachine*    _" + toCamel(_rootMachine->attributes.value("name")->asString())+";",1);
    }
    for(int i = 0 ; i < _childrenMachines.count(); i++)
    {
        hPrintln("QStateMachine*    _" + toCamel(_childrenMachines.at(i)->attributes.value("name")->asString())+";",1);
    }
    for(int i = 0 ; i < _children.count(); i++)
    {
        hPrintln("QState*    _"+ toCamel(_children.at(i)->attributes.value("name")->asString())+";",1);
    }
}

void CodeWriter::hPrint(QString text)
{
    hOut << text;
}

void CodeWriter::hPrintln(QString text)
{
    hOut << text <<"\n";
}

void CodeWriter::hPrintln(QString text, int tabLevel)
{
    if(text.at(0)=='\n')
    {
        hOut << "\n";
        for(int i = 0 ; i < tabLevel; i++)
        {
            hOut << "    ";
        }
        hOut << text.mid(1,text.size()) <<"\n";
    }
    else
    {
        for(int i = 0 ; i < tabLevel; i++)
        {
            hOut << "    ";
        }
        hOut << text <<"\n";
    }
}

void CodeWriter::cPrint(QString text)
{
    cOut << text;
}

void CodeWriter::cPrintln(QString text)
{
    cOut << text <<"\n";
}

void CodeWriter::cPrintln(QString text, int tabLevel)
{
    if(text.at(0)=='\n')
    {
        cOut << "\n";
        for(int i = 0 ; i < tabLevel; i++)
        {
            cOut << "    ";
        }
        cOut << text.mid(1,text.size()) <<"\n";
    }
    else
    {
        for(int i = 0 ; i < tabLevel; i++)
        {
            cOut << "    ";
        }
        cOut << text <<"\n";
    }
}


CodeWriter::~CodeWriter()
{

}


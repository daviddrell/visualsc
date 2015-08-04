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
/*
    stateMachineName = "_"+toCamel(_rootMachine->attributes.value("name")->asString());
    CWState* root = new CWState(stateMachineName,"","","","","","");
    root->_readyRelaySignal = "Signal_StateReady"+stateMachineName+"()";
    //_states.insert(_rootMachine, root);
    _stateMachines.insert(_rootMachine, root);*/




    //CWStateMachine* root = new CWStateMachine(_rootMachine, true);
    //_machineHash.insert(_rootMachine, root);
}

void CodeWriter::createStateMachines()
{
    SCState* machine;
    CWStateMachine* cwsm;

    for(int i = 0; i < _machines.size(); i++)
    {
        machine = _machines.at(i);
        cwsm = new CWStateMachine(machine); // creates a state machine
        cwsm->createSignalsAndSlots();      // creates the signals and slots for all states that belong to this state machine
        _machineHash.insert(machine, cwsm);
    }

}
/*
void CodeWriter::createSignalsAndSlots()
{
    // link the root machine's state name
    QString stateName;          // _stateName                       state's name in camel case and preceeding "_"
    QString entryRelaySlot;     // Slot_StateEntry_stateName        QState's private corresponding entry slot called when a transition leads to this state
    QString exitRelaySlot;      // Slot_StateExit_stateName         QState's private corresponding exit slot called when a transition exits this state
    QString entryRelaySignal;   // Signal_StateEntry_stateName      QState's public signal connected to its private entered() signal
    QString exitRelaySignal;    // Signal_StateExit_stateName       QState's public signal connected to its private exited() signal
    QString entryAction;        // EntryAction_eventName            signal that is emitted in the entry relay slot
    QString exitAction;         // ExitAction_eventName             signal that is emitted in the exit relay slot

    SCState* state;
    CWState* cwState;


    QList<SCTransition*> transitions;
    SCTransition* trans;
    QString eventName;
    QString relaySignal;
    CWTransition* cwTransition;

    // for each child state, determine its QState name, its relay slot function name, and its relay slot signal name.
    // also for each of the out transitions of each child, determine the transition's event name and relay signal name
    for(int i = 0 ; i < _children.size();i++)
    {
        state = _children.at(i);
        stateName =         "_" + toCamel(state->attributes.value("name")->asString());

        entryRelaySignal =  "Signal_StateEntry"+stateName+"()";
        exitRelaySignal =   "Signal_StateExit"+stateName+"()";

        entryRelaySlot =    "Slot_StateEntry" + stateName+"()";
        exitRelaySlot =     "Slot_StateExit"+ stateName+"()";

        entryAction = state->attributes.value("entryAction")->asString();

        if(!entryAction.isEmpty())
        {
            qDebug()<<"writing entry action: " <<entryAction;
            entryAction =   "EntryAction_"+toCamel(entryAction)+"()";
            qDebug()<<"here's entry action: " <<entryAction;
        }

        exitAction = state->attributes.value("exitAction")->asString();
        if(!exitAction.isEmpty())
        {
            exitAction =    "ExitAction_" +toCamel(exitAction)+"()";
        }

        // create this codewriter state and link them using a QHash
        cwState = new CWState(stateName,entryRelaySlot,exitRelaySlot,entryRelaySignal,exitRelaySignal,entryAction,exitAction);
        _states.insert(state,cwState);


        // for each out transition, determine the transition's event name and relay signal name
        transitions.clear();
        state->getTransitions(transitions);
        for(int k = 0 ; k < transitions.size(); k++)
        {
            trans = transitions.at(k);
            eventName = "Event_"+toCamel(trans->attributes.value("event")->asString())+"()";
            relaySignal = "Relay_"+eventName;

            // create a new codewriter transition and link them with a QHash
            cwTransition = new CWTransition(eventName, relaySignal);
            _transitions.insert(trans, cwTransition);
        }

        // determine the inital state
        QString initState = state->attributes.value("initialState")->asString();
        if(initState.toLower() == "true")
        {
            qDebug() << "CW::createSignalsAndSlots() found initial state: " << state->objectName();
            _initialState = state;
            cwState->_readyRelaySignal = "Signal_StateReady"+stateName+"()";
        }
    }
}
*/

/*
void CodeWriter::addState(SCState* state)
{
    _children.append(state);
}
*/
void CodeWriter::addStateMachine(SCState* state)
{
    _machines.append(state);
}

void CodeWriter::setRootMachine(SCState* state)
{
    _rootMachine = state;
}
/*
void CodeWriter::setChildren(QList<SCState*> states)
{
    _children = states;
}*/

void CodeWriter::cWriteDeconstructor()
{
    cPrintln(className+"::~"+className+"()");
    cPrintln("{\n");
    cPrintln("}");
}

void CodeWriter::cWriteConstructor()
{
#if 0
    QList<QString> childrenNames;
    for(int i = 0; i < _machines.size();i++)
    {
        childrenNames.append("_"+toCamel(_machines.at(i)->attributes.value("name")->asString()));
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

    // set the machine's initial state
    cPrintln(stateMachineName+"->setInitialState("+_states.value(_initialState)->_stateName+");",1);

    cPrintln("\n//",1);
    cPrintln("//    add transitions for the QStates using the transitions' private relay signals",1);
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
            cPrintln(_states.value(state)->_stateName+"->addTransition(this,SIGNAL("+_transitions.value(trans)->relaySignal+"),"+ _states.value(trans->targetState())->_stateName+");",1);


        }

        // connect state entered/exit signals to the StateEntry/StateExit signals



    }

// RELAY the state's enter/exit signal to the public signal
    cPrintln("\n//    Propogate the private QState signals to public signals",1);

    // set the started signal to the inital state
    cPrintln("connect("+stateMachineName+", SIGNAL(started()), this, SIGNAL("+_states.value(_rootMachine)->_readyRelaySignal+"));",1);

    for(int i = 0 ; i < _children.size(); i++)
    {
        state = _children.at(i);



        childName = "_"+toCamel(state->attributes.value("name")->asString());
        cPrintln("connect("+_states.value(state)->_stateName+", SIGNAL(entered()), this, SIGNAL("+_states.value(state)->_entryRelaySignal+"));",1);
        cPrintln("connect("+_states.value(state)->_stateName+", SIGNAL(exited()), this, SIGNAL("+_states.value(state)->_entryRelaySignal+"));",1);


    }



    // connect(state, SIGNAL(entered))


// Connect the private QState entry/exit signals to their own private entry/exit slots
    cPrintln("\n//    Connect the private QState signals to private slots for entry/exit handlers",1);

    for(int i = 0; i < _children.size(); i++)
    {
        state = _children.at(i);
        cPrintln("connect("+_states.value(state)->_stateName+", SIGNAL(entered()), this, SLOT("+_states.value(state)->_entryRelaySlot+"));",1);
        cPrintln("connect("+_states.value(state)->_stateName+", SIGNAL(exited()), this, SLOT("+_states.value(state)->_exitRelaySlot+"));",1);


    }

    cPrintln("}\n");
#endif
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
#if 0
    cPrintln("//    PUBLIC");
    cPrintln("//    these functions connect external Event slots to internal signals to drive the inputs to the state machine");
    cPrintln("//");


    // write the start machine event function

    cPrintln("void "+className+"::Event_startMachine"+_states.value(_rootMachine)->_stateName+"()");
    cPrintln("{");
    cPrintln(_states.value(_rootMachine)->_stateName+"->start();",1);
    cPrintln("}\n");

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
            cPrintln("void "+className+"::"+_transitions.value(trans)->eventName);

            cPrintln("{");
            cPrintln("emit "+_transitions.value(trans)->relaySignal+";",1);
            cPrintln("}\n");
        }
    }
#endif
}

void CodeWriter::cWriteEntryExitSlots()
{
#if 0
    cPrintln("//");
    cPrintln("//    these slots register the state entry/exits to generate event signals for any given entry or exit events");
    cPrintln("//");

    QString actionSignal;
    SCState* state;
    for(int i = 0; i < _children.size(); i++)
    {
        state = _children.at(i);
        cPrintln("void "+className+"::"+_states.value(state)->_entryRelaySlot+"");
        cPrintln("{");
        actionSignal = _states.value(state)->_entryAction;

        if(!actionSignal.isEmpty())
        {
            cPrintln("emit "+actionSignal+";",1);
        }
        cPrintln("}\n");


        cPrintln("void "+className+"::"+_states.value(state)->_exitRelaySlot+"");
        cPrintln("{");
        actionSignal = _states.value(state)->_exitAction;

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
 #endif
}


bool CodeWriter::writeCppFile()
{
    if(!cFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "ERROR writing to .cpp file!";
        return false;
    }

    // intro
    cPrintln("// This .cpp file was generated using VisualSC's Export Code Function\n");
    cPrintln("/*\nABOUT");
    cPrintln("the state machine class is modeled as a QStateMachine and encapsulates a hierarchical state machine by holding QStates for each state");
    cPrintln("every state has its own pair of private entry and exit slots AND pair of public entry and exit signals that are connected to the QState's");
    cPrintln("entered() and exited() signals (represents a private handler for each state's entry and exit operations AND public signals for entry and ");
    cPrintln("exit events visible to outside classes). These slots are empty by default. any text in entryAction or exitAction will have the");
    cPrintln("corresponding function emit a signal with the value found in these attributes.\n");
    cPrintln(" every state will");
    cPrintln("connect its private entered()/exited() signals to the public pair of                                        StateEntry_stateName()          StateExit_stateName()",1);
    cPrintln("connect its private entered()/exited() signals to private entry and exit slots                              Slot_StateEntry_stateName()     Slot_StateExit_stateName()",1);
    cPrintln("*for any entryActions and exitActions attributes: have the private entry and exit slots emit a signal       EntryAction_entryActionValue()  ExitAction_exitActionValue()",1);
    cPrintln("addTransition for each outgoing transition using private signals specific to each transition                Relay_Event_eventName()\n",1);
    cPrintln("every transition has its own public slot that emits a private (RELAY)signal named after the slot/transition (this signal is also what is ");
    cPrintln("registered when using addTransition to trigger transitions between the QStates), and represents an external event triggering a transition between states.");
    cPrintln(" every transition will");
    cPrintln("have its own public slot    Event_eventName()",1);
    cPrintln("emit a private signal       Relay_Event_eventName()\n",1);
    cPrintln("*/");

    // include statements
    cPrintln("#include \""+className.toLower()+".h\"");
    cPrintln("#include <QDebug>\n");

    // write the constructor
    this->cWriteConstructor();

    // write the deconstructor
    this->cWriteDeconstructor();

    // write the public event slots
    this->cWriteEventSlots();

    // write the private slots
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
    hPrintln("\nsignals:");
    hPrintln("//",1);
    hPrintln("//    OUTPUT ACTION SIGNALS",1);
    hPrintln("//",1);
    hPrintln("//    Connect to these signals to a slot corresponding to a entryAction or exitAction",1);
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
#if 0
    // write the start machine event
    hPrintln("void Event_startMachine"+_states.value(_rootMachine)->_stateName+"();",1);

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
            hPrintln("void "+_transitions.value(trans)->eventName+";",1);
        }
    }
#endif
}

/**
 * @brief CodeWriter::hWriteActionSignals
 *
 * these signals correspond to non empty, custom entry events to every state
 *
 */
void CodeWriter::hWriteActionSignals()
{
#if 0
    SCState* state;
    QString entryAction;
    QString exitAction;
    for(int i = 0 ; i < _children.count(); i++)
    {
        state = _children.at(i);
        entryAction = _states.value(state)->_entryAction;

        if(entryAction!="")
        {
            hPrintln("void "+entryAction+";",1);
        }

        exitAction = _states.value(state)->_exitAction;

        if(exitAction!="")
        {
            hPrintln("void "+exitAction+";",1);
        }
    }
#endif
}

/**
 * @brief CodeWriter::hWriteStateChangeSignals
 *
 * every state is given a set of entry and exit signals by default
 *
 */
void CodeWriter::hWriteStateChangeSignals()
{
#if 0
    SCState* state;

    // write the state machine ready signal

    hPrintln("void "+_states.value(_rootMachine)->_readyRelaySignal+";",1);

    for(int i = 0; i < _children.count(); i++)
    {
        state = _children.at(i);



        hPrintln("void "+_states.value(state)->_entryRelaySignal+";",1);
        hPrintln("void "+_states.value(state)->_exitRelaySignal+";",1);


    }
#endif
}

/**
 * @brief CodeWriter::hWriteRelaySlots
 * a signal to relay to the state machine that the event slot occurred
 */
void CodeWriter::hWriteEventRelaySignals()
{
#if 0
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
            hPrintln("void "+_transitions.value(trans)->relaySignal+";",1);
        }
    }

 #endif
}

/**
 * @brief CodeWriter::hWriteRelaySignals
 *
 * Private state machine slot for the entry events of states
 *
 */
void CodeWriter::hWriteActionRelaySlots()
{
#if 0
    SCState* state;
    for(int i = 0 ; i < _children.count(); i++)
    {
        state = _children.at(i);

        hPrintln("void "+_states.value(state)->_entryRelaySlot+";",1);
        hPrintln("void "+_states.value(state)->_exitRelaySlot+";",1);
    }
#endif
}

void CodeWriter::hWriteStates()
{
/*
    if(_rootMachine)
    {
        hPrintln("QStateMachine*    _" + toCamel(_rootMachine->attributes.value("name")->asString())+";",1);
    }
*/
    for(int i = 0 ; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        hPrintln("QStateMachine*    "+cwsm->_stateName+";",1);
        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);
            if(cws->getState()->isFinal())
            {
                hPrintln("QFinalState*    "+cws->_stateName+";",1);
            }
            else
            {
                hPrintln("QState*    "+ cws->_stateName+";",1);
            }
        }
//
    }
#if 0
    for(int i = 0 ; i < _childrenMachines.count(); i++)
    {
        hPrintln("QStateMachine*    _" + toCamel(_childrenMachines.at(i)->attributes.value("name")->asString())+";",1);
    }
    for(int i = 0 ; i < _children.count(); i++)
    {
        SCState* state = _children.at(i);
        if(state->isFinal())
        {
            hPrintln("QFinalState*    "+ _states.value(state)->_stateName+";",1);
        }
        else
        {
            hPrintln("QState*    "+ _states.value(state)->_stateName+";",1);
        }

    }
#endif

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


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

}



void CodeWriter::createStateMachines()
{
    SCState* machine;
    CWStateMachine* cwsm;
    QList<CWState*> allStates;
    QList<CWStateMachine*> machs;

    for(int i = 0; i < _machines.size(); i++)
    {
        machine = _machines.at(i);
        cwsm = new CWStateMachine(machine, _stateHash); // creates a state machine
//        cwsm->createSignalsAndSlots();      // creates the signals and slots for all states that belong to this state machine
        cwsm->createChildren();
        //allStates.append(cwsm);
        allStates.append(cwsm->getStates());
        _machineHash.insert(machine, cwsm);
        machs.append(cwsm);
    }
    // add the root machine's cwstatemachine
    allStates.append(_machineHash.value(_rootMachine));
    resolveCollisions(allStates);

//    this->resolveCollisionsInsideStateMachines(machs);
//    this->resolveCollisionsBetweenStateMachines(machs);

    for(int i = 0 ; i < machs.size(); i++)
    {
        machs.at(i)->createSignalsAndSlots();
        machs.at(i)->createTransitions();
    }

}

//void CodeWriter::resolveCollisionsInsideStateMachines(QList<CWStateMachine *> stateMachines)
//{
//    qDebug() << "CodeWriter::resolveCollisions INSIDE on states";
//    for(int i = 0; i < stateMachines.size(); i++)
//    {
//        CWStateMachine* cwsm = stateMachines.at(i);
//        for(int k = 0 ; k < cwsm->getStates().size()-1; k++)
//        {
//            CWState* one = cwsm->getStates().at(k);
//            for(int q = k+1; q < cwsm->getStates().size(); q++)
//            {
//                CWState* two = cwsm->getStates().at(q);
//                if(one->_stateName == two->_stateName)
//                {
//                    resolveCollision(one,two);
//                }
//            }
//        }
//    }
//}



//void CodeWriter::resolveCollisionsBetweenStateMachines(QList<CWStateMachine *> stateMachines)
//{
//    qDebug() << "CodeWriter::resolveCollisions BETWEEN on states";
//    bool collision = false;
//    for(int i = 0; i < stateMachines.size() - 1 ; i++)
//    {
//        CWStateMachine* cwsm = stateMachines.at(i);
//        for(int k = i + 1; k < stateMachines.size(); k++)
//        {
//            CWStateMachine *cwsmOther = stateMachines.at(k);
//            for(int x = 0; x < cwsm->getStates().size(); x ++)
//            {
//                // compare the children states of these two state machines
//                CWState* cws = cwsm->getStates().at(x);
//                for(int y = 0; y < cwsmOther->getStates().size(); y++)
//                {
//                    CWState* cwsOther = cwsmOther->getStates().at(y);

//                    // if there exists two children states that have the same stateName, then modify both of them
//                    if(cws->_stateName == cwsOther->_stateName)
//                    {
//                        collision = true;
//                        this->resolveCollision(cws, cwsOther);
//                    }
//                }
//            }
//        }
//    }

//    // if there was a collision, run this method again to check for any further induced collisions
//    if(collision)
//    {
//        resolveCollisionsBetweenStateMachines(stateMachines);
//    }
//}


/**
 * @brief CodeWriter::resolveCollisions
 * @param states
 *
 * clears up any name collisions between any two states in the list
 * if a collision occurs, resolveCollision is called, which recursively sets state name until the names are unique
 *
 */
void CodeWriter::resolveCollisions(QList<CWState*> states)
{
    bool collision = false;

    // compare two different CWState's state Name
    for(int i = 0; i < states.size()-1; i++)
    {
        CWState* x = states.at(i);
        for(int k = i+1 ; k < states.size(); k++)
        {
            CWState* y = states.at(k);

            // if they are equivalent, then call resolve collision
            qDebug() << "i: " << i<<"\tk: "<<k<<"\tx: "<<x->_stateName<<"\ty: "<<y->_stateName;
            if(x->_stateName == y->_stateName)
            {
                collision = true;
                qDebug() << "collision found: " << x->_stateName<<"\t"<<y->_stateName;
                this->resolveCollision(x,y);
            }
        }
    }

    // if we fixed at least one collision, then ensure any new names have no collisions either
    if(collision)
        this->resolveCollisions(states);
}


/**
 * @brief CodeWriter::resolveCollision
 * @param one
 * @param two
 *
 * given two CWStates, recursively add parent names to the state name until no collision occurs
 * if the ancestor is NULL( top level children collide) or is at any time the same one, then this scheme will not resolve the collision
 * so we attach the uid if all else fails.
 *
 */
void CodeWriter::resolveCollision(CWState* one, CWState* two)
{
    // get the ancestor at the CWState's current ancestor level
    SCState* xParent = one->getState()->parentAt(one->_parentNameLevel++);
    SCState* yParent = two->getState()->parentAt(two->_parentNameLevel++);

    // if at least one of them is a top level child or they share a parent, then we resolve the collision using the uid
    if(!xParent || !yParent || xParent == yParent)
    {
        // there are no more grand parents to use, so use the uid
        //qDebug() << "CodeWriter::resolveCollisions ran out of options for states: "<<one->getState()->objectName()<<"\t"<<two->getState()->objectName();

        one->_stateName.append("___"+one->getState()->getUidFirstName());
        two->_stateName.append("___"+two->getState()->getUidFirstName());
    }
    else // append the parent's name
    {
        one->_stateName.append("___" + toCamel(xParent->getName()));
        two->_stateName.append("___" + toCamel(yParent->getName()));
    }

    // while the names are still equivalent, run this function again
    if(one->_stateName == two->_stateName)
        resolveCollision(one, two);
}

void CodeWriter::addStateMachine(SCState* state)
{
    _machines.append(state);
}

void CodeWriter::setRootMachine(SCState* state)
{
    _rootMachine = state;
}

void CodeWriter::cWriteDeconstructor()
{
    cPrintln(className+"::~"+className+"()");
    cPrintln("{\n");
    cPrintln("}");
}

void CodeWriter::cWriteConstructor()
{
    // print the constructor
    cPrintln(className+"::"+className+"(QObject* parent):");
    cPrintln("QObject(parent),",1);

    //QList<QString> init;
    QStringList init;
    // initialize member QStates (QStateMachine/QFinalState/QState)
    for(int i = 0 ; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        init.append("//////// State Machine: "+cwsm->_stateName+" ////////");
        //cOut<<QString("\b");

        // create this as a QStateMachine
        if(machine == _rootMachine)
        {
            if(cwsm->isParallel())
            {
                init.append(QString(cwsm->_stateName+"(new QStateMachine(QState::ParallelStates, this)),"));
            }
            else
            {
                init.append(QString(cwsm->_stateName+"(new QStateMachine(this)),"));
            }
        }
        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            // initialize all direct descendants of this state machine
            CWState* cws = cwsm->_states.at(k);


            if(machine==_rootMachine)
            {
                if(cws->getState()->isFinal())
                {

                    if(cws->getState()->isParallel())
                       init.append(cws->_stateName+"(new QFinalState(QState::ParallelStates)),");

                    else
                        init.append(cws->_stateName+"(new QFinalState("+")),");
                }
                else
                {
                    if(cws->getState()->isParallel())
                        init.append(cws->_stateName+"(new QState(QState::ParallelStates)),");

                    else
                        init.append(cws->_stateName+"(new QState("+")),");
                }
            }
            else
            {

                if(cws->getState()->isFinal())
                {
                    if(cws->getState()->isParallel())
                        init.append(cws->_stateName+"(new QFinalState(QState::ParallelStates, "+cwsm->_stateName+")),");

                    else
                        init.append(cws->_stateName+"(new QFinalState("+cwsm->_stateName+")),");

                }
                else
                {
                    if(cws->getState()->isParallel())
                        init.append(cws->_stateName+"(new QState(QState::ParallelStates, "+cwsm->_stateName+")),");

                    else
                        init.append(cws->_stateName+"(new QState("+cwsm->_stateName+")),");
                }

            }



//            // do not initialize again if this state is a state machine
//            if(cws->getState()->isStateMachine())
//            {

//                //cPrintln("// child initialized elsewhere: QState* "+cws->_stateName+" ",1);
//                //init.append(QString("// child state machine initialized elsewhere: QState* "+cws->_stateName+" "));
//                //init.append(cwsm->_stateName+"(new QState()),");
//            }
//            else    // this is not a state machine
//            {
//                {
//                    if(cwsm->isParallel())
//                    {
//                        if(cws->getState()->isFinal())
//                        {
//                           // cPrintln(cws->_stateName+"(new QFinalState(QState::ParallelStates)),",1);
//                            init.append(QString(cws->_stateName+"(new QFinalState(QState::ParallelStates)),"));
//                        }
//                        else
//                        {
//                           // cPrintln(cws->_stateName+"(new QState(QState::ParallelStates)),",1);
//                            init.append(QString(cws->_stateName+"(new QState(QState::ParallelStates)),"));
//                        }
//                    }
//                    else
//                    {
//                        if(cws->getState()->isFinal())
//                        {
//                           // cPrintln(cws->_stateName+"(new QFinalState()),",1);
//                            init.append(QString(cws->_stateName+"(new QFinalState()),"));
//                        }
//                        else
//                        {
//                           // cPrintln(cws->_stateName+"(new QState()),",1);
//                            init.append(QString(cws->_stateName+"(new QState()),"));
//                        }
//                    }
//                }
//            }

        }
       // cPrintln("");
        init.append("");
    }

    qDebug() << "initsize: " <<init.size();

//    for(int i = 0; i < init.size(); i++)
//    {
//        qDebug()<< init.at(i);
//    }

    for(int i = init.size()-1; i > -1; i--)
    {
        if(!init.at(i).isEmpty() && (init.at(i).at(0)!=QChar('/')))
        {
            QString s = init.at(i);
            s.chop(1);
            init.removeAt(i);
            init.insert(i,s);
            i=-1;
        }
    }
        for(QStringList::Iterator it = init.begin(); it!= init.end(); ++it)
        {
            cPrintln(*it,1);
        }


    // open curly brace
    cPrintln("{");



    for(int i = 0 ; i < _machines.size(); i++)
    {
        // for every state machine, set the initial state and add each state
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        cPrintln("//////// State Machine: "+cwsm->_stateName+" ////////",1);

        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);


            // only the root machine adds states
            if(machine == _rootMachine)
                cPrintln(cwsm->_stateName+"->addState("+cws->_stateName+");",1);


            if(cws->getState()->isInitial()&&!cwsm->isParallel())
            {
                cPrintln(cwsm->_stateName+"->setInitialState("+cws->_stateName+");",1);
            }
        }
        //cPrintln("\n//",1);
        cPrintln("\n//    Add transitions for the QStates using the transitions' private relay signals",1);
        //cPrintln("//",1);



        // for every state machine, for every direct child state, add every out transition
        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);
            for(int x = 0; x < cws->getTransitions().size(); x++)
            {
                CWTransition* cwt = cws->getTransitions().at(x);

                if(cwt->_comments.isEmpty())
                {
                    cPrintln(cws->_stateName+"->addTransition(this, SIGNAL("+cwt->_relaySignal+"), "+cwt->_targetName+");",1);
                }
                else
                {
                     cPrintln(cws->_stateName+"->addTransition(this, SIGNAL("+cwt->_relaySignal+"), "+cwt->_targetName+");     // "+cwt->_comments,1);
                }




                // for every out transition, check the connectToFinishedAttribute, and connect the finished signal of the state


               if(cwt->getTransition()->isConnectToFinished())
               {
                   cPrintln(cws->_stateName+"->addTransition("+cws->_stateName+", SIGNAL(finished()), "+cwt->_targetName+");",1);
                   //cPrintln("connect("+cws->_stateName+", SIGNAL(finished()), this, SLOT("+cwt->_eventName+"));",1);
               }


            }
        }



        // for every state machine connect its private started signal to a public one, for every direct child state, connect its private entered and exit signals to public ones

        // RELAY the state's enter/exit signal to the public signal
        cPrintln("\n//    Propogate the private QState signals to public signals",1);

        // only do this for the root machine
        if(machine == _rootMachine)
            cPrintln("connect("+cwsm->_stateName+", SIGNAL(started()), this, SIGNAL("+cwsm->_readyRelaySignal+"));",1);

//        if(cwsm->isParallel())
//        {
//             cPrintln("connect("+cwsm->_stateName+", SIGNAL(finished()), this, SIGNAL("+cwsm->_finishedRelaySignal+"));",1);
//        }

        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);

            // go through every entry/exit

            cPrintln("connect("+cws->_stateName+", SIGNAL(entered()), this, SIGNAL("+cws->_entryRelaySignal+"));",1);
            cPrintln("connect("+cws->_stateName+", SIGNAL(exited()), this, SIGNAL("+cws->_exitRelaySignal+"));",1);
        }



        // Connect the private QState entry/exit signals to their own private entry/exit slots
        cPrintln("\n//    Connect the private QState signals to private slots for entry/exit handlers",1);

        //if(cwsm->isParallel())


        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);
            cPrintln("connect("+cws->_stateName+", SIGNAL(entered()), this, SLOT("+cws->_entryRelaySlot+"));",1);
            cPrintln("connect("+cws->_stateName+", SIGNAL(exited()), this, SLOT("+cws->_exitRelaySlot+"));",1);


        }




        cPrintln("\n");
    }

    // close curly brace, end of constructor
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
    cPrintln("//    PUBLIC");
    cPrintln("//    these functions connect external Event slots to internal signals to drive the inputs to the state machine");
    cPrintln("//    Each State Machine Section shows all transitions between its direct children");

    // for every state machine
    for(int i = 0 ; i < _machines.size(); i++)
    {
        // write the start machine public event slot
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        cPrintln("//////// State Machine: "+cwsm->_stateName+" ////////",1);

        if(machine==_rootMachine)
        {
            cPrintln("void "+className+"::Event_startMachine"+cwsm->_stateName+"()");
            cPrintln("{");
            cPrintln(cwsm->_stateName+"->start();",1);
            cPrintln("}\n");
        }


        // write the public event slots for every out transition of each state
        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);
            for(int x = 0; x < cws->getTransitions().size(); x++)
            {
                CWTransition* cwt = cws->getTransitions().at(x);
                cPrintln("void "+className+"::"+cwt->_eventName);

                cPrintln("{");
                cPrintln("emit "+cwt->_relaySignal+";",1);
                cPrintln("}\n");
            }
        }
        cPrintln("");
    }
}

void CodeWriter::cWriteEntryExitSlots()
{
    cPrintln("//");
    cPrintln("//    these slots register the state entry/exits to generate event signals for any given entry or exit events");
    cPrintln("//");

    // for every state machine
    for(int i = 0 ; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        cPrintln("//////// State Machine: "+cwsm->_stateName+" ////////",1);

//        // if this is a parallel state machine, create a public slot that corresponds to its finished
//        if(cwsm->isParallel())
//        {
//            cPrintln("void "+className+"::"+cwsm->_finishedRelaySlot+"");
//            cPrintln("{");
//            for(int k = 0; k < cwsm->_finishedTransitions.size(); k++)
//            {
//                SCTransition* trans = cwsm->_finishedTransitions.at(k);
//                cPrintln("Event_"+toCamel(trans->getEventName())+"_"+trans->getUidFirstName()+"();",1);
//            }
//            cPrintln("}\n");
//        }

        // for every direct child state, write the private entry/exit slots for every entry/exit action
        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);
            cPrintln("void "+className+"::"+cws->_entryRelaySlot+"");
            cPrintln("{");

            if(!cws->_entryActions.isEmpty())
            {
                for(int i = 0; i < cws->_entryActions.size(); i++)
                {
                    cPrintln("emit "+cws->_entryActions.at(i)+";",1);
                }
            }
            else
            {
               cPrintln("");
            }
            cPrintln("}\n");

            cPrintln("void "+className+"::"+cws->_exitRelaySlot+"");
            cPrintln("{");

            if(!cws->_exitActions.isEmpty())
            {
                for(int i = 0; i < cws->_exitActions.size(); i++)
                {
                    cPrintln("emit "+cws->_exitActions.at(i)+";",1);
                }
            }
            else
            {
                cPrintln("");
            }
            cPrintln("}\n");
        }
        cPrintln("");
    }
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
    hPrintln("// Visual Statechart is an open source project. Visit us at https://github.com/daviddrell/visualsc");
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
    hPrintln("// A Transition/Event slot's corresponding signal emitted in the slot",1);
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
    // go through every state machine
    for(int i = 0; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        hPrintln("//////// State Machine: "+cwsm->_stateName+" ////////",1);

        // write the start machine event for the root machine
        if(machine == _rootMachine)
            hPrintln("void Event_startMachine"+cwsm->_stateName+"();",1);

        // go through each state that belongs to this machine (not including the machine)
        // and add an event for every out transition for that state
        QList<CWTransition*> transitions;
        for(int k = 0; k < cwsm->_states.size(); k++)
        {
            transitions = cwsm->_states.at(k)->getTransitions();
            for(int x = 0; x < transitions.size(); x++)
            {
                CWTransition* cwtrans = transitions.at(x);
                hPrintln("void "+cwtrans->_eventName+";",1);
            }
        }
        hPrintln("");
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
    // go through every state machine
    for(int i = 0; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        hPrintln("//////// State Machine: "+cwsm->_stateName+" ////////",1);

        // go through every state that belongs to this machine
        for(int k = 0; k < cwsm->_states.size(); k++)
        {
            // if there is an entry or exit action, then create a signal for it
            CWState* cws = cwsm->_states.at(k);
            if(!cws->_entryActions.isEmpty())
            {
                for(int i = 0; i < cws->_entryActions.size(); i++)
                {
                    hPrintln("void "+cws->_entryActions.at(i)+";",1);
                }
            }

            if(!cws->_exitActions.isEmpty())
            {
                for(int i = 0; i < cws->_exitActions.size(); i++)
                {
                    hPrintln("void "+cws->_exitActions.at(i)+";",1);
                }
            }
        }
        hPrintln("");
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
    // for every state machine, write a state ready signal. Then for each child state, write a stateEntry and stateExit signal
    for(int i = 0 ; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        hPrintln("//////// State Machine: "+cwsm->_stateName+" ////////",1);

        // only create a state ready signal for the root machine
        if(machine == _rootMachine)
            hPrintln("void "+cwsm->_readyRelaySignal+";",1);

        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);
            hPrintln("void "+cws->_entryRelaySignal+";",1);
            hPrintln("void "+cws->_exitRelaySignal+";",1);
        }
        hPrintln("");
    }
}

/**
 * @brief CodeWriter::hWriteRelaySlots
 * a signal to relay to the state machine that the event slot occurred
 */
void CodeWriter::hWriteEventRelaySignals()
{
    // go through each state machine, for each state's out transitions, write a private signal that is emitted in the corresponding event's public slot
    for(int i = 0 ; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        hPrintln("//////// State Machine: "+cwsm->_stateName+" ////////",1);
        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);
            QList<CWTransition*> transitions = cws->getTransitions();
            for(int x = 0; x < transitions.size(); x++)
            {
                CWTransition* cwt = transitions.at(x);
                hPrintln("void "+cwt->_relaySignal+";",1);
            }
        }
        hPrintln("");
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
    // go through each state machine, and add a private entry/exit slot for each state
    for(int i = 0 ; i < _machines.size(); i++)
    {
        SCState* machine = _machines.at(i);
        CWStateMachine* cwsm = _machineHash.value(machine);
        hPrintln("//////// State Machine: "+cwsm->_stateName+" ////////",1);

//        // if this is a parallel state machine, create a public slot that corresponds to its finished
//        if(cwsm->isParallel())
//        {
//            hPrintln("void "+cwsm->_finishedRelaySlot+";",1);
//        }

        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);
            hPrintln("void "+cws->_entryRelaySlot+";",1);
           /* if(cws->getState()->isFinal() && cwsm->isParallel())
            {
                hPrintln("void "+cws->_finishedRelaySlot+";",1);
            }
            */
            hPrintln("void "+cws->_exitRelaySlot+";",1);
        }
        hPrintln("");
    }
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
        hPrintln("//////// State Machine: "+cwsm->_stateName+" ////////",1);
        // if this is the root machine, write QStateMachine

        if(machine == _rootMachine)
        {
            if(cwsm->_comments.isEmpty())
            {
                hPrintln("QStateMachine*    "+cwsm->_stateName+";",1);
            }
            else
            {
                hPrintln("QStateMachine*    "+cwsm->_stateName+";    // "+cwsm->_comments,1);
            }
        }
        else
        {
            hPrintln("// child QState declared elsewhere "+cwsm->_stateName,1);
        }

        for(int k = 0; k < cwsm->_states.size(); k ++)
        {
            CWState* cws = cwsm->_states.at(k);
            if(cws->getState()->isFinal())
            {
                if(cws->_comments.isEmpty())
                    hPrintln("QFinalState*    "+cws->_stateName+";",1);
                else
                    hPrintln("QFinalState*    "+cws->_stateName+";    // "+cws->_comments,1);

            }
            else
            {
                if(cws->_comments.isEmpty())
                    hPrintln("QState*    "+ cws->_stateName+";",1);
                else
                    hPrintln("QState*    "+ cws->_stateName+";    // "+cws->_comments,1);

            }


        }
        hPrintln("");
    }
}

void CodeWriter::hPrint(QString text)
{
    if(text.isEmpty())
    {
        return;
    }
    hOut << text;
}

void CodeWriter::hPrintln(QString text)
{
    if(text.isEmpty())
    {
        hOut<< "\n";
        return;
    }
    hOut << text <<"\n";
}

void CodeWriter::hPrintln(QString text, int tabLevel)
{

    if(text.isEmpty())
    {
        hOut<< "\n";
        return;
    }

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
    if(text.isEmpty())
    {
        return;
    }
    cOut << text;
}

void CodeWriter::cPrintln(QString text)
{
    if(text.isEmpty())
    {
        cOut << "\n";
        return;
    }
    cOut << text <<"\n";
}

void CodeWriter::cPrintln(QString text, int tabLevel)
{
    qDebug() << text;
    if(text.isEmpty())
    {
        cOut << "\n";
        return;
    }

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


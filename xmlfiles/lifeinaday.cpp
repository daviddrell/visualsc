// This .cpp file was generated using VisualSC's Export Code Function

/*
ABOUT
the state machine class is modeled as a QStateMachine and encapsulates a hierarchical state machine by holding QStates for each state
every state has its own pair of private entry and exit slots AND pair of public entry and exit signals that are connected to the QState's
entered() and exited() signals (represents a private handler for each state's entry and exit operations AND public signals for entry and 
exit events visible to outside classes). These slots are empty by default. any text in entryAction or exitAction will have the
corresponding function emit a signal with the value found in these attributes.

 every state will
    connect its private entered()/exited() signals to the public pair of                                        Signal_StateEntry___stateName()     Signal_StateExit___stateName()
    connect its private entered()/exited() signals to private entry and exit slots                              Slot_StateEntry___stateName()       Slot_StateExit___stateName()
    *for any entryActions and exitActions attributes: have the private entry and exit slots emit a signal       Action_entryAction()
    addTransition for each outgoing transition using private signals specific to each transition                Relay_Event___eventName()

every transition has its own public slot that emits a private (RELAY) signal named after the slot/transition (this signal is also what is 
registered when using addTransition to trigger transitions between the QStates), and represents an external event triggering a transition between states.
 every transition will
    have a public slot    Event_eventName()
    that will emit private signal(s) for all events sharing the same event name      Relay_Event___eventName()

*/
#include "lifeinaday.h"
#include <QDebug>

LifeInADay::LifeInADay(QObject* parent):
    QObject(parent),
    //////// State Machine: _lifeInADay ////////
    _lifeInADay(new QStateMachine(this)),
    _play(new QState()),
    _sleep(new QState()),
    _work(new QState(QState::ParallelStates)),

    //////// State Machine: _sleep ////////
    _nREM(new QState(_sleep)),
    _rEM(new QState(_sleep)),

    //////// State Machine: _work ////////
    _talk(new QState(_work)),
    _program(new QState(_work)),
    _think(new QState(_work))

{
    //////// State Machine: _lifeInADay ////////
    _lifeInADay->addState(_play);
    _lifeInADay->addState(_sleep);
    _lifeInADay->setInitialState(_sleep);
    _lifeInADay->addState(_work);

    //    Add transitions for the QStates using the transitions' private relay signals
    _play->addTransition(this, SIGNAL(Relay_Event___goToBed_play()), _sleep);
    _sleep->addTransition(this, SIGNAL(Relay_Event___wakeUp_sleep()), _work);
    _work->addTransition(this, SIGNAL(Relay_Event___finishTasks_work()), _play);

    //    Propogate the private QState signals to public signals
    connect(_lifeInADay, SIGNAL(started()), this, SIGNAL(Signal_StateReady___lifeInADay()));
    connect(_play, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___play()));
    connect(_play, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___play()));
    connect(_sleep, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___sleep()));
    connect(_sleep, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___sleep()));
    connect(_work, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___work()));
    connect(_work, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___work()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_play, SIGNAL(entered()), this, SLOT(Slot_StateEntry___play()));
    connect(_play, SIGNAL(exited()), this, SLOT(Slot_StateExit___play()));
    connect(_sleep, SIGNAL(entered()), this, SLOT(Slot_StateEntry___sleep()));
    connect(_sleep, SIGNAL(exited()), this, SLOT(Slot_StateExit___sleep()));
    connect(_work, SIGNAL(entered()), this, SLOT(Slot_StateEntry___work()));
    connect(_work, SIGNAL(exited()), this, SLOT(Slot_StateExit___work()));


    //////// State Machine: _sleep ////////
    _sleep->setInitialState(_nREM);

    //    Add transitions for the QStates using the transitions' private relay signals
    _nREM->addTransition(this, SIGNAL(Relay_Event___timePasses_nREM()), _rEM);
    _rEM->addTransition(this, SIGNAL(Relay_Event___timePassesAgain_rEM()), _nREM);

    //    Propogate the private QState signals to public signals
    connect(_nREM, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___nREM()));
    connect(_nREM, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___nREM()));
    connect(_rEM, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___rEM()));
    connect(_rEM, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___rEM()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_nREM, SIGNAL(entered()), this, SLOT(Slot_StateEntry___nREM()));
    connect(_nREM, SIGNAL(exited()), this, SLOT(Slot_StateExit___nREM()));
    connect(_rEM, SIGNAL(entered()), this, SLOT(Slot_StateEntry___rEM()));
    connect(_rEM, SIGNAL(exited()), this, SLOT(Slot_StateExit___rEM()));


    //////// State Machine: _work ////////

    //    Add transitions for the QStates using the transitions' private relay signals

    //    Propogate the private QState signals to public signals
    connect(_talk, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___talk()));
    connect(_talk, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___talk()));
    connect(_program, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___program()));
    connect(_program, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___program()));
    connect(_think, SIGNAL(entered()), this, SIGNAL(Signal_StateEntry___think()));
    connect(_think, SIGNAL(exited()), this, SIGNAL(Signal_StateExit___think()));

    //    Connect the private QState signals to private slots for entry/exit handlers
    connect(_talk, SIGNAL(entered()), this, SLOT(Slot_StateEntry___talk()));
    connect(_talk, SIGNAL(exited()), this, SLOT(Slot_StateExit___talk()));
    connect(_program, SIGNAL(entered()), this, SLOT(Slot_StateEntry___program()));
    connect(_program, SIGNAL(exited()), this, SLOT(Slot_StateExit___program()));
    connect(_think, SIGNAL(entered()), this, SLOT(Slot_StateEntry___think()));
    connect(_think, SIGNAL(exited()), this, SLOT(Slot_StateExit___think()));


}

LifeInADay::~LifeInADay()
{

}
//    PUBLIC
//    these functions connect external Event slots to internal signals to drive the inputs to the state machine
//    Each State Machine Section shows all transitions between its direct children
    //////// State Machine: _lifeInADay ////////
void LifeInADay::Event_startMachine___lifeInADay()
{
    _lifeInADay->start();
}

void LifeInADay::Event___goToBed()
{
    emit Relay_Event___goToBed_play();
}

void LifeInADay::Event___wakeUp()
{
    emit Relay_Event___wakeUp_sleep();
}

void LifeInADay::Event___finishTasks()
{
    emit Relay_Event___finishTasks_work();
}


    //////// State Machine: _sleep ////////
void LifeInADay::Event___timePasses()
{
    emit Relay_Event___timePasses_nREM();
}

void LifeInADay::Event___timePassesAgain()
{
    emit Relay_Event___timePassesAgain_rEM();
}


    //////// State Machine: _work ////////

//
//    these slots register the state entry/exits to generate event signals for any given entry or exit events
//
    //////// State Machine: _lifeInADay ////////
void LifeInADay::Slot_StateEntry___play()
{
    emit Action___unwind();
}

void LifeInADay::Slot_StateExit___play()
{
    emit Action___feelRefreshed();
}

void LifeInADay::Slot_StateEntry___sleep()
{
    emit Action___goUnconcious();
    emit Action___dream();
    emit Action___snore();
}

void LifeInADay::Slot_StateExit___sleep()
{
    emit Action___openEyes();
}

void LifeInADay::Slot_StateEntry___work()
{
    emit Action___meetPeople();
    emit Action___turnOnComputer();
}

void LifeInADay::Slot_StateExit___work()
{
    emit Action___getPaid();
    emit Action___accomplishGoals();
}


    //////// State Machine: _sleep ////////
void LifeInADay::Slot_StateEntry___nREM()
{

}

void LifeInADay::Slot_StateExit___nREM()
{

}

void LifeInADay::Slot_StateEntry___rEM()
{

}

void LifeInADay::Slot_StateExit___rEM()
{

}


    //////// State Machine: _work ////////
void LifeInADay::Slot_StateEntry___talk()
{

}

void LifeInADay::Slot_StateExit___talk()
{

}

void LifeInADay::Slot_StateEntry___program()
{

}

void LifeInADay::Slot_StateExit___program()
{

}

void LifeInADay::Slot_StateEntry___think()
{

}

void LifeInADay::Slot_StateExit___think()
{

}



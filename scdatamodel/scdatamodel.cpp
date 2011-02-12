#include "scdatamodel.h"
#include <QFile>
#include <QDebug>

SCDataModel::SCDataModel(QObject * parent) :
        QObject (parent), _reader(), _writer(0), _level(0),_topLevel(0),
        _currentState(NULL), _currentTransition(NULL),_topState(NULL)
{
}



bool SCDataModel::save(QString fileName, QString& errorMessage)
{


    QFile file(fileName);

    if ( ! file.open(QIODevice::WriteOnly))
    {
        errorMessage = QString("error opening file for write");
        return false;
    }

    _writer = new QXmlStreamWriter(&file);

    _writer->setAutoFormatting(true);

    _writer->writeStartDocument();
    _writer->writeStartElement("scxml");
    _writer->writeAttribute("xmlns", "http://www.w3.org/2005/07/scxml");

    _topState->writeSCVXML( *_writer );

    _writer->writeEndDocument();

    file.close();

    errorMessage = QString("sucess");
    return true;
}

void SCDataModel::open(QString file)
{
    connect(&_reader, SIGNAL(done(bool, QStringList)), this, SLOT(handleReaderDone(bool, QStringList)));

    connect(&_reader, SIGNAL(makeANewState(StateAttributes*)), this, SLOT(handleMakeANewState(StateAttributes*)));
    connect(&_reader, SIGNAL(enterStateElement()), this, SLOT(handleTransitDown()));
    connect(&_reader, SIGNAL(leaveStateElement()), this, SLOT(handleTransitUp()));

    connect(&_reader, SIGNAL(makeANewTransistion(TransitionAttributes*)), this, SLOT(handleMakeANewTransition(TransitionAttributes*)));
    connect(&_reader, SIGNAL(leaveTransistionElement()), this, SLOT(handleLeaveTransitionElement()));
    connect(&_reader, SIGNAL(makeANewTransistionPath(TransitionPathAttributes*)), this, SLOT(handleMakeANewTransitionPath(TransitionPathAttributes*)));

    _reader.readFile(file);
    _reader.start();
}


void SCDataModel::getStates(QList<SCState *>& list)
{
    return _topState->getStates(list);
}

void SCDataModel::handleTransitDown()
{
    _level ++;
    qDebug() << "down level = " + QString::number(_level);
}

void SCDataModel::handleTransitUp()
{
    _level --;

    qDebug() << "up level = " + QString::number(_level);

    if ( ! _currentState ) return;


    if (_currentState->parent())
        _currentState = dynamic_cast<SCState*> (_currentState->parent());
}

void SCDataModel::handleMakeANewState(StateAttributes * sa)
{
    SCState * state = NULL;

    if ( _currentState == 0)
        _topLevel = _level;

    if ( _topLevel == _level )
    {
        state = new SCState();
        _topState = state;
        qDebug() << "adding new state at top level : " + sa->name.asString();
    }
    else if ( _level > _topLevel)
    {
        state = new SCState(_currentState);
        qDebug() << "adding state at level  :" + QString::number(_level) + ", name : " + sa->name.asString();
    }

    state->setAttributes( * sa);
    delete sa; sa = NULL;

    _currentState  = state;

    emit newStateSignal(state);

}



void SCDataModel::handleMakeANewTransition(TransitionAttributes * ta)
{
    qDebug() << "handleMakeANewTransition, : "  + ta->target;

    SCTransition * transition = new SCTransition(_currentState);

    if ( _currentState == 0)
        return;

    transition->setAttributes(*ta);

    _currentTransition = transition;
    _currentState->addTransistion(transition);

    delete ta; ta = 0;
}

void SCDataModel::handleLeaveTransitionElement()
{
    qDebug() << "handleLeaveTransitionElement ";

//    _currentTransition->creationDone();
}

void SCDataModel::handleMakeANewTransitionPath(TransitionPathAttributes * tp)
{

    qDebug() << "handleMakeANewTransitionPath : " ;

    if ( _currentState == 0)
        return;

    if ( _currentTransition == 0 ) return;

    _currentTransition->setPath(tp->path);


}

void SCDataModel::handleReaderDone(bool sucess, QStringList message)
{

    if ( ! sucess )
    {
        emit openCompleted ( sucess, message);
        return;
    }
    // connect transistions to target states

    QList<SCTransition*> * masterTransistionList = new QList<SCTransition*>();


    // first query all states for their originating transitions
    // and make a master list
    _topState->getAllTransitions(*masterTransistionList);


    // second, pass the master list to all states/substates so they
    // can look up themselves in the target fields and make the target
    // connections

    _topState->makeTargetConnections( * masterTransistionList);

    delete masterTransistionList;

    message.append(QString("sucessfully linked transitions"));

    emit openCompleted ( sucess, message);

}


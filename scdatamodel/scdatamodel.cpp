#include "scdatamodel.h"
#include <QFile>
#include <QDebug>
#include <QStandardItemModel>
#include <QMapIterator>

SCDataModel::SCDataModel(QObject * parent) :
        QObject (parent), _reader(), _writer(0), _level(0),_topLevel(0),
        _currentState(NULL), _currentTransition(NULL),_topState(NULL),
        _qtDM(0)
{
    _qtDM = new QStandardItemModel(this);

    // TODO destructor
}

QStandardItemModel * SCDataModel::getStandardModel()
{
    return _qtDM;
}

QStandardItem * SCDataModel::makeAStateItem(SCState *st)
{

    QStandardItem * item = new QStandardItem(0,1);
    QString name = st->attributes.value("name")->asString();
    item->setData( name, Qt::UserRole );

    return item;
}

QStandardItem * SCDataModel::makeATransitionItem(SCTransition *tr)
{

    QStandardItem * item = new QStandardItem(0,0);

    return item;

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

    connect(&_reader, SIGNAL(makeANewState(StateAttributes*)), this, SLOT(handleMakeANewState(StateAttributes*)), Qt::QueuedConnection);
    connect(&_reader, SIGNAL(enterStateElement()), this, SLOT(handleTransitDown()), Qt::QueuedConnection);
    connect(&_reader, SIGNAL(leaveStateElement()), this, SLOT(handleTransitUp()), Qt::QueuedConnection);

    connect(&_reader, SIGNAL(makeANewTransistion(TransitionAttributes*)), this, SLOT(handleMakeANewTransition(TransitionAttributes*)), Qt::QueuedConnection);
    connect(&_reader, SIGNAL(leaveTransistionElement()), this, SLOT(handleLeaveTransitionElement()), Qt::QueuedConnection);
    connect(&_reader, SIGNAL(makeANewTransistionPath(QString)), this, SLOT(handleMakeANewTransitionPath(QString)), Qt::QueuedConnection);

    _reader.readFile(file);
    _reader.start();
}


void SCDataModel::getStates(QList<SCState *>& list)
{
    if ( _topState == NULL ) list = QList<SCState *>();

    return _topState->getStates(list);
}


SCState * SCDataModel::getTopState()
{
    return _topState;
}


SCState * SCDataModel::getStateByName(QString name)
{
    QList<SCState *> list;

    list.append(_topState);

    _topState->getAllStates(list);

    for(int i =0; i < list.count(); i++)
    {
        SCState *st = list.at(i);
        if ( st->attributes.value("name")->asString() == name)
            return st;
    }
    return NULL;
}


void SCDataModel::getAllStates(QList<SCState *>& list)
{
    return _topState->getAllStates(list);
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

void SCDataModel::handleMakeANewState(StateAttributes*  sa)
{
    SCState * state = NULL;

    if ( _currentState == 0 )
    {
        _topLevel = _level;
        state = new SCState();
        state->attributes.setAttributes( *sa);

        _topState = state;

        QString name;

        if ( ! sa->contains("name"))
        {
            name = "State Machine";

            StateAttributes::StateName *nm = new StateAttributes::StateName(NULL,"name",name);
            state->attributes.addItem(nm);
        }
        else
        {
            name = sa->value("name")->asString();
        }

        state->setObjectName( name);

        // insert the new state into the Qt Data Model

        QStandardItem * root = _qtDM->invisibleRootItem();
        QStandardItem * thisItem = makeAStateItem(state);
        root->appendRow( thisItem );
        state->setParentItem(root);
        state->setItem (thisItem);

        qDebug() << "adding new state at top level : " + state->attributes.value("name")->asString();
    }
    else if ( _level > _topLevel)
    {
        state = new SCState(_currentState);
        state->attributes.setAttributes( *sa);

        QString name ;
        if (! sa->contains("name"))
        {
            // is this an initial state?
            if ( sa->value("type")->asString() == "initial")
            {
                name =     _currentState->objectName() + "_initial" ;
            }
            else if ( sa->value("type")->asString() == "final")
            {
                name =     _currentState->objectName() + "_final" ;
            }
            else
            {
                name = _currentState->objectName() + "_" + QString::number( _currentState->children().count() );
            }

            StateAttributes::StateName *nm = new StateAttributes::StateName(NULL,"name",name);
            state->attributes.addItem(nm);
        }
        else
        {
            name = sa->value("name")->asString();
        }

        state->setObjectName( name );


        // insert the new state into the Qt Data Model

        QStandardItem * parent = _currentState->getItem();
        QStandardItem * thisItem = makeAStateItem(state);
        state->setItem (thisItem);
        parent->appendRow( thisItem );

        qDebug() << "adding state at level  :" + QString::number(_level) + ", name : " + name;
    }



    _currentState  = state;

    emit newStateSignal(state);


    delete sa;

}



SCTransition* SCDataModel::insertNewTransition(SCState *source, QString event )
{
     if ( source == NULL)  return NULL;

     SCTransition * transition = new SCTransition(source);

     transition->setAttributeValue("event", event);

     emit newTransitionSignal(transition);

     return transition;
 }


void SCDataModel::handleMakeANewTransition(TransitionAttributes * ta)
{
    qDebug() << "handleMakeANewTransition, : "  + ta->value("target")->asString();

    SCTransition * transition = new SCTransition(_currentState);

    if ( _currentState == 0)
        return;

    transition->attributes.setAttributes(*ta);

    _currentTransition = transition;
    _currentState->addTransistion(transition);



    delete ta;

    qDebug() << "leave handleMakeANewTransition, : "  ;

}

void SCDataModel::handleLeaveTransitionElement()
{
    qDebug() << "handleLeaveTransitionElement ";

    emit newTransitionSignal(_currentTransition);

}


void SCDataModel::handleMakeANewTransitionPath (QString pathStr)
{

    qDebug() << "handleMakeANewTransitionPath : " ;

    if ( _currentState == 0)
        return;

    if ( _currentTransition == 0 ) return;

    TransitionAttributes::TransitionPathAttribute *path =
            dynamic_cast<TransitionAttributes::TransitionPathAttribute *>( _currentTransition->attributes.value("path"));

    path->setValue( pathStr );

    _currentTransition->attributes.addItem(path);

     qDebug() << "leave handleMakeANewTransitionPath : " ;

}

void SCDataModel::handleReaderDone(bool sucess, QStringList message)
{

    if ( ! _topState  )
    {
        emit openCompleted ( sucess, message);
        return;
    }

    if ( ! sucess  )
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

    //_topState->makeTargetConnections( * masterTransistionList);

    delete masterTransistionList;

    message.append(QString("sucessfully linked transitions"));

    emit openCompleted ( sucess, message);

}


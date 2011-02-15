
#include <QString>
#include <QXmlStreamWriter>
#include <QStandardItem>
#include "scstate.h"

SCState::SCState(QObject *parent) :
    QObject(parent),
    _stateAttributes(),
   _stateCount(0),
   _parentDMItem(0),
   _thisDMItem(0)
{

}


SCState::~SCState()
{

}

void SCState::setItem(QStandardItem * item)
{
    _thisDMItem = item;
}

void SCState::setParentItem(QStandardItem * item)
{
    _parentDMItem = item;
}

QStandardItem * SCState::getParentItem()
{
    return _parentDMItem;
}

QStandardItem * SCState::getItem()
{
    return _thisDMItem;
}

bool SCState::hasBeenSized()
{
    return _stateAttributes.hasBeenSized;
}

void SCState::makeTargetConnections(QList<SCTransition*> & transitionList)
{

    for (int t = 0; t < transitionList.count(); t++)
    {
        SCTransition * trans = transitionList.value(t);
        if ( trans->target == this->objectName() )
        {
     //       connect (trans, SIGNAL(selected()), this, SLOT(handleTransitionSelected()) );
   //         connect (trans, SIGNAL(unselected()), this, SLOT(handleTransitionUnSelected()) );
        }

    }

    for (int c = 0; c < this->children().count(); c++)
    {
        SCState * child = dynamic_cast<SCState*> (this->children()[c]);
        if ( child == NULL )  continue;
       // child->makeTargetConnections( transitionList);
    }

}


void SCState::addTransistion(SCTransition * t)
{

    t->setParent(this);

//    connect (t, SIGNAL(selected()), this, SLOT(handleTransitionSelected()) );
 //   connect (t, SIGNAL(unselected()), this, SLOT(handleTransitionUnSelected()) );

    emit changed();

}



void SCState::addState(SCState * s)
{
    _stateCount++;

    s->setParent(this);

    emit changed();

}

void SCState::getAllTransitions(QList<SCTransition*> & transitionList)
{

    for (int c = 0; c < this->children().count(); c++)
    {

        // collect this state's transitions

        SCTransition* trans = dynamic_cast< SCTransition*>(this->children()[c]);
        if ( trans )
            transitionList.append(  trans );

        // collect the transitions owned by all children states

        SCState * child = dynamic_cast<SCState*> (this->children()[c]);
        if ( child  )
            child->getAllTransitions(transitionList);

    }

}


int SCState::getStateCount()
{
    return _stateCount;
}

void SCState::getStates(QList<SCState *> & stateList)
{

    for (int c = 0; c < this->children().count(); c++)
    {
        SCState* state = dynamic_cast< SCState*>(this->children()[c]);
        if ( state )
            stateList.append(  state );
    }
}


void SCState::getAllStates(QList<SCState *> & stateList)
{

    for (int c = 0; c < this->children().count(); c++)
    {
        SCState* state = dynamic_cast< SCState*>(this->children()[c]);
        if ( state )
        {
            stateList.append(  state );
            state->getAllStates(stateList);
        }
    }
}



void SCState::setPosition(QPoint pos)
{
    _stateAttributes.position.set(pos);

    emit changed();
}

void SCState::setSize(QPoint size)
{
    _stateAttributes.size.set(size);
    _stateAttributes.hasBeenSized = true;

    emit changed();
}

void SCState::setAttributes(StateAttributes & sa)
{

    _stateAttributes.size.set(  sa.size.asPoint());
    _stateAttributes.name.set(  sa.name.asString());
    _stateAttributes.position.set(  sa.position.asPoint());
    _stateAttributes.hasBeenSized = sa.hasBeenSized;

    _thisDMItem->setText(sa.name.asString());

    emit changed();

}


void SCState::getAttributes(StateAttributes& attr)
{
     attr.name.set( _stateAttributes.name.asString());
     attr.position.set( _stateAttributes.position.asPoint());
     attr.size.set( _stateAttributes.size.asPoint());
}



void SCState::writeSCVXML(QXmlStreamWriter & sw)
{
    sw.writeStartElement(QString("state"));
    sw.writeAttribute(QString("id"), _stateAttributes.name.asString());
    sw.writeAttribute(QString("position"),_stateAttributes.position.asString());
    sw.writeAttribute(QString("size"),_stateAttributes.size.asString());

    for(int i=0; i < children().length(); i++)
    {
        SCState * sc = dynamic_cast<SCState*>(children()[i]);
        if (sc)
            sc->writeSCVXML(sw);
    }

    sw.writeEndElement();
}

void SCState::setStateName(QString n)
{
    this->setObjectName(n);

    emit changed();

}



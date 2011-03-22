
#include <QString>
#include <QXmlStreamWriter>
#include <QStandardItem>
#include "scstate.h"
#include <QVariant>


SCState::SCState(QObject *parent) :
    QObject(parent),
    attributes(this, "stateAttributes"),
   _stateCount(0),
   _parentDMItem(0),
   _thisDMItem(0)
{
   initCommon();
}


SCState::SCState(const SCState& st) :
    QObject(st.parent()),
    attributes(st.attributes),
   _stateCount(st._stateCount),
   _parentDMItem(st._parentDMItem),
   _thisDMItem(st._thisDMItem)
{
    initCommon();
}

SCState::~SCState()
{

}

void SCState::initCommon()
{
    StateAttributes::StateName * name = new StateAttributes::StateName (this, "name",QString());
    StateAttributes::StateSize * size = new StateAttributes::StateSize (this, "size",QPoint(100,50));
    StateAttributes::StatePosition * position = new StateAttributes::StatePosition (this, "position",QPoint(0,0));

    attributes.addItem(name);
    attributes.addItem(size);
    attributes.addItem(position);

}

QString SCState::getAttributeValue(QString key)
{
    IAttribute * attr = attributes.value(key);
    if ( attr )
    {
        return attr->asString();
    }
    else return QString();
}

void SCState::setAttributeValue(QString key, QString value)
{
    IAttribute * attr = attributes.value(key);
    if ( attr )
    {
        attr->setValue(value);
    }
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

void SCState::setSize(QPointF &size)
{
    StateAttributes::StateSize * sz = dynamic_cast<StateAttributes::StateSize *> (attributes.value("size"));
    sz->setValue(size);
}

void SCState::setSize(QPoint &size)
{
    QPointF pointF = QPointF(size.x(),size.y());
    setSize(pointF);
}

void SCState::setPosition(QPointF &position)
{
    StateAttributes::StatePosition * pos = dynamic_cast<StateAttributes::StatePosition *> (attributes.value("position"));
    pos->setValue(position);
}

#if 0
void SCState::makeTargetConnections(QList<SCTransition*> & transitionList)
{

    for (int t = 0; t < transitionList.count(); t++)
    {
        SCTransition * trans = transitionList.value(t);
        TransitionAttributes attr;
        trans->getAttributes(attr);
        if ( attr.target == this->objectName() )
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
#endif


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

void SCState::getTransitions(QList<SCTransition*> & transitionList)
{

    for (int c = 0; c < this->children().count(); c++)
    {

        // collect this state's transitions

        SCTransition* trans = dynamic_cast< SCTransition*>(this->children()[c]);
        if ( trans )
            transitionList.append(  trans );
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


void SCState::writeSCVXML(QXmlStreamWriter & sw)
{
    sw.writeStartElement(QString("state"));
    sw.writeAttribute(QString("id"), attributes.value("name")->asString());
    sw.writeAttribute(QString("position"),attributes.value("position")->asString());
    sw.writeAttribute(QString("size"),attributes.value("size")->asString());

    for(int i=0; i < children().length(); i++)
    {
        SCState * sc = dynamic_cast<SCState*>(children()[i]);
        if (sc)
            sc->writeSCVXML(sw);

        SCTransition * st = dynamic_cast<SCTransition*>(children()[i]);
        if (st)
            st->writeSCVXML(sw);
    }

    sw.writeEndElement();
}


void SCState::setStateName(QString n)
{
    this->setObjectName(n);

    attributes.value("name")->setValue(n);
}



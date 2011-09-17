/*
    visualsc - Visual State Chart Editing for SCXML State Engines
    Copyright (C) 2011  David W. Drell (david@davidwdrell.net) and
    Contributing Authors:


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QString>
#include <QXmlStreamWriter>
#include "scstate.h"
#include <QVariant>
#include <QDebug>

SCState::SCState(QObject *parent) :
    SCItem(parent),
    attributes(this, "stateAttributes"),
    _IdTextBlock(new TextBlock())
{
   initCommon();
}


SCState::SCState(const SCState& st) :
    SCItem(st.parent()),
    attributes(st.attributes),
    _IdTextBlock(new TextBlock())
{
    initCommon();
}



SCState::SCState( bool topState) :
    SCItem(NULL),
    attributes(this, "stateAttributes"),
    _IdTextBlock(new TextBlock())
{
    initCommon();

    if ( topState )
    {
        this->setStateName( "State Machine" );
    }
}


SCState::~SCState()
{

}

void SCState::initCommon()
{
    QString defaultName = QString();


    SCState * parent = dynamic_cast<SCState *>(this->parent());

    if  ( parent )
    {
        QString parentsName = parent->attributes.value("name")->asString();

        int childCount = parent->getStateCount();

        defaultName = "sub_of_" + parentsName + "_" + QString::number(childCount);
    }


    StateAttributes::StateName * name = new StateAttributes::StateName (this, "name",defaultName);
    SizeAttribute * size = new SizeAttribute (this, "size",QPoint(100,50));
    PositionAttribute * position = new PositionAttribute (this, "position",QPoint(0,0));

    attributes.addItem(name);
    attributes.addItem(size);
    attributes.addItem(position);

    this->setObjectName(defaultName);// to support debug tracing

    _IdTextBlock->setParent(this);
    _IdTextBlock->setText(name->asString());
    connect (name, SIGNAL(changed(IAttribute*)), this, SLOT(handleNameChanged(IAttribute*)));
    connect (_IdTextBlock, SIGNAL(textChanged()), this, SLOT(handleTextBlockChanged()));

    qDebug()<< "_IdTextBlock = " +QString::number((int)_IdTextBlock) +", state = " + defaultName;
}


TextBlock* SCState::getIDTextBlock()
{
    return  _IdTextBlock;
}

void SCState::handleTextBlockChanged()
{
    qDebug()<<"SCState::handleTextBlockChanged";
    StateAttributes::StateName * name = dynamic_cast<StateAttributes::StateName *>(attributes["name"]);
  //  disconnect (name, SIGNAL(changed(IAttribute*)), this, SLOT(handleNameChanged(IAttribute*)));
    QString nameText = _IdTextBlock->getText();
    this->setObjectName(nameText);
    name->setValue(nameText);
  //  connect (name, SIGNAL(changed(IAttribute*)), this, SLOT(handleNameChanged(IAttribute*)));
}

void SCState::handleNameChanged(IAttribute *name)
{
    this->setObjectName(name->asString());// to support debug tracing
    _IdTextBlock->setText(name->asString());
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

IAttributeContainer * SCState::getAttributes()
{
    return   & attributes;
}

void SCState::setAttributeValue(QString key, QString value)
{
    IAttribute * attr = attributes.value(key);
    if ( attr )
    {
        attr->setValue(value);
    }
}


void SCState::setSize(QPointF &size)
{
    SizeAttribute * sz = dynamic_cast<SizeAttribute *> (attributes.value("size"));
    sz->setValue(size);
}

void SCState::setSize(QPoint &size)
{
    QPointF pointF = QPointF(size.x(),size.y());
    setSize(pointF);
}

void SCState::setPosition(QPointF &position)
{
    PositionAttribute * pos = dynamic_cast<PositionAttribute *> (attributes.value("position"));
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
    QList<SCState*> children;
    this->getStates(children);
    return (children.count());
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

        TextBlock * tb = dynamic_cast<TextBlock*>(children()[i]);
        if (tb)
            tb->writeSCVXML(sw);

    }

    sw.writeEndElement();
}


void SCState::setStateName(QString n)
{
    this->setObjectName(n);


    // no need to set the textblock here, its been connected to the name attribute
   // _IdTextBlock.setText(n);

    attributes.value("name")->setValue(n);
}



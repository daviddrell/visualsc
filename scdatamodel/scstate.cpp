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
    _IdTextBlock(new SCTextBlock())
{
   initCommon();
}


SCState::SCState(const SCState& st) :
    SCItem(st.parent()),
    attributes(st.attributes),
    _IdTextBlock(new SCTextBlock())
{
    initCommon();
}



SCState::SCState( bool topState) :
    SCItem(NULL),
    attributes(this, "stateAttributes"),
    _IdTextBlock(new SCTextBlock())
{
    initCommon();

    if ( topState )
    {
        this->setStateName( "State Machine" );
    }
}


SCState::~SCState()
{
    // moved transition reference deletion to the deconstructor for the transition

    // also delete any transitions that were transiting in and their reference to the source state

    this->removeAllTransitionsIn();
    delete _IdTextBlock;
}

void SCState::initCommon()
{
   // _stateBoxGraphic = NULL;
    QString defaultName = QString();


    SCState * parent = dynamic_cast<SCState *>(this->parent());

    if  ( parent )
    {
        QString parentsName = parent->attributes.value("name")->asString();

        int childCount = parent->getStateCount();

        defaultName = "" + parentsName + "_" + QString::number(childCount);
    }


    DEFAULT_PROPERTIES_LIST << "name" << "size" << "position" <<"type" <<"onEntryAction"<<"onExitAction"<<"finalState"<<"initialState"; // type is added to the state in scxml reader.

    StateName * name = new StateName (this, "name",defaultName);
    SizeAttribute * size = new SizeAttribute (this, "size",QPoint(100,50));
    PositionAttribute * position = new PositionAttribute (this, "position",QPoint(0,0));
    StateString * type = new StateString(this, "type", "default type");
    StateString * onEntryAction = new StateString(this, "onEntryAction","default entry action");
    StateString * onExitAction = new StateString(this, "onExitAction", "default exit action");
    StateString * finalState = new StateString(this, "finalState", "false");
    StateString * initialState = new StateString(this, "initialState", "false");
    StateString * isParallelState = new StateString(this, "isParallelState", "false");


    attributes.addItem(name);
    attributes.addItem(size);
    attributes.addItem(position);
    attributes.addItem(type);
    attributes.addItem(onEntryAction);
    attributes.addItem(onExitAction);
    attributes.addItem(finalState);
    attributes.addItem(initialState);
    attributes.addItem(isParallelState);

    this->setObjectName(defaultName);// to support debug tracing

    _IdTextBlock->setParent(this);
    _IdTextBlock->setText(name->asString());     // default state text
    //_IdTextBlock->setText("DEFAULT ID TEXT");

    // connect change the state name to handleNameChanged


   // connect (name, SIGNAL(changed(IAttribute*)), this, SLOT(handleNameChanged(IAttribute*)));
   // connect (size, SIGNAL(changed(IAttribute*)), this, SLOT(handleSizeChanged(IAttribute*)));

    // connect changing the SCTextBlock to handleTextBlockChanged()
    connect (_IdTextBlock, SIGNAL(textChanged()), this, SLOT(handleTextBlockChanged()));

    qDebug()<< "_IdTextBlock = " +QString::number((int)_IdTextBlock) +", state = " + defaultName;
}

/**
 * @brief SCState::setText
 * @param text
 *
 * emits textChanged
 *
 * updates the datamodel for an item's textblock and then signals textChanged
 *
 * SIGNAL textChanged
 *
 *
 * connect (SCTextBlock, SIGNAL(textChanged()), SCState, SLOT(handleTextBlockChanged()));
 * connect (SCTextBlock, SIGNAL(textChanged()), SelectableTextBlock, SLOT(handleTextChanged()), Qt::QueuedConnection);
 *
 */
void SCState::setText(QString text)
{
    _IdTextBlock->setText(text);
    /*if  ( text != _text)
    {
         _text = text;
        this->setObjectName(text);
        emit textChanged();
    }*/
}



/**
 * @brief SCState::setPosition
 * @param position
 *
 * SIGNAL
 * connect in scformview
 * connect(st, SIGNAL(positionChangedInDataModel(SCState*,QPointF)), this, SLOT(handleItemPositionChangedInDataModel(SCState*,QPointF)));
 *
 * sets the position attribute of the state and then signals the formview
 *
 */
void SCState::setPosition(QPointF &position)
{
    PositionAttribute * pos = dynamic_cast<PositionAttribute *> (attributes.value("position"));
    pos->setValue(position);
    emit positionChangedInDataModel(this, position);
}

/**
 * @brief SCState::setSize
 * @param size
 *
 * SIGNAL
 * connect in scformview
 * connect(SCState, SIGNAL(sizeChangedInDataModel() ,  SCFormView, handleItemSizeChangedInDataModel)
 *
 * sets the size attribute then signals the formview to update the property table if necessary.
 *
 */
void SCState::setSize(QPointF &size)
{
    SizeAttribute* sz = dynamic_cast<SizeAttribute*>(attributes.value("size"));
    sz->setValue(size);
    emit sizeChangedInDataModel(this, size);
}

SCTextBlock* SCState::getIDTextBlock()
{
    return  _IdTextBlock;
}

/**
 * @brief SCState::handleTextBlockChanged
 *
 * SLOT
 * connect in scstate
 * connect (SCTextBlock, SIGNAL(textChanged()), SCState, SLOT(handleTextBlockChanged()));
 * connects an SCTextBlock textChanged to an SCState handleTextBlockChanged
 *
 *
 * propogates a change of a statebox's main textbox (SCTextBlock) to update the state as well
 *
 *                   -> SelectableTextBlock changed
 * textBlock Changed -> SCState's name changed -> tree/table update
 *
 *
 * SIGNAL
 * connect in scformview
 * connect(SCState, SIGNAL(nameChangedInDataModel(SCState*,QString)), SCFormView, SLOT(handleItemNameChangedInDataModel(SCState*,QString)));
 *
 */
void SCState::handleTextBlockChanged()
{
    qDebug()<<"SCState::handleTextBlockChanged";
    StateName * name = dynamic_cast<StateName *>(attributes["name"]);

    QString nameText = _IdTextBlock->getText();
    this->setObjectName(nameText);
    name->setValue(nameText);
    emit nameChangedInDataModel(this,nameText); // connected to SCFormView::handleItemNameChangedInDataModel()
}

/*
void SCState::setGraphic(StateBoxGraphic* graphic)
{
    _stateBoxGraphic = graphic;
}
*/
/*
void SCState::updateGraphic()
{

}
*/

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

bool SCState::removeAttribute(QString key)
{
    if(DEFAULT_PROPERTIES_LIST.indexOf(key)==-1)
    {
        attributes.remove(key);
        return true;
    }
    return false;
}

void SCState::addAttribute(QString key, QString value)
{
    qDebug()  << "adding state property: " << key;
    TransitionAttributes::TransitionStringAttribute * attr = new TransitionAttributes::TransitionStringAttribute (this, key,QString());
    attr->setValue(value);
    qDebug() << "attributes count before " << attributes.count();
    attributes.addItem(attr);
    qDebug() << "attributes count after " << attributes.count();
}

void SCState::setAttributeValue(QString key, QString value)
{
    IAttribute * attr = attributes.value(key);
    if ( attr )
    {
        attr->setValue(value);
    }
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


QList<SCTransition*> SCState::getTransitionsIn()
{
    return  _transitingTransitionsIn;
}
QList<SCTransition*> SCState::getTransitionsTerminating()
{
    return _transitionsTerminatingHere;
}





/**
 * @brief SCState::removeTargetsTransitionIn
 *
 * Called when this state is deleted.
 *
 * for any state targeted, remove the transition targeting that state from its inbound transitions list
 *
 */

void SCState::removeTargetsTransitionIn()
{
    qDebug() <<"SCState::removeTargetsTransitonIn";
    SCTransition* trans;
    SCState* target;
    for(int i = 0; i < _transitingTransitionsOut.count();i++)
    {
        trans = _transitingTransitionsOut.at(i);
        target = trans->targetState();
        target->removeTransitionReferenceIn(trans);
        //  qDebug() << "... removing " << _transitingTransitionsOut.count() << " Out Transitions for state: " <<target->objectName();

    }
}

void SCState::removeSourcesTransitionOut()
{
    qDebug() << "SCState::removeSourcesTransitionIn";
    SCTransition* trans;
    SCState* source;

    for(int i = 0; i < _transitingTransitionsIn.count(); i++)
    {
        trans = _transitingTransitionsIn.at(i);
        source = trans->parentSCState();
        source->removeTransitionReferenceOut(trans);
        qDebug() << "... removing out transition for state: " <<  source->objectName();
        if(trans)
        {
            qDebug() << "deleting trans: " << trans;
            delete trans;
        }
    }
}

/**
 * @brief SCState::removeTransitionIn
 * @param trans
 *
 * Remove the passed transition from this state's inbound transitions list
 *
 *
 */
void SCState::removeTransitionReferenceIn(SCTransition* trans)
{
    int k = _transitingTransitionsIn.indexOf(trans);
    if(k!=-1)
    {
        _transitingTransitionsIn.removeAt(k);
        return;
    }
    qDebug() << "ERROR SCState::deleteTransitionsIn transition not found!";
}



void SCState::removeTransitionReferenceOut(SCTransition* trans)
{
    int k = _transitingTransitionsOut.indexOf(trans);
    if(k!=-1)
    {
        _transitingTransitionsOut.removeAt(k);
        return;
    }
    qDebug() << "ERROR SCState::deleteTransitionsOut transition not found!";
}

void SCState::addTransitionReference(SCTransition* t, TransitionTransitDirection d)
{
    if ( d == kTransitOut)
    {
        _transitingTransitionsOut.append(t);
    }
    if ( d == kTransitIn)
    {
        _transitingTransitionsIn.append(t);
    }
    if ( d == kDestination)
    {
        _transitionsTerminatingHere.append(t);
    }
}

/**
 * @brief SCState::deleteAllInTransitions
 * @param state
 *
 * deletes all inbound transitions on this state
 *
 * Called when a SCState is deleted and ALL of the inbound transitions should also be deleted
 *
 */
void SCState::removeAllTransitionsIn()
{
    SCState* state = this;

    if(!state)
    {
        qDebug() << "delete In Transitions skipping because this is not a state!";
        return;
    }

    QList<SCTransition*> dest = state->getTransitionsIn();
    qDebug() << "Deleting destination transitions for state: " << state->attributes.value("name")->asString() << " with # inbound transitions "<<dest.count();
    SCTransition* tr;
    for(int i = 0; i < dest.count(); i++)
    {
        tr = dest.at(i); 
        qDebug() << "deleting transition: tr " << tr->attributes.value("event")->asString();
        delete tr;
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

SCState* SCState::getStateByName(QString name)
{
    SCState *target=NULL;
    qDebug() <<"passed "<< name<<"getStateByName: " << name<< "num children: " << children().count();
    for (int c = 0; c < this->children().count(); c++)
    {
        SCState* state = dynamic_cast< SCState*>(this->children()[c]);
        if ( state )
        {
            qDebug() <<"state object name: "<<state->objectName()<<" state attribute name: " <<state->attributes.value("name")->asString();
            if ( state->objectName() == name)
            {
                qDebug() <<"returning state as target... : "<<state->objectName();
                return state;
            }
            else
            {
                target = state->getStateByName(name);
                if ( target != NULL)
                    return target;
            }
        }
    }

    return target;
}


int SCState::getLevel()
{
    return _level;
}

void SCState::setLevel(int level)
{
    _level = level;
}


SCState * SCState::getParentState()
{
    return dynamic_cast<SCState*>(this->parent());
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


/**
 * @brief SCState::writeSCVXML
 * @param sw
 *
 * scxml writer
 *
 * called by the data model when the save button is pressed in the graphics view.
 *
 * this function is specific to when states are saved
 *
 */
void SCState::writeSCVXML(QXmlStreamWriter & sw)
{
    // xml start element "state"
    sw.writeStartElement(QString("state"));

    // get the keys of the attributes
    QMapIterator<QString, IAttribute*> i(attributes);

    // for every attribute of this state, write into the scxml
    while(i.hasNext())
    {
        QString key = i.next().key();
        qDebug() << "writing " << key <<"...";
        sw.writeAttribute(key, attributes.value(key)->asString());
    }


    // additionally, write each of the attributes of this state's children.
    for(int k = 0 ; k < children().length(); k++)
    {
        SCState * sc = dynamic_cast<SCState*>(children()[k]);
        if (sc)
            sc->writeSCVXML(sw);

        SCTransition * st = dynamic_cast<SCTransition*>(children()[k]);
        if (st)
            st->writeSCVXML(sw);

        SCTextBlock * tb = dynamic_cast<SCTextBlock*>(children()[k]);
        if (tb)
            tb->writeSCVXML(sw);
    }




    /*
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

        SCTextBlock * tb = dynamic_cast<SCTextBlock*>(children()[i]);
        if (tb)
            tb->writeSCVXML(sw);

    }*/

    sw.writeEndElement();
}


void SCState::setStateName(QString n)
{
    this->setObjectName(n);


    // no need to set the textblock here, its been connected to the name attribute
   // _IdTextBlock.setText(n);

    attributes.value("name")->setValue(n);
}



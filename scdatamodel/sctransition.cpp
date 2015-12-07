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
#include "sctransition.h"
#include "scstate.h"
#include "QXmlStreamWriter"
#include "transitionattributes.h"
#include <QDebug>
#include <QFont>



SCTransition::SCTransition(QObject * parent):
        SCItem(parent),
        attributes(this, "transition.attributes"),
        _eventTextBlock(new SCTextBlock()),
        _targetState(NULL)
{
    /*
     event
     cond
     target
     type  : "internal" "external"
     position
     path
     */

    // items in this list cannot be deleted by the user
    DEFAULT_ATTRIBUTES_LIST << "target" << "event" << "comments" << "path" << "uid";

    //DO_NOT_DISPLAY_HASH.insert("uid",0);

    // SCTransition will load with target, event, commentary, and path
    TransitionStringAttribute * target = new TransitionStringAttribute (this, "target",QString());
    attributes.addItem(target);

    TransitionStringAttribute * event = new TransitionStringAttribute (this, "event","event");
    attributes.addItem(event);
    this->_eventTextBlock->setText(event->asString());
    this->setObjectName(event->asString());

    TransitionStringAttribute * comments = new TransitionStringAttribute (this, "comments",QString());
    attributes.addItem(comments);

    QList<QPointF> emptyPath;
    TransitionPathAttribute * path = new TransitionPathAttribute (this, QString("path"),emptyPath);
    attributes.addItem(path);

    TransitionStringAttribute* uid = new TransitionStringAttribute(this, "uid", QString());
    attributes.addItem(uid);

    TransitionStringAttribute* connectToFinished = new TransitionStringAttribute(this, "connectToFinished", "false");
    attributes.addItem(connectToFinished);

    // handle textBlock Changed for the event text box
    connect(_eventTextBlock, SIGNAL(textChanged()), this, SLOT(handleTextBlockChanged()));
    connect(event, SIGNAL(changed(IAttribute*)), _eventTextBlock, SLOT(handleAttributeChanged(IAttribute*)));

    // set the event sctextblock's parent
    _eventTextBlock->setParent(this);
}

SCTransition::SCTransition(const SCTransition& rhs):
    SCItem(NULL),
    attributes(this, "transition.attributes"),
    _eventTextBlock(new SCTextBlock()),
    _targetState(NULL)
{
    // items in this list cannot be deleted by the user
    DEFAULT_ATTRIBUTES_LIST << "target" << "event" << "comments" << "path" << "uid";

    //DO_NOT_DISPLAY_HASH.insert("uid",0);

    // SCTransition will load with target, event, commentary, and path
    TransitionStringAttribute * target = new TransitionStringAttribute (this, "target",rhs.attributes.value("target")->asString());
    attributes.addItem(target);

    TransitionStringAttribute * event = new TransitionStringAttribute (this, "event",rhs.attributes.value("event")->asString());
    attributes.addItem(event);
    this->setObjectName(event->asString());

    TransitionStringAttribute * comments = new TransitionStringAttribute (this, "comments",rhs.attributes.value("comments")->asString());
    attributes.addItem(comments);

    QList<QPointF> pathPts;
    TransitionPathAttribute* srcPathAttr = dynamic_cast<TransitionPathAttribute*> (rhs.attributes.value("path"));
    foreach(QPointF pt, srcPathAttr->asQPointFList())
    {
        pathPts.append(pt);
    }

    TransitionPathAttribute * path = new TransitionPathAttribute (this, QString("path"),pathPts);
    attributes.addItem(path);

    TransitionStringAttribute* uid = new TransitionStringAttribute(this, "uid", QUuid::createUuid().toString() );
    attributes.addItem(uid);

    TransitionStringAttribute* connectToFinished = new TransitionStringAttribute(this, "connectToFinished", rhs.attributes.value("connectToFinished")->asString());
    attributes.addItem(connectToFinished);

    // handle textBlock Changed for the event text box
    connect(_eventTextBlock, SIGNAL(textChanged()), this, SLOT(handleTextBlockChanged()));
    connect(event, SIGNAL(changed(IAttribute*)), _eventTextBlock, SLOT(handleAttributeChanged(IAttribute*)));

    // set the event sctextblock's parent
    _eventTextBlock->setParent(this);
    this->_eventTextBlock->setText(event->asString());



}

SCTransition::~SCTransition()
{
    qDebug()<< "SCTransition destroyed: " + QString::number((long)this);
    delete _eventTextBlock;
}

TransitionPathAttribute* SCTransition::getPathAttr()
{
    return dynamic_cast<TransitionPathAttribute*>(this->attributes.value("path"));
}

void SCTransition::setPathAttr(QString pathString)
{
    this->getPathAttr()->setValue(pathString);
}


void SCTransition::handleTargetStateNameChanged(IAttribute * stateName)
{
    this->attributes.value("target")->setValue(stateName->asString());
}

TransitionStringAttribute* SCTransition::getTransStringAttr(QString key)
{
    return dynamic_cast<TransitionStringAttribute*>(this->attributes.value(key));
}

SCState* SCTransition::parentSCState()
{
    return dynamic_cast<SCState *>(this->parent());
}

void SCTransition::setText(QString eventText)
{
    _eventTextBlock->setText(eventText);
}

void SCTransition::setUid(QString uid)
{
    attributes.value("uid")->setValue(uid);
}

QString SCTransition::getUid()
{
    return attributes.value("uid")->asString();
}

QString SCTransition::getUidFirstName()
{
    QStringList qsl = getUid().split("-");
    return qsl.at(0).mid(1,qsl.at(0).size());
}

bool SCTransition::doNotPrint(QString attribute)
{
    return DO_NOT_DISPLAY_HASH.contains(attribute);
}

int SCTransition::doNotPrintSize()
{
    return DO_NOT_DISPLAY_HASH.size();
}

/**
 * @brief SCTransition::handleTextBlockChanged
 *
 *
 * when the textblock is changed through the graphics view, use set EventName to set the object name and attribute value for the event of this transition
 *
 */
void SCTransition::handleTextBlockChanged()
{
    //this->attributes.value("event")->setValue(_eventTextBlock->getText());
    this->setEventName(_eventTextBlock->getText());
}

/**
 * @brief SCTransition::setEventName
 * @param text
 *
 * called by scformview when the property table is changed for a transition's event name
 */
void SCTransition::setEventName(QString text)
{
    this->setObjectName(text);
    attributes.value("event")->setValue(text);
}

QString SCTransition::getEventName()
{
    return attributes.value("event")->asString();
}

SCState *SCTransition::targetState()
{
    return _targetState;
}

/**
 * @brief SCTransition::setTargetState
 * @param state
 *
 * reselects the transition's target state
 *
 * SIGNAL
 * changedTarget(SCTransiton*, SCState*)
 *
 * notifies the SCFormView and SCGraphicsView to update the transition change.
 */
void SCTransition::setTargetState(SCState* state)
{
    // first update the formview and graphicsview
    // emit changedTarget(this, state);
    if(_targetState)
    {
//        disconnect(_targetState,SIGNAL(markedForDeletion(QObject*)), this, SLOT(detachFromSink(QObject*)));
    }
    _targetState = state;

    // set the transition to point to this new state
    //this->setUid(_targetState->getUid());
//    connect(_targetState,SIGNAL(markedForDeletion(QObject*)), this, SLOT(detachFromSink(QObject*)));
}

SCTextBlock* SCTransition::getEventTextBlock()
{
    return _eventTextBlock;
}

QString SCTransition::getAttributeValue(QString key)
{
    IAttribute * attr = attributes.value(key);
    if ( attr )
    {
        return attr->asString();
    }
    else return QString();
}
/*
void SCTransition::addTextBlock(QString key, QString value)
{
    SCTextBlock* tb = new SCTextBlock();

    tb->setObjectName(key);
    tb->setText(value);

    this->_textBlocks.append(tb);
}

QList<SCTextBlock*> SCTransition::getTextBlocks()
{
    return _textBlocks;
}

SCTextBlock* SCTransition::getTextBlock(QString textBlockName)
{
    for(int i = 0; i < _textBlocks.count(); i++)
    {

                                                if(_textBlocks.at(i)->objectName()== textBlockName)
                                                return _textBlocks.at(i);
    }
    return NULL;
}
*/


void SCTransition::setFont(QFont *font)
{
    SCTextBlock* idtb = this->getEventTextBlock();

    if(!font->family().isEmpty())
    {
        idtb->getFontFamilyAttr()->setValue(font->family());
    }

    if(font->pointSize()!=1)
    {
        idtb->getFontSizeAttr()->setValue(font->pointSize());
    }

    idtb->getFontBoldAttr()->setValue(font->bold());
}


IAttributeContainer * SCTransition::getAttributes()
{
    return & attributes;
}

void SCTransition::addAttribute(QString key, QString value)
{
    TransitionStringAttribute * attr = new TransitionStringAttribute (this, key,QString());
    attr->setValue(value);
    attributes.addItem(attr);
}

bool SCTransition::removeAttribute(QString key)
{
    if(DEFAULT_ATTRIBUTES_LIST.indexOf(key)==-1)   // remove the attribute it it's not part of the core properties
    {
        attributes.remove(key);
        return true;
    }
    return false;
}

void SCTransition::setAttributeValue(QString key, QString value)
{
    IAttribute * attr = attributes.value(key);
    if ( attr )
    {
        attr->setValue(value);
    }
}

bool SCTransition::isConnectToFinished()
{
    return (attributes.value("connectToFinished")->asString()=="true");
}

void SCTransition::deleteSafely()
{
    qDebug() << "emit markedForDeletion in SCTransiton::deleteSafely()";

    // unhook the transition from its parent and target before deleting it
    this->detachFromSink(this);
    this->detachFromSource(this);

    emit markedForDeletion(this);
    this->deleteLater();
}

void SCTransition::detachFromSource(QObject*)
{
    SCState* source = this->parentSCState();
    if(source)
    {
        source->removeTransitionReferenceOut(this);
        qDebug() << "detachFromSource Transition: "<< this->getEventName() << "from source: " << source->getName();
    }
}

void SCTransition::detachFromSink(QObject*)
{
    SCState* target = this->targetState();
    if(target)
    {
        target->removeTransitionReferenceIn(this);
        qDebug() << "detachFromSink Transition: " << this->getEventName() << " from target: " << target->getName();
    }
}

/*
void SCTransition::handleLineSelected()
{
    emit selected();
}

void SCTransition::handleLineUnSelected()
{
    emit unselected();
}*/


/**
 * @brief SCTransition::writeSCVXML
 * @param sw
 *
 * writes each specified attribute from the transition into the saved .scxml file.
 *
 */
void SCTransition::writeSCVXML(QXmlStreamWriter & sw)
{
    sw.writeStartElement(QString("transition"));
    QMapIterator<QString, IAttribute*> i(attributes);       // get the keys of the attributes

    while(i.hasNext())              // for every attribute, write into the scxml
    {
        QString key = i.next().key();
        qDebug() << "writing " << key <<"...";
        sw.writeAttribute(key, attributes.value(key)->asString());
    }

    // additionally, write each of the attributes of this transitions's children.
    for(int k = 0 ; k < children().length(); k++)
    {
        SCTextBlock * tb = dynamic_cast<SCTextBlock*>(children()[k]);
        if (tb)
            tb->writeSCVXML(sw);
    }

    sw.writeEndElement();
}


#if 0
void SCTransition::setPath(QList<QPointF>& path)
{
    TransitionPathAttribute * path = attributes.value("path");

    path->
        }

void SCTransition::getPath(QList<QPointF>& path)
{
    path = _attributes.path.pathPoints;
}
#endif


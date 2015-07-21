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

#include <QDebug>

SCTransition::SCTransition(QObject * parent):
        SCItem(parent),
        attributes(this, "transition.attributes"),
        _eventTextBlock(new SCTextBlock())

{
    /*
     event
     cond
     target
     type  : "internal" "external"
     position
     path
     */

    DEFAULT_PROPERTIES_LIST << "target" << "event" << "comments" << "path";


    // SCTransition will load with target, event, commentary, and path
    TransitionAttributes::TransitionStringAttribute * target = new TransitionAttributes::TransitionStringAttribute (this, "target",QString());
    attributes.addItem(target);

    TransitionAttributes::TransitionStringAttribute * event = new TransitionAttributes::TransitionStringAttribute (this, "event",QString());
    attributes.addItem(event);

    TransitionAttributes::TransitionStringAttribute * comments = new TransitionAttributes::TransitionStringAttribute (this, "comments",QString());
    attributes.addItem(comments);

    QList<QPointF> emptyPath;
    TransitionAttributes::TransitionPathAttribute * path = new TransitionAttributes::TransitionPathAttribute (this, QString("path"),emptyPath);
    attributes.addItem(path);



    _eventTextBlock->setParent(this);
    _eventTextBlock->setText("event");
    // handle textBlock Changed for the event text box
    connect(_eventTextBlock, SIGNAL(textChanged()), this, SLOT(handleTextBlockChanged()));

    /*

    TransitionAttributes::TransitionStringAttribute * cond = new TransitionAttributes::TransitizgonStringAttribute (this, "cond",QString());
    attributes.addItem(cond);

    TransitionAttributes::TransitionStringAttribute * type = new TransitionAttributes::TransitionStringAttribute (this, "type",QString("internal"));
    attributes.addItem(type);


    TransitionAttributes::TransitionPositionAttribute * position = new TransitionAttributes::TransitionPositionAttribute (this, "position",QPointF(0,0));
    attributes.addItem(position);
*/


}

SCTransition::~SCTransition()
{
    qDebug()<< "SCTransition destroyed: " + QString::number((long)this);
    delete _eventTextBlock;
}

void SCTransition::setText(QString eventText)
{
    _eventTextBlock->setText(eventText);
}

void SCTransition::handleTextBlockChanged()
{
    IAttribute* event = attributes.value("event");
    QString eventText = _eventTextBlock->getText();

    this->setObjectName(eventText);
    event->setValue(eventText);
    emit nameChanged(this, eventText);
}

SCState *SCTransition::targetState()
{
    return _targetState;
}

void SCTransition::setTargetState(SCState* state)
{
    _targetState = state;
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
IAttributeContainer * SCTransition::getAttributes()
{
    return & attributes;
}

void SCTransition::addAttribute(QString key, QString value)
{
    TransitionAttributes::TransitionStringAttribute * attr = new TransitionAttributes::TransitionStringAttribute (this, key,QString());
    attr->setValue(value);
    attributes.addItem(attr);
}

bool SCTransition::removeAttribute(QString key)
{
    if(DEFAULT_PROPERTIES_LIST.indexOf(key)==-1)   // remove the attribute it it's not part of the core properties
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

        if(key != "path")           // do a special write for the path
        {
            qDebug() << "writing " << key <<"...";
            sw.writeAttribute(key, attributes.value(key)->asString());
        }
    }

    if (  attributes.contains("path"))  // write all the path values in a separate element
    {
         sw.writeStartElement(QString("path"));
         QString path = attributes.value("path")->asString();
         sw.writeAttribute(QString("d"),path);
         sw.writeEndElement();
    }

    // additionally, write each of the attributes of this state's children.
    for(int k = 0 ; k < children().length(); k++)
    {

        SCTransition * st = dynamic_cast<SCTransition*>(children()[k]);
        if (st)
            st->writeSCVXML(sw);

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


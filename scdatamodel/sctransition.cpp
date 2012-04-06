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
        SCItem(parent), attributes(this, "transition.attributes")
{
    /*
     event
     cond
     target
     type  : "internal" "external"
     position
     path
     */

    TransitionAttributes::TransitionStringAttribute * target = new TransitionAttributes::TransitionStringAttribute (this, "target",QString());
    attributes.addItem(target);

    TransitionAttributes::TransitionStringAttribute * cond = new TransitionAttributes::TransitionStringAttribute (this, "cond",QString());
    attributes.addItem(cond);

    TransitionAttributes::TransitionStringAttribute * type = new TransitionAttributes::TransitionStringAttribute (this, "type",QString("internal"));
    attributes.addItem(type);

    TransitionAttributes::TransitionPositionAttribute * position = new TransitionAttributes::TransitionPositionAttribute (this, "position",QPointF(0,0));
    attributes.addItem(position);

    QList<QPointF> emptyPath;
    TransitionAttributes::TransitionPathAttribute * path = new TransitionAttributes::TransitionPathAttribute (this, QString("path"),emptyPath);
    attributes.addItem(path);


}

SCTransition::~SCTransition()
{

}

SCState *SCTransition::targetState()
{
    return _targetState;
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

IAttributeContainer * SCTransition::getAttributes()
{
    return & attributes;
}

void SCTransition::setAttributeValue(QString key, QString value)
{
    IAttribute * attr = attributes.value(key);
    if ( attr )
    {
        attr->setValue(value);
    }
}


void SCTransition::handleLineSelected()
{
    emit selected();
}

void SCTransition::handleLineUnSelected()
{
    emit unselected();
}


void SCTransition::writeSCVXML(QXmlStreamWriter & sw)
{
    sw.writeStartElement(QString("transition"));
    sw.writeAttribute(QString("target"), attributes.value("target")->asString());

    if (  attributes.contains("path"))
    {
         sw.writeStartElement(QString("path"));
         QString path = attributes.value("path")->asString();
         sw.writeAttribute(QString("d"),path);
         sw.writeEndElement();
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


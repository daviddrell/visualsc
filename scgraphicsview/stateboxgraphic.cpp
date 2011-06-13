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

#include "stateboxgraphic.h"

#include <QBrush>
#include <QLinearGradient>
#include <QDebug>
#include <QTextDocument>
#include "math.h"
#include "scstate.h"
#include <QTimer>



StateBoxGraphic::StateBoxGraphic(QGraphicsObject * parent,SCState *stateModel):
        SelectableBoxGraphic(parent),
        _title(),
        _stateModel(stateModel)
{

    // this graphic representation of a state is linked to a state in the model


    StateAttributes::StateName * name = dynamic_cast<StateAttributes::StateName *> ( _stateModel->attributes.value("name"));
    connect (name, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(name);

    StateAttributes::StateSize * size = dynamic_cast<StateAttributes::StateSize *> (  _stateModel->attributes.value("size"));
    connect (size, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(size);

    StateAttributes::StatePosition * position =dynamic_cast<StateAttributes::StatePosition*> ( _stateModel->attributes.value("position"));
    connect (position, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(position);


    _title.setPos(25,10);

    _title.setParentItem(this);


}


StateBoxGraphic::~StateBoxGraphic()
{

}


void StateBoxGraphic::handleAttributeChanged(IAttribute *attr)
{
    StateAttributes::StateName * name = dynamic_cast<StateAttributes::StateName *> (attr);
    StateAttributes::StateSize * size = dynamic_cast<StateAttributes::StateSize *> ( attr);
    StateAttributes::StatePosition * position =dynamic_cast<StateAttributes::StatePosition*> (attr);


    if ( name )
    {
        this->setObjectName(name->asString()); // help debug tracing - id which object this is
        _title.setPlainText(name->asString());
    }
    else if ( size )
    {
        QPoint pt = size->asPointF().toPoint();
        SelectableBoxGraphic::setSize(pt);
    }
    else if ( position)
    {
        QPointF ps = position->asPointF();
        SelectableBoxGraphic::setPos(ps);
    }

    QGraphicsItem *parent = this->parentItem();
    if ( parent)
        parent->update();
    else
        this->update();
}



void StateBoxGraphic::setHighlighted(bool on)
{
    qDebug()<<"StateBoxGraphic::setHighlighted highlight = " + QString(on?"True":"False");
    SelectableBoxGraphic::setHighlighted(  on );
}


void StateBoxGraphic::setTitle(QString t)
{
    _title.setPlainText(t);
}



QString StateBoxGraphic::getPositionAsString()
{
    return SelectableBoxGraphic::getPositionAsString();
}

void  StateBoxGraphic::getSize(QPoint& size)
{
    SelectableBoxGraphic::getSize(size);
}

QString StateBoxGraphic::getSizeAsString()
{
    return SelectableBoxGraphic::getSizeAsString();
}


void StateBoxGraphic::setSize(QPoint size)
{
    return SelectableBoxGraphic::setSize(size);

    if ( _stateModel )
    {
        QPointF sizePointF =  QPointF ( size.x(),size.y() );
        _stateModel->setSize (sizePointF );
    }
}



void StateBoxGraphic::graphicHasChanged ()
{

    if ( _stateModel )
    {
        QPointF ps = this->pos();
        _stateModel->setPosition(ps);
        QPoint sz;
        this->getSize(sz);
        _stateModel->setSize(sz);
    }
}



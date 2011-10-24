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
#include "textblock.h"
#include "selectablelinesegmentgraphic.h"


StateBoxGraphic::StateBoxGraphic(QGraphicsObject * parent,SCState *stateModel):
        SelectableBoxGraphic(parent),
        TextItem(parent,stateModel->getIDTextBlock()),
        _stateModel(stateModel),
        _diagLineStart(),
        _diagLineEnd(),
        _diagLineDrawIt(false),
        _intersection()
{


    // this graphic representation of a state is linked to a state in the model

    setShowBoxLineStyle ( SelectableBoxGraphic::kAlways );
    setDrawBoxLineStyle  ( SelectableBoxGraphic::kDrawSolid );
    setBoxStyle(SelectableBoxGraphic::kSolidWithShadow );

    StateAttributes::StateName * name = dynamic_cast<StateAttributes::StateName *> ( _stateModel->attributes.value("name"));
    connect (name, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(name);

    SizeAttribute * size = dynamic_cast<SizeAttribute *> (  _stateModel->attributes.value("size"));
    connect (size, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(size);

    PositionAttribute * position =dynamic_cast<PositionAttribute*> ( _stateModel->attributes.value("position"));
    connect (position, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(position);


    TextItem.setPos(25,10);

    TextItem.setParentItem(this);

}


StateBoxGraphic::~StateBoxGraphic()
{

}

void StateBoxGraphic::handleTransitionLineStartMoved(QPointF newPos)
{

    // draw a diag line from the newPos to the center of the box, blue dotted line

    SelectableLineSegmentGraphic *transition = dynamic_cast<SelectableLineSegmentGraphic *> ( QObject::sender());

    QPointF cursorPos = mapFromItem(transition, newPos);

    _diagLineStart =  getVisibleCenter();
    _diagLineEnd = mapFromItem(transition, cursorPos);
    _diagLineDrawIt = true;

    // find the side which is closest to the newPos
    QRectF box = getUsableArea();
    double d[4];

    /*

     ----- 0 -----
     |           |
     |           |
    3             1
     |           |
     |           |
     ----- 2 -----

     */

    d[3] =  fabs(box.x() - cursorPos.x());
    d[1] = fabs(box.width() - cursorPos.x());
    d[0] = fabs(box.y() -  cursorPos.y());
    d[2] = fabs(box.height() -  cursorPos.y());

    double min = 9999999;
    int closest = -1;
    for(int i = 0; i < 4 ; i++)
    {
        if ( d[i] < min)
        {
            min = d[i];
            closest = i;
        }
    }

    // now project a point from the cursor position to the nearest point on the nearest line

    switch ( closest )
    {
    case 0 :
        {
            double x = cursorPos.x() ;
            if ( cursorPos.x() >= box.width()) x = box.width()-1;
            if ( cursorPos.x() < box.x()) x = box.x();

            _intersection.setX( x );
            _intersection.setY( box.y() );
        }
        break;

    case 1 :
        {
            double y =cursorPos.y() ;
            if ( cursorPos.y() >= box.height()) y = box.height()-1;
            if ( cursorPos.y() < box.y()) y = box.y();

            _intersection.setX( box.width() );
            _intersection.setY( y );
        }
        break;

    case 2 :
        {
            double x=cursorPos.x() ;
            if ( cursorPos.x() >= box.width()) x = box.width()-1;
            if ( cursorPos.x() < box.x()) x = box.x();

            _intersection.setX( x );
            _intersection.setY( box.height() );
        }
        break;

    case 3 :
        {
            double y =cursorPos.y() ;
            if ( cursorPos.y() >= box.height()) y = box.height()-1;
            if ( cursorPos.y() < box.y()) y = box.y();

            _intersection.setX( box.x() );
            _intersection.setY( y );
        }
        break;
    }

    transition->setStartEndPosition(this->mapToItem( transition, _intersection) );
}

void  StateBoxGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    SelectableBoxGraphic::paint(painter, option, widget);

#if 0  // debug stuff
    if ( _diagLineDrawIt )
    {
        QPen p;
        p.setColor(Qt::blue);
        p.setStyle(Qt::DotLine);
        painter->setPen(p);
        painter->drawLine(_diagLineStart, _diagLineEnd);

        painter->drawEllipse(_intersection, 3,3);
    }
#endif

}

void StateBoxGraphic::handleAttributeChanged(IAttribute *attr)
{
    StateAttributes::StateName * name = dynamic_cast<StateAttributes::StateName *> (attr);
    SizeAttribute * size = dynamic_cast<SizeAttribute *> ( attr);
    PositionAttribute * position =dynamic_cast<PositionAttribute*> (attr);


    if ( name )
    {
        this->setObjectName(name->asString()); // help debug tracing - id which object this is
        SCTextBlock * tb  = _stateModel->getIDTextBlock();
        tb->setText(name->asString());
        //TextItem.setPlainText(name->asString());
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


QString StateBoxGraphic::getPositionAsString()
{
    return SelectableBoxGraphic::getPositionAsString();
}

QPointF  StateBoxGraphic::getSize()
{
    QPointF size = SelectableBoxGraphic::getSize();
    return size;
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

        QPointF sz =this->getSize();
        _stateModel->setSize(sz);
    }
}



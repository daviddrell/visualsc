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

#include "selectablelinesegmentgraphic.h"
#include "math.h"

#include "sctransition.h"
#include "transitionattributes.h"

#include <QDebug>


SelectableLineSegmentGraphic::SelectableLineSegmentGraphic(QPointF position, QPointF start, QPointF end, SCTransition* transition ):
        QGraphicsPolygonItem(),
        _pen(),
        _dragStart(0,0),
        _cornerDragStart(0,0),
        _XcornerGrabBuffer(10),
        _YcornerGrabBuffer(10),
        _lineEnd_0 ( this->mapFromParent(start)),
        _lineEnd_1 ( this->mapFromParent(end)),
        _cornerGrabbed(false),
        _selectRegion(),
        _transitionModel(transition)
{
    _corners[0] = NULL;
    _corners[1] = NULL;

    this->setPos(position);


    _pen.setWidth(2);
    _pen.setColor(Qt::black);

    this->setAcceptHoverEvents(true);


    QPointF p1  (_lineEnd_0.x() - _XcornerGrabBuffer, _lineEnd_0.y() - _YcornerGrabBuffer);
    QPointF p2  (_lineEnd_0.x() + _XcornerGrabBuffer, _lineEnd_0.y() - _YcornerGrabBuffer);
    QPointF p3  (_lineEnd_0.x() + _XcornerGrabBuffer, _lineEnd_0.y() + _YcornerGrabBuffer);
    QPointF p4  (_lineEnd_0.x() - _XcornerGrabBuffer, _lineEnd_0.y() + _YcornerGrabBuffer);


    QPointF p5  (_lineEnd_1.x() - _XcornerGrabBuffer, _lineEnd_1.y() - _YcornerGrabBuffer);
    QPointF p6  (_lineEnd_1.x() + _XcornerGrabBuffer, _lineEnd_1.y() - _YcornerGrabBuffer);
    QPointF p7  (_lineEnd_1.x() + _XcornerGrabBuffer, _lineEnd_1.y() + _YcornerGrabBuffer);
    QPointF p8  (_lineEnd_1.x() - _XcornerGrabBuffer, _lineEnd_1.y() + _YcornerGrabBuffer);


    _selectRegion << p1 << p2  << p5 << p6 << p7 << p8 << p3 << p4 << p1;

    this->setPolygon(_selectRegion);

    TransitionAttributes::TransitionStringAttribute * name = dynamic_cast<TransitionAttributes::TransitionStringAttribute *> ( _transitionModel->attributes.value("target"));
    connect (name, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(name);

    TransitionAttributes::TransitionPathAttribute * path = dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (  _transitionModel->attributes.value("path"));
    connect (path, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(path);

    TransitionAttributes::TransitionPositionAttribute * tpos =dynamic_cast<TransitionAttributes::TransitionPositionAttribute*> ( _transitionModel->attributes.value("position"));
    connect (tpos, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(tpos);

}



void SelectableLineSegmentGraphic::handleAttributeChanged(IAttribute *attr)
{
    TransitionAttributes::TransitionStringAttribute * name = dynamic_cast<TransitionAttributes::TransitionStringAttribute *> (attr);
    TransitionAttributes::TransitionPathAttribute * path = dynamic_cast<TransitionAttributes::TransitionPathAttribute *> ( attr);
    TransitionAttributes::TransitionPositionAttribute * position =dynamic_cast< TransitionAttributes::TransitionPositionAttribute*> (attr);

    if ( name )
    {
        qDebug()<<"transition attr changed, name: " + name->asString();
    }

    if ( position)
    {
        QPointF ps = position->asQPointF();
        setPos( ps  );
        qDebug()<<"transition attr changed, position: " + QString( QString::number(ps.x()) + ", " + QString::number(ps.y()) );
    }
    else if ( path )
    {
        QList<QPointF> pts = path->asQPointFList();
        if ( pts.count() < 3) return;
        setPos(pts[0]);
        _lineEnd_0 = pts[1];
        _lineEnd_1 = pts[2];

        enclosePathInItemCoordiates(_lineEnd_0.x(), _lineEnd_0.y(), _lineEnd_1.x(), _lineEnd_1.y()  );

        qDebug()<<"transition attr changed, path: " + QString(
                QString::number (_lineEnd_0.x()) +" " + QString::number ( _lineEnd_0.y()) +" " +  QString::number (_lineEnd_1.x()) +" " + QString::number ( _lineEnd_1.y())
                );


    }
    setCornerPositions();

    this->update();
}




/**
  * This scene event filter has been registered with all four corner grabber items.
  * When called, a pointer to the sending item is provided along with a generic
  * event.  A dynamic_cast is used to determine if the event type is one of the events
  * we are interrested in.
  */
bool SelectableLineSegmentGraphic::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
{
    //qDebug() << " QEvent == " + QString::number(event->type());

    CornerGrabber * corner = dynamic_cast<CornerGrabber *>(watched);
    if ( corner == NULL) return false; // not expected to get here

    QGraphicsSceneMouseEvent * mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if ( mevent == NULL)
    {
        // this is not one of the mouse events we are interrested in
        return false;
    }


    switch (event->type() )
    {
        // if the mouse went down, record the x,y coords of the press, record it inside the corner object
    case QEvent::GraphicsSceneMousePress:
        {
            corner->setMouseState(CornerGrabber::kMouseDown);

            QPointF scenePosition =  corner->mapToScene(mevent->pos());
            corner->mouseDownY = scenePosition.y();
            corner->mouseDownX= scenePosition.x();

            _cornerGrabbed = true;

        }
        break;


    case QEvent::GraphicsSceneMouseRelease:
        {
            corner->setMouseState(CornerGrabber::kMouseReleased);
            _cornerGrabbed = false;
            updateModel();
        }
        break;


    case QEvent::GraphicsSceneMouseMove:
        {
            corner->setMouseState(CornerGrabber::kMouseMoving );
        }
        break;


    default:
        // we dont care about the rest of the events
        return false;
        break;
    }


    if ( corner->getMouseState() == CornerGrabber::kMouseMoving )
    {

        if ( corner == _corners[0])
        {
            // tell the state box graphic that owns this transition, so that it can detemine where to anchor the corner, it will
            // call back into this class via setStartEndPosition()
            emit startEndMoved( this->mapFromItem(corner, mevent->pos()));
        }
        else
        {
            createCustomPath(mevent->pos(), corner);
        }

        this->update();
    }

    return true;// true => do not send event to watched - we are finished with this event
}




void SelectableLineSegmentGraphic::createCustomPath(QPointF mouseLocation, CornerGrabber* corner)
{

    qreal lineEndX = 0;
    qreal lineEndY = 0;
    qreal lineStartX = 0;
    qreal lineStartY = 0;

    qreal mouseX =0, mouseY = 0;

    QPointF scenePosition = corner->mapToScene(mouseLocation);
    mouseY = scenePosition.y();
    mouseX = scenePosition.x();

    // which corner needs to get moved?
    if ( corner == _corners[0])
    {

        lineStartY =  mouseY;
        lineStartX =  mouseX;

        lineEndY = this->mapToScene( _corners[1]->getCenterPoint()).y();
        lineEndX = this->mapToScene( _corners[1]->getCenterPoint()).x();

    }
    else
    {
        lineStartY = this->mapToScene( _corners[0]->getCenterPoint()).y();
        lineStartX = this->mapToScene( _corners[0]->getCenterPoint()).x();

        lineEndY =  mouseY;
        lineEndX =  mouseX;

    }


    _lineEnd_0.setX(  mapFromScene(lineStartX, lineStartY).x()  );
    _lineEnd_0.setY(  mapFromScene(lineStartX, lineStartY).y() );


    _lineEnd_1.setX(  mapFromScene(lineEndX,lineEndY ).x()  );
    _lineEnd_1.setY(  mapFromScene(lineEndX,lineEndY ).y());

    enclosePathInSceneCoordiates(lineStartX, lineStartY,  lineEndX, lineEndY  );

    setCornerPositions();

}

void SelectableLineSegmentGraphic::setStartEndPosition(QPointF position)
{
    createCustomPath( mapToItem(_corners[0], position), _corners[0])  ;
}

void SelectableLineSegmentGraphic::enclosePathInItemCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  )
{
    QPointF lineEnd_0;
    QPointF lineEnd_1;

    lineEnd_0.setX(  mapToScene(lineStartX, lineStartY).x()  );
    lineEnd_0.setY(  mapToScene(lineStartX, lineStartY).y() );

    lineEnd_1.setX(  mapToScene(lineEndX,lineEndY ).x());
    lineEnd_1.setY(  mapToScene(lineEndX,lineEndY ).y());

    enclosePathInSceneCoordiates( lineEnd_0.x(), lineEnd_0.y(), lineEnd_1.x(), lineEnd_1.y()  );

}

void SelectableLineSegmentGraphic::enclosePathInSceneCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  )
{


    /* to draw the select region around the line,
        define a large box(rect p1,p2,p3,p4) around each end of the line.
        then define a box(rect p5,p6,p7,p8) that connects the two end boxes.
        now no matter the angle of the line, we can be sure that its
        surrounded by a symetrical region that will pick up the mouse.


        .    .             .    .
           X-----------------X
        .    .             .    .

        the squares have to connect at the two pairs of square corners that are closest.

       */

    QList<QPointF> pointsStart;
    QList<QPointF> pointsEnd;

    pointsStart.append( QPointF(lineStartX  - _XcornerGrabBuffer, lineStartY - _YcornerGrabBuffer));
    pointsStart.append(  QPointF(lineStartX + _XcornerGrabBuffer, lineStartY - _YcornerGrabBuffer));
    pointsStart.append(  QPointF(lineStartX + _XcornerGrabBuffer, lineStartY + _YcornerGrabBuffer));
    pointsStart.append(  QPointF(lineStartX - _XcornerGrabBuffer, lineStartY + _YcornerGrabBuffer));

    pointsEnd.append(  QPointF(lineEndX - _XcornerGrabBuffer, lineEndY - _YcornerGrabBuffer));
    pointsEnd.append(  QPointF(lineEndX + _XcornerGrabBuffer, lineEndY - _YcornerGrabBuffer));
    pointsEnd.append(  QPointF(lineEndX + _XcornerGrabBuffer, lineEndY + _YcornerGrabBuffer));
    pointsEnd.append(  QPointF(lineEndX - _XcornerGrabBuffer, lineEndY + _YcornerGrabBuffer));

    qreal minDistance = 0 ;
    qreal secondMinDistance = 0;
    QPointF p1 ;
    QPointF p2 ;
    QPointF p3 ;
    QPointF p4 ;
    int i1=0;
    int i2=0;

    for (int i =0 ; i < 4; i++)
    {
        for (int j = 0; j < 4 ; j++)
        {
            qreal d1 = pow(pointsStart[i].x() - pointsEnd[j].x(), 2.0)
                       + pow( pointsStart[i].y() - pointsEnd[j].y(), 2.0);
            if ( d1 > minDistance)
            {
                minDistance = d1;
                p1 = pointsStart[i];
                p2 = pointsEnd[j];
                i1 = i;
                i2 = j;
            }
        }
    }



    for (int i =0 ; i < 4; i++)
    {
        if ( i == i1) continue;

        for (int j = 0; j < 4 ; j++)
        {
            if ( j == i2 ) continue;

            qreal d1 = pow(pointsStart[i].x() - pointsEnd[j].x(), 2.0)
                       + pow( pointsStart[i].y() - pointsEnd[j].y(), 2.0);
            if ( d1 > secondMinDistance)
            {
                secondMinDistance = d1;
                p3 = pointsStart[i];
                p4 = pointsEnd[j];
            }
        }
    }

    _selectRegion.clear();

    _selectRegion << mapFromScene(p1) << mapFromScene(p3)  << mapFromScene(p4) << mapFromScene(p2) << mapFromScene(p1) ;

    this->setPolygon(_selectRegion);

    this->update();

}


void SelectableLineSegmentGraphic::updateModel ()
{

    if ( _transitionModel )
    {
        TransitionAttributes::TransitionPositionAttribute *posAttr = dynamic_cast<TransitionAttributes::TransitionPositionAttribute *> ( _transitionModel->attributes.value("position"));

        QPointF ps = this->pos();
        posAttr->setValue(ps);

        TransitionAttributes::TransitionPathAttribute * pathAttr = dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (_transitionModel->attributes.value("path"));

        QList<QPointF> path;

        path.append(ps);
        path.append(_lineEnd_0);
        path.append(_lineEnd_1);

        pathAttr->setValue(path);

    }
}

// for supporting moving the box across the scene
void SelectableLineSegmentGraphic::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    updateModel();
}


// for supporting moving the box across the scene
void SelectableLineSegmentGraphic::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    _dragStart = event->pos();
}


// for supporting moving the box across the scene
void SelectableLineSegmentGraphic::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{

    QPointF location = this->pos();
    location += (event->pos() - _dragStart);
    this->setPos(location);
}

// remove the corner grabbers

void SelectableLineSegmentGraphic::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    _corners[0]->setParentItem(NULL);
    _corners[1]->setParentItem(NULL);

    delete _corners[0];
    _corners[0]  = NULL;

    delete _corners[1];
    _corners[1]  = NULL;

    qDebug()<<"SelectableLineSegmentGraphic::hoverLeaveEvent";
    emit unselected();
}


// create the corner grabbers

void SelectableLineSegmentGraphic::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    _corners[0] = new CornerGrabber(this,0, false);
    _corners[1] = new CornerGrabber(this,1, false);

    _corners[0]->installSceneEventFilter(this);
    _corners[1]->installSceneEventFilter(this);

    setCornerPositions();

    qDebug()<<"SelectableLineSegmentGraphic::hoverEnterEvent";
    emit selected();
}

void SelectableLineSegmentGraphic::setCornerPositions()
{
    if ( _corners[0] == NULL || _corners[1] == NULL )return;

    _corners[0]->setPos(_lineEnd_0.x(), _lineEnd_0.y()  );
    _corners[1]->setPos(_lineEnd_1.x(), _lineEnd_1.y()  );

}



void SelectableLineSegmentGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // this paint will draw the bounding box for debugging:
 //   QGraphicsPolygonItem::paint(painter, i, w );

    // draw the line

    _pen.setStyle(Qt::SolidLine);
    painter->setPen(_pen);

    _pen.setColor(Qt::red);
    painter->drawLine(_lineEnd_0, _lineEnd_1);

}



void SelectableLineSegmentGraphic::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void SelectableLineSegmentGraphic::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

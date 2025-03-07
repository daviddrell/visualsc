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
#include "keycontroller.h"
#include "transitiongraphic.h"


#include <QDebug>





SelectableLineSegmentGraphic::SelectableLineSegmentGraphic(ElbowGrabber* startPoint, ElbowGrabber* endPoint, SCTransition* transition, TransitionGraphic* parentGraphic,KeyController * keys ):
        QGraphicsPolygonItem(),
        _pen(),
        _dragStart(0,0),
        _cornerDragStart(0,0),
        _XcornerGrabBuffer(10),
        _YcornerGrabBuffer(10),
       // _lineEnd_0 ( this->mapFromParent(start)),
       // _lineEnd_1 ( this->mapFromParent(end)),
        _cornerGrabbed(false),
        _selectRegion(),
        _transitionModel(transition),
        _isTerminal(false),
        _parentGraphic(parentGraphic),
        _keyController(keys)
{
    _elbows[0] = startPoint;
    _elbows[1] = endPoint;

    this->setFlag(QGraphicsItem::ItemIsMovable, false);
    //this->setAcceptHoverEvents(true);
    this->setParentItem(parentGraphic);

    // TODO Bug: The line segment's position works as if the first elbow were its parent even though its not.
    this->setPos(startPoint->pos());
    //this->setPos(0,0);

    //qDebug() << "selec Parent: " << parentGraphic << " pos: "<< this->pos();
    //this->setPos(0,0);

    _pen.setWidth(2);
    _pen.setColor(Qt::black);

    QPointF p1  (_elbows[0]->x()- _XcornerGrabBuffer, _elbows[0]->y() - _YcornerGrabBuffer);
    QPointF p2  (_elbows[0]->x() + _XcornerGrabBuffer, _elbows[0]->y() - _YcornerGrabBuffer);
    QPointF p3  (_elbows[0]->x() + _XcornerGrabBuffer, _elbows[0]->y() + _YcornerGrabBuffer);
    QPointF p4  (_elbows[0]->x() - _XcornerGrabBuffer, _elbows[0]->y() + _YcornerGrabBuffer);


    QPointF p5  (_elbows[1]->x() - _XcornerGrabBuffer, _elbows[1]->y() - _YcornerGrabBuffer);
    QPointF p6  (_elbows[1]->x() + _XcornerGrabBuffer, _elbows[1]->y() - _YcornerGrabBuffer);
    QPointF p7  (_elbows[1]->x() + _XcornerGrabBuffer, _elbows[1]->y() + _YcornerGrabBuffer);
    QPointF p8  (_elbows[1]->x() - _XcornerGrabBuffer, _elbows[1]->y() + _YcornerGrabBuffer);

    _selectRegion << p1 << p2  << p5 << p6 << p7 << p8 << p3 << p4 << p1;

    this->setPolygon(_selectRegion);
}


SelectableLineSegmentGraphic::~SelectableLineSegmentGraphic()
{

}

void SelectableLineSegmentGraphic::setMouseState(int state)
{
    _mouseState = state;
}

int SelectableLineSegmentGraphic::getMouseState()
{
    return _mouseState;
}

void SelectableLineSegmentGraphic::handleAttributeChanged(IAttribute *attr)
{
    TransitionStringAttribute * name = dynamic_cast<TransitionStringAttribute *> (attr);
    TransitionPathAttribute * path = dynamic_cast<TransitionPathAttribute *> ( attr);
    TransitionPositionAttribute * position =dynamic_cast< TransitionPositionAttribute*> (attr);

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
        qDebug() << "handle Attribute changed";
        QList<QPointF> pts = path->asQPointFList();
        if ( pts.count() < 3) return;
        setPos(pts[0]);
        //_lineEnd_0 = pts[1];
        //_lineEnd_1 = pts[2];

        _elbows[0]->setPos(pts[1]);
        _elbows[1]->setPos(pts[2]);

        enclosePathInItemCoordiates(_elbows[0]->x(), _elbows[0]->y(), _elbows[1]->x(), _elbows[1]->y());

        qDebug()<<"transition attr changed, path: " + QString(
                QString::number (_elbows[0]->x()) +" " + QString::number ( _elbows[0]->y()) +" " +  QString::number (_elbows[1]->x()) +" " + QString::number ( _elbows[1]->y())
                );


    }
    setCornerPositions();

    this->update();
}




/**
 * @brief SelectableLineSegmentGraphic::sceneEventFilter
 * @param watched
 * @param event
 * @return
 *
 * function promoted to transition graphic
 *
 */

bool SelectableLineSegmentGraphic::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
{
    /*
    static int count=0;
    qDebug() << "SLSG SceneEventFilter QEvent == " + QString::number(event->type()) << " " << count++;
//qDebug() << "QGraphicsItem Scene Event " << watched->
    ElbowGrabber * corner = dynamic_cast<ElbowGrabber *>(watched);
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
        qDebug() << "mouse press";
            corner->setMouseState(ElbowGrabber::kMouseDown);

            QPointF scenePosition =  corner->mapToScene(mevent->pos());
            corner->mouseDownY = scenePosition.y();
            corner->mouseDownX= scenePosition.x();

            _cornerGrabbed = true;
            qDebug() << "Corner Position: " << corner->mouseDownX<<" ," << corner->mouseDownY;
        }
        break;


    case QEvent::GraphicsSceneMouseRelease:
        {
        qDebug() << "mouse release";
            corner->setMouseState(ElbowGrabber::kMouseReleased);
            _cornerGrabbed = false;
            updateModel();
        }
        break;


    case QEvent::GraphicsSceneMouseMove:
        {
       //     qDebug() << "mouse moving";
            corner->setMouseState(ElbowGrabber::kMouseMoving );
        }
        break;


    default:
        // we dont care about the rest of the events
        return false;
        break;
    }


    if ( corner->getMouseState() == ElbowGrabber::kMouseMoving )
    {
//TODO

        if ( corner == _elbows[0])
        {
            // tell the state box graphic that owns this transition, so that it can detemine where to anchor the corner, it will
            // call back into this class via setStartEndPosition()
            //emit startEndMoved( this->mapFromItem(corner, mevent->pos()));
            qDebug() << "START END MOVED MUST BE REMOVED IF YOU SEE THIS";
        }
        else
        {
            //createCustomPath(mevent->pos(), corner);
            qDebug() << "START END MOVED MUST BE REMOVED IF YOU SEE THIS";
        }

        this->update();

    }
*/
    return true;// true => do not send event to watched - we are finished with this event

}


//FUNCTION NOT CURRENTLY USED IN UPDATE ELBOW
void SelectableLineSegmentGraphic::createCustomPath(QPointF mouseLocation, ElbowGrabber* corner)
{//TODO

    qreal lineEndX = 0;
    qreal lineEndY = 0;
    qreal lineStartX = 0;
    qreal lineStartY = 0;

    qreal mouseX =0, mouseY = 0;

    QPointF scenePosition = corner->mapToScene(mouseLocation);
    mouseY = scenePosition.y();
    mouseX = scenePosition.x();

    // which corner needs to get moved?
    if ( corner == _elbows[0])  // source elbow
    {

        lineStartY =  mouseY;   // start at mouse
        lineStartX =  mouseX;

        lineEndY = this->mapToScene( _elbows[1]->getCenterPoint()).y(); // end at second elbow
        lineEndX = this->mapToScene( _elbows[1]->getCenterPoint()).x();

    }
    else
    {
        lineStartY = this->mapToScene( _elbows[0]->getCenterPoint()).y();
        lineStartX = this->mapToScene( _elbows[0]->getCenterPoint()).x();

        lineEndY =  mouseY;
        lineEndX =  mouseX;

    }




    _elbows[0]->setX(  mapFromScene(lineStartX, lineStartY).x()  );
    _elbows[0]->setY(  mapFromScene(lineStartX, lineStartY).y() );


    _elbows[1]->setX(  mapFromScene(lineEndX,lineEndY ).x()  );
    _elbows[1]->setY(  mapFromScene(lineEndX,lineEndY ).y());

    enclosePathInSceneCoordiates(lineStartX, lineStartY,  lineEndX, lineEndY  );

     setCornerPositions();

}


ElbowGrabber* SelectableLineSegmentGraphic::getElbow(int index)
{
    return _elbows[index];
}

QPointF SelectableLineSegmentGraphic::getStart()
{
   return _elbows[0]->pos();
    // return _lineEnd_0;
}

QPointF SelectableLineSegmentGraphic::getEnd()
{
    return _elbows[1]->pos();
    //return _lineEnd_1;
}



void SelectableLineSegmentGraphic::setTerminator(bool isTerm)
{
    //TODO
    _isTerminal = isTerm;

    if (  _elbows[1]== NULL )
        return;

    if ( _isTerminal )
    {
        _elbows[1]->setPaintStyle(ElbowGrabber::kArrowHead);
        _elbows[1]->setVisible(true);
        //_elbows[1]->installSceneEventFilter(this);
    }
    else
        _elbows[1]->setPaintStyle(ElbowGrabber::kBox);

}

void SelectableLineSegmentGraphic::setStartEndPosition(QPointF position)
{
    //TODO
    // allows the line start to snap to the outter edge of a box
    createCustomPath( mapToItem(_elbows[0], position), _elbows[0])  ;
    qDebug() << "setstartendposition should have been called %%%";
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

void SelectableLineSegmentGraphic::printInfo()
{
    //TODO
    //qDebug() << "Sel Line Seg info: Corner[0]" << this->_elbows[0]->x() << ", " << this->_elbows[0]->y();
}

//void SelectableLineSegmentGraphic::updateModel ()
//{

//    if ( _transitionModel )
//    {
//        TransitionPositionAttribute *posAttr = dynamic_cast<TransitionPositionAttribute *> ( _transitionModel->attributes.value("position"));

//        QPointF ps = this->pos();
//        posAttr->setValue(ps);

//        TransitionPathAttribute * pathAttr = dynamic_cast<TransitionPathAttribute *> (_transitionModel->attributes.value("path"));

//        QList<QPointF> path;

//        path.append(ps);
//        path.append(_lineEnd_0);
//        path.append(_lineEnd_1);

//        pathAttr->setValue(path);

//    }
//}

// for supporting moving the box across the scene
void SelectableLineSegmentGraphic::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    emit updateModel();
}


// for supporting moving the box across the scene
void SelectableLineSegmentGraphic::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // no longer allow moving of line segements. only move ends of lines
    event->setAccepted(true);
//    _dragStart = event->pos();
}


// for supporting moving the box across the scene
void SelectableLineSegmentGraphic::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
   // QPointF location = this->pos();
  //  location += (event->pos() - _dragStart);
   // this->setPos(location);
}

// remove the corner grabbers

void SelectableLineSegmentGraphic::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    //qDebug() << "leaving Hover";

    // clear parent's currently hovered object
    //this->parentItemAsTransitionGraphic()->clearCurrentlyHoveredSegment();

    // disconnect the key controller and the transition graphic key press handler
    disconnect(_keyController, SIGNAL(keyPressed(int)), _parentGraphic, SLOT(handleKeyPressEvent(int)));


    // make the corner and elbows invisible
    //this->parentItemAsTransitionGraphic()->setGrabbersVisible(false);


    _elbows[0]->setVisible(false);
    if ( _isTerminal )
        _elbows[1]->setVisible(true);
    else
     _elbows[1]->setVisible(false);


    emit unselected();
}


// create the corner grabbers

void SelectableLineSegmentGraphic::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    qDebug() << "SelectableLineSegment HoverEnterEvent";

    // set parent's currently hovered object to this line segment
    //this->parentItemAsTransitionGraphic()->setCurrentlyHoveredSegment(this);

    // connect the key controller to this transition graphic
    connect(_keyController, SIGNAL(keyPressed(int)), _parentGraphic, SLOT(handleKeyPressEvent(int)));


    // make the corner and elbows visible
    //this->parentItemAsTransitionGraphic()->setGrabbersVisible(true);
//TODO

    _elbows[0]->setVisible(true);
    _elbows[1]->setVisible(true);

//    _elbows[0]->installSceneEventFilter(this);
//    _elbows[1]->installSceneEventFilter(this);

    setCornerPositions();


    if ( _isTerminal )
    {
        _elbows[1]->setPaintStyle(ElbowGrabber::kArrowHead);
    }
    else
        _elbows[1]->setPaintStyle(ElbowGrabber::kBox);


    emit selected(); 
}

void SelectableLineSegmentGraphic::setCornerPositions()
{
    qDebug() << "set corner positions";
    if ( _elbows[0] == NULL || _elbows[1] == NULL )return;

    _elbows[0]->setPos(_elbows[0]->x(), _elbows[0]->y()  );
    _elbows[1]->setPos(_elbows[1]->x(), _elbows[1]->y()  );
}

TransitionGraphic* SelectableLineSegmentGraphic::parentItemAsTransitionGraphic()
{
    return static_cast<TransitionGraphic*>(this->parentItem());
}

void SelectableLineSegmentGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // draw the line
    _pen.setStyle(Qt::SolidLine);
    painter->setPen(_pen);

    _pen.setColor(Qt::red);
    //painter->drawLine(_lineEnd_0, _lineEnd_1);
    painter->drawLine(_elbows[0]->pos(), _elbows[1]->pos());

    // if this is the terminal line segment, then redraw the arrow head with correct angles
    if ( _isTerminal && _elbows[1] )
    {
        double dy = _elbows[1]->y() -_elbows[0]->y();
        double dx = _elbows[1]->x() - _elbows[0]->x();
        int quadrant =0;

        //qDebug()<< "initial values dx = " << dx << ", dy = " << dy ;


        //double angle = (atan ( (dy) / ( dx) ) + ( k * 3.14159265)) * (180.0/3.14159265);

        if ( dx < 0 && dy >= 0  )
        {
            quadrant = 1;
            dx *= -1;
        }
        else if ( dx < 0 && dy < 0  )
        {
            quadrant = 2;
            dx *= -1;
        }

        double angle = atan ( (dy) / ( dx) )  * (180.0/3.14159265);

        if ( quadrant == 1 )
            angle = 180 - angle  ;
        else if ( quadrant == 2 )
            angle = -180 -angle;

        _elbows[1]->setAngle(angle);
        _elbows[1]->update();
    }

}



void SelectableLineSegmentGraphic::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void SelectableLineSegmentGraphic::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

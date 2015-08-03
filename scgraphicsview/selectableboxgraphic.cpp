#include "selectableboxgraphic.h"


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

#include "selectableboxgraphic.h"

#include <QBrush>
#include <QLinearGradient>
#include <QDebug>
#include <QTextDocument>
#include "math.h"
#include "scstate.h"
#include <QTimer>
#include <QKeyEvent>
#include "stateboxgraphic.h"



class StateBoxGraphic;

/**
  *  This box can be re-sized and it can be moved. For moving, we capture
  *  the mouse move events and move the box location accordingly.
  *
  *  To resize the box, we place small indicator boxes on the four corners that give the user
  *  a visual cue to grab with the mouse. The user then drags the corner to stretch
  *  or shrink the box.  The corner grabbers are implemented with the CornerGrabber class.
  *  The CornerGrabber class captures the mouse when the mouse is over the corner's area,
  *  but the SelectableBoxGraphic object (which owns the corners) captures and processes the mouse
  *  events on behalf of the CornerGrabbers (because the owner wants to be
  *  resized, not the CornerGrabbers themselves). This is accomplished by installing a scene event filter
  *  on the CornerGrabber objects:
          _corners[0]->installSceneEventFilter(this);
  *
  *
  *
  */

SelectableBoxGraphic::SelectableBoxGraphic(QGraphicsObject * parent):
        QGraphicsObject(parent),
        _pen(),
        _dragStart(0,0),
        _gridSpace(10),
        _width(0),
        _height(0),
        _cornerDragStart(0,0),
        _XcornerGrabBuffer(7),
        _YcornerGrabBuffer(7),
        _drawingWidth(  _width -   _XcornerGrabBuffer),
        _drawingHeight( _height -  _YcornerGrabBuffer),
        _drawingOrigenX( _XcornerGrabBuffer),
        _drawingOrigenY( _YcornerGrabBuffer),
        _isHighlighted(false),
        _isHovered(false),
        _showBoxStyle(kWhenSelected),
        _drawBoxLineStyle(kDrawDotted),
        _boxStyle(kTransparent),
        _hoverLineThickness(3),
        _keepInsideParent(false),
        _minSize(QPoint(40,40))
{

    _corners[0] = NULL;
    _corners[1] = NULL;
    _corners[2] = NULL;
    _corners[3] = NULL;

    this->setAcceptHoverEvents(true);
    //this->installEventFilter(this);
}

SelectableBoxGraphic::SelectableBoxGraphic(QGraphicsObject * parent, bool keepInsideParent):
        QGraphicsObject(parent),
        _pen(),
        _dragStart(0,0),
        _gridSpace(10),
        _width(0),
        _height(0),
        _cornerDragStart(0,0),
        _XcornerGrabBuffer(7),
        _YcornerGrabBuffer(7),
        _drawingWidth(  _width -   _XcornerGrabBuffer),
        _drawingHeight( _height -  _YcornerGrabBuffer),
        _drawingOrigenX( _XcornerGrabBuffer),
        _drawingOrigenY( _YcornerGrabBuffer),
        _isHighlighted(false),
        _isHovered(false),
        _showBoxStyle(kWhenSelected),
        _drawBoxLineStyle(kDrawDotted),
        _boxStyle(kTransparent),
        _hoverLineThickness(3),
        _keepInsideParent(keepInsideParent)
{

    _corners[0] = NULL;
    _corners[1] = NULL;
    _corners[2] = NULL;
    _corners[3] = NULL;

    this->setAcceptHoverEvents(true);
    //this->installEventFilter(this);
}


SelectableBoxGraphic::~SelectableBoxGraphic()
{
    delete _corners[0];
    delete _corners[1];
    delete _corners[2];
    delete _corners[3];
}


void SelectableBoxGraphic::setHoverLineThickness(int t )
{
    _hoverLineThickness = t;
}


void SelectableBoxGraphic::setShowBoxLineStyle(ShowBoxStyle s )
{
    _showBoxStyle = s;
}

void SelectableBoxGraphic::setDrawBoxLineStyle( DrawBoxLineStyle s)
{
    _drawBoxLineStyle = s;
}

void SelectableBoxGraphic::setBoxStyle( BoxStyle s)
{
    _boxStyle = s;
}

void SelectableBoxGraphic::setHighlighted(bool on)
{
    _isHighlighted = on;
}



void SelectableBoxGraphic::increaseWidth(int inc)
{
    adjustDrawingSize(inc, 0);
}

QString SelectableBoxGraphic::getPositionAsString()
{
    QString v= QString::number( this->pos().x() ) + "," + QString::number(this->pos().y() );
    return v;
}

QPointF  SelectableBoxGraphic::getSize()
{
    QPointF size;
    size.setX(_width);
    size.setY(_height);

    return size;
}

QString SelectableBoxGraphic::getSizeAsString()
{
    return ( QString::number( _width) + "," + QString::number(_height));
}


void SelectableBoxGraphic::setSize(QPointF size)
{
    _width = size.x();
    _height = size.y();

    _drawingWidth =  _width - _XcornerGrabBuffer;
    _drawingHeight=  _height - _YcornerGrabBuffer;
}

/**
 *  To allow the user to grab the corners to re-size, we need to get a hover
 *  indication. But if the mouse pointer points to the left, then when the mouse
 *  tip is to the left but just outsize the box, we will not get the hover.
 *  So the solution is to tell the graphics scene the box is larger than
 *  what the painter actually paints in. This way when the user gets the mouse
 *  within a few pixels of what appears to be the edge of the box, we get
 *  the hover indication.

 *  So the cornerGrabBuffer is a few pixel wide buffer zone around the outside
 *  edge of the box.
 *
 */
void SelectableBoxGraphic::adjustDrawingSize(int x, int y)
{

    QPoint newSize  (_width + x, _height+y);

    // call the virtual setSize() so that children can capture this and edit size before applying
    setSize (newSize);

    _drawingWidth =  _width - _XcornerGrabBuffer;
    _drawingHeight=  _height - _YcornerGrabBuffer;

}

/**
  * This scene event filter has been registered with all four corner grabber items.
  * When called, a pointer to the sending item is provided along with a generic
  * event.  A dynamic_cast is used to determine if the event type is one of the events
  * we are interrested in.
  */
bool SelectableBoxGraphic::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
{
    //    qDebug() << " QEvent == " + QString::number(event->type());

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
            corner->mouseDownX = mevent->pos().x();
            corner->mouseDownY = mevent->pos().y();
            //corner->setHovered(true);
        }
        break;

    case QEvent::GraphicsSceneMouseRelease:
        {
            corner->setMouseState(CornerGrabber::kMouseReleased);
            //corner->setHovered(false);
            graphicHasChanged();
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


        qreal x = mevent->pos().x(), y = mevent->pos().y();
        QPointF mts = mapToScene(pos());
        mts = pos();
        QRectF oldBox = QRectF(mts.x(), mts.y(), _width, _height);

        // depending on which corner has been grabbed, we want to move the position
        // of the item as it grows/shrinks accordingly. so we need to eitehr add
        // or subtract the offsets based on which corner this is.

        int XaxisSign = 0;
        int YaxisSign = 0;
        switch( corner->getCorner() )
        {
        case 0:
            {
                XaxisSign = +1;
                YaxisSign = +1;
            }
            break;

        case 1:
            {
                XaxisSign = -1;
                YaxisSign = +1;
            }
            break;

        case 2:
            {
                XaxisSign = -1;
                YaxisSign = -1;
            }
            break;

        case 3:
            {
                XaxisSign = +1;
                YaxisSign = -1;
            }
            break;

        }

        // if the mouse is being dragged, calculate a new size and also re-position
        // the box to give the appearance of dragging the corner out/in to resize the box

        QPointF old(this->pos());
        const qreal w = this->getSize().x();
        const qreal h = this->getSize().y();


        SelectableBoxGraphic* parentGraphic = this->parentAsSelectableBoxGraphic();


         qreal parentW;
         qreal parentH;






        int xMoved = corner->mouseDownX - x;
        int yMoved = corner->mouseDownY - y;

        int newWidth = _width + ( XaxisSign * xMoved);
        if ( newWidth < _minSize.x() ) newWidth  = _minSize.x();

        int newHeight = _height + (YaxisSign * yMoved) ;
        if ( newHeight < _minSize.y() ) newHeight = _minSize.y();

        int deltaWidth;
        int deltaHeight;


        // if this box is resized, keep it inside its parent's area
        if(_keepInsideParent&&parentGraphic)
        {
            parentW = parentGraphic->getSize().x();
            parentH = parentGraphic->getSize().y();



            // applies to when pos is static and width and height are changing
            // check if the new width and height are too far out

/*
            if(newWidth + old.x() > parentW - INSIDE_PARENT_BUFFER)
                newWidth = parentW - INSIDE_PARENT_BUFFER - old.x();

            if(newHeight + old.y() > parentH - INSIDE_PARENT_BUFFER)
                newHeight = parentH - INSIDE_PARENT_BUFFER - old.y();
*/



            deltaWidth =   newWidth - _width ;
            deltaHeight =   newHeight - _height ;


            //adjustDrawingSize(  deltaWidth ,   deltaHeight);

            deltaWidth *= (-1);
            deltaHeight *= (-1);

            qreal newXpos, newYpos;

            if ( corner->getCorner() == 0 )
            {
                newXpos = this->pos().x() + deltaWidth;
                newYpos = this->pos().y() + deltaHeight;


                if(newXpos <  0+INSIDE_PARENT_BUFFER)
                {
                    deltaWidth = 0+INSIDE_PARENT_BUFFER - pos().x();
                    newXpos = 0+INSIDE_PARENT_BUFFER;
                }

                if(newYpos < 0+INSIDE_PARENT_BUFFER)
                {
                    deltaHeight =0+INSIDE_PARENT_BUFFER - pos().y();
                    newYpos = 0+INSIDE_PARENT_BUFFER;
                }



                this->setPos(newXpos, newYpos);
            }
            else   if ( corner->getCorner() == 1 )
            {

                if(newWidth + old.x() > parentW - INSIDE_PARENT_BUFFER)
                    newWidth = parentW - INSIDE_PARENT_BUFFER - old.x();

                if(newHeight + old.y() > parentH - INSIDE_PARENT_BUFFER)
                    newHeight = parentH - INSIDE_PARENT_BUFFER - old.y();




                deltaWidth =   newWidth - _width ;
                deltaHeight =   newHeight - _height ;

                deltaWidth *= (-1);
                deltaHeight *= (-1);

                newYpos = this->pos().y() + deltaHeight;

                if(newYpos < 0+INSIDE_PARENT_BUFFER)
                {
                    deltaHeight = 0+INSIDE_PARENT_BUFFER - pos().y();
                    newYpos = 0+INSIDE_PARENT_BUFFER;
                }

                this->setPos(this->pos().x(), newYpos);
            }
            else if(corner->getCorner() == 2)
            {

                if(newWidth + old.x() > parentW - INSIDE_PARENT_BUFFER)
                    newWidth = parentW - INSIDE_PARENT_BUFFER - old.x();

                if(newHeight + old.y() > parentH - INSIDE_PARENT_BUFFER)
                    newHeight = parentH - INSIDE_PARENT_BUFFER - old.y();




                deltaWidth =   newWidth - _width ;
                deltaHeight =   newHeight - _height ;

                deltaWidth *= (-1);
                deltaHeight *= (-1);


            }
            else   if ( corner->getCorner() == 3 )
            {

                if(newWidth + old.x() > parentW - INSIDE_PARENT_BUFFER)
                    newWidth = parentW - INSIDE_PARENT_BUFFER - old.x();

                if(newHeight + old.y() > parentH - INSIDE_PARENT_BUFFER)
                    newHeight = parentH - INSIDE_PARENT_BUFFER - old.y();




                deltaWidth =   newWidth - _width ;
                deltaHeight =   newHeight - _height ;

                deltaWidth *= (-1);
                deltaHeight *= (-1);
                newXpos = this->pos().x() + deltaWidth;

                    if(newXpos < 0+INSIDE_PARENT_BUFFER)
                    {
                        deltaWidth = 0+INSIDE_PARENT_BUFFER - pos().x();
                        newXpos = 0+INSIDE_PARENT_BUFFER;
                    }


                this->setPos(newXpos,this->pos().y());
            }

            //this->setPos(newXpos, newYpos);

            deltaWidth *= (-1);
            deltaHeight *= (-1);
            qDebug() << "dw: " << deltaWidth << "dh: " <<deltaHeight;
            adjustDrawingSize(  deltaWidth ,   deltaHeight);
        }
        else
        {



            deltaWidth =   newWidth - _width ;
            deltaHeight =   newHeight - _height ;


            //adjustDrawingSize(  deltaWidth ,   deltaHeight);

            deltaWidth *= (-1);
            deltaHeight *= (-1);

            qreal newXpos, newYpos;


            if ( corner->getCorner() == 0 )
            {
                 newXpos = this->pos().x() + deltaWidth;
                 newYpos = this->pos().y() + deltaHeight;




                this->setPos(newXpos, newYpos);
            }
            else   if ( corner->getCorner() == 1 )
            {
                newYpos = this->pos().y() + deltaHeight;



                this->setPos(this->pos().x(), newYpos);
            }
            else if(corner->getCorner() == 2)
            {
                qDebug() << "corner 2";


            }
            else   if ( corner->getCorner() == 3 )
            {
                newXpos = this->pos().x() + deltaWidth;

                this->setPos(newXpos,this->pos().y());
            }

            deltaWidth *= (-1);
            deltaHeight *= (-1);
            adjustDrawingSize(  deltaWidth ,   deltaHeight);

        }










        setCornerPositions();

        QPointF nmts = pos();
        QRectF newBox = QRectF(nmts.x(), nmts.y(), newWidth, newHeight);
        //qDebug() << "Drag Start:\t\t"<<_dragStart<<"\nnewPos: "<<newPos<<"\ntest:\t\t"<<test;

        int corner = this->getHoveredCorner();
        if(corner==-1)
            qDebug() << "ERROR there was no hovered corner, should not be allowed";
        else
        {
            //qDebug() << "the corner used: " << corner;
            //qDebug() <<"old box: " <<oldBox << "new Box: " << newBox;
            emit stateBoxResized(oldBox, newBox, corner);
        }
        this->update();
    }

    return true;// true => do not send event to watched - we are finished with this event
}

SelectableBoxGraphic* SelectableBoxGraphic::parentAsSelectableBoxGraphic()
{
    return dynamic_cast<SelectableBoxGraphic*>(this->parentItem());
}

int SelectableBoxGraphic::getHoveredCorner()
{

    for(int i = 0; i < 4 ; i++)
    {
        //qDebug() << "Corner " << i <<" is hovered?: " << _corners[i]->isHovered();

        if(_corners[i]->isHovered())
            return i;

    }
    return -1;  // no corner is hovered.
}

/**
 * @brief SelectableBoxGraphic::getAllChildren
 * @param stateList
 *
 * this recursive function adds all the children states and grandchildren states recursively into the Given QList
 */
void SelectableBoxGraphic::getAllChildren(QList<SelectableBoxGraphic*> &stateList)
{
    //qDebug() << "children items "<< this->childItems().count();
    for(int i = 0; i < this->childItems().count(); i++)
    {
        SelectableBoxGraphic* state = dynamic_cast<SelectableBoxGraphic*>(this->childItems()[i]);
        if(state)
        {
            stateList.append(state);
            state->getAllChildren(stateList);
        }
    }
}

// for supporting moving the box across the scene
void SelectableBoxGraphic::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    if(_keepInsideParent)
    {

    }
    else
    {
        event->setAccepted(true);
        QPointF location = this->pos();
        location.setX( ( static_cast<qreal>(location.x()) / _gridSpace) * _gridSpace );
        location.setY( ( static_cast<qreal>(location.y()) / _gridSpace) * _gridSpace );

        this->setPos(location);

        //qDebug() << "MOUSE RELEASE : " << this->pos() << "";
       // emit stateBoxMoved(this->pos());
    }

    // will call the corresponding overrided graphicHasChanged function for a subclass
    graphicHasChanged();
}


// for supporting moving the box across the scene
void SelectableBoxGraphic::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    _dragStart = event->pos();
}


// for supporting moving the box across the scene
/**
 * @brief SelectableBoxGraphic::mouseMoveEvent
 * @param event
 */
void SelectableBoxGraphic::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    qDebug() << "mouse move event!";
    QPointF newPos = event->pos() ;
    QPointF location = this->pos();
    QPointF diff = newPos -_dragStart;
    location += diff;

    //qDebug() << "Drag Start:\t\t"<<_dragStart<<"\nnewPos: "<<newPos<<"\ntest:\t\t"<<test;

    // if keep inside parent is true, then restrict movement of the box to within the parent
    SelectableBoxGraphic* parent = parentAsSelectableBoxGraphic();
    if(_keepInsideParent && parent)
    {
        qreal x = pos().x()+diff.x();
        qreal y = pos().y()+diff.y();
        const qreal w = this->getSize().x();
        const qreal h = this->getSize().y();


        const qreal parentW = parent->getSize().x();
        const qreal parentH = parent->getSize().y();

        if(x < 0+INSIDE_PARENT_BUFFER)
            x=0+INSIDE_PARENT_BUFFER;

        else if(x+w > parentW-INSIDE_PARENT_BUFFER)
            x=parentW-INSIDE_PARENT_BUFFER-w;

        if(y < 0+INSIDE_PARENT_BUFFER)
            y=0+INSIDE_PARENT_BUFFER;

        else if(y+h > parentH-INSIDE_PARENT_BUFFER)
            y=parentH-INSIDE_PARENT_BUFFER-h;


        QPointF newPoint(x,y);


        this->setPos(newPoint);

        //this->graphicHasChanged();
    }
    else
    {
        emit stateBoxMoved(diff);     // emit stateBoxMoved to signal the children transition graphics to update their sink anchors
        QList<SelectableBoxGraphic*> children;
        this->getAllChildren(children);

        // also emit statebox moved for all children STATE BOXES

        for(int i = 0; i < children.size();i++)
        {
            SelectableBoxGraphic* st = dynamic_cast<SelectableBoxGraphic*>(children.at(i));
            SelectableTextBlock* tb = dynamic_cast<SelectableTextBlock*> (children.at(i));

            // enforce that this is a state and not a text block
            if(st && !tb)
            {
                emit children.at(i)->stateBoxMoved(diff);
            }
        }
        this->setPos(location);
    }

}

void SelectableBoxGraphic::setMinSize(QPoint size)
{
    _minSize = size;
}


/**
 * @brief SelectableBoxGraphic::setSizeAndUpdateAnchors
 * @param size
 *
 * in addition to setting the size, update the anchors attached to this state
 *
 * SIGNAL emitStateBoxResized
 * connect in SCGraphicsView
 * updates the target and parent graphics associated with a transition
 *
 *  // create the connect to automatically move anchor elbows when state graphics are moved.
    connect(parentGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleParentStateGraphicResized(QRectF, QRectF, int)));
    connect(targetGraphic, SIGNAL(stateBoxResized(QRectF, QRectF, int)),transGraphic, SLOT(handleTargetStateGraphicResized(QRectF, QRectF, int)));
 *
 */
void SelectableBoxGraphic::setSizeAndUpdateAnchors(QPointF size)
{
    //QRectF oldBox(pos(), getSize());
    //QRectF newBox(pos(), size);

    QPointF mts = (pos());

    QRectF oldBox = QRectF(mts.x(), mts.y(), _width, _height);
    QRectF newBox = QRectF(mts.x(), mts.y(), size.x(), size.y());

    _width = size.x();
    _height = size.y();
    //getSize();
    this->setSize(size);


    emit stateBoxResized(oldBox, newBox, 2);    // treat as if the bottom rightcorner was manipulated

    this->update();

}

/**
 * @brief SelectableBoxGraphic::setPosAndUpdateAnchors
 *
 * in addition to setPos, this function will also update all anchors
 *
 */
void SelectableBoxGraphic::setPosAndUpdateAnchors(QPointF newPos)
{
    QPointF location = this->pos();
    QPointF diff = newPos - location;

    //qDebug() << "Drag Start:\t\t"<<_dragStart<<"\nnewPos: "<<newPos<<"\ntest:\t\t"<<test;


    emit stateBoxMoved(diff);     // emit stateBoxMoved to signal the children transition graphics to update their anchors
    QList<SelectableBoxGraphic*> children;
    this->getAllChildren(children);

    // also emit statebox moved for all children
    for(int i = 0; i < children.size();i++)
    {
       // qDebug() << "i " << children.at(i)->objectName();
        emit children.at(i)->stateBoxMoved(diff);
    }

    this->setPos(newPos);
}

// remove the corner grabbers
void SelectableBoxGraphic::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    _isHovered = false;

    _corners[0]->setParentItem(NULL);
    _corners[1]->setParentItem(NULL);
    _corners[2]->setParentItem(NULL);
    _corners[3]->setParentItem(NULL);

    delete _corners[0];
    _corners[0] = NULL;

    delete _corners[1];
    _corners[1] = NULL;

    delete _corners[2];
    _corners[2] = NULL;

    delete _corners[3];
    _corners[3] = NULL;

}


// create the corner grabbers

void SelectableBoxGraphic::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    //qDebug() << "SelectableBoxGraphic HoverEnterEvent";
    _isHovered = true;


    _corners[0] = new CornerGrabber(this,0, true);
    _corners[1] = new CornerGrabber(this,1, true);
    _corners[2] = new CornerGrabber(this,2, true);
    _corners[3] = new CornerGrabber(this,3, true);


    _corners[0]->setPaintStyle( CornerGrabber::kCrossHair);
    _corners[1]->setPaintStyle( CornerGrabber::kCrossHair);
    _corners[2]->setPaintStyle( CornerGrabber::kCrossHair);
    _corners[3]->setPaintStyle( CornerGrabber::kCrossHair);

    _corners[0]->installSceneEventFilter(this);
    _corners[1]->installSceneEventFilter(this);
    _corners[2]->installSceneEventFilter(this);
    _corners[3]->installSceneEventFilter(this);

    setCornerPositions();

}

void SelectableBoxGraphic::setCornerPositions()
{

    _corners[0]->setPos(_drawingOrigenX, _drawingOrigenY);
    _corners[1]->setPos(_drawingWidth,  _drawingOrigenY);
    _corners[2]->setPos(_drawingWidth , _drawingHeight);
    _corners[3]->setPos(_drawingOrigenX, _drawingHeight);

}

QRectF SelectableBoxGraphic::boundingRect() const
{
    return QRectF(0,0,_width,_height);
}

/**
 * @brief SelectableBoxGraphic::getVisibleCenter
 * @return
 *
 * get the scene coordinates of the center of the box
 */
QPointF SelectableBoxGraphic::getVisibleCenter()
{
    QRectF area = getUsableArea();

    double centerX = ((area.x() + area.width())/2.0);
    double centerY = ((area.y() + area.height())/2.0);

    return mapToScene(QPointF(centerX, centerY));
}

QRectF SelectableBoxGraphic::getUsableArea()
{
    int x0 = _drawingOrigenX + 2;
    int y0 = _drawingOrigenY + 2;
    int width = _drawingWidth -2 ;
    int height = _drawingHeight -2;

    return QRectF(x0,y0, width, height);
}


QPointF SelectableBoxGraphic::getSideCenterPointInSceneCoord(int side)
{
    switch (side)
    {
    case NORTH:
        return mapToScene( QPointF ( (_drawingOrigenX +  _drawingWidth)/2, _drawingOrigenY ));
        break;

    case EAST:
        return  mapToScene( QPointF ( _drawingOrigenX +_drawingWidth, (_drawingOrigenY + _drawingHeight)/2));
        break;

    case SOUTH:
        return  mapToScene( QPointF ( (_drawingOrigenX +  _drawingWidth)/2,_drawingOrigenY +  _drawingHeight));
        break;

    case WEST:
        return  mapToScene( QPointF ( _drawingOrigenX, (_drawingOrigenY + _drawingHeight)/2));
        break;
    }

    return QPointF();
}

void SelectableBoxGraphic::paintWithVisibleBox (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    int shadowThickness = 5;

    if ( _boxStyle == kSolidWithShadow )
    {

        /*
     The drop shadow effect will be created by drawing a filled, rounded corner rectangle with a gradient fill.
     Then on top of this will be drawn  filled, rounded corner rectangle, filled with a solid color, and offset such that the gradient filled
     box is only visible below for a few pixels on two edges.

     The total box size is _width by _height. So the top box will start at (0,0) and go to (_width-shadowThickness, _height-shadowThickness),
     while the under box will be offset, and start at (shadowThickness+0, shadowThickness+0) and go to  (_width, _height).
       */


        QLinearGradient gradient;
        gradient.setStart(_drawingOrigenX,_drawingOrigenY);
        gradient.setFinalStop( _drawingWidth ,_drawingOrigenY);
        QColor grey1(125,125,125,125);// starting color of the gradient - can play with the starting color and ,point since its not visible anyway

        // grey2 is ending color of the gradient - this is what will show up as the shadow. the last parameter is the alpha blend, its set
        // to 125 allowing a mix of th color and and the background, making more realistic shadow effect.
        QColor grey2(225,225,225,125);


        gradient.setColorAt((qreal)0, grey1 );
        gradient.setColorAt((qreal)1, grey2 );

        QBrush brush(gradient);

        painter->setBrush( brush);

        // for the desired effect, no border will be drawn, and because a brush was set, the drawRoundRect will fill the box with the gradient brush.
        QPen pen;
        pen.setStyle(Qt::NoPen);
        painter->setPen(pen);

        QPointF topLeft (_drawingOrigenX,_drawingOrigenX);
        QPointF bottomRight ( _drawingWidth , _drawingHeight);

        QRectF rect (topLeft, bottomRight);

        painter->drawRoundRect(rect,25,25); // corner radius of 25 pixels
    }

    // draw the top box, the visible one

    if ( _isHighlighted )
        _pen.setColor(Qt::red);
    else
        _pen.setColor(Qt::black);

    if ( _isHovered )
         _pen.setWidth(BOX_HOVER_PEN_WIDTH);
    else
        _pen.setWidth(BOX_DEFAULT_PEN_WIDTH);

    if ( _drawBoxLineStyle == kDrawSolid )
        _pen.setStyle( Qt::SolidLine );
    else
        _pen.setStyle( Qt::DotLine );

    painter->setPen(_pen);


    if ( _boxStyle != kTransparent )
    {
        // QBrush brush2(QColor(187,250,185,255),Qt::SolidPattern);  // the box fill color
         QBrush brush2(QColor(255,255,255,255),Qt::SolidPattern);  // white fill
         painter->setBrush( brush2);
    }


    // draw outter layer box
    // if shadowed box, draw inside, else draw on the outter edge
    QRectF rect2;

    if ( _boxStyle == kSolidWithShadow )
    {
        QPointF topLeft2 (_drawingOrigenX, _drawingOrigenY);
        QPointF bottomRight2 ( _drawingWidth - shadowThickness, _drawingHeight - shadowThickness);

        rect2 = QRectF (topLeft2, bottomRight2);
    }
    else
    {
        QPointF topLeft2 (_drawingOrigenX, _drawingOrigenY);
        QPointF bottomRight2 ( _drawingWidth, _drawingHeight);

        rect2 = QRectF (topLeft2, bottomRight2);
    }

    painter->drawRoundRect(rect2,25,25);

}


void SelectableBoxGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    if  ( (_showBoxStyle == kAlways) || (( _showBoxStyle == kWhenSelected) && ( _isHovered == true)))
    {
        painter->setRenderHint(QPainter::HighQualityAntialiasing);
        paintWithVisibleBox (painter,0,0);
    }
    //else no painting required
}


void SelectableBoxGraphic::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void SelectableBoxGraphic::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

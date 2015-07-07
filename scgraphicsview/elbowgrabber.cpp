#include "elbowgrabber.h"
#include "transitiongraphic.h"
#include "arrowheadgraphic.h"

ElbowGrabber::ElbowGrabber(TransitionGraphic* parentGraphic) :
    mouseDownX(0),
    mouseDownY(0),
    _outterborderColor(Qt::black),
    _outterborderPen(),
    _width(6),
    _height(6),
    _corner(0),
    _mouseButtonState(kMouseReleased),
    _placedOnASquare(false),
    _paintStyle(kBox),
    _arrowAngle(0),
    _arrowHead(NULL)
{
    this->setParent(parentGraphic);
    _outterborderPen.setWidth(2);
    _outterborderPen.setColor(_outterborderColor);
    this->setAcceptHoverEvents(true);
}

ElbowGrabber::~ElbowGrabber()
{

}

void ElbowGrabber::setPaintStyle(PaintStyle s)
{
    _paintStyle  = s;

    if (s == kArrowHead )
    {
        if ( ! _arrowHead )
        {
            _arrowHead = new ArrowHeadGraphic(this);
            _arrowHead->setWidth(_width);
            _arrowHead->setHeight(_height);
            _arrowHead->setPos( - _width/2, - _height/2);
            _arrowHead->setColor(Qt::red);
        }
    }
    else
    {
        if ( _arrowHead )
        {
            delete _arrowHead;
            _arrowHead = NULL;
        }
    }
    this->update();
}

void ElbowGrabber::setMouseState(int s)
{
    _mouseButtonState = s;
}

int ElbowGrabber::getMouseState()
{
    return _mouseButtonState;
}

int ElbowGrabber::getCorner()
{
    return _corner;
}


// we have to implement the mouse events to keep the linker happy,
// but just set accepted to false since are not actually handling them

void ElbowGrabber::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void ElbowGrabber::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void ElbowGrabber::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
}

void ElbowGrabber::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(false);
}

void ElbowGrabber::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(false);
}


// change the color on hover events to indicate to the use the object has
// been captured by the mouse

void ElbowGrabber::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    _outterborderColor = Qt::black;
    this->update(0,0,_width,_height);
}

void ElbowGrabber::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    _outterborderColor = Qt::red;
    this->update(0,0,_width,_height);
}

QRectF ElbowGrabber::boundingRect() const
{
    int captureMargin = 12;
    QPointF topLeft (0-(_width/2)-captureMargin, 0-(_height/2)-captureMargin);
    QPointF bottomRight ( (_width/2)+captureMargin ,(_height/2)+captureMargin);

    QRectF rect (topLeft, bottomRight);
    return rect;
}

QPointF ElbowGrabber::getCenterPoint()
{

     if (_placedOnASquare)
     {
         if ( _corner == 0 )
             return QPointF(pos().x() + _width/2,pos().y() + _height/2);
         else  if ( _corner == 1 )
             return QPointF(pos().x() - _width/2,pos().y() + _height/2);
         else  if ( _corner == 2 )
             return QPointF(pos().x() - _width/2,pos().y() - _height/2);
         else
             return QPointF(pos().x() + _width/2,pos().y() - _height/2);
     }
     else
     {
         return QPointF(pos()) ;
     }

}

void ElbowGrabber::setAngle(int angle)
{
    _arrowAngle = angle - 45; // subtract 45 because of how we draw it
}

void ElbowGrabber::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    if ( _paintStyle == kBox)
    {
        // fill the box with solid color, use sharp corners

        _outterborderPen.setCapStyle(Qt::SquareCap);
        _outterborderPen.setStyle(Qt::SolidLine);
        painter->setPen(_outterborderPen);

        QPointF topLeft (0-(_width/2), 0-(_height/2));
        QPointF bottomRight ( _width/2 ,_height/2);

        QRectF rect (topLeft, bottomRight);

        QBrush brush (Qt::SolidPattern);
        brush.setColor (_outterborderColor);

        if ( _outterborderColor == Qt::red)
        {
            painter->fillRect(rect,brush);
        }
        else
        {
            painter->drawRect(rect);
        }
    }
    else  if (_paintStyle == kCrossHair)
    {
        painter->setPen( _outterborderColor );

        // center of the box is at 0,0
        painter->drawLine( QPointF(0,0), QPointF(0,-(_height/2)) );
        painter->drawLine( QPointF(0,0), QPointF(0, (_height/2)) );
        painter->drawLine( QPointF(0,0), QPointF(-(_width/2) , 0) );
        painter->drawLine( QPointF(0,0), QPointF((_width/2) , 0) );
    }
    else  if (_paintStyle == kArrowHead)
    {
       // qDebug()<< "setting arrowhead angle: " << _arrowAngle;

        _arrowHead->setRotation(_arrowAngle);

        _arrowHead->update();
    }

}

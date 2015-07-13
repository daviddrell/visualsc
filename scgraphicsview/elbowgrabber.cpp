#include "elbowgrabber.h"
#include "transitiongraphic.h"
#include "arrowheadgraphic.h"
#include <QDebug>



ElbowGrabber::ElbowGrabber(TransitionGraphic* parentGraphic) :
    mouseDownX(0),
    mouseDownY(0),
    _outterborderColor(TRANSITION_DEFAULT_COLOR),
    _outterborderPen(),
    _width(6),
    _height(6),
    _snappedSide(0),
    _mouseButtonState(kMouseReleased),
    _placedOnASquare(false),
    _paintStyle(ELBOW_DEFAULT_PAINT_STYLE),
    _arrowAngle(0),
    _arrowHead(NULL),
    _anchor(false),
    _terminator(false)
{

    this->setParentItem(parentGraphic);
   // qDebug() << "Elbow Parent: " << parentGraphic << " pos: "<< this->pos();

    _outterborderPen.setWidth(ELBOW_DEFAULT_WIDTH);
    _outterborderPen.setColor(_outterborderColor);
    //this->setAcceptHoverEvents(true);

    _segments[0] = NULL;
    _segments[1] = NULL;
}

ElbowGrabber::ElbowGrabber(TransitionGraphic* parentGraphic, QPointF point) :
    mouseDownX(0),
    mouseDownY(0),
    _outterborderColor(TRANSITION_DEFAULT_COLOR),
    _outterborderPen(),
    _width(6),
    _height(6),
    _snappedSide(0),
    _mouseButtonState(kMouseReleased),
    _placedOnASquare(false),
    _paintStyle(ELBOW_DEFAULT_PAINT_STYLE),
    _arrowAngle(0),
    _arrowHead(NULL),
    _anchor(false),
    _terminator(false)
{

    this->setParentItem(parentGraphic);

    this->setPos(point);
   // this->setPos(0,0);
   // qDebug() << "Elbow Parent: " << parentGraphic << " pos: "<< this->pos();

    _outterborderPen.setWidth(ELBOW_DEFAULT_WIDTH);
    _outterborderPen.setColor(_outterborderColor);
    //this->setAcceptHoverEvents(true);

    _segments[0] = NULL;
    _segments[1] = NULL;
}

void ElbowGrabber::setSnappedSide(int side)
{
    _snappedSide = side;
}

int ElbowGrabber::getSnappedSide()
{
    return _snappedSide;
}

bool ElbowGrabber::isAnchor()
{

    return _anchor;
}

void ElbowGrabber::setAnchor(bool isAnchor)
{
    _anchor = isAnchor;
}

ElbowGrabber::~ElbowGrabber()
{

}

bool ElbowGrabber::isTerminal()
{
    return _terminator;
}

void ElbowGrabber::setTerminal(bool isTerminal)
{
    _terminator = isTerminal;
}

void ElbowGrabber::setSegmentOne(LineSegmentGraphic* segOne)
{
    _segments[0] = segOne;
}

void ElbowGrabber::setSegmentTwo(LineSegmentGraphic* segTwo)
{
    _segments[1] = segTwo;
}

LineSegmentGraphic* ElbowGrabber::getSegment(int index)
{
    return _segments[index];
}

TransitionGraphic* ElbowGrabber::parentAsTransitionGraphic()
{
    return static_cast<TransitionGraphic*>(this->parentItem());
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
            _arrowHead->setColor(TRANSITION_DEFAULT_COLOR);
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

void ElbowGrabber::forceHoverEnterEvent()
{
    this->hoverEnterEvent(NULL);
}

void ElbowGrabber::forceHoverLeaveEvent()
{
    this->hoverLeaveEvent(NULL);
}

void ElbowGrabber::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    _outterborderColor = TRANSITION_DEFAULT_COLOR;
    _outterborderPen.setWidth(ELBOW_DEFAULT_WIDTH);
    _outterborderPen.setColor(TRANSITION_DEFAULT_COLOR);
    _width = ELBOW_ARROWHEAD_DEFAULT_WIDTH;
    _height = ELBOW_ARROWHEAD_DEFAULT_WIDTH;
    //this->update(0,0,_width,_height);
}

void ElbowGrabber::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
   // qDebug() << "ElbowGrabber HoverEnterEvent";
    _outterborderColor = TRANSITION_HOVER_COLOR;
    _outterborderPen.setWidth(ELBOW_HOVER_WIDTH);
    _outterborderPen.setColor(TRANSITION_HOVER_COLOR);
    _width = ELBOW_ARROWHEAD_HOVER_WIDTH;
    _height = ELBOW_ARROWHEAD_HOVER_WIDTH;
    //_outterborderPen.setStyle();
    //this->update(0,0,_width,_height);
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
         /*
         if ( _corner == 0 )
             return QPointF(pos().x() + _width/2,pos().y() + _height/2);
         else  if ( _corner == 1 )
             return QPointF(pos().x() - _width/2,pos().y() + _height/2);
         else  if ( _corner == 2 )
             return QPointF(pos().x() - _width/2,pos().y() - _height/2);
         else
             return QPointF(pos().x() + _width/2,pos().y() - _height/2);
             */
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

void ElbowGrabber::updateArrowHead()
{
    // if this is the terminal line segment, then redraw the arrow head with correct angles
    if ( _terminator )
    {
        ElbowGrabber* sisterElbow = this->getSegment(0)->getElbow(0); // a terminator elbow only has one socketed line
        double dy =  this->y() - sisterElbow->y();
        double dx =  this->x() - sisterElbow->x();
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

        this->setAngle(angle);
        this->update();
    }
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
        updateArrowHead();
        _arrowHead->setRotation(_arrowAngle);
        _arrowHead->setColor(_outterborderPen.color());
        _arrowHead->setWidth(_width);
        _arrowHead->setHeight(_height);
        _arrowHead->setPos( - _width/2, - _height/2);
        _arrowHead->update();
    }

}

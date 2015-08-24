#include "elbowgrabber.h"
#include "transitiongraphic.h"
#include "arrowheadgraphic.h"
#include <QDebug>

#define PI                  3.14159265359
#define SNAP_ANGLE          11
#define SNAP_DISTANCE       30

#define ELBOW_DEFAULT_WIDTH 2
#define ELBOW_HOVER_WIDTH 3
#define ELBOW_DEFAULT_PAINT_STYLE kBox
#define DEFAULT_OUTTER_BORDER_WIDTH 2.61
#define DEFAULT_OUTTER_BORDER_HEIGHT 2.61
#define HOVER_OUTTER_BORDER_WIDTH 5.61
#define HOVER_OUTTER_BORDER_HEIGHT 5.61
#define ELBOW_ARROWHEAD_DEFAULT_WIDTH 6;
#define ELBOW_ARROWHEAD_HOVER_WIDTH 8;


ElbowGrabber::ElbowGrabber(TransitionGraphic* parentGraphic, KeyController* keys) :
    mouseDownX(0),
    mouseDownY(0),
    //_outterborderColor(_defaultColor),
    _outterborderPen(),
    _width(DEFAULT_OUTTER_BORDER_WIDTH),
    _height(DEFAULT_OUTTER_BORDER_HEIGHT),
    _snappedSide(0),
    _mouseButtonState(kMouseReleased),
    _placedOnASquare(false),
    _paintStyle(ELBOW_DEFAULT_PAINT_STYLE),
    _arrowAngle(0),
    _arrowHead(NULL),
    _anchor(false),
    _terminator(false),
    _keyController(keys)
{
    _segments[0] = NULL;
    _segments[1] = NULL;
    _defaultColor=QColor(0,0,125,255);
    _hoverColor=QColor(216,0,0,255);

    this->setParentItem(parentGraphic);
   // qDebug() << "Elbow Parent: " << parentGraphic << " pos: "<< this->pos();



    _outterborderPen.setWidth(ELBOW_DEFAULT_WIDTH);
    _outterborderPen.setColor(_defaultColor);
    //this->setAcceptHoverEvents(true);




}

ElbowGrabber::ElbowGrabber(TransitionGraphic* parentGraphic, QPointF point, KeyController* keys) :
    mouseDownX(0),
    mouseDownY(0),
    //_outterborderColor(_defaultColor),
    _outterborderPen(),
    _width(DEFAULT_OUTTER_BORDER_WIDTH),
    _height(DEFAULT_OUTTER_BORDER_HEIGHT),
    _snappedSide(0),
    _mouseButtonState(kMouseReleased),
    _placedOnASquare(false),
    _paintStyle(ELBOW_DEFAULT_PAINT_STYLE),
    _arrowAngle(0),
    _arrowHead(NULL),
    _anchor(false),
    _terminator(false),
    _keyController(keys)
{
    _segments[0] = NULL;
    _segments[1] = NULL;
    _defaultColor=QColor(0,0,125,255);
    _hoverColor=QColor(216,0,0,255);

    this->setParentItem(parentGraphic);

     this->setPos(point);
   // this->setPos(0,0);
   // qDebug() << "Elbow Parent: " << parentGraphic << " pos: "<< this->pos();

    _outterborderPen.setWidth(ELBOW_DEFAULT_WIDTH);
    _outterborderPen.setColor(_defaultColor);
    //this->setAcceptHoverEvents(true);




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

void ElbowGrabber::setSegmentAt(int index,LineSegmentGraphic* segOne)
{
    _segments[index] = segOne;
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
            _width = ELBOW_ARROWHEAD_DEFAULT_WIDTH;
            _height = ELBOW_ARROWHEAD_DEFAULT_WIDTH;
            _arrowHead->setWidth(_width);
            _arrowHead->setHeight(_height);
            _arrowHead->setPos( - _width/2, - _height/2);
            _arrowHead->setColor(_defaultColor);
        }
    }
    else
    {
        if ( _arrowHead )
        {
            _width = DEFAULT_OUTTER_BORDER_WIDTH;
            _height = DEFAULT_OUTTER_BORDER_HEIGHT;
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
    qDebug() <<"ElbowGraber::mouseMoveEvent QGraphicsSceneDragDropEvent";
    event->setAccepted(false);
}

void ElbowGrabber::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() <<"ElbowGraber::mousePressEvent QGraphicsSceneDragDropEvent";
    event->setAccepted(false);
}

void ElbowGrabber::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    qDebug() <<"ElbowGraber::mouseReleaseEvent QGraphicsSceneMouseEvent";
    event->setAccepted(true);
}

void ElbowGrabber::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    qDebug() <<"ElbowGraber::mousePressEvent QGraphicsSceneMouseEvent";
    event->setAccepted(false);
}

void ElbowGrabber::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    qDebug() << "ElbowGrabber::mouseMoveEvent QGraphicsSceneMouseEvent";
    event->setAccepted(false);
}

void ElbowGrabber::forceLineHoverLeaveEvent()
{
    LineSegmentGraphic* line = _segments[0];
    qDebug() << "forcine line... to stop";
    if(line && line->isHovered())
    {
        qDebug() << "forcine line 0 to stop";
        line->forceHoverLeaveEvent();
    }
    line = _segments[1];
    if(line && line->isHovered())
    {
        qDebug() << "forcing line 1 to stop";
        line->forceHoverLeaveEvent();
    }
}

// change the color on hover events to indicate to the use the object has
// been captured by the mouse

void ElbowGrabber::forceHoverEnterEvent()
{

    // set this parent's hovered elbow to this
    this->parentAsTransitionGraphic()->setCurrentlyHoveredElbow(this);

    connect(_keyController, SIGNAL(keyPressed(int)), dynamic_cast<QObject*>(this->parentItem()), SLOT(handleElbowKeyPressEvent(int)));

    _outterborderColor = _hoverColor;
    _outterborderPen.setWidth(ELBOW_HOVER_WIDTH);
    _outterborderPen.setColor(_hoverColor);

    if(_paintStyle==kArrowHead)
    {
        _width = ELBOW_ARROWHEAD_HOVER_WIDTH;
        _height = ELBOW_ARROWHEAD_HOVER_WIDTH;
    }
    else
    {
        _width = HOVER_OUTTER_BORDER_WIDTH;
        _height = HOVER_OUTTER_BORDER_WIDTH;
    }

    //_outterborderPen.setStyle();
    //this->update(0,0,_width,_height);
}

void ElbowGrabber::forceHoverLeaveEvent()
{

    // clear this parent's hovered elbow
    this->parentAsTransitionGraphic()->clearCurrentlyHoveredElbow();

    disconnect(_keyController, SIGNAL(keyPressed(int)), dynamic_cast<QObject*>(this->parentItem()), SLOT(handleElbowKeyPressEvent(int)));

    _outterborderColor = _defaultColor;
    _outterborderPen.setWidth(ELBOW_DEFAULT_WIDTH);
    _outterborderPen.setColor(_defaultColor);
    if(_paintStyle==kArrowHead)
    {
        _width = ELBOW_ARROWHEAD_DEFAULT_WIDTH;
        _height = ELBOW_ARROWHEAD_DEFAULT_WIDTH;
    }
    else
    {
        _width = DEFAULT_OUTTER_BORDER_WIDTH;
        _height = DEFAULT_OUTTER_BORDER_HEIGHT;
    }

    //this->update(0,0,_width,_height);
}


// Hover Events now received in Transition Graphic Scene Event filter
/*
void ElbowGrabber::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    qDebug() << "ElbowGrabber HoverLeaveEvent";
    _outterborderColor = _defaultColor;
    _outterborderPen.setWidth(ELBOW_DEFAULT_WIDTH);
    _outterborderPen.setColor(_defaultColor);
    _width = ELBOW_ARROWHEAD_DEFAULT_WIDTH;
    _height = ELBOW_ARROWHEAD_DEFAULT_WIDTH;
    //this->update(0,0,_width,_height);
}

void ElbowGrabber::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
   qDebug() << "ElbowGrabber HoverEnterEvent";
    _outterborderColor = _hoverColor;
    _outterborderPen.setWidth(ELBOW_HOVER_WIDTH);
    _outterborderPen.setColor(_hoverColor);
    _width = ELBOW_ARROWHEAD_HOVER_WIDTH;
    _height = ELBOW_ARROWHEAD_HOVER_WIDTH;
    //_outterborderPen.setStyle();
    //this->update(0,0,_width,_height);
}
*/
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


         return QPointF(pos()) ;


}

void ElbowGrabber::setAngle(int angle)
{
    _arrowAngle = angle - 45; // subtract 45 because of how we draw it
}

//void ElbowGrabber::setX(qreal x)
//{
//    straightenLines(this);
//    QGraphicsItem::setX(x);
//}

void ElbowGrabber::setXSnap(qreal x)
{
    QGraphicsItem::setX(x);
    straightenLines();
}

void ElbowGrabber::setYSnap(qreal y)
{
    QGraphicsItem::setY(y);
    straightenLines();
}

void ElbowGrabber::setPosSnap(QPointF point)
{
    QGraphicsItem::setPos(point);
    straightenLines();
}

void ElbowGrabber::setPosSnap(qreal x, qreal y)
{
    QGraphicsItem::setPos(x,y);
    straightenLines();
}

qreal ElbowGrabber::distanceX(QPointF one, QPointF two)
{
    return fabs(one.x()-two.x());
}

qreal ElbowGrabber::distanceY(QPointF one, QPointF two)
{
    return fabs(one.y()-two.y());
}

qreal ElbowGrabber::distance(ElbowGrabber* one, ElbowGrabber* two)
{
    qreal x1 = one->x();
    qreal y1 = one->y();
    qreal x2 = two->x();
    qreal y2 = two->y();

    return sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
}

int ElbowGrabber::getZone(qreal angle)
{
   if( angle > (90 - SNAP_ANGLE) && angle <= 90)
       return Zone::VERTICAL;
   else if( angle >= 0 && angle < SNAP_ANGLE)
       return Zone::FLAT;

   return Zone::ANGLED;
}

int ElbowGrabber::getZone(QPointF pos)
{
    qreal xDist = distanceX(this->pos(), pos);
    qreal yDist = distanceY(this->pos(), pos);

    bool xCross = xDist <= SNAP_DISTANCE;
    bool yCross = yDist <= SNAP_DISTANCE;

    if(xCross&&yCross)
    {
        if(xDist < yDist)
            return Zone::VERTICAL;
        else
            return Zone::FLAT;

    }
    else if(xCross )
    {
        // check if it is within a range
        return Zone::VERTICAL;
    }
    else if(yCross)
    {
        return Zone::FLAT;
    }
    else
    {
        return Zone::ANGLED;
    }
}


/**
 * @brief TransitionGraphic::straightenLines
 * @param mid
 *
 * used to snap elbows to other elbows
 *
 */
void ElbowGrabber::straightenLines()
{
    ElbowGrabber* mid = this;

    int leftZone = -1;
    int rightZone = -1;

    qreal leftDeg;
    qreal rightDeg;

    // check the elbows around the middle one, get their angle and relative zone (~90, ~45, ~0) if the elbow exists
    LineSegmentGraphic* one = mid->getSegment(0);
    ElbowGrabber* left;
    if(one)
    {
        left = one->getElbow(0);
        if(left)
        {
            qreal m = (mid->y() - left->y())/(mid->x()- left->x());  // y = mx + b
            qreal theta = atan(m);          // find the angle between the two points
            leftDeg = fabs(theta * 180/PI);

            //leftZone = getZone(leftDeg);
            leftZone = getZone(left->pos());
            //qDebug()<<"angle between left and mid is " << thetaDeg<<" in zone "<<zone;
        }
    }

    LineSegmentGraphic* two = mid->getSegment(1);
    ElbowGrabber* right;
    if(two)
    {
        right = two->getElbow(1);
        if(right)
        {
            qreal m = (mid->y() - right->y())/(mid->x()- right->x());  // y = mx + b
            qreal theta = atan(m);          // find the angle between the two points
            rightDeg = fabs(theta * 180/PI);

            //rightZone = getZone(rightDeg);
            rightZone = getZone(right->pos());
            //qDebug()<<"angle between mid and right is " << thetaDeg<<" in zone "<<zone;

        }
    }

    // if the elbows compete for the same zone, the select the closer one to snap to
    if(rightZone == leftZone && rightZone!=-1)
    {
        qreal leftToMid = distance(left,mid);
        qreal rightToMid = distance(mid,right);

        if(leftToMid > rightToMid)
        {
            switch(rightZone)
            {
            case Zone::FLAT:
                mid->QGraphicsItem::setY(right->y());
                break;
            case Zone::ANGLED:
                break;
            case Zone::VERTICAL:
                mid->QGraphicsItem::setX(right->x());
                break;
            }
        }
        else
        {
            switch(leftZone)
            {
            case Zone::FLAT:
                mid->QGraphicsItem::setY(left->y());
                break;
            case Zone::ANGLED:
                break;
            case Zone::VERTICAL:
                mid->QGraphicsItem::setX(left->x());
                break;
            }
        }
    }
    else    // check the neighbor elbows and snap them if needed
    {
        if(rightZone!=-1)
        {
            switch(rightZone)
            {
            case Zone::FLAT:
                mid->QGraphicsItem::setY(right->y());
                break;
            case Zone::ANGLED:
                break;
            case Zone::VERTICAL:
                mid->QGraphicsItem::setX(right->x());
                break;
            }
        }
        if(leftZone!=-1)
        {
            switch(leftZone)
            {
            case Zone::FLAT:
                mid->QGraphicsItem::setY(left->y());
                break;
            case Zone::ANGLED:
                break;
            case Zone::VERTICAL:
                mid->QGraphicsItem::setX(left->x());
                break;
            }
        }
    }




    // if this is an anchor, ensure that it is still attached to its parent state box graphic
    if(mid->isAnchor())
        emit mid->anchorMoved(this->parentAsTransitionGraphic()->parentItemAsStateBoxGraphic()->mapToScene(this->pos()));

    // make sure the line segment path is updated as well
    this->encloseLinePaths();

    this->update();

}

/**
 * @brief ElbowGrabber::encloseLinePaths
 *
 * redraws the paths of line segments attached to this elbow
 *
 */
void ElbowGrabber::encloseLinePaths()
{
    if(this->getSegment(0))
        this->getSegment(0)->enclosePathInElbows();

    if(this->getSegment(1))
        this->getSegment(1)->enclosePathInElbows();
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

void ElbowGrabber::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    if ( _paintStyle == kBox)
    {
        // fill the box with solid color, use sharp corners

        _outterborderPen.setCapStyle(Qt::RoundCap);
        _outterborderPen.setStyle(Qt::SolidLine);
        painter->setPen(_outterborderPen);

        QPointF topLeft (0-(_width/2.0), 0-(_height/2.0));
        QPointF bottomRight ( _width/2.0 ,_height/2.0);

        QRectF rect (topLeft, bottomRight);

        QBrush brush (Qt::SolidPattern);
        brush.setColor (_outterborderColor);





        painter->drawEllipse(rect);
        painter->fillRect(rect, brush);

//            painter->drawRect(rect);
//            painter->fillRect(rect,brush);

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

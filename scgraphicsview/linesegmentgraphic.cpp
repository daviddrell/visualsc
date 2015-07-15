#include "linesegmentgraphic.h"
#include <QDebug>

LineSegmentGraphic::LineSegmentGraphic()
{

}

LineSegmentGraphic::LineSegmentGraphic(ElbowGrabber *startPoint, ElbowGrabber *endPoint, TransitionGraphic *parentGraphic, KeyController *keys):
    _keyController(keys),
    _isHovered(false)
{
    this->setParentItem(parentGraphic);
    _elbows[0] = startPoint;
    _elbows[1] = endPoint;

    _pen.setWidth(TRANSITION_DEFAULT_WIDTH);
    _pen.setColor(TRANSITION_DEFAULT_COLOR);
    _pen.setStyle(TRANSITION_DEFAULT_LINE_STYLE);

    enclosePathInElbows();
}


LineSegmentGraphic::~LineSegmentGraphic()
{

}

void LineSegmentGraphic::setElbowAt(int index, ElbowGrabber *elb)
{
    _elbows[index] = elb;
}

ElbowGrabber* LineSegmentGraphic::getElbow(int index)
{
    return _elbows[index];
}


TransitionGraphic* LineSegmentGraphic::parentItemAsTransitionGraphic()
{
    return static_cast<TransitionGraphic*>(this->parentItem());
}

void LineSegmentGraphic::setHovered(bool hovered)
{
    _isHovered = hovered;
}

bool LineSegmentGraphic::isHovered()
{
    return _isHovered;
}

void LineSegmentGraphic::forceHoverEnterEvent()
{
   //qDebug() << "hover enter for line";

    _isHovered = true;
    // set parent's currently hovered object to this line segment
     this->parentItemAsTransitionGraphic()->setCurrentlyHoveredLineSegment(this);

    // connect the key controller to this transition graphic
    connect(_keyController, SIGNAL(keyPressed(int)), dynamic_cast<QObject *>(this->parentItem()), SLOT(handleLineSegmentKeyPressEvent(int)));


    _pen.setWidth(TRANSITION_HOVER_WIDTH);
    _pen.setColor(TRANSITION_HOVER_COLOR);
    _pen.setStyle(TRANSITION_HOVER_LINE_STYLE);
}


void LineSegmentGraphic::forceHoverLeaveEvent()
{
   // this->hoverLeaveEvent(NULL);
    _isHovered = false;
    // clear parent's currently hovered object
    this->parentItemAsTransitionGraphic()->clearCurrentlyHoveredLineSegment();

    // connect the key controller to this transition graphic
    disconnect(_keyController, SIGNAL(keyPressed(int)), dynamic_cast<QObject *>(this->parentItem()), SLOT(handleLineSegmentKeyPressEvent(int)));

    _pen.setWidth(TRANSITION_DEFAULT_WIDTH);
    _pen.setColor(TRANSITION_DEFAULT_COLOR);
    _pen.setStyle(TRANSITION_DEFAULT_LINE_STYLE);
}

/*
void LineSegmentGraphic::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    qDebug() << "hover enter for line";

    _isHovered = true;
    // set parent's currently hovered object to this line segment
     this->parentItemAsTransitionGraphic()->setCurrentlyHoveredSegment(this);

    // connect the key controller to this transition graphic
    connect(_keyController, SIGNAL(keyPressed(int)), dynamic_cast<QObject *>(this->parentItem()), SLOT(handleKeyPressEvent(int)));


    _pen.setWidth(TRANSITION_HOVER_WIDTH);
    _pen.setColor(TRANSITION_HOVER_COLOR);
    _pen.setStyle(TRANSITION_HOVER_LINE_STYLE);



}

void LineSegmentGraphic::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    qDebug() << "hover leave for line";
    _isHovered = false;
    // clear parent's currently hovered object
    this->parentItemAsTransitionGraphic()->clearCurrentlyHoveredSegment();

    // connect the key controller to this transition graphic
    disconnect(_keyController, SIGNAL(keyPressed(int)), dynamic_cast<QObject *>(this->parentItem()), SLOT(handleKeyPressEvent(int)));

    _pen.setWidth(TRANSITION_DEFAULT_WIDTH);
    _pen.setColor(TRANSITION_DEFAULT_COLOR);
    _pen.setStyle(TRANSITION_DEFAULT_LINE_STYLE);
}
*/
/**
 * @brief LineSegmentGraphic::enclosePathInElbows
 * called by a line segment to remake its polygon based on its elbows' positions
 */
void LineSegmentGraphic::enclosePathInElbows()
{
    this->enclosePathInCoordindates(_elbows[0]->getCenterPoint().x(), _elbows[0]->getCenterPoint().y(), _elbows[1]->getCenterPoint().x(), _elbows[1]->getCenterPoint().y());
}

void LineSegmentGraphic::enclosePathInItemCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  )
{
    QPointF lineEnd_0;
    QPointF lineEnd_1;

    lineEnd_0.setX(  mapToScene(lineStartX, lineStartY).x()  );
    lineEnd_0.setY(  mapToScene(lineStartX, lineStartY).y() );

    lineEnd_1.setX(  mapToScene(lineEndX,lineEndY ).x());
    lineEnd_1.setY(  mapToScene(lineEndX,lineEndY ).y());

    enclosePathInSceneCoordiates( lineEnd_0.x(), lineEnd_0.y(), lineEnd_1.x(), lineEnd_1.y()  );

}

/**
 * @brief LineSegmentGraphic::enclosePathInCoordindates
 * @param lineStartX
 * @param lineStartY
 * @param lineEndX
 * @param lineEndY
 *
 * Recreates the polygon, or rectangle, for the line segment hover collision using two points
 *
 *
 */
void LineSegmentGraphic::enclosePathInCoordindates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  )
{
    /*
     Line segment hover box

    |<elbow>|
    |<guard>|
             ___________________
            |        ^          |
    X-------x   hover|width     x-------X
            |________v__________|

    ^                                   ^
  elbow                               elbow

    */


    //qDebug() << "theta is: " << theta * 180.0/3.141536 << "\tthe x  multiple would be " << cos(theta) << "\tthe y multiple would be " << sin(theta);

    qreal m = (lineEndY - lineStartY)/(lineEndX - lineStartX);  // y = mx + b
    qreal theta = atan(m);          // find the angle between the two points

    // if the two points are within 2 elbow guards of each other, then the line segment hover box will be a line in the middle of them
    qreal dist = sqrt( (lineEndY - lineStartY)* (lineEndY - lineStartY) + (lineEndX - lineStartX)*(lineEndX - lineStartX)   );
    qreal xElbowGuard, yElbowGuard;
    if(dist < 2*ELBOW_GUARD_BUFFER)
    {
        xElbowGuard = cos(theta) * dist/2.0;
        yElbowGuard = sin(theta) * dist/2.0;
    }
    else
    {
        xElbowGuard = cos(theta) * ELBOW_GUARD_BUFFER;
        yElbowGuard = sin(theta) * ELBOW_GUARD_BUFFER;
    }

    qreal xLineWidth = cos(theta) * LINE_HOVER_WIDTH;
    qreal yLineWidth = sin(theta) * LINE_HOVER_WIDTH;

    QPointF p1, p2, p3, p4;
    if( lineEndX >= lineStartX)     // for quadrants 4 and 1
    {
        // move the two points closer to another by a total of the elbowguard units
        lineStartX += xElbowGuard;
        lineStartY += yElbowGuard;

        lineEndX -= xElbowGuard;
        lineEndY -= yElbowGuard;

        // create the rectangle corners based on the new elbow points
        p1 = QPointF(lineStartX  +  yLineWidth, lineStartY - xLineWidth);
        p2 = QPointF(lineStartX  -  yLineWidth, lineStartY + xLineWidth);
        p3 = QPointF(lineEndX + yLineWidth, lineEndY - xLineWidth);
        p4 = QPointF(lineEndX - yLineWidth, lineEndY + xLineWidth);
    }
    else                        // for quadrants 2 and 3
    {
        // move the two points closer to another by a total of the elbowguard units
        lineStartX -= xElbowGuard;
        lineStartY -= yElbowGuard;

        lineEndX += xElbowGuard;
        lineEndY += yElbowGuard;

         // create the rectangle corners based on the new elbow points
        p1 = QPointF(lineStartX  +  yLineWidth, lineStartY - xLineWidth);
        p2 = QPointF(lineStartX  -  yLineWidth, lineStartY + xLineWidth);
        p3 = QPointF(lineEndX + yLineWidth, lineEndY - xLineWidth);
        p4 = QPointF(lineEndX - yLineWidth, lineEndY + xLineWidth);
    }

    // clear the old polygon
    _selectRegion.clear();

    // connect the four corners of the rectangle
    _selectRegion<<p1<<p2<<p4<<p3<<p1;

    this->setPolygon(_selectRegion);
}

void LineSegmentGraphic::enclosePathInSceneCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  )
{


    /* to draw the select region around the line,
        define a large box(rect p1,p2,p3,p4) around each end of the line.
        then define a box(rect p5,p6,p7,p8) that connects the two end boxes.
        now no matter the angle of the line, we can be sure that its
        surrounded by a symetrical region that will pick up the mouse.


        .    .             .    .
           X-----------------X
        .    .             .    .

        p1  p2             p5  p6

        p4  p3             p8  p7

        the squares have to connect at the two pairs of square corners that are closest.

       */

    QList<QPointF> pointsStart;
    QList<QPointF> pointsEnd;

    pointsStart.append(  QPointF(lineStartX  - CORNER_GRAB_BUFFER, lineStartY - CORNER_GRAB_BUFFER));
    pointsStart.append(  QPointF(lineStartX + CORNER_GRAB_BUFFER, lineStartY - CORNER_GRAB_BUFFER));
    pointsStart.append(  QPointF(lineStartX + CORNER_GRAB_BUFFER, lineStartY + CORNER_GRAB_BUFFER));
    pointsStart.append(  QPointF(lineStartX - CORNER_GRAB_BUFFER, lineStartY + CORNER_GRAB_BUFFER));

    pointsEnd.append(  QPointF(lineEndX - CORNER_GRAB_BUFFER, lineEndY - CORNER_GRAB_BUFFER));
    pointsEnd.append(  QPointF(lineEndX + CORNER_GRAB_BUFFER, lineEndY - CORNER_GRAB_BUFFER));
    pointsEnd.append(  QPointF(lineEndX + CORNER_GRAB_BUFFER, lineEndY + CORNER_GRAB_BUFFER));
    pointsEnd.append(  QPointF(lineEndX - CORNER_GRAB_BUFFER, lineEndY + CORNER_GRAB_BUFFER));

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

    //TODO do we need this?
    //this->update();

}

/**
 * @brief LineSegmentGraphic::paint
 * @param painter
 *
 * paints the line
 */
//TODO move terminal and arrow head paint to elbowgrabber
void LineSegmentGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  //  enclosePathInElbows();
    //_pen.setWidth(20);
//#define SHOW_HOVER_BOXES
    painter->setPen(_pen);
    painter->setRenderHint(QPainter::Antialiasing);

#ifndef SHOW_HOVER_BOXES
    painter->drawLine(_elbows[0]->pos(), _elbows[1]->pos());
#endif


#ifdef SHOW_HOVER_BOXES

    painter->drawPolygon(_selectRegion,Qt::WindingFill);
#endif

}

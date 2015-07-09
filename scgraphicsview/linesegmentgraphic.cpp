#include "linesegmentgraphic.h"


LineSegmentGraphic::LineSegmentGraphic()
{

}

LineSegmentGraphic::LineSegmentGraphic(ElbowGrabber *startPoint, ElbowGrabber *endPoint, TransitionGraphic *parentGraphic, KeyController *keys):
    _keyController(keys),
    _isTerminal(false)
{
    this->setParentItem(parentGraphic);
    _elbows[0] = startPoint;
    _elbows[1] = endPoint;

    _pen.setWidth(TRANSITION_DEFAULT_WIDTH);
    _pen.setColor(TRANSITION_DEFAULT_COLOR);
    _pen.setStyle(TRANSITION_DEFAULT_LINE_STYLE);
/*
    QPointF p1  (_elbows[0]->x() - 10, _elbows[0]->y() - 10);
    QPointF p2  (_elbows[0]->x() + 10, _elbows[0]->y() - 10);
    QPointF p3  (_elbows[0]->x() + 10, _elbows[0]->y() + 10);
    QPointF p4  (_elbows[0]->x() - 10, _elbows[0]->y() + 10);


    QPointF p5  (_elbows[1]->x() - 10, _elbows[1]->y() - 10);
    QPointF p6  (_elbows[1]->x() + 10, _elbows[1]->y() - 10);
    QPointF p7  (_elbows[1]->x() + 10, _elbows[1]->y() + 10);
    QPointF p8  (_elbows[1]->x() - 10, _elbows[1]->y() + 10);


    _selectRegion << p1 << p2  << p5 << p6 << p7 << p8 << p3 << p4 << p1;

    this->setPolygon(_selectRegion);*/

    //enclosePathInItemCoordiates(_elbows[0]->x(), _elbows[0]->y(), _elbows[1]->x(), _elbows[1]->y());
    enclosePathInElbows();
}

void LineSegmentGraphic::setTerminal(bool isTerm)
{
    _isTerminal=isTerm;
}

LineSegmentGraphic::~LineSegmentGraphic()
{

}

void LineSegmentGraphic::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    _pen.setWidth(TRANSITION_HOVER_WIDTH);
    _pen.setColor(TRANSITION_HOVER_COLOR);
    _pen.setStyle(TRANSITION_HOVER_LINE_STYLE);

}

void LineSegmentGraphic::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    _pen.setWidth(TRANSITION_DEFAULT_WIDTH);
    _pen.setColor(TRANSITION_DEFAULT_COLOR);
    _pen.setStyle(TRANSITION_DEFAULT_LINE_STYLE);
}

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

void LineSegmentGraphic::enclosePathInCoordindates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  )
{
    QPointF p1(  QPointF(lineStartX  - CORNER_GRAB_BUFFER, lineStartY - CORNER_GRAB_BUFFER));
    QPointF p2(  QPointF(lineStartX + CORNER_GRAB_BUFFER, lineStartY - CORNER_GRAB_BUFFER));
    QPointF p3(  QPointF(lineStartX + CORNER_GRAB_BUFFER, lineStartY + CORNER_GRAB_BUFFER));
    QPointF p4(  QPointF(lineStartX - CORNER_GRAB_BUFFER, lineStartY + CORNER_GRAB_BUFFER));

    QPointF p5(  QPointF(lineEndX - CORNER_GRAB_BUFFER, lineEndY - CORNER_GRAB_BUFFER));
    QPointF p6(  QPointF(lineEndX + CORNER_GRAB_BUFFER, lineEndY - CORNER_GRAB_BUFFER));
    QPointF p7(  QPointF(lineEndX + CORNER_GRAB_BUFFER, lineEndY + CORNER_GRAB_BUFFER));
    QPointF p8(  QPointF(lineEndX - CORNER_GRAB_BUFFER, lineEndY + CORNER_GRAB_BUFFER));

    _selectRegion.clear();
    //_selectRegion << p1 << p2 << p5 << p6 << p7 << p8 << p3 << p4 << p1;
    //_selectRegion << p1 << p5 << p6 << p2 << p3 << p7 << p8 << p4 << p1;
    _selectRegion << p1 << p5 << p6 << p2 << p3 << p7 << p8 << p4 << p1 << p2 << p4 << p3 << p5 << p8 << p6 << p7 << p1;
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

    this->update();

}

/**
 * @brief LineSegmentGraphic::paint
 * @param painter
 *
 * paints the line and an arrow head if this is the terminating segment
 */
//TODO move terminal and arrow head paint to elbowgrabber
void LineSegmentGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //#define SHOW_HOVER_BOXES
    painter->setPen(_pen);

#ifndef SHOW_HOVER_BOXES
    painter->drawLine(_elbows[0]->pos(), _elbows[1]->pos());
#endif
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


#ifdef SHOW_HOVER_BOXES

    painter->drawPolygon(_selectRegion,Qt::WindingFill);
#endif

}

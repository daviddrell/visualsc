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

    _pen.setWidth(2);
    _pen.setColor(Qt::red);
    _pen.setStyle(Qt::SolidLine);
}

void LineSegmentGraphic::setTerminal(bool isTerm)
{
    _isTerminal=isTerm;
}

LineSegmentGraphic::~LineSegmentGraphic()
{

}

void LineSegmentGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
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

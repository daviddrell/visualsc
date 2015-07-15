#include "arrowheadgraphic.h"

#include <QPainter>
#include <QPen>

ArrowHeadGraphic::ArrowHeadGraphic(QGraphicsItem *parent): QGraphicsItem(parent),
    _width(10),
    _height(10),
    _color(Qt::black)
{
    setTransformOriginPoint(_width/2,_height/2);
}

void ArrowHeadGraphic::setWidth(int w)
{
    _width = w;
    setTransformOriginPoint(_width/2,_height/2);
}

void ArrowHeadGraphic::setHeight(int h)
{
    _height = h;
    setTransformOriginPoint(_width/2,_height/2);
}

void ArrowHeadGraphic::setColor(QColor  color)
{
    _color = color;
}

QRectF ArrowHeadGraphic::boundingRect() const
{
    QRectF rect (0,0,_width,_height);
    return rect;
}


void ArrowHeadGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(_color);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawLine(QPointF(_width/2,_height/2), QPointF(_width, _height));
    painter->drawLine(QPointF(0,_height), QPointF(_width, _height));
    painter->drawLine(QPointF(_width,0), QPointF(_width, _height));

}

#ifndef ARROWHEADGRAPHIC_H
#define ARROWHEADGRAPHIC_H

#include <QGraphicsItem>
#include <QColor>

class ArrowHeadGraphic : public QGraphicsItem
{
public:
    ArrowHeadGraphic(QGraphicsItem *parent);

    void setWidth(int w);
    void setHeight(int h);

    void setColor(QColor color);
    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView

private:

    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event


    int _width;
    int _height;
    QColor _color;

};

#endif // ARROWHEADGRAPHIC_H

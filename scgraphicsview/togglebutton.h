#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
//#include "stateboxgraphic.h"


class StateBoxGraphic;
class SizeAttribute;

class ToggleButton : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    ToggleButton(StateBoxGraphic* parentGraphic, int corner);
    ~ToggleButton();

    virtual QRectF boundingRect() const;

    void reposition();

    StateBoxGraphic* parentAsStateBoxGraphic();

    bool isHovered();

    void toggle();
    bool isOn();

signals:
    void toggled();

public slots:
    void handleStateSizeChanged(SizeAttribute*);


protected:
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
     void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
     void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
     void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
     void mousePressEvent ( QGraphicsSceneMouseEvent * event );
     void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
     void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);


private:
    QPixmap* _defaultGraphic;
    QPixmap* _toggleGraphic;

    qreal _width;
    qreal _height;

    bool _hovered;
    QPen _pen;

    bool _on;
    int _corner;
};

#endif // TOGGLEBUTTON_H

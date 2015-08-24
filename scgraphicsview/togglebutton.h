#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QObject>
#include <QGraphicsItem>
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

signals:
    void toggled();

public slots:
    void handleStateSizeChanged(SizeAttribute*);


protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
//    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
//    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);


private:
    QPixmap* _defaultGraphic;
    QPixmap* _toggleGraphic;

    qreal _width;
    qreal _height;

    bool _hovered;

    bool _on;
    int _corner;
};

#endif // TOGGLEBUTTON_H

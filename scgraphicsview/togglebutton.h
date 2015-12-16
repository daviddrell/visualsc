#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include "iattribute.h"
#include "keycontroller.h"
#include "scdatamodel.h"
#include "scstate.h"
class StateBoxGraphic;

class ToggleButton : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    ToggleButton(StateBoxGraphic* parentGraphic, int corner, KeyController* kc, SCDataModel*dm, SCState* sm);
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
    void handleStateSizeChanged(IAttribute*);
    void setOn(bool on);


protected:
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
     void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
     void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
     void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
     void mousePressEvent ( QGraphicsSceneMouseEvent * event );
     void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
     void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);


private slots:
     void handleKey(int);

private:
    QPixmap* _defaultGraphic;
    QPixmap* _toggleGraphic;
    qreal _width;
    qreal _height;
    bool _hovered;
    QPen _pen;
    bool _on;
    int _corner;
    KeyController* _keyController;
    SCDataModel*   _dm;
    SCState*       _state;
};

#endif // TOGGLEBUTTON_H

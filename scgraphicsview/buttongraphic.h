#ifndef BUTTONGRAPHIC_H
#define BUTTONGRAPHIC_H

#include <QGraphicsObject>
class QPixmap;

/**
   \class ButtonGraphic

*/

class ButtonGraphic : public QGraphicsObject
{
    Q_OBJECT

public:
    ButtonGraphic();
    ~ButtonGraphic();

    enum MouseState { kNormal, kHovered, kPressed};

    void setNormalGraphic(QString source);
    void setHoveredGraphic(QString source);
    void setDepressedGraphic(QString source);

    virtual QRectF boundingRect() const;/// virtual from QGraphicsObject base

signals:
    void clicked();


protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);/// virtual from QGraphicsObject base
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )  ;
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );


private:
    QPixmap *_normalGraphic;
    QPixmap *_hoveredGraphic;
    QPixmap *_depressedGraphic;
    MouseState _mouseState;

};

#endif // BUTTONGRAPHIC_H

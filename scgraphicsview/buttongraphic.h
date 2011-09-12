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

    void setNormalGraphic(QString source);
    void setHoveredGraphic(QString source);
    void setDepressedGraphic(QString source);

    virtual QRectF boundingRect() const;/// virtual from QGraphicsObject base

signals:
    void hovered();
    void pressed();
    void released();
    void clicked();


protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);/// virtual from QGraphicsObject base

private:
    QPixmap *_normalGraphic;
    QPixmap *_hoveredGraphic;
    QPixmap *_depressedGraphic;

};

#endif // BUTTONGRAPHIC_H

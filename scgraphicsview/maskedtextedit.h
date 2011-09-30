#ifndef MASKEDTEXTEDIT_H
#define MASKEDTEXTEDIT_H

#include <QGraphicsTextItem>

/**
    \class MaskedTextEdit
    \brief this class provides a QGraphicsTextItem but with a controlable display area.
    Set the bounding rect to control the visible text area. The coordinates in the set
    should be relative to this object.
  */

class MaskedTextEdit : public QGraphicsTextItem
{
public:
    MaskedTextEdit(QGraphicsItem *parent = NULL, QRectF rect = QRectF());
    void setBoundingRect(QRectF rect);

    virtual QRectF boundingRect();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF _rect;
};

#endif // MASKEDTEXTEDIT_H

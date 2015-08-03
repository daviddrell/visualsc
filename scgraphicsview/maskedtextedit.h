#ifndef MASKEDTEXTEDIT_H
#define MASKEDTEXTEDIT_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include "selectableboxgraphic.h"
/**
    \class MaskedTextEdit
    \brief this class provides a QGraphicsTextItem but with a controlable display area.
    Set the bounding rect to control the visible text area. The coordinates in the set
    should be relative to this object.
  */



class MaskedTextEdit : public QGraphicsTextItem
{
    //Q_OBJECT
public:
    MaskedTextEdit(QGraphicsItem *parent = NULL, QRectF rect = QRectF());
    void setBoundingRect(QRectF rect);

    virtual QRectF boundingRect();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);



    SelectableBoxGraphic* parentAsSelectableBoxGraphic();
private:
    QRectF _rect;
};

#endif // MASKEDTEXTEDIT_H

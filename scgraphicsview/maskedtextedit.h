#ifndef MASKEDTEXTEDIT_H
#define MASKEDTEXTEDIT_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
//#include "selectableboxgraphic.h"

class SelectableBoxGraphic;
class SelectableTextBlock;
/**
    \class MaskedTextEdit
    \brief this class provides a QGraphicsTextItem but with a controlable display area.
    Set the bounding rect to control the visible text area. The coordinates in the set
    should be relative to this object.
  */



class MaskedTextEdit : public QGraphicsTextItem
{
    Q_OBJECT
public:

    MaskedTextEdit(QGraphicsItem *parent = NULL, QRectF rect = QRectF());
    void setBoundingRect(QRectF rect);

    virtual QRectF boundingRect();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    SelectableBoxGraphic* parentAsSelectableBoxGraphic();

    SelectableTextBlock* parentAsSelectableTextBlock();

    void resizeRectToTextBlock();
    void resizeRectToTextSize();

    void setWidth(qreal w);
    void setHeight(qreal h);

signals:
    void focusOut();


private:

    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    QRectF _rect;
    QRectF _document;
};

#endif // MASKEDTEXTEDIT_H

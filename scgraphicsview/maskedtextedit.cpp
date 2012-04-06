#include "maskedtextedit.h"
#include <QPainter>

MaskedTextEdit::MaskedTextEdit(QGraphicsItem *parent ,QRectF rect ) :
        QGraphicsTextItem(parent),
        _rect(rect)
{

}

void MaskedTextEdit::setBoundingRect(QRectF rect)
{
    _rect = rect;
}

QRectF MaskedTextEdit::boundingRect()
{
    return _rect;
}

void MaskedTextEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipping(true);
    painter->setClipRect(_rect);
    QGraphicsTextItem::paint(painter, option, widget);
}


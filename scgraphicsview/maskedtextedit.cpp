#include "maskedtextedit.h"
#include <QPainter>
#include <QDebug>
#include "selectableboxgraphic.h"
#include "selectabletextblock.h"


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
    painter->setRenderHints(QPainter::TextAntialiasing);
    painter->setClipping(true);
    painter->setClipRect(_rect);
    QGraphicsTextItem::paint(painter, option, widget);
}

SelectableBoxGraphic* MaskedTextEdit::parentAsSelectableBoxGraphic()
{
    return dynamic_cast<SelectableBoxGraphic*>(this->parentItem());
}

SelectableTextBlock* MaskedTextEdit::parentAsSelectableTextBlock()
{
    return dynamic_cast<SelectableTextBlock*> (this->parentItem());
}


/**
 * @brief MaskedTextEdit::setWidth
 * @param w
 *
 * univseral set width for the QGraphicsTextItem
 */
void MaskedTextEdit::setWidth(qreal w)
{
    _rect.setWidth(w);
    this->setTextWidth(w);
}

void MaskedTextEdit::setHeight(qreal h)
{
    _rect.setHeight(h);
}

void MaskedTextEdit::focusOutEvent(QFocusEvent *event)
{
    qDebug() << "leaving focus!";
    QGraphicsTextItem::focusOutEvent(event);
    emit focusOut();
}

void MaskedTextEdit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    resizeRectToTextBlock();
    QGraphicsTextItem::mousePressEvent(event);

}


void MaskedTextEdit::resizeRectToTextBlock()
{
    SelectableTextBlock* stb = this->parentAsSelectableTextBlock();

    // rect should always be at 0,0
    // to set the position, use set Pos
    // the w and h of the rect will be a smaller rect of the text block, specifically shorter by cornergrab buffer + text item buffer
    QRectF editRect(0,
                    0,
                    stb->getUsableWidth(),
                    stb->getUsableHeight());

    this->setWidth(editRect.width());
    _rect.setHeight(editRect.height());

    // set the position to be at the top left point plus the total offsets
    QPointF offset(stb->getTotalTextItemBufferX(), stb->getTotalTextItemBufferY());
    this->setPos(editRect.topLeft()+offset);
}

void MaskedTextEdit::resizeRectToTextSize()
{
    this->adjustSize();
    qreal width = this->textWidth();
    //qreal height = this->document().
   // _rect.setWidth(document()->size().width());
}

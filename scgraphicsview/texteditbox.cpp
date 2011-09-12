#include "texteditbox.h"
#include <QDebug>
#include <QImage>

TextEditBox::TextEditBox(TextBlock * textBlock) :
                QGraphicsTextItem(this),
                _textBlock(textBlock),
                _button(new ButtonGraphic())
{
    _button->setNormalGraphic(":/SCGraphicsView/savefileicon_32_32.png");
    _button->setHoveredGraphic(":/SCGraphicsView/savefileicon_32_32.png");
    _button->setDepressedGraphic(":/SCGraphicsView/savefileicon_32_32.png");
    _button->setParentItem(this);

    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFlag(QGraphicsItem::ItemIsMovable, false );

    setPos( this->pos().x()+50, this->pos().y()+50  );

    setPlainText(_textBlock->getText());

    this->setTextWidth(200);

    //connect (& _textItem, SIGNAL(scaleChanged()), this, SLOT(handleScaleChanged()));
}

void TextEditBox::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    QImage background(":/SCGraphicsView/indexcard.png");
    QRectF rect = boundingRect();
    painter->drawImage(rect, background);

    QBrush brush(QColor(253,255,147,200), Qt::SolidPattern);
    painter->fillRect(rect,brush);
    QGraphicsTextItem::paint(painter,  option, widget);
}

void TextEditBox::keyPressEvent ( QKeyEvent *  event)
{
    QGraphicsTextItem::keyPressEvent(event);

    qDebug()<<"keyPressEvent "  + QString::number(boundingRect().width()) + ", " + QString::number(boundingRect().height());


}

void TextEditBox::handleScaleChanged()
{
    qDebug()<<"handleScaleChanged " ;
}

void TextEditBox::graphicHasChanged()
{

}

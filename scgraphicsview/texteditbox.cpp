#include "texteditbox.h"
#include <QDebug>
#include <QImage>

TextEditBox::TextEditBox(TextBlock * textBlock) :
        QGraphicsObject(NULL),
        _textBlock(textBlock),
        _button(new ButtonGraphic()),
        _textItem(new QGraphicsTextItem(this))
{
    this->setAcceptHoverEvents(true);
    _button->setNormalGraphic(":/SCGraphicsView/savefileicon_16_16.png");
    _button->setHoveredGraphic(":/SCGraphicsView/savefileicon_hovered_16_16.png");
    _button->setDepressedGraphic(":/SCGraphicsView/savefileicon_depressed_16_16.png");

    _button->setParentItem(this);

    _textItem->setPos(0,16);
    _textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    _textItem->setFlag(QGraphicsItem::ItemIsMovable, false );


    _textItem->setPlainText(_textBlock->getText());

}

TextEditBox::~TextEditBox()
{
    delete _button;
    delete _textItem;
}

QRectF TextEditBox::boundingRect() const
{
    QRectF r = _textItem->boundingRect();

    QRectF myRect (0,0,r.width(),16+r.height());

    return myRect;
}

void TextEditBox::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    QImage background(":/SCGraphicsView/indexcard.png");
    QRectF rect = boundingRect();
    painter->drawImage(rect, background);

    QBrush brush(QColor(253,255,147,200), Qt::SolidPattern);
    painter->fillRect(rect,brush);

}

void TextEditBox::keyPressEvent ( QKeyEvent *  )
{
    //QGraphicsTextItem::keyPressEvent(event);

    qDebug()<<"TextEditBox::keyPressEvent "  + QString::number(boundingRect().width()) + ", " + QString::number(boundingRect().height());


}

void TextEditBox::handleScaleChanged()
{
    qDebug()<<"handleScaleChanged " ;
}

void TextEditBox::graphicHasChanged()
{

}

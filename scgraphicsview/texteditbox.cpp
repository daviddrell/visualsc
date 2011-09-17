#include "texteditbox.h"
#include <QDebug>
#include <QImage>

TextEditBox::TextEditBox(TextBlock * textBlock) :
        QGraphicsObject(NULL),
        _textBlock(textBlock),
        _button(new ButtonGraphic()),
        _textItem(new QGraphicsTextItem(this)),
        _minWidth(200),
        _minHeight(200)
{
    this->setAcceptHoverEvents(true);
    _button->setNormalGraphic(":/SCGraphicsView/savefileicon_16_16.png");
    _button->setHoveredGraphic(":/SCGraphicsView/savefileicon_hovered_16_16.png");
    _button->setDepressedGraphic(":/SCGraphicsView/savefileicon_depressed_16_16.png");

    _button->setParentItem(this);
    connect(_button,SIGNAL(clicked()), this, SLOT(handleSaveButtonClicked()));

    _textItem->setPos(0,16);
    _textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    _textItem->setFlag(QGraphicsItem::ItemIsMovable, false );

    QString text = _textBlock->getText();
    if (text.isEmpty())
        text = "click here to edit text";
    _textItem->setPlainText(text);

    this->setFlag(QGraphicsItem::ItemIsMovable, true );
    this->setFlag( QGraphicsItem::ItemIsSelectable, true );
    this->setPanelModality( QGraphicsItem::PanelModal);
}


TextEditBox::~TextEditBox()
{
    delete _button;
    delete _textItem;
}

void TextEditBox::handleSaveButtonClicked()
{
    emit saveButtonClicked(_textItem->toPlainText());
    this->deleteLater();
}

QRectF TextEditBox::boundingRect() const
{
    QRectF r = _textItem->boundingRect();

    if ( r.width() < _minWidth)
    {
        r.setWidth(_minWidth);
    }
    if ( r.height() < _minHeight)
    {
        r.setHeight(_minHeight);
    }

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

    qDebug()<< "TextEditBox::keyPressEvent "  + QString::number(boundingRect().width()) + ", " + QString::number(boundingRect().height());
}


void TextEditBox::graphicHasChanged()
{

}

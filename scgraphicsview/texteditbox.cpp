#include "texteditbox.h"
#include <QDebug>
#include <QImage>

TextEditBox::TextEditBox(SCTextBlock * textBlock) :
        QGraphicsObject(NULL),
        _textBlock(textBlock),
        _SaveButton(new ButtonGraphic(0,0)),
        _CancelButton(new ButtonGraphic(20,0)),
        _textItem(new QGraphicsTextItem(this)),
        _minWidth(200),
        _minHeight(200)
{
    this->setAcceptHoverEvents(true);
    _SaveButton->setNormalGraphic(":/SCGraphicsView/savefileicon_16_16.png");
    _SaveButton->setHoveredGraphic(":/SCGraphicsView/savefileicon_hovered_16_16.png");
    _SaveButton->setDepressedGraphic(":/SCGraphicsView/savefileicon_depressed_16_16.png");

    _SaveButton->setParentItem(this);
    connect(_SaveButton,SIGNAL(clicked()), this, SLOT(handleSaveButtonClicked()));

    _CancelButton->setNormalGraphic(":/SCGraphicsView/red_x_16_16.png");
    _CancelButton->setHoveredGraphic(":/SCGraphicsView/red_x_hovered_16_16.png");
    _CancelButton->setDepressedGraphic(":/SCGraphicsView/red_x_depressed_16_16.png");

    _CancelButton->setParentItem(this);
    connect(_CancelButton,SIGNAL(clicked()), this, SLOT(handleCancelButtonClicked()));

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
    delete _SaveButton;
    delete _textItem;
}

void TextEditBox::handleCancelButtonClicked()
{
    emit cancelButtonClicked();
    this->deleteLater();
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

void TextEditBox::paint ( QPainter * painter, const QStyleOptionGraphicsItem * , QWidget *  )
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

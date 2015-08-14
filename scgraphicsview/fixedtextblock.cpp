#include "fixedtextblock.h"
#include <QDebug>
#include <QTextDocument>
#include "stateboxgraphic.h"

/**
 * @brief FixedTextBlock::FixedTextBlock
 * @param parent
 * @param topFraction
 * @param bottomFraction
 *
 * creates a fixed Text block inside of the parent graphics object
 * will keep the text block fixed at the levels specified by topFraction and bottomFraction
 *
 * the fixed box will always match the width of the usable area of its parent
 *
 * topFraction is the percentage at which the box will begin
 * bottom fraction is the percetange at which the box ends
 *
 *
 * so if topFraction is 0.4 and bottom fraction is 0.6
 *
 * then 40% from the top of the box, the fixed box will start and end at 60% of the box's height (20% total height)
 *
 *
 * ALTNERATE MODE: fixed height
 * top fraction works the same, but height will be fixed at bottom's value
 *
 */
FixedTextBlock::FixedTextBlock(QGraphicsObject* parent, qreal topFraction, qreal bottom, bool fixedHeight):
    QGraphicsObject(parent),
    _pen(),
    _width(70),
    _height(40),
    _topFraction(topFraction),
    _bottom(bottom),
    _fixedHeight(fixedHeight),
    _textItem(this, QRect(0,0, TEXTBLOCK_DEFAULT_WIDTH-2*(TEXT_ITEM_X_BUFFER), TEXTBLOCK_DEFAULT_HEIGHT-2*(TEXT_ITEM_Y_BUFFER)))
{
    switchPen(PenStyle::Default);


    _textItem.setPlainText(this->parentAsStateBoxGraphic()->getStateName());
    connect(&_textItem, SIGNAL(focusOut()), this, SLOT(handleTextItemEdited()));
    reposition();
    resize();
    recenterText();
}

FixedTextBlock::~FixedTextBlock()
{

}

void FixedTextBlock::setText(QString text)
{
    _textItem.setPlainText(text);
    this->recenterText();
}

QPointF FixedTextBlock::getSize()
{
    return QPointF(_width,_height);
}

/**
 * @brief FixedTextBlock::handleTextItemEdited
 *
 * SLOT
 * called when the text item leaves edit mode
 *
 * signal to the statebox graphic that the name has changed
 * the state box will send this signal out to the data model
 *
 * additionally, recenter the text item
 */
void FixedTextBlock::handleTextItemEdited()
{
    // update data model
    emit nameChanged(_textItem.toPlainText());

    // recenter the text
    this->recenterText();
}

void FixedTextBlock::handleStateBoxResized(QRectF, QRectF, int)
{
    this->resize();
    this->recenterText();

}

StateBoxGraphic* FixedTextBlock::parentAsStateBoxGraphic()
{
    return dynamic_cast<StateBoxGraphic*>(this->parentItem());
}

void FixedTextBlock::recenterText()
{


    // sets the width and height of the textItem based on the plainText

    _textItem.adjustSize();


    // update the rect for the text item
  //  qreal textWidth = _textItem.document()->size().width();
//    qreal textWidth = _textItem.document()->pageSize().width();
    qreal textWidth = _width;
    _textItem.setWidth(textWidth);


   // qreal textHeight = _textItem.document()->size().height();
//    qreal textHeight = _textItem.document()->pageSize().height();
    qreal textHeight = _height;
    _textItem.setHeight(textHeight);

    // check if the text width is less than the width availble for this text item
    qreal width =  _width;

    // if it will not fit, then resize the text item to the dimensions of the allowable area
    if(width < textWidth)
    {
        qDebug() << "width < text WIdth";
        _textItem.resizeRectToFixedTextBlock();
    }
    else    // center the text in the text block
    {
        // set the position to be in the middle of the text block
        qreal newX = this->getSize().x()/2.0 - _textItem.document()->size().width()/2.0;
        qreal newY = this->getSize().y()/2.0 - _textItem.document()->size().height()/2.0;
        _textItem.setPos(this->clampMin(newX,0), this->clampMin(newY,0));
    }

    //_textItem.document()->setPageSize(QSizeF(_width,_height));




}

qreal FixedTextBlock::clampMin(qreal value, qreal min)
{
    if(value < min)
        return min;
    return value;
}

/**
 * @brief FixedTextBlock::reposition
 *
 * moves the fixed text block to hug the left wall of the usable area of its parent
 * and down to the percentage level of _topFraction
 */
void FixedTextBlock::reposition()
{
    QRectF pRect = this->parentAsSelectableBoxGraphic()->getUsableArea();
    qreal x = pRect.x();
    qreal y = pRect.height() * _topFraction + pRect.y();
    this->setPos(x,y);
}



/**
 * @brief FixedTextBlock::resize
 *
 * resizes the width so the right wall hugs the usable area of its parent
 * adjusts the height to match the level given in _bottomFraction
 *
 */
void FixedTextBlock::resize()
{
    QRectF pRect = this->parentAsSelectableBoxGraphic()->getUsableArea();
    _width = pRect.width();
    if(_fixedHeight)
    {
        _height = _bottom;
    }
    else
    {
        _height = pRect.height() * (_bottom-_topFraction);
    }
}

SelectableBoxGraphic* FixedTextBlock::parentAsSelectableBoxGraphic()
{
    return dynamic_cast<SelectableBoxGraphic*> (this->parentItem());
}

void FixedTextBlock::switchPen(int style)
{
    switch(style)
    {
    case PenStyle::Default:
        _pen.setWidthF(2);
        _pen.setStyle(Qt::SolidLine);
        _pen.setColor(Qt::black);
        break;

    }
}

void FixedTextBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    //painter->drawRoundRect(this->boundingRect(),0,0);

    painter->setPen(_pen);
    QRectF r = this->boundingRect();
    painter->drawLine(r.bottomLeft(),r.bottomRight());
}

QRectF FixedTextBlock::boundingRect() const
{
    return QRectF(0,0,_width, _height);
}

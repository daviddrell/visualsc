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
 *
 * If attached to top is true, then the fixed text block will be fixed on the north wall of the state
 *  top determines how far from the top the box will be
 *  bottom = height
 *
 * If attached to top is false, then the fixed text block will be fixed on the south wall of the state
 *  top = height
 *  bottom determines how far from the bottom the box will be
 *
 *
 */

#define DEFAULT_WIDTH       50
#define DEFAULT_HEIGHT      50
#define TEXT_BUFFER         5

// the text position will be pushed down to its position in edit mode
#define TEXT_PUSH_DOWN      1

FixedTextBlock::FixedTextBlock(QGraphicsObject* parent, qreal top, qreal bottom, bool attachedToTop):
    QGraphicsObject(parent),
    _pen(),
    _width(DEFAULT_WIDTH),
    _height(DEFAULT_HEIGHT),
    _top(top),
    _bottom(bottom),
    _attachedToTop(attachedToTop),
    _textItem(this, QRect(0,0, DEFAULT_WIDTH-2*(TEXT_BUFFER), DEFAULT_HEIGHT-2*(TEXT_BUFFER)))      // doesn't matter what this is set to here because attributes get loaded anyways
{
    switchPen(PenStyle::Default);
    setFont(Font::Normal);

    connect(&_textItem, SIGNAL(focusOut()), this, SLOT(handleTextItemEdited()));
    reposition();
    resize();
    recenterText();
}

FixedTextBlock::~FixedTextBlock()
{

}

void FixedTextBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->parentAsStateBoxGraphic()->mouseMoveEvent(event);
}

void FixedTextBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->parentAsStateBoxGraphic()->mouseReleaseEvent(event);
}

void FixedTextBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->parentAsStateBoxGraphic()->mousePressEvent(event);
}

void FixedTextBlock::setFont(int font)
{
    switch (font) {
    case Font::Normal:
        _textItem.setFont(QFont("Arial",10,QFont::Bold));
        break;
    case Font::Small:
        _textItem.setFont(QFont("Arial",8));
        break;
    }
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

void FixedTextBlock::handleStateSizeChanged(SizeAttribute *)
{
//    qDebug() << "FixedTextBlock::handleStateSizeChanged";
    this->resize();
    this->reposition();
    this->recenterText();
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
    emit changed(_textItem.toPlainText());

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
        qDebug() << "width < text Width resizing rect to to fixed text block size";
        _textItem.resizeRectToFixedTextBlock();

        // adjust y position of text to match where it is in the edit mode
        _textItem.setPos(_textItem.pos().x(), _textItem.pos().y()-TEXT_PUSH_DOWN);
    }
    else    // center the text in the text block
    {
        // set the position to be in the middle of the text block
        qreal newX = this->getSize().x()/2.0 - _textItem.document()->size().width()/2.0;
        qreal newY = this->getSize().y()/2.0 - _textItem.document()->size().height()/2.0;
        _textItem.setPos(this->clampMin(newX,0), this->clampMin(newY,0)-TEXT_PUSH_DOWN);
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
 * and down to the percentage level of _top
 */
void FixedTextBlock::reposition()
{
    if(_attachedToTop)
    {
        QRectF pRect = this->parentAsSelectableBoxGraphic()->getUsableArea();
        qreal x = pRect.x();
        qreal y = _top + pRect.y();
        this->setPos(x,y);
    }
    else
    {
        QRectF pRect = this->parentAsSelectableBoxGraphic()->getUsableArea();
        qreal x = pRect.x();
        qreal y = pRect.y()+pRect.height() - _bottom - (_top) + 2;
        this->setPos(x,y);
    }
}


/**
 * @brief FixedTextBlock::resize
 *
 * resizes the width so the right wall hugs the usable area of its parent
 * adjusts the height to match the level given in _bottom
 *
 */
void FixedTextBlock::resize()
{
    QRectF pRect = this->parentAsSelectableBoxGraphic()->getUsableArea();
    _width = pRect.width();

    if(_attachedToTop)
        _height = _bottom;
    else
        _height = _top;
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

    case PenStyle::Experimental:
        _pen.setWidthF(0.25);
        _pen.setStyle(Qt::SolidLine);
        _pen.setColor(Qt::blue);
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

    if(this->_textItem.document()->toPlainText().isEmpty())
        return;

    if(_attachedToTop)
    {
        painter->drawLine(r.bottomLeft(),r.bottomRight());
    }
    else
    {
        painter->drawLine(r.topLeft()+QPointF(0,-2),r.topRight()+QPointF(0,-2));
    }
}

QRectF FixedTextBlock::boundingRect() const
{
    return QRectF(0,0,_width, _height);
}

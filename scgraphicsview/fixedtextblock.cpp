#include "fixedtextblock.h"
#include <QDebug>
#include <QTextDocument>
#include "stateboxgraphic.h"
//#include "selectableboxgraphic.h"


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
 * right will shrink the width from the right wall. visually it will appear the same as the original width, but the hover box will be smaller
 *
 */

#define DEFAULT_WIDTH       50
#define DEFAULT_HEIGHT      50
#define TEXT_BUFFER         1

// the text position will be pushed down to its position in edit mode
#define TEXT_PUSH_DOWN      1

FixedTextBlock::FixedTextBlock(QGraphicsObject* parent, qreal top, qreal bottom, qreal right, bool attachedToTop):
    QGraphicsObject(parent),
    _pen(),
    _width(DEFAULT_WIDTH),
    _height(DEFAULT_HEIGHT),
    _top(top),
    _bottom(bottom),
    _attachedToTop(attachedToTop),
    _right(right),
    _hovered(false),
    _textItem(this, QRect(0,0, DEFAULT_WIDTH-2*(TEXT_BUFFER), DEFAULT_HEIGHT-2*(TEXT_BUFFER)))      // doesn't matter what this is set to here because state size attribute get loaded right after this is created and will change the size of the text item
{
    switchPen(PenStyle::Default);
    setFont(Font::Normal);

    connect(&_textItem, SIGNAL(focusOut()), this, SLOT(handleTextItemEdited()));
    reposition();
    resize();
    cropDocument();


    // control the hovered variable using the text item's hover events
    connect(&_textItem,SIGNAL(hovered()), this, SLOT(handleHoverEnter()));
    connect(&_textItem, SIGNAL(unhovered()), this, SLOT(handleHoverLeave()));  
}

FixedTextBlock::~FixedTextBlock()
{

}

void FixedTextBlock::handleHoverEnter()
{
    _hovered = true;
}
void FixedTextBlock::handleHoverLeave()
{
    _hovered = false;
}

bool FixedTextBlock::isHovered()
{
    return _hovered;
}

void FixedTextBlock::handleFontSizeChanged(FontSizeAttribute * fa)
{
    qDebug() << "stb::handleFOntSizeChanged ";
    QFont f = _textItem.font();
    f.setPointSize(fa->asInt());
    _textItem.setFont(f);
//    this->adjustHeight();
    this->reposition();



}

void FixedTextBlock::handleFontFamilyChanged(FontFamilyAttribute *ga)
{
    qDebug() << "stb::handleFontFamilyChanged()";
    QFont f = _textItem.font();
    f.setFamily(ga->asString());
    _textItem.setFont(f);
//    this->adjustHeight();
    this->reposition();
}

/**
 * @brief FixedTextBlock::mouseMoveEvent
 * @param event
 *
 * pass the event to a higher level for all mouse events
 *
 */
void FixedTextBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    event->setAccepted(false);
    this->parentAsStateBoxGraphic()->mouseMoveEvent(event);
}

void FixedTextBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    event->setAccepted(false);
    this->parentAsStateBoxGraphic()->mouseReleaseEvent(event);
}

void FixedTextBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    event->setAccepted(false);
    qDebug() << "fte mouse press event";
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
    this->cropDocument();
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
    this->cropDocument();
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
    this->cropDocument();
}

void FixedTextBlock::handleStateBoxResized(QRectF, QRectF, int)
{
    this->resize();
    this->cropDocument();
}

StateBoxGraphic* FixedTextBlock::parentAsStateBoxGraphic()
{
    return dynamic_cast<StateBoxGraphic*>(this->parentItem());
}

/**
 * @brief FixedTextBlock::recenterText
 *
 * updates the dimensions of the masked text edit object based on the text graphic dimensions and useable area dimensions
 *
 *
 */
void FixedTextBlock::cropDocument()
{
    qDebug () << "FTB::cropDocument()";

    _textItem.setWidth(_width);
    _textItem.setHeight(_height);















//#ifdef USE_OLD_RESIZE
//    // check if the text width is less than the width availble for this text item
//    qreal width =  _width;

//    // if it will not fit, then resize the text item to the dimensions of the allowable area
//    if(width < textWidth)
//    {
//        qDebug() << "width < text Width... resizing rect to to fixed text block size";
//        _textItem.resizeRectToFixedTextBlock();

//        // adjust y position of text to match where it is in the edit mode
//        _textItem.setPos(_textItem.pos().x(), _textItem.pos().y()-TEXT_PUSH_DOWN);
//    }
//    else    // center the text in the text block
//    {
//        qDebug() << "width <= textwidth!... centering textitem";
//        // set the position to be in the middle of the text block
//        qreal newX = this->getSize().x()/2.0 - _textItem.document()->size().width()/2.0;
//        qreal newY = this->getSize().y()/2.0 - _textItem.document()->size().height()/2.0;

//        qreal xPos = this->clampMin(newX,0);
//        qreal yPos = this->clampMin(newY,0)-TEXT_PUSH_DOWN;

//        qDebug ()<< newX<<"\t"<<newY<<"\tsetting textitem pos: " <<xPos <<"\t"<< yPos;
//        _textItem.setPos(xPos, yPos);
//    }
//#endif
//#ifndef USE_OLD_RESIZE
////    _textItem.resizeRectToFixedTextBlock();
//#endif

//    //_textItem.document()->setPageSize(QSizeF(_width,_height));
}

/**
 * @brief FixedTextBlock::adjustHeight
 *
 * When the text size is changed, automatically adjust the maximum rectangle height
 *
 *
 *
 */
void FixedTextBlock::adjustHeight()
{
    if(_attachedToTop)
    {
        qreal docH = _textItem.document()->size().height();
//        _textItem.setHeight(docH+2);
        _bottom = docH+2*TEXT_BUFFER;
        this->parentAsStateBoxGraphic()->setMinHeight(_bottom);
        this->resize();
        qDebug () << "_textItem document dimensions: " <<_textItem.document()->size() <<"\tchanged bottom: " << this->_bottom << "\tnew dimensions: " << this->getSize();
    }
    else // bottom anchored
    {

    }
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
 * x: moves the fixed text block to hug the left wall of the usable area of its parent
 * y: and down to the level of _top
 */
void FixedTextBlock::reposition()
{
    if(_attachedToTop)
    {
        QRectF pRect = this->parentAsSelectableBoxGraphic()->getContentAreaRect();
        qreal x = pRect.x();
        qreal y = _top + pRect.y();
        this->setPos(x,y);
    }
    else
    {
        QRectF pRect = this->parentAsSelectableBoxGraphic()->getContentAreaRect();
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
    QRectF pRect = this->parentAsSelectableBoxGraphic()->getContentAreaRect();
    _width = pRect.width() - _right;

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
        painter->drawLine(r.bottomLeft(),r.bottomRight() + QPointF(_right,0));
    }
    else
    {
        painter->drawLine(r.topLeft()+QPointF(0,-2) ,r.topRight()+QPointF(0,-2)+ QPointF(_right,0));
    }
}

QRectF FixedTextBlock::boundingRect() const
{
    return QRectF(0,0,_width, _height);
}

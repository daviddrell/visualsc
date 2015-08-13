#include "maskedtextedit.h"
#include <QPainter>
#include <QDebug>
#include <QTextEdit>
#include "selectableboxgraphic.h"
#include "selectabletextblock.h"


MaskedTextEdit::MaskedTextEdit(QGraphicsItem *parent ,QRectF rect ) :
        QGraphicsTextItem(parent),
        _rect(rect)
{

    setFlags(ItemIsFocusable);
    //setFlag(QGraphicsItem::ItemIsMovable, false );
    setTextInteractionFlags(Qt::NoTextInteraction);
}

void MaskedTextEdit::setBoundingRect(QRectF rect)
{
    _rect = rect;
}

QRectF MaskedTextEdit::boundingRect()
{
    return _rect;
}

//void MaskedTextEdit::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
//{
//    this->installSceneEventFilter(this->parentAsSelectableTextBlock());
//}

//void MaskedTextEdit::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
//{
//    this->removeSceneEventFilter(this->parentAsSelectableTextBlock());
//}

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
    setTextInteraction(false,false);
    //setSelected(false);
    emit focusOut();
}

#ifdef MTE_MOUSE_EVENTS

/**
 * @brief MaskedTextEdit::mousePressEvent
 * @param event
 *
 * Edit mode: event goes to text item
 * Default mode: event goes to parent text block
 *
 */
void MaskedTextEdit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    SelectableTextBlock* parent = parentAsSelectableTextBlock();

    if(this->isSelected())
        QGraphicsTextItem::mousePressEvent(event);
    else
        parentAsSelectableTextBlock()->mousePressEvent(event);
}

/**
 * @brief MaskedTextEdit::mouseMoveEvent
 * @param event
 *
 *  * Edit mode: event goes to text item
 * Default mode: event goes to parent text block
 */
void MaskedTextEdit::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    SelectableTextBlock* parent = parentAsSelectableTextBlock();

    if(this->isSelected())
        QGraphicsTextItem::mouseMoveEvent(event);
    else
        parentAsSelectableTextBlock()->mouseMoveEvent(event);
}
/**
 * @brief MaskedTextEdit::mouseReleaseEvent
 * @param event
 * Edit mode: event goes to text item
 * Default mode: event goes to parent text block
*/
void MaskedTextEdit::mouseReleaseEvent (QGraphicsSceneMouseEvent * event )
{
    SelectableTextBlock* parent = parentAsSelectableTextBlock();

    if(this->isSelected())
        QGraphicsTextItem::mouseReleaseEvent(event);

    else
        parentAsSelectableTextBlock()->mouseReleaseEvent(event);
}


void MaskedTextEdit::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *evt)
{
    qDebug("mouse double clicke");
    //qDebug("mouseDoubleClickEvent '%s'", this->toPlainText().toStdString().c_str());
    if(textInteractionFlags() == Qt::TextEditorInteraction)
    {
        // if editor mode is already on: pass double click events on to the editor:
        QGraphicsTextItem::mouseDoubleClickEvent(evt);
        return;
    }

    // if editor mode is off:
    // 1. turn editor mode on and set selected and focused:
    setTextInteraction(true, true);
    resizeRectToTextBlock();


}
#endif

/**
 * @brief MaskedTextEdit::setTextInteraction
 * @param on
 * @param selectAll
 *
 * if on != textInteractionOn, then it will either disable text interaction
 */
void MaskedTextEdit::setTextInteraction(bool on, bool selectAll)
{
    if(on && textInteractionFlags() == Qt::NoTextInteraction)
    {
        // switch on editor mode:
        setTextInteractionFlags(Qt::TextEditorInteraction);
        // manually do what a mouse click would do else:
        setFocus(Qt::MouseFocusReason); // this gives the item keyboard focus
        setSelected(true); // this ensures that itemChange() gets called when we click out of the item
        if(selectAll) // option to select the whole text (e.g. after creation of the TextItem)
        {
            QTextCursor c = textCursor();
            c.select(QTextCursor::Document);
            setTextCursor(c);
        }
    }
    else if(!on && textInteractionFlags() == Qt::TextEditorInteraction)
    {
        // turn off editor mode:
        setTextInteractionFlags(Qt::NoTextInteraction);
        // deselect text (else it keeps gray shade):
        QTextCursor c = this->textCursor();
        c.clearSelection();
        this->setTextCursor(c);
        clearFocus();
        setSelected(false);
    }
}

//QVariant MaskedTextEdit::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
//{
//    if(change == QGraphicsItem::ItemSelectedChange) qDebug("itemChange '%s', selected=%s", this->toPlainText().toStdString().c_str(), value.toString().toStdString().c_str());
//    if(change == QGraphicsItem::ItemSelectedChange
//        && textInteractionFlags() != Qt::NoTextInteraction
//        && !value.toBool())
//    {
//        // item received SelectedChange event AND is in editor mode AND is about to be deselected:
//        setTextInteraction(false, false); // leave editor mode
//    }
//    return QGraphicsTextItem::itemChange(change, value);
//}

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

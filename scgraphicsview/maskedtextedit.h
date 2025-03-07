#ifndef MASKEDTEXTEDIT_H
#define MASKEDTEXTEDIT_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QVariant>
//#include "selectableboxgraphic.h"


#define MTE_MOUSE_EVENTS

class FixedTextBlock;
class SelectableBoxGraphic;
class SelectableTextBlock;
class StateBoxGraphic;
/**
    \class MaskedTextEdit
    \brief this class provides a QGraphicsTextItem but with a controlable display area.
    Set the bounding rect to control the visible text area. The coordinates in the set
    should be relative to this object.
  */



class MaskedTextEdit : public QGraphicsTextItem
{
    Q_OBJECT
public:

    MaskedTextEdit(QGraphicsItem *parent = NULL, QRectF rect = QRectF());
    void setBoundingRect(QRectF rect);

    virtual QRectF boundingRect();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    SelectableTextBlock* parentAsSelectableTextBlock();
    FixedTextBlock* parentAsFixedTextBlock();
    void resizeRectToFixedTextBlock();
    void resizeRectToTextBlock();
//    void resizeRectToTextSize();

    void setWidth(qreal w);
    void setHeight(qreal h);

    void setTextInteraction(bool on, bool selectAll);

signals:
    void focusOut();
    void hovered();
    void unhovered();

public slots:


private:

    bool eventFilter(QObject* o, QEvent* e);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
//    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
#ifdef MTE_MOUSE_EVENTS
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *evt);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );///< allows the main object to be moved in the scene by capturing the mouse move events
    void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );
#endif
    QRectF _rect;
    QRectF _document;
};

#endif // MASKEDTEXTEDIT_H

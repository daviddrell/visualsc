#ifndef FIXEDTEXTBLOCK_H
#define FIXEDTEXTBLOCK_H

#include <QGraphicsObject>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <maskedtextedit.h>
#include "selectableboxgraphic.h"
#include "textblock.h"

class StateBoxGraphic;
enum Font{
        Normal,
        Small
    };
enum PenStyle{
    Default,
    Experimental
};


class FixedTextBlock : public QGraphicsObject
{
    Q_OBJECT



public:
    FixedTextBlock(QGraphicsObject* parent,qreal topFraction, qreal bottomFraction, bool attachedToTop);
    ~FixedTextBlock();
    void switchPen(int);
    virtual QRectF boundingRect() const;
    SelectableBoxGraphic* parentAsSelectableBoxGraphic();
    StateBoxGraphic* parentAsStateBoxGraphic();
    void reposition();
    void resize();
    void recenterText();
    QPointF getSize();
    void setText(QString);
    void setFont(int);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );///< allows the main object to be moved in the scene by capturing the mouse move events
    void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );

signals:
    void changed(QString);

public slots:
    void handleTextItemEdited();
    void handleStateBoxResized(QRectF, QRectF, int);
    void handleStateSizeChanged(SizeAttribute*);

protected:
    qreal clampMin(qreal value, qreal min);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    qreal _width;
    qreal _height;
    qreal _top;
    qreal _bottom;
    bool _attachedToTop;
    QPen _pen;

private:
    MaskedTextEdit _textItem;
};

#endif // FIXEDTEXTBLOCK_H

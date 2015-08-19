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

class FixedTextBlock : public QGraphicsObject
{
    Q_OBJECT

    enum PenStyle{
        Default,
        Experimental
    };

public:
    FixedTextBlock(QGraphicsObject* parent,qreal topFraction, qreal bottomFraction, bool fixedHeight);
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

signals:
    void nameChanged(QString);

public slots:
    void handleTextItemEdited();
    void handleStateBoxResized(QRectF, QRectF, int);
    void handleStateSizeChanged(SizeAttribute*);

protected:
    qreal clampMin(qreal value, qreal min);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    qreal _width;
    qreal _height;
    qreal _topFraction;
    qreal _bottom;
    bool _fixedHeight;
    QPen _pen;

private:
    MaskedTextEdit _textItem;
};

#endif // FIXEDTEXTBLOCK_H

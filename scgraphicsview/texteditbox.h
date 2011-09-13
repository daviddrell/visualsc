#ifndef TEXTEDITBOX_H
#define TEXTEDITBOX_H
#include <QGraphicsObject>
#include <QString>
#include <QPainter>
#include "textblock.h"
#include "textblockattributes.h"
#include "buttongraphic.h"
#include "QGraphicsTextItem"

class TextEditBox :  public QGraphicsObject
{
    Q_OBJECT

public:
    TextEditBox(TextBlock  *textBlock);
    ~TextEditBox();

    virtual void graphicHasChanged(); ///< subclass must implement this, used to record user changes back to the data model

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) ;//virtual
    QRectF boundingRect() const;

protected:
    void keyPressEvent ( QKeyEvent * event ); // [virtual protected]

private slots:
    void handleScaleChanged();

private:
    TextBlock *_textBlock;
    ButtonGraphic *_button;

    QGraphicsTextItem * _textItem;

};

#endif // TEXTEDITBOX_H

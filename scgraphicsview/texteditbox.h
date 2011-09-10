#ifndef TEXTEDITBOX_H
#define TEXTEDITBOX_H
#include "selectableboxgraphic.h"
#include <QString>
#include <QPainter>
#include "textblock.h"
#include "textblockattributes.h"

class TextEditBox :  public QGraphicsTextItem
{
    Q_OBJECT

public:
    TextEditBox(TextBlock  *textBlock);

    virtual void graphicHasChanged(); ///< subclass must implement this, used to record user changes back to the data model

    void keyPressEvent ( QKeyEvent * event ); // [virtual protected]
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) ;//virtual

private slots:
    void handleScaleChanged();

private:
    TextBlock *_textBlock;
    // QGraphicsTextItem _textItem;

};

#endif // TEXTEDITBOX_H

#include "textbox.h"

TextBox::TextBox(QGraphicsItem* parent) :
    QGraphicsTextItem(parent)
{

}
TextBox::TextBox(QGraphicsItem *parent, const QString &text) :
    QGraphicsTextItem(text, parent)
{

}

TextBox::~TextBox()
{

}


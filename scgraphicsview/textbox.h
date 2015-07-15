#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QGraphicsTextItem>
#include <QString>

class TextBox : public QGraphicsTextItem
{
public:
    TextBox(QGraphicsItem* parent);
    TextBox(QGraphicsItem* parent, const QString& text);
    ~TextBox();
};

#endif // TEXTBOX_H

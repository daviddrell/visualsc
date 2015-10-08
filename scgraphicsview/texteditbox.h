#ifndef TEXTEDITBOX_H
#define TEXTEDITBOX_H
#include <QGraphicsObject>
#include <QString>
#include <QPainter>
#include "textblock.h"
#include "textblockattributes.h"
#include "buttongraphic.h"
#include "QGraphicsTextItem"


/**
  * \class TextEditBox
    \ingroup GraphicsView
    \brief Provides a text editor for the user.
    This class allows the user to edit text and includes save and abandon buttons.
  */

class TextEditBox :  public QGraphicsObject
{
    Q_OBJECT

public:
    TextEditBox(SCTextBlock  *textBlock);
    ~TextEditBox();

    virtual void graphicHasChanged(); ///< subclass must implement this, used to record user changes back to the data model

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) ;//virtual
    QRectF boundingRect() const;

signals:
    void saveButtonClicked(QString text);
    void cancelButtonClicked();

protected:
    void keyPressEvent ( QKeyEvent * event ); // [virtual protected]

private slots:
    void handleSaveButtonClicked();
    void handleCancelButtonClicked();

private:
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events


    SCTextBlock *_textBlock;
    ButtonGraphic *_SaveButton;
    ButtonGraphic *_CancelButton;

    QGraphicsTextItem * _textItem;
    int                 _minWidth;
    int                 _minHeight;
};

#endif // TEXTEDITBOX_H

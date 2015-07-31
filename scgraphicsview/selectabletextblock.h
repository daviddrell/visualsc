#ifndef SELECTABLETEXTBLOCK_H
#define SELECTABLETEXTBLOCK_H

#include "maskedtextedit.h"
#include "selectableboxgraphic.h"
#include "textblock.h"
#include "iattribute.h"
class QKeyEvent;
class QGraphicsSceneMouseEvent;

/**
  * \class SelectableTextBlock
  * \ingroup GraphicsView
  * \brief This item holds text such as the state name, transition names, commentary blocks, etc.
  *
  * This text block is selectable, movable, re-sizable, (TBD: and it can be given a fill color and border design).
  * The text attributes are stored into the scxml file.

The amount of viewable text is determined by the box size.
It is used to display text for state, state transitions, commentary text,
or any other text in the scene. Another class, TextEditBox, allows users to edit and save the text. But the text box in
this class is only for displaying text. The user can move the text box and re-size it for optimal placement in the scene.

The class is named SelecableTextBlock. It is derived from the base SelectableBoxGraphic.
SelectableBoxGraphic provides a basic box with four corner-grabbers which provides the moving and resizing of the box.
The SelectableTextBlock class adds a QGraphicsTextItem for text display.

This trick to making this work is re-sizing the QGraphicsTextItem when the user re-sizes the base SelectableBoxGraphic.
This is accomplished via the setSize() virtual method in the base class. The derived SelectableTextBlock class re-implements the setSize() method,
but the base class calls this method when the user re-sizes the block.

*/

class SelectableTextBlock : public SelectableBoxGraphic
{
    Q_OBJECT

public:
    SelectableTextBlock(QGraphicsObject *parent=NULL,SCTextBlock *textBlockModel=NULL);
    ~SelectableTextBlock();


    virtual void setSize(QPoint size); ///< from base class


protected:

    void keyPressEvent ( QKeyEvent * event ); // [virtual protected]
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );// [virtual protected]


private slots:

    void handleTextChanged();
    void handleAttributeChanged(IAttribute *attr);
    void handleAttributeChanged(SizeAttribute *attr);
    void handleAttributeChanged(PositionAttribute* attr);
    void handleAttributeAdded(IAttribute *attr);
    void handleAttributeDeleted(IAttribute *attr);
    void handleEditBoxSavedText(QString text);

private:

    // private methods

   void connectAttributes(IAttributeContainer *attributes);
   virtual void graphicHasChanged(); ///< pure virtual in base SelectableBoxGraphic, subclass must implement this, used to record user changes back to the data model

   // private data

   QPoint _minSize;
   int           _verticalTextMargin;
   int           _horizontalTextMargin;
   MaskedTextEdit _textItem;

   SCTextBlock * _textBlockModel;
};

#endif // SELECTABLETEXTBLOCK_H

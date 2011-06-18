#ifndef SELECTABLETEXTBLOCK_H
#define SELECTABLETEXTBLOCK_H

#include <QGraphicsTextItem>
#include "selectableboxgraphic.h"

/**
  * \class SelectableTextBlock
  *
  * This text block is selectable, movable, re-sizable, and its can be given a fill color and border design.
  * The text attributes are stored into the scxml file.
  */

class SelectableTextBlock : public SelectableBoxGraphic
{
public:
    SelectableTextBlock(QGraphicsObject *parent=NULL);

    void setPlainText(QString text);
    virtual void setSize(QPoint size); ///< from base class

private:

    // private methods


   virtual void graphicHasChanged(); ///< pure virtual in base SelectableBoxGraphic, subclass must implement this, used to record user changes back to the data model

   // private data
   QGraphicsTextItem _textItem;

   QPoint _minSize;

};

#endif // SELECTABLETEXTBLOCK_H

#ifndef SELECTABLETEXTBLOCK_H
#define SELECTABLETEXTBLOCK_H

#include <QGraphicsTextItem>
#include "selectableboxgraphic.h"
#include "textblock.h"
#include "iattribute.h"

/**
  * \class SelectableTextBlock
  * \ingroup GraphicsView
  * \brief This item holds text such as the state name, transition names, commentary blocks, etc.
  *
  * This text block is selectable, movable, re-sizable, and its can be given a fill color and border design.
  * The text attributes are stored into the scxml file.
  */

class SelectableTextBlock : public SelectableBoxGraphic
{
    Q_OBJECT

public:
    SelectableTextBlock(QGraphicsObject *parent=NULL,TextBlock *textBlockModel=NULL);
    ~SelectableTextBlock();

    void setPlainText(QString text);
    virtual void setSize(QPoint size); ///< from base class


private slots:

    void handleAttributeChanged(IAttribute *attr);

private:

    // private methods


   virtual void graphicHasChanged(); ///< pure virtual in base SelectableBoxGraphic, subclass must implement this, used to record user changes back to the data model

   // private data
   QGraphicsTextItem _textItem;

   QPoint _minSize;

   TextBlock * _textBlockModel;
};

#endif // SELECTABLETEXTBLOCK_H

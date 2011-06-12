#ifndef SELECTABLETEXTBLOCK_H
#define SELECTABLETEXTBLOCK_H

#include <QGraphicsTextItem>

class CornerGrabber;

/**
  * \class SelectableTextBlock
  *
  * This text block is selectable, movable, re-sizable, and its can be given a fill color and border design.
  * The text attributes are stored into the scxml file.
  */

class SelectableTextBlock : public QGraphicsTextItem
{
public:
    SelectableTextBlock();

private: //data

       CornerGrabber*  _corners[4];// 0,1,2,3  - starting at x=0,y=0 and moving clockwise around the box
};

#endif // SELECTABLETEXTBLOCK_H

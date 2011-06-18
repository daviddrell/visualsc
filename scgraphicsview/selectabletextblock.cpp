#include "selectabletextblock.h"
#include <QDebug>


SelectableTextBlock::SelectableTextBlock(QGraphicsObject *parent) :
        SelectableBoxGraphic(parent),
        _textItem(this),
        _minSize(QPoint(100,50))
{
    _textItem.setTextInteractionFlags(Qt::NoTextInteraction);
    _textItem.setFlag(QGraphicsItem::ItemIsMovable, false );
    _textItem.setPos( this->pos().x()+5,this->pos().y()+5  );


    setShowBoxLineStyle(SelectableBoxGraphic::kWhenSelected  );

    setDrawBoxLineStyle( SelectableBoxGraphic::kDrawDotted );

    setBoxStyle(SelectableBoxGraphic::kTransparent );

    setHoverLineThickness( 1 );

    setFlags(QGraphicsItem::ItemClipsChildrenToShape);

    setSize (_minSize);
}

 void SelectableTextBlock::setSize(QPoint size)
 {
     if ( size.x() < _minSize.x())
         size.setX( _minSize.x());

     if ( size.y() < _minSize.y())
         size.setY( _minSize.y());


     _textItem.setTextWidth(size.x());
     SelectableBoxGraphic::setSize(size);
 }

void SelectableTextBlock::setPlainText(QString text)
{
    _textItem.setPlainText(text);
}


void SelectableTextBlock::graphicHasChanged()
{
    // to be done:
    // write user changes to text block back to data model
    // size, position, text
}

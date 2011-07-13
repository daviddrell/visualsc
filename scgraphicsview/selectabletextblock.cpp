#include "selectabletextblock.h"
#include "positionattribute.h"
#include "sizeattribute.h"
#include <QDebug>


SelectableTextBlock::SelectableTextBlock(QGraphicsObject *parent,TextBlock *textBlockModel) :
        SelectableBoxGraphic(parent),
        _textItem(this),
        _minSize(QPoint(100,50)),
        _textBlockModel(textBlockModel)
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

    TextAttribute * text = dynamic_cast<TextAttribute *> (  _textBlockModel->attributes.value("text"));
    connect (text, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(text);


    FontFamilyAttribute * ff = dynamic_cast<FontFamilyAttribute *> (  _textBlockModel->attributes.value("font-family"));
    connect (ff, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(ff);


    FontSizeAttribute * fs = dynamic_cast<FontSizeAttribute *> (  _textBlockModel->attributes.value("font-size"));
    connect (fs, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(ff);


    FontColorAttribute * fc = dynamic_cast<FontColorAttribute *> (  _textBlockModel->attributes.value("font-color"));
    connect (fc, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(fc);


    FontBoldAttribute * fb = dynamic_cast<FontBoldAttribute *> (  _textBlockModel->attributes.value("font-bold"));
    connect (fb, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(fb);


    FontUnderlineAttribute * fu = dynamic_cast<FontUnderlineAttribute *> (  _textBlockModel->attributes.value("font-underline"));
    connect (fu, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(fu);


    SizeAttribute * size = dynamic_cast<SizeAttribute *> (  _textBlockModel->attributes.value("size"));
    connect (size, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(size);

    PositionAttribute * position =dynamic_cast<PositionAttribute*> ( _textBlockModel->attributes.value("position"));
    connect (position, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(position);

}


SelectableTextBlock::~SelectableTextBlock()
{
}



void SelectableTextBlock::handleAttributeChanged(IAttribute *attr)
{
    TextAttribute * text         = dynamic_cast<TextAttribute *> ( attr);
//    FontFamilyAttribute * ff     = dynamic_cast<FontFamilyAttribute *> ( attr);
//    FontSizeAttribute * fs       = dynamic_cast<FontSizeAttribute *> ( attr);
    FontColorAttribute * fc      = dynamic_cast<FontColorAttribute *> ( attr);
//    FontBoldAttribute * fb       = dynamic_cast<FontBoldAttribute *> ( attr);
//    FontUnderlineAttribute * fu  = dynamic_cast<FontUnderlineAttribute *> ( attr);
    SizeAttribute * size         = dynamic_cast<SizeAttribute *> ( attr);
    PositionAttribute * position = dynamic_cast<PositionAttribute*> (attr);


    if ( text )
    {
        this->setObjectName(text->asString()); // help debug tracing - id which object this is
        _textItem.setPlainText(text->asString());
    }
    else if ( size )
    {
        QPoint pt = size->asPointF().toPoint();
        SelectableBoxGraphic::setSize(pt);
    }
    else if ( position)
    {
        QPointF ps = position->asPointF();
        SelectableBoxGraphic::setPos(ps);
    }
    else if ( fc )
    {
        QColor c = fc->asQColor();
        _textItem.setDefaultTextColor(c);
    }

    QGraphicsItem *parent = this->parentItem();
    if ( parent)
        parent->update();
    else
        this->update();
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


}

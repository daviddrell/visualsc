#include "selectabletextblock.h"
#include "positionattribute.h"
#include "sizeattribute.h"
#include <QDebug>
#include <QKeyEvent>
#include "texteditbox.h"
#include "scdatamodel.h"
#include <QGraphicsScene>

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

    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setSize (_minSize);

    connect ( _textBlockModel, SIGNAL(textChanged()), this, SLOT(handleTextChanged()), Qt::QueuedConnection);
    connect ( & _textBlockModel->attributes,SIGNAL(attributeAdded(IAttribute*)), SLOT(handleAttributeAdded(IAttribute*)) );
    connect ( & _textBlockModel->attributes,SIGNAL(attributeDeleted(IAttribute*)), SLOT(handleAttributeDeleted(IAttribute*)) );

    connectAttributes( & _textBlockModel->attributes);

    _textItem.setPlainText( _textBlockModel->getText() );

}


SelectableTextBlock::~SelectableTextBlock()
{
}

void SelectableTextBlock::handleTextChanged()
{
    _textItem.setPlainText( _textBlockModel->getText() );
}

void SelectableTextBlock::keyPressEvent ( QKeyEvent * event )
{
    if(event->key() == Qt::Key_F4)
    {
        TextEditBox * editBox = new TextEditBox( _textBlockModel);
        connect (editBox, SIGNAL(saveButtonClicked(QString)), this, SLOT(handleEditBoxSavedText(QString)));
        QPointF myPos = this->mapToScene( this->pos() );
        SCDataModel::singleton()->getScene()->addItem( editBox);
        editBox->setPos( myPos.x(), myPos.y() +100 );
    }
}

void SelectableTextBlock::handleEditBoxSavedText(QString text)
{
    _textItem.setPlainText( text );
    _textBlockModel->setText(text);

}

void SelectableTextBlock::connectAttributes(IAttributeContainer *attributes)
{

    FontFamilyAttribute * ff = dynamic_cast<FontFamilyAttribute *> (  attributes->value("font-family"));
    if ( ff )
    {
        connect (ff, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
        handleAttributeChanged(ff);
    }


    FontSizeAttribute * fs = dynamic_cast<FontSizeAttribute *> (  attributes->value("font-size"));
    if ( fs )
    {
        connect (fs, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
        handleAttributeChanged(ff);
    }

    FontColorAttribute * fc = dynamic_cast<FontColorAttribute *> (  attributes->value("font-color"));
    if ( fc )
    {
        connect (fc, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
        handleAttributeChanged(fc);
    }


    FontBoldAttribute * fb = dynamic_cast<FontBoldAttribute *> (  attributes->value("font-bold"));
    if ( fb )
    {
        connect (fb, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
        handleAttributeChanged(fb);
    }

    FontUnderlineAttribute * fu = dynamic_cast<FontUnderlineAttribute *> (  attributes->value("font-underline"));
    if ( fu )
    {
        connect (fu, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
        handleAttributeChanged(fu);
    }

    SizeAttribute * size = dynamic_cast<SizeAttribute *> (  attributes->value("size"));
    if ( fs )
    {
        connect (size, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
        handleAttributeChanged(size);
    }

    PositionAttribute * position =dynamic_cast<PositionAttribute*> ( attributes->value("position"));
    if  (position)
    {
        connect (position, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
        handleAttributeChanged(position);
    }

}

void SelectableTextBlock::handleAttributeAdded(IAttribute *attr)
{
    handleAttributeChanged(attr);
}

void SelectableTextBlock::handleAttributeDeleted(IAttribute *)
{
}

void SelectableTextBlock::handleAttributeChanged(IAttribute *attr)
{
//    FontFamilyAttribute * ff     = dynamic_cast<FontFamilyAttribute *> ( attr);
//    FontSizeAttribute * fs       = dynamic_cast<FontSizeAttribute *> ( attr);
    FontColorAttribute * fc      = dynamic_cast<FontColorAttribute *> ( attr);
    FontBoldAttribute * fb       = dynamic_cast<FontBoldAttribute *> ( attr);
//    FontUnderlineAttribute * fu  = dynamic_cast<FontUnderlineAttribute *> ( attr);
    SizeAttribute * size         = dynamic_cast<SizeAttribute *> ( attr);
    PositionAttribute * position = dynamic_cast<PositionAttribute*> (attr);

    if ( size )
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
    else if ( fb )
    {

        QFont font = _textItem.font();

        _textItem.setFont(font);

        if ( fb->asBool() )
        {
            font.setWeight( QFont::Bold );
        }
        else
        {
            font.setWeight( QFont::Normal);
        }

        _textItem.setFont(font);
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


 //void SelectableTextBlock::setPlainText(QString text)
 //{
 //    _textItem.setPlainText(text);
 //}


void SelectableTextBlock::graphicHasChanged()
{


}

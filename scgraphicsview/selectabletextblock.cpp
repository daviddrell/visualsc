
/*
    visualsc - Visual State Chart Editing for SCXML State Engines
    Copyright (C) 2011  David W. Drell (david@davidwdrell.net) and
    Contributing Authors:


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "selectabletextblock.h"
#include "positionattribute.h"
#include "sizeattribute.h"
#include <QDebug>
#include <QKeyEvent>
#include "texteditbox.h"
#include "scdatamodel.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>

SelectableTextBlock::SelectableTextBlock(QGraphicsObject *parent,SCTextBlock *textBlockModel) :
        SelectableBoxGraphic(parent),
        _minSize(QPoint(100,50)),
        _verticalTextMargin(10),
        _horizontalTextMargin(10),
        _textItem(this, QRect(0,0, _minSize.x()-_horizontalTextMargin, _minSize.y()-_verticalTextMargin)),
        _textBlockModel(textBlockModel)
{
    _textItem.setTextInteractionFlags(Qt::NoTextInteraction);
    _textItem.setFlag(QGraphicsItem::ItemIsMovable, false );
    QRectF viewArea = this->getUsableArea();
    _textItem.setPos( viewArea.x() , viewArea.y() );

    setShowBoxLineStyle(SelectableBoxGraphic::kWhenSelected  );

    setDrawBoxLineStyle( SelectableBoxGraphic::kDrawDotted );

    setBoxStyle(SelectableBoxGraphic::kTransparent );

    setHoverLineThickness( 1 );

    setFlags(QGraphicsItem::ItemClipsChildrenToShape);

    setFlag(QGraphicsItem::ItemIsFocusable, true);

    _textItem.setPlainText( _textBlockModel->getText() );

   // setSize (_minSize);

    connect ( _textBlockModel, SIGNAL(textChanged()), this, SLOT(handleTextChanged()), Qt::QueuedConnection);
    connect ( & _textBlockModel->attributes,SIGNAL(attributeAdded(IAttribute*)), SLOT(handleAttributeAdded(IAttribute*)), Qt::QueuedConnection );
    connect ( & _textBlockModel->attributes,SIGNAL(attributeDeleted(IAttribute*)), SLOT(handleAttributeDeleted(IAttribute*)), Qt::QueuedConnection );

    connectAttributes( & _textBlockModel->attributes );

}


SelectableTextBlock::~SelectableTextBlock()
{
}

void SelectableTextBlock::handleTextChanged()
{
    _textItem.setPlainText( _textBlockModel->getText() );
}

void SelectableTextBlock::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
    event->accept();
    TextEditBox * editBox = new TextEditBox( _textBlockModel);
    connect (editBox, SIGNAL(saveButtonClicked(QString)), this, SLOT(handleEditBoxSavedText(QString)));
    QPointF myPos = this->mapToScene( this->pos() );
    SCDataModel::singleton()->getScene()->addItem( editBox);
    editBox->setPos( myPos.x(), myPos.y() +100 );

}

void SelectableTextBlock::keyPressEvent ( QKeyEvent * event )
{
    event->accept();
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
        qDebug()<<"SelectableTextBlock::handleAttributeChanged ("+  _textBlockModel->getText()+") setting size = (" +QString::number(pt.x())+","+QString::number(pt.y())+")";
        this->setSize(pt);
    }
    else if ( position)
    {
        QPointF ps = position->asPointF();
        qDebug()<<"SelectableTextBlock::handleAttributeChanged ("+  _textBlockModel->getText()+") setting position = (" +QString::number(ps.x())+","+QString::number(ps.y())+")";
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
     if ( size.manhattanLength()< 100)
     {
         size.setX( _minSize.x());
         size.setY( _minSize.y());
     }

     SelectableBoxGraphic::setSize(size);

     QRectF viewArea = getUsableArea();
     int width = viewArea.width() - _horizontalTextMargin;
     int height = viewArea.height() - _verticalTextMargin;

     QRect bRect( 0,0 , width, height);
     _textItem.setBoundingRect(bRect);

     update();
 }


void SelectableTextBlock::graphicHasChanged()
{

    QPointF sz =this->getSize();

    QPointF ps = this->pos();

    SizeAttribute * size         = dynamic_cast<SizeAttribute *> ( _textBlockModel->attributes["size"]);
    PositionAttribute * position = dynamic_cast<PositionAttribute*> (_textBlockModel->attributes["position"]);

    size->setValue(sz);
    position->setValue(ps);


}

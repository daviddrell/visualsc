
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
        _minSize(QPoint(150,40)),
        _verticalTextMargin(10),
        _horizontalTextMargin(10),
        _textItem(this, QRect(0,0, _minSize.x()-_horizontalTextMargin, _minSize.y()-_verticalTextMargin)),
        _textBlockModel(textBlockModel)
{
    _textItem.setTextInteractionFlags(Qt::NoTextInteraction);
    _textItem.setFlag(QGraphicsItem::ItemIsMovable, false );

    // set the text into the viewable area of the rectangle
    QRectF viewArea = this->getUsableArea();
    _textItem.setPos( viewArea.x() , viewArea.y() );

    // set the initial text to what the datamodel loaded
    _textItem.setPlainText( _textBlockModel->getText() );


    setShowBoxLineStyle(SelectableBoxGraphic::kWhenSelected  );
    setDrawBoxLineStyle( SelectableBoxGraphic::kDrawDotted );
    setBoxStyle(SelectableBoxGraphic::kTransparent );
    setHoverLineThickness( 6 );
    setFlags(QGraphicsItem::ItemClipsChildrenToShape);
    setFlag(QGraphicsItem::ItemIsFocusable, true);



   // setSize (_minSize);


    // if the textblock model changed, signal handle text changed to update it from the textblock model
    connect ( _textBlockModel, SIGNAL(textChanged()), this, SLOT(handleTextChanged()), Qt::QueuedConnection);



    connect ( & _textBlockModel->attributes,SIGNAL(attributeAdded(IAttribute*)), SLOT(handleAttributeAdded(IAttribute*)), Qt::QueuedConnection );
    connect ( & _textBlockModel->attributes,SIGNAL(attributeDeleted(IAttribute*)), SLOT(handleAttributeDeleted(IAttribute*)), Qt::QueuedConnection );

    qDebug() << "SelectableTextBLock::connectAttributes!";

    // connect the data model attributes to handlers for the graphics object
    connectAttributes( & _textBlockModel->attributes );

}


SelectableTextBlock::~SelectableTextBlock()
{
}

void SelectableTextBlock::handleTextChanged()
{
    qDebug() << "SelectableTextBlock::handleTextChanged";
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
    editBox->setZValue(999);    // temporary fix for getting the box to appear in front of everything in the scene

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

/**
 * @brief SelectableTextBlock::handleEditBoxSavedText
 * @param text
 *
 * when the check box is clicked, update the textItem (GraphicsView) and update the textBlockModel (DataModel)
 *
 */
void SelectableTextBlock::handleEditBoxSavedText(QString text)
{
    _textItem.setPlainText( text );
    _textBlockModel->setText(text);
}

/**
 * @brief SelectableTextBlock::connectAttributes
 * @param attributes
 *
 * called by constructor to connect each of the properties to handling an attribute change
 *
 */
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


    SizeAttribute * size = dynamic_cast<SizeAttribute*> (  attributes->value("size"));
    if ( size )
    {
        qDebug() << "connecting size!";
        connect (size, SIGNAL(changed(SizeAttribute*)), this, SLOT(handleAttributeChanged(SizeAttribute*)), Qt::QueuedConnection);
        handleAttributeChanged(size);
    }

    PositionAttribute * position =dynamic_cast<PositionAttribute*> ( attributes->value("position"));
    if  (position)
    {
        connect (position, SIGNAL(changed(PositionAttribute*)), this, SLOT(handleAttributeChanged(PositionAttribute*)), Qt::QueuedConnection);
        handleAttributeChanged(position);
    }

}


// for supporting moving the box across the scene
void SelectableTextBlock::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    /*
    event->setAccepted(true);
    QPointF location = this->pos();
    location.setX( ( static_cast<qreal>(location.x()) / _gridSpace) * _gridSpace );
    location.setY( ( static_cast<qreal>(location.y()) / _gridSpace) * _gridSpace );

    this->setPos(location);

    //qDebug() << "MOUSE RELEASE : " << this->pos() << "";
   // emit stateBoxMoved(this->pos());

    // will call the corresponding overrided graphicHasChanged function for a subclass

     */

    graphicHasChanged();
}


// for supporting moving the box across the scene
void SelectableTextBlock::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    _dragStart = event->pos();
}


// for supporting moving the box across the scene
/**
 * @brief SelectableBoxGraphic::mouseMoveEvent
 * @param event
 *
 *
 * SIGNAL   textBlockMoved(QPointF)
 *
 * connect in StateBoxGraphic
 *
 */
void SelectableTextBlock::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    //qDebug() << "MY mouse move event!";
    QPointF newPos = event->pos() ;
    QPointF location = this->pos();
    QPointF diff = newPos -_dragStart;
    location += diff;

    qreal x = pos().x()+diff.x();
    qreal y = pos().y()+diff.y();
    const qreal w = this->getSize().x();
    const qreal h = this->getSize().y();
    SelectableBoxGraphic* parent = parentAsSelectableBoxGraphic();

    const qreal parentW = parent->getSize().x();
    const qreal parentH = parent->getSize().y();

    if(x < 0)
        x=0;

    else if(x+w > parentW)
        x=parentW-w;

    if(y < 0)
        y=0;

    else if(y+h > parentH)
        y=parentH-h;


    QPointF newPoint(x,y);


    this->setPos(newPoint);

}

SelectableBoxGraphic* SelectableTextBlock::parentAsSelectableBoxGraphic()
{
    return dynamic_cast<SelectableBoxGraphic*>(this->parentItem());
}

void SelectableTextBlock::handleAttributeAdded(IAttribute *attr)
{
    handleAttributeChanged(attr);
}

void SelectableTextBlock::handleAttributeDeleted(IAttribute *)
{
}

void SelectableTextBlock::handleAttributeChanged(SizeAttribute* size)
{
    QPoint pt = size->asPointF().toPoint();
    this->setSize(pt);

    qDebug()<<"SelectableTextBlock::handleAttributeChanged Size Attribute ("+  _textBlockModel->getText()+") setting size = (" +QString::number(pt.x())+","+QString::number(pt.y())+")";
}

void SelectableTextBlock::handleAttributeChanged(PositionAttribute* pos)
{
    QPointF ps = pos->asPointF();
    qDebug()<<"SelectableTextBlock::handleAttributeChanged ("+  _textBlockModel->getText()+") setting position = (" +QString::number(ps.x())+","+QString::number(ps.y())+")";
    SelectableBoxGraphic::setPos(ps);
}

/**
 * @brief SelectableTextBlock::handleAttributeChanged
 * @param attr
 *
 * SLOT
 *
 * connected in connectAttributes method
 *
 */
void SelectableTextBlock::handleAttributeChanged(IAttribute *attr)
{
    qDebug()<<"selectabletextblock::handleattributechanged";
    //TODO make these attributes do something

//    FontFamilyAttribute * ff     = dynamic_cast<FontFamilyAttribute *> ( attr);
//    FontSizeAttribute * fs       = dynamic_cast<FontSizeAttribute *> ( attr);
    FontColorAttribute * fc      = dynamic_cast<FontColorAttribute *> ( attr);
    FontBoldAttribute * fb       = dynamic_cast<FontBoldAttribute *> ( attr);
//    FontUnderlineAttribute * fu  = dynamic_cast<FontUnderlineAttribute *> ( attr);
    PositionAttribute * position = dynamic_cast<PositionAttribute*> (attr);

    if ( position)
    {
        QPointF ps = position->asPointF();
        qDebug()<<"SelectableTextBlock::handleAttributeChanged ("+  _textBlockModel->getText()+") setting position = (" +QString::number(ps.x())+","+QString::number(ps.y())+")";
        SelectableBoxGraphic::setPos(ps);
        //emit _textBlockModel->
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


 /**
 * @brief SelectableTextBlock::graphicHasChanged
 *
 * SIGNAL
 *
 * update the datamodel to match the graphic when the text block graphic has changed
 * additionally, notify the formview of this change to update the textblock property table
 *
 */
void SelectableTextBlock::graphicHasChanged()
{
    qDebug() << "selectable textblock graphic has changed.";

    QPointF sz =this->getSize();
    QPointF ps = this->pos();

    SizeAttribute * size         = dynamic_cast<SizeAttribute *> ( _textBlockModel->attributes["size"]);
    PositionAttribute * position = dynamic_cast<PositionAttribute*> (_textBlockModel->attributes["position"]);


    size->setValue(sz);
    position->setValue(ps);


    //emit size->changed(size);
    //set
    //emit _textBlockModel->sizeChangedInDataModel(_textBlockModel, sz);
    //emit _textBlockModel->positionChangedInDataModel(_textBlockModel, ps);
}

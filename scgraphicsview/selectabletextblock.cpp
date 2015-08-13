
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

#define DEFAULT_PEN_WIDTH 1
#define HOVER_PEN_WIDTH 1

#define MIN_WIDTH 50
#define MIN_HEIGHT      24+ 2*(CORNER_GRAB_Y_BUFFER+BOX_DRAW_BUFFER+TEXT_ITEM_Y_BUFFER)

#define HORIZONTAL_TEXT_MARGIN  5
#define VERTICAL_TEXT_MARGIN    5

//#define POP_UP_EDIT_MODE

SelectableTextBlock::SelectableTextBlock(QGraphicsObject *parent,SCTextBlock *textBlockModel) :
        SelectableBoxGraphic(parent,true),
       // _minSize(QPoint(MIN_WIDTH,MIN_HEIGHT)),
        _verticalTextMargin(10),
        _horizontalTextMargin(5),
        _textItem(this, QRect(0,0, TEXTBLOCK_DEFAULT_WIDTH-2*(TEXT_ITEM_X_BUFFER), TEXTBLOCK_DEFAULT_HEIGHT-2*(TEXT_ITEM_Y_BUFFER))),
        _textBlockModel(textBlockModel),
        _centerText(true)
{

    // Set accept hover events to false so the corners hover events are not interfered with
    _textItem.setAcceptHoverEvents(false);

#ifdef POP_UP_EDIT_MODE
    _textItem.setTextInteractionFlags(Qt::NoTextInteraction);
#endif
#ifndef POP_UP_EDIT_MODE
    //moved to constructor for masked text edit
 //   _textItem.setTextInteractionFlags(Qt::TextEditorInteraction);


#endif

    connect(&_textItem, SIGNAL(focusOut()), this, SLOT(handleTextItemEdited()));

    // set the text into the viewable area of the rectangle
//    QRectF viewArea = this->getUsableArea();
//    _textItem.setPos( viewArea.x() , viewArea.y() );
    _textItem.setPos(TEXT_ITEM_X_BUFFER, TEXT_ITEM_Y_BUFFER);
    qDebug()<<"text item is :" << _textItem.pos();
    QFont serifFont("Arial", 10, QFont::Bold);
    _textItem.setFont(serifFont);

    //_textItem.installSceneEventFilter(this);
    // set the initial text to what the datamodel loaded
    //_textItem.setPlainText( _textBlockModel->getText());
    this->setText(_textBlockModel->getText());
    qDebug() << "tb text" << _textBlockModel->getText();


    setShowBoxLineStyle(SelectableBoxGraphic::kWhenSelected  );
    setDrawBoxLineStyle( SelectableBoxGraphic::kDrawDotted );
    setBoxStyle(SelectableBoxGraphic::kTransparent );
    setFlags(QGraphicsItem::ItemClipsChildrenToShape);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setMinSize(QPoint(MIN_WIDTH,MIN_HEIGHT));
    setPenWidth(DEFAULT_PEN_WIDTH,HOVER_PEN_WIDTH);

#ifdef POP_UP_EDIT_MODE
    // if the textblock model changed, signal handle text changed to update it from the textblock model
    connect ( _textBlockModel, SIGNAL(textChanged()), this, SLOT(handleTextChanged()), Qt::QueuedConnection);
#endif


    connect ( & _textBlockModel->attributes,SIGNAL(attributeAdded(IAttribute*)), SLOT(handleAttributeAdded(IAttribute*)), Qt::QueuedConnection );
    connect ( & _textBlockModel->attributes,SIGNAL(attributeDeleted(IAttribute*)), SLOT(handleAttributeDeleted(IAttribute*)), Qt::QueuedConnection );

//    qDebug() << "SelectableTextBLock::connectAttributes!";

    // connect the data model attributes to handlers for the graphics object
    connectAttributes( & _textBlockModel->attributes );


}


SelectableTextBlock::~SelectableTextBlock()
{
}

void SelectableTextBlock::handleTextItemEdited()
{

    //_textItem.setTextWidth(this->getSize().x());
//    QRectF bound(-5, 0, this->getSize().x() - (HORIZONTAL_TEXT_MARGIN+HORIZONTAL_TEXT_MARGIN), this->getSize().y() - (VERTICAL_TEXT_MARGIN+VERTICAL_TEXT_MARGIN));


    qDebug() << "text item rect: " <<_textItem.boundingRect() << "\tmypos: "<< this->boundingRect();





    // update the data model for this text

    _textBlockModel->setText(_textItem.toPlainText());
    this->recenterText();
}

void SelectableTextBlock::resizeToFitParent()
{
    qreal w = this->getSize().x();
    qreal h = this->getSize().y();
    qreal x = this->pos().x();
    qreal y = this->pos().y();

    qreal parentW = this->parentAsSelectableBoxGraphic()->pos().x();
    qreal parentH = this->parentAsSelectableBoxGraphic()->pos().y();

    bool changed = false;

    if( x+w > parentW - INSIDE_PARENT_BUFFER)
    {
        w = parentW - INSIDE_PARENT_BUFFER - x;
        changed = true;
    }

    if(y+h > parentH - INSIDE_PARENT_BUFFER)
    {
        h = parentH - INSIDE_PARENT_BUFFER - y;
        changed = true;
    }

    if(changed)
    {

        this->setSize(QPoint(w,h));
    }


}


/**
  * This scene event filter has been registered with all four corner grabber items.
  * When called, a pointer to the sending item is provided along with a generic
  * event.  A dynamic_cast is used to determine if the event type is one of the events
  * we are interrested in.
  */
bool SelectableTextBlock::sceneEventFilter( QGraphicsItem * watched, QEvent * event )
{
    //    qDebug() << " QEvent == " + QString::number(event->type());

    CornerGrabber * corner = dynamic_cast<CornerGrabber *>(watched);
    if ( corner == NULL) return false; // not expected to get here

    QGraphicsSceneMouseEvent * mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if ( mevent == NULL)
    {
        // this is not one of the mouse events we are interrested in
        return false;
    }


    switch (event->type() )
    {
        // if the mouse went down, record the x,y coords of the press, record it inside the corner object
    case QEvent::GraphicsSceneMousePress:
        {
            corner->setMouseState(CornerGrabber::kMouseDown);
            corner->mouseDownX = mevent->pos().x();
            corner->mouseDownY = mevent->pos().y();
            //corner->setHovered(true);
        }
        break;

    case QEvent::GraphicsSceneMouseRelease:
        {
            corner->setMouseState(CornerGrabber::kMouseReleased);
            //corner->setHovered(false);
            graphicHasChanged();
        }
        break;

    case QEvent::GraphicsSceneMouseMove:
        {
            corner->setMouseState(CornerGrabber::kMouseMoving );
        }
        break;

    default:
        // we dont care about the rest of the events
        return false;
        break;
    }


    if ( corner->getMouseState() == CornerGrabber::kMouseMoving )
    {

         qDebug() << "corner mouse moving";
        qreal x = mevent->pos().x(), y = mevent->pos().y();
        QPointF mts = mapToScene(pos());
        mts = pos();
        QRectF oldBox = QRectF(mts.x(), mts.y(), _width, _height);

        // depending on which corner has been grabbed, we want to move the position
        // of the item as it grows/shrinks accordingly. so we need to eitehr add
        // or subtract the offsets based on which corner this is.

        int XaxisSign = 0;
        int YaxisSign = 0;
        switch( corner->getCorner() )
        {
        case 0:
            {
                XaxisSign = +1;
                YaxisSign = +1;
            }
            break;

        case 1:
            {
                XaxisSign = -1;
                YaxisSign = +1;
            }
            break;

        case 2:
            {
                XaxisSign = -1;
                YaxisSign = -1;
            }
            break;

        case 3:
            {
                XaxisSign = +1;
                YaxisSign = -1;
            }
            break;

        }

        // if the mouse is being dragged, calculate a new size and also re-position
        // the box to give the appearance of dragging the corner out/in to resize the box

        QPointF old(this->pos());
        //const qreal w = this->getSize().x();
        //const qreal h = this->getSize().y();


        SelectableBoxGraphic* parentGraphic = this->parentAsSelectableBoxGraphic();


         qreal parentW;
         qreal parentH;






        int xMoved = corner->mouseDownX - x;
        int yMoved = corner->mouseDownY - y;

        int newWidth = _width + ( XaxisSign * xMoved);
        if ( newWidth < _minSize.x() ) newWidth  = _minSize.x();

        int newHeight = _height + (YaxisSign * yMoved) ;
        if ( newHeight < _minSize.y() ) newHeight = _minSize.y();

        int deltaWidth;
        int deltaHeight;


        // if this box is resized, keep it inside its parent's area
        if(_keepInsideParent&&parentGraphic)
        {
            parentW = parentGraphic->getSize().x();
            parentH = parentGraphic->getSize().y();



            // applies to when pos is static and width and height are changing
            // check if the new width and height are too far out

/*
            if(newWidth + old.x() > parentW - INSIDE_PARENT_BUFFER)
                newWidth = parentW - INSIDE_PARENT_BUFFER - old.x();

            if(newHeight + old.y() > parentH - INSIDE_PARENT_BUFFER)
                newHeight = parentH - INSIDE_PARENT_BUFFER - old.y();
*/



            deltaWidth =   newWidth - _width ;
            deltaHeight =   newHeight - _height ;


            //adjustDrawingSize(  deltaWidth ,   deltaHeight);

            deltaWidth *= (-1);
            deltaHeight *= (-1);

            qreal newXpos, newYpos;

            if ( corner->getCorner() == 0 )
            {
                qDebug() << "corner 0 is grabbed!";
                newXpos = this->pos().x() + deltaWidth;
                newYpos = this->pos().y() + deltaHeight;


                if(newXpos <  0+INSIDE_PARENT_BUFFER)
                {
                    qDebug() << "newXPos";
                    deltaWidth = 0+INSIDE_PARENT_BUFFER - pos().x();
                    newXpos = 0+INSIDE_PARENT_BUFFER;
                }

                if(newYpos < 0+INSIDE_PARENT_BUFFER)
                {
                    qDebug() << "newYPos";
                    deltaHeight =0+INSIDE_PARENT_BUFFER - pos().y();
                    newYpos = 0+INSIDE_PARENT_BUFFER;
                }



                this->setPos(newXpos, newYpos);
            }
            else   if ( corner->getCorner() == 1 )
            {

                if(newWidth + old.x() > parentW - INSIDE_PARENT_BUFFER)
                    newWidth = parentW - INSIDE_PARENT_BUFFER - old.x();

                if(newHeight + old.y() > parentH - INSIDE_PARENT_BUFFER)
                    newHeight = parentH - INSIDE_PARENT_BUFFER - old.y();




                deltaWidth =   newWidth - _width ;
                deltaHeight =   newHeight - _height ;

                deltaWidth *= (-1);
                deltaHeight *= (-1);

                newYpos = this->pos().y() + deltaHeight;

                if(newYpos < 0+INSIDE_PARENT_BUFFER)
                {
                    deltaHeight = 0+INSIDE_PARENT_BUFFER - pos().y();
                    newYpos = 0+INSIDE_PARENT_BUFFER;
                }

                this->setPos(this->pos().x(), newYpos);
            }
            else if(corner->getCorner() == 2)
            {

                if(newWidth + old.x() > parentW - INSIDE_PARENT_BUFFER)
                    newWidth = parentW - INSIDE_PARENT_BUFFER - old.x();

                if(newHeight + old.y() > parentH - INSIDE_PARENT_BUFFER)
                    newHeight = parentH - INSIDE_PARENT_BUFFER - old.y();




                deltaWidth =   newWidth - _width ;
                deltaHeight =   newHeight - _height ;

                deltaWidth *= (-1);
                deltaHeight *= (-1);


            }
            else   if ( corner->getCorner() == 3 )
            {
                if(newHeight + old.y() > parentH - INSIDE_PARENT_BUFFER)
                    newHeight = parentH - INSIDE_PARENT_BUFFER - old.y();


                deltaHeight =   newHeight - _height ;
                deltaHeight *= (-1);

                newXpos = this->pos().x() + deltaWidth;
                if(newXpos <  0+INSIDE_PARENT_BUFFER)
                {
                    qDebug() << "newXPos";
                    deltaWidth = 0+INSIDE_PARENT_BUFFER - pos().x();
                    newXpos = 0+INSIDE_PARENT_BUFFER;
                }

                this->setPos(newXpos,this->pos().y());
            }

            //this->setPos(newXpos, newYpos);

            deltaWidth *= (-1);
            deltaHeight *= (-1);
            qDebug() << "dw: " << deltaWidth << "dh: " <<deltaHeight;
            adjustDrawingSize(  deltaWidth ,   deltaHeight);
        }
        else
        {



            deltaWidth =   newWidth - _width ;
            deltaHeight =   newHeight - _height ;


            //adjustDrawingSize(  deltaWidth ,   deltaHeight);

            deltaWidth *= (-1);
            deltaHeight *= (-1);

            qreal newXpos, newYpos;


            if ( corner->getCorner() == 0 )
            {
                 newXpos = this->pos().x() + deltaWidth;
                 newYpos = this->pos().y() + deltaHeight;




                this->setPos(newXpos, newYpos);
            }
            else   if ( corner->getCorner() == 1 )
            {
                newYpos = this->pos().y() + deltaHeight;



                this->setPos(this->pos().x(), newYpos);
            }
            else if(corner->getCorner() == 2)
            {
                //qDebug() << "corner 2";


            }
            else   if ( corner->getCorner() == 3 )
            {
                newXpos = this->pos().x() + deltaWidth;

                this->setPos(newXpos,this->pos().y());
            }

            deltaWidth *= (-1);
            deltaHeight *= (-1);
            adjustDrawingSize(  deltaWidth ,   deltaHeight);

        }










        setCornerPositions();

        QPointF nmts = pos();
        QRectF newBox = QRectF(nmts.x(), nmts.y(), newWidth, newHeight);
        //qDebug() << "Drag Start:\t\t"<<_dragStart<<"\nnewPos: "<<newPos<<"\ntest:\t\t"<<test;

        if(corner==NULL)
            qDebug() << "ERROR there was no hovered corner, should not be allowed";
        else
        {
            //qDebug() << "the corner used: " << corner;
            //qDebug() <<"old box: " <<oldBox << "new Box: " << newBox;
            emit stateBoxResized(oldBox, newBox, corner->getCorner());
            this->recenterText();
        }
        //this->update();
    }

    return true;// true => do not send event to watched - we are finished with this event
}


void SelectableTextBlock::handleParentStateGraphicResized(QRectF oldBox, QRectF newBox, int corner)
{

    /*
     *                         B
     *                         U
     *                         F
     *                         F
     *                         E
     *                         R
     *  ________________________
     * |  ____________________  |  BUFFER
     * | |                    | |
     * | |      TextBlock     | |
     * | |   Allowable Area   | |
     * | |____________________| |
     * |________________________|
     *
     */

   // qDebug() << "SelectableTextBlock::handleParentStateGraphicResized";

    qreal w = this->getSize().x();
    qreal h = this->getSize().y();
    qreal x = this->pos().x();
    qreal y = this->pos().y();

   // qDebug() << "x: " << x << "y: " << y;

    QRectF box(INSIDE_PARENT_BUFFER,INSIDE_PARENT_BUFFER,newBox.width()-(INSIDE_PARENT_BUFFER+INSIDE_PARENT_BUFFER),newBox.height()-(INSIDE_PARENT_BUFFER+INSIDE_PARENT_BUFFER));

    QPointF tl(x,y);
    QPointF tr(x+w,y);
    QPointF br(x+w,y+h);
    QPointF bl(x,y+h);


    bool tlloc = getGridLocation(box, tl)== C;
    bool trloc = getGridLocation(box, tr)== C;
    bool brloc = getGridLocation(box ,br)== C;
    bool blloc = getGridLocation(box, bl)== C;

    //qDebug() << "0: "<<tlloc <<"\t1: " << trloc<<"\t2: " << brloc<<"\t3: "<<blloc;


    // ensure the text block is inside of the box


    // first check if the text block needs to be resized in order to fit.
    bool changeSize = (w > box.width()) || (h > box.height());
    if(changeSize)
    {
        qreal newWidth = w;
        qreal newHeight = h;
        if(!trloc&&!brloc)
        {
            // the right wall of the textblock is out of bounds.

            newWidth = newBox.width()-INSIDE_PARENT_BUFFER - x;

        }
        if(!brloc&&!blloc)
        {
            // the bottom wall of the text block is out of bounds

            // adjust the height to fit the parent

            newHeight = newBox.height()-INSIDE_PARENT_BUFFER - y;

        }

        this->setSize(QPoint(newWidth,newHeight));

        w = newWidth;
        h = newHeight;

        tl = QPointF(x,y);
        tr = QPointF(x+newWidth,y);
        br = QPointF(x+newWidth,y+newHeight);
        bl = QPointF(x,y+newHeight);


        tlloc = getGridLocation(box, tl)== C;
        trloc = getGridLocation(box, tr)== C;
        brloc = getGridLocation(box ,br)== C;
        blloc = getGridLocation(box, bl)== C;
    }

    // then move the text block to be inside the box

    bool change = false;
    qreal newX = x;
    qreal newY = y;
    if(!trloc&&!brloc)
    {
        // the right wall of the textblock is out of bounds.
        // decrease the x position
        newX = box.width() - w + box.x();
        change = true;

    }
    if(!brloc&&!blloc)
    {
        // the bottom wall of the text block is out of bounds
        // decrease the y position
        newY = box.height() - h + box.y();
        change = true;
    }

    if(change)
        this->setPos(newX, newY);


}

void SelectableTextBlock::handleTextChanged()
{
    qDebug() << "SelectableTextBlock::handleTextChanged";
    //_textItem.setPlainText( _textBlockModel->getText() , true);
    this->setText(_textBlockModel->getText());
    if(_centerText)
    {
        recenterText();
    }
}

void SelectableTextBlock::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
#ifdef POP_UP_EDIT_MODE
    //_textItem.setVisible(false);
    event->accept();
    TextEditBox * editBox = new TextEditBox( _textBlockModel);
    connect (editBox, SIGNAL(saveButtonClicked(QString)), this, SLOT(handleEditBoxSavedText(QString)));
     QPointF myPos = this->mapToScene( this->pos() );
    SCDataModel::singleton()->getScene()->addItem( editBox);
    editBox->setPos(myPos);
    editBox->setZValue(std::numeric_limits<double>::max());    // temporary fix for getting the box to appear in front of everything in the scene
#endif

}
void SelectableTextBlock::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    qDebug() << "mouse pressed for stb";

//    if(_textItem.textInteractionFlags() == Qt::TextEditorInteraction)
//    {
//        _textItem.setTextInteraction(false,false);
//        qDebug() << "turning off interaction";
//    }
    SelectableBoxGraphic::mousePressEvent(event);
}

void SelectableTextBlock::keyPressEvent ( QKeyEvent * event )
{
    event->accept();
    if(event->key() == Qt::Key_F4)
    {
        TextEditBox * editBox = new TextEditBox( _textBlockModel);
        connect (editBox, SIGNAL(saveButtonClicked(QString)), this, SLOT(handleEditBoxSavedText(QString)));
        //QPointF myPos = this->mapToScene( this->pos() );
        QPointF myPos = pos();
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
    //_textItem.setPlainText( text, true );
    this->setText(text);
    //_textItem.setVisible(true);
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
//        qDebug() << "connecting size!";
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

/*
// for supporting moving the box across the scene
void SelectableTextBlock::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{


    graphicHasChanged();
}


// for supporting moving the box across the scene
void SelectableTextBlock::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    _dragStart = event->pos();
}


// for supporting moving the box across the scene

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
*/
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



    //qDebug()<<"SelectableTextBlock::handleAttributeChanged Size Attribute ("+  _textBlockModel->getText()+") setting size = (" +QString::number(pt.x())+","+QString::number(pt.y())+")";
}

void SelectableTextBlock::handleAttributeChanged(PositionAttribute* pos)
{
    QPointF ps = pos->asPointF();
    //qDebug()<<"SelectableTextBlock::handleAttributeChanged ("+  _textBlockModel->getText()+") setting position = (" +QString::number(ps.x())+","+QString::number(ps.y())+")";
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
    //qDebug()<<"selectabletextblock::handleattributechanged";
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
    /*
    if ( size.manhattanLength()< 100)
    {
        size.setX( _minSize.x());
        size.setY( _minSize.y());
    }*/

    if(size.y() < _minSize.y())
    {
       size.setY(_minSize.y());
    }

    // this will prevent resizes below what fits the text
   #if 0
    if(size.x() < _textItem.document()->size().width())
    {
        size.setX(_textItem.document()->size().width());
    }
   #endif

    if(size.x() < _minSize.x())
    {
        size.setX(_minSize.x());
    }
    SelectableBoxGraphic::setSize(size);

    recenterText();

    update();

}

int SelectableTextBlock::clamp(int value, int min, int max)
{
    if(value < min)
        return min;

    if(value > max)
        return max;

    return value;
}

qreal SelectableTextBlock::clampMin(qreal value, qreal min)
{
    if(value < min)
        return min;

    return value;
}


 /**
 * @brief SelectableTextBlock::recenterText
 *
 * Determines the new size of the text item and centers the text item in the text block's allowable area if applicable
 *
 */
void SelectableTextBlock::recenterText()
{
    // sets the width and height of the textItem based on the plainText
    _textItem.adjustSize();

    // check if the text width is less than the width availble for this text item
    qreal textWidth = _textItem.document()->size().width();
    qreal width = this->getUsableWidth();

    // if it will not fit, then resize the text item to the dimensions of the allowable area
    if(width < textWidth)
    {
        _textItem.resizeRectToTextBlock();
    }
    else    // center the text in the text block
    {
        // set the position to be in the middle of the text block
        qreal newX = this->getSize().x()/2.0 - _textItem.document()->size().width()/2.0;
        qreal newY = this->getSize().y()/2.0 - _textItem.document()->size().height()/2.0;
        _textItem.setPos(this->clampMin(newX,getTotalTextItemBufferX()), this->clampMin(newY,getTotalTextItemBufferY()));
    }
}




/*
 * The text block has a true rectangle, but we have some buffer distances so these helper functions get those dimenions easily
 *
 */

qreal SelectableTextBlock::getTotalTextItemBufferX()
{
    return CORNER_GRAB_X_BUFFER +BOX_DRAW_BUFFER+ TEXT_ITEM_X_BUFFER;
}

qreal SelectableTextBlock::getTotalTextItemBufferY()
{
    return CORNER_GRAB_Y_BUFFER +BOX_DRAW_BUFFER+ TEXT_ITEM_Y_BUFFER;
}

/**
 * @brief SelectableTextBlock::getUsableX
 * @return
 * helper function that returns the topleft of the usuable area for a text block's text item
 */
qreal SelectableTextBlock::getUsableX()
{
    return this->pos().x() + getTotalTextItemBufferX();
}

qreal SelectableTextBlock::getUsableY()
{
    return this->pos().y() + getTotalTextItemBufferY();
}

qreal SelectableTextBlock::getUsableWidth()
{
    return this->getSize().x() - 2*(getTotalTextItemBufferX());
}

qreal SelectableTextBlock::getUsableHeight()
{
    return this->getSize().y() - 2* (getTotalTextItemBufferY());
}

/**
 * @brief SelectableTextBlock::setWidth
 * @param w
 * universal set text width for a text block's QGraphicsTextItem
 */
void SelectableTextBlock::setTextWidth(qreal w)
{
    _textItem.setWidth(w);
}

/**
 * @brief SelectableTextBlock::setText
 * @param text
 *
 * overrides any text setting for the masked text item because we also want to check to center the text
 *
 */
void SelectableTextBlock::setText(QString text)
{
    _textItem.setPlainText(text);
//    if(_centerText)
//    {
//        recenterText();
//    }
}

void SelectableTextBlock::setTextHeight(qreal h)
{
    _textItem.setHeight(h);
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

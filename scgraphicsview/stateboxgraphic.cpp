#include "stateboxgraphic.h"

#include <QBrush>
#include <QLinearGradient>
#include <QDebug>
#include <QTextDocument>
#include "math.h"
#include "scstate.h"
#include <QTimer>

/**
  *  This box can be re-sized and it can be moved. For moving, we capture
  *  the mouse move events and move the box location accordingly.
  *
  *  To resize the box, we place small indicator boxes on the four corners that give the user
  *  a visual cue to grab with the mouse. The user then drags the corner to stretch
  *  or shrink the box.  The corner grabbers are implemented with the CornerGrabber class.
  *  The CornerGrabber class captures the mouse when the mouse is over the corner's area,
  *  but the StateBox object (which owns the corners) captures and processes the mouse
  *  events on behalf of the CornerGrabbers (because the owner wants to be
  *  resized, not the CornerGrabbers themselves). This is accomplished by installed a scene event filter
  *  on the CornerGrabber objects:
          _corners[0]->installSceneEventFilter(this);
  *
  *
  *
  */

StateBoxGraphic::StateBoxGraphic(QGraphicsObject * parent,SCState *stateModel):
        QGraphicsObject(parent),
        _title(),
        _outterborderColor(Qt::black),
        _outterborderPen(),
        _dragStart(0,0),
        _gridSpace(10),
        _width(250),
        _height(100),
        _cornerDragStart(0,0),
        _XcornerGrabBuffer(7),
        _YcornerGrabBuffer(7),
        _drawingWidth(  _width -   _XcornerGrabBuffer),
        _drawingHeight( _height -  _YcornerGrabBuffer),
        _drawingOrigenX( _XcornerGrabBuffer),
        _drawingOrigenY( _YcornerGrabBuffer),
        _useDefaultSize(true),
        _isHighlighted(false),
        _stateModel(stateModel)
{

    // this graphic representation of a state is linked to a state in the model

    connect (_stateModel, SIGNAL(changed()), this, SLOT(handleModelChanged()), Qt::QueuedConnection);

    // load up the initial values

    handleModelChanged();

    _corners[0] = NULL;
    _corners[1] = NULL;
    _corners[2] = NULL;
    _corners[3] = NULL;

    _outterborderPen.setWidth(2);
    _outterborderPen.setColor(_outterborderColor);

    _title.setPos(35,35);

    //  QTextDocument doc();
    // doc.allFormats();
    //   _text.setDocument()
    //Qt::AlignHCenter
    _title.setParentItem(this);

    this->setAcceptHoverEvents(true);

    // has a user re-sized the state graphic or has it already been
    // set in the scxml? if not, then set the default sizes


}


StateBoxGraphic::~StateBoxGraphic()
{
    delete _corners[0];
    delete _corners[1];
    delete _corners[2];
    delete _corners[3];
}


void StateBoxGraphic::handleModelChanged()
{

    StateAttributes::StateName * name = dynamic_cast<StateAttributes::StateName *> ( _stateModel->attributes.value("name"));
    StateAttributes::StateSize * size = dynamic_cast<StateAttributes::StateSize *> (  _stateModel->attributes.value("size"));
    StateAttributes::StatePosition * position =dynamic_cast<StateAttributes::StatePosition*> ( _stateModel->attributes.value("position"));

    _title.setPlainText(name->asString());

    QPoint pt = size->asPointF().toPoint();
    setSize(pt);

    QPointF ps = position->asPointF();
    setPos( ps  );

}

void StateBoxGraphic::queuedThisUpdate()
{
    this->update();
}

void StateBoxGraphic::setHighlighted(bool on)
{
    qDebug()<<"StateBoxGraphic::setHighlighted highlight = " + QString(on?"True":"False");
    _isHighlighted = on;
}

bool StateBoxGraphic::useDefaultSize()
{
    return _useDefaultSize;
}



void StateBoxGraphic::setUseDefaultSize(bool s)
{
    _useDefaultSize = s;
}


void StateBoxGraphic::setTitle(QString t)
{
    _title.setPlainText(t);
}


void StateBoxGraphic::increaseWidth(int inc)
{
    adjustDrawingSize(inc, 0);
}

QString StateBoxGraphic::getPositionAsString()
{
    QString v= QString::number( this->pos().x() ) + "," + QString::number(this->pos().y() );
    return v;
}

void  StateBoxGraphic::getSize(QPoint& size)
{
    size.setX(_width);
    size.setY(_height);
}

QString StateBoxGraphic::getSizeAsString()
{
    return ( QString::number( _width) + "," + QString::number(_height));
}


void StateBoxGraphic::setSize(QPoint size)
{
    _width = size.x();
    _height = size.y();

    _drawingWidth =  _width - _XcornerGrabBuffer;
    _drawingHeight=  _height - _YcornerGrabBuffer;

    _useDefaultSize = false;

    if ( _stateModel )
    {
        QPointF sizePointF =  QPointF ( size.x(),size.y() );
        _stateModel->setSize (sizePointF );
    }
}

/**
 *  To allow the user to grab the corners to re-size, we need to get a hover
 *  indication. But if the mouse pointer points to the left, then when the mouse
 *  tip is to the left but just outsize the box, we will not get the hover.
 *  So the solution is to tell the graphics scene the box is larger than
 *  what the painter actually paints in. This way when the user gets the mouse
 *  within a few pixels of what appears to be the edge of the box, we get
 *  the hover indication.

 *  So the cornerGrabBuffer is a few pixel wide buffer zone around the outside
 *  edge of the box.
 *
 */
void StateBoxGraphic::adjustDrawingSize(int x, int y)
{
    _width += x;
    _height += y;

    _drawingWidth =  _width - _XcornerGrabBuffer;
    _drawingHeight=  _height - _YcornerGrabBuffer;

}

/**
  * This scene event filter has been registered with all four corner grabber items.
  * When called, a pointer to the sending item is provided along with a generic
  * event.  A dynamic_cast is used to determine if the event type is one of the events
  * we are interrested in.
  */
bool StateBoxGraphic::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
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
        }
        break;

    case QEvent::GraphicsSceneMouseRelease:
        {
            corner->setMouseState(CornerGrabber::kMouseReleased);
            updateModel();
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

        qreal x = mevent->pos().x(), y = mevent->pos().y();

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

        int xMoved = corner->mouseDownX - x;
        int yMoved = corner->mouseDownY - y;

        int newWidth = _width + ( XaxisSign * xMoved);
        if ( newWidth < 40 ) newWidth  = 40;

        int newHeight = _height + (YaxisSign * yMoved) ;
        if ( newHeight < 40 ) newHeight = 40;

        int deltaWidth  =   newWidth - _width ;
        int deltaHeight =   newHeight - _height ;

        adjustDrawingSize(  deltaWidth ,   deltaHeight);

        deltaWidth *= (-1);
        deltaHeight *= (-1);

        if ( corner->getCorner() == 0 )
        {
            int newXpos = this->pos().x() + deltaWidth;
            int newYpos = this->pos().y() + deltaHeight;
            this->setPos(newXpos, newYpos);
        }
        else   if ( corner->getCorner() == 1 )
        {
            int newYpos = this->pos().y() + deltaHeight;
            this->setPos(this->pos().x(), newYpos);
        }
        else   if ( corner->getCorner() == 3 )
        {
            int newXpos = this->pos().x() + deltaWidth;
            this->setPos(newXpos,this->pos().y());
        }

        setCornerPositions();


        this->update();
    }

    return true;// true => do not send event to watched - we are finished with this event
}


void StateBoxGraphic::updateModel ()
{

    if ( _stateModel )
    {
        QPointF ps = this->pos();
        _stateModel->setPosition(ps);
        QPoint sz;
        this->getSize(sz);
        _stateModel->setSize(sz);
    }
}

// for supporting moving the box across the scene
void StateBoxGraphic::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    QPointF location = this->pos();
    location.setX( ( static_cast<int>(location.x()) / _gridSpace) * _gridSpace );
    location.setY( ( static_cast<int>(location.y()) / _gridSpace) * _gridSpace );
    this->setPos(location);


    updateModel();
}


// for supporting moving the box across the scene
void StateBoxGraphic::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    _dragStart = event->pos();
}


// for supporting moving the box across the scene
void StateBoxGraphic::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    QPointF newPos = event->pos() ;
    QPointF location = this->pos();
    location += (newPos - _dragStart);

    this->setPos(location);
}

// remove the corner grabbers

void StateBoxGraphic::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    _outterborderColor = Qt::black;

    _corners[0]->setParentItem(NULL);
    _corners[1]->setParentItem(NULL);
    _corners[2]->setParentItem(NULL);
    _corners[3]->setParentItem(NULL);

    delete _corners[0];
    _corners[0] = NULL;

    delete _corners[1];
    _corners[1] = NULL;

    delete _corners[2];
    _corners[2] = NULL;

    delete _corners[3];
    _corners[3] = NULL;

}


// create the corner grabbers

void StateBoxGraphic::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    _outterborderColor = Qt::red;

    _corners[0] = new CornerGrabber(this,0, true);
    _corners[1] = new CornerGrabber(this,1, true);
    _corners[2] = new CornerGrabber(this,2, true);
    _corners[3] = new CornerGrabber(this,3, true);


    _corners[0]->installSceneEventFilter(this);
    _corners[1]->installSceneEventFilter(this);
    _corners[2]->installSceneEventFilter(this);
    _corners[3]->installSceneEventFilter(this);

    setCornerPositions();

}

void StateBoxGraphic::setCornerPositions()
{
    _corners[0]->setPos(_drawingOrigenX, _drawingOrigenY);
    _corners[1]->setPos(_drawingWidth,  _drawingOrigenY);
    _corners[2]->setPos(_drawingWidth , _drawingHeight);
    _corners[3]->setPos(_drawingOrigenX, _drawingHeight);
}

QRectF StateBoxGraphic::boundingRect() const
{
    return QRectF(0,0,_width,_height);
}


// example of a drop shadow effect on a box, using QLinearGradient and two boxes

void StateBoxGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{


    /*
     The drop shadow effect will be created by drawing a filled, rounded corner rectangle with a gradient fill.
     Then on top of this will be drawn  filled, rounded corner rectangle, filled with a solid color, and offset such that the gradient filled
     box is only visible below for a few pixels on two edges.

     The total box size is _width by _height. So the top box will start at (0,0) and go to (_width-shadowThickness, _height-shadowThickness),
     while the under box will be offset, and start at (shadowThickness+0, shadowThickness+0) and go to  (_width, _height).
       */

    int shadowThickness = 3;

    QLinearGradient gradient;
    gradient.setStart(_drawingOrigenX,_drawingOrigenY);
    gradient.setFinalStop( _drawingWidth ,_drawingOrigenY);
    QColor grey1(150,150,150,125);// starting color of the gradient - can play with the starting color and ,point since its not visible anyway

    // grey2 is ending color of the gradient - this is what will show up as the shadow. the last parameter is the alpha blend, its set
    // to 125 allowing a mix of th color and and the background, making more realistic shadow effect.
    QColor grey2(225,225,225,125);


    gradient.setColorAt((qreal)0, grey1 );
    gradient.setColorAt((qreal)1, grey2 );

    QBrush brush(gradient);

    painter->setBrush( brush);

    // for the desired effect, no border will be drawn, and because a brush was set, the drawRoundRect will fill the box with the gradient brush.
    _outterborderPen.setStyle(Qt::NoPen);
    painter->setPen(_outterborderPen);

    QPointF topLeft (_drawingOrigenX,_drawingOrigenX);
    QPointF bottomRight ( _drawingWidth , _drawingHeight);

    QRectF rect (topLeft, bottomRight);

    painter->drawRoundRect(rect,10,10); // corner radius of 25 pixels

    // draw the top box, the visible one
    _outterborderPen.setStyle(Qt::SolidLine);

    //      qDebug()<<"StateBoxGraphic::paint highlight = " + QString(_isHighlighted?"True":"False");

    if ( _isHighlighted )
    {
        QPen pen (Qt::red);
        pen.setWidth(5);
        painter->setPen(pen);
    }
    else
    {
        painter->setPen(_outterborderPen);
    }

    QBrush brush2(QColor(187,250,185,255),Qt::SolidPattern);  // the box fill color
    //   QBrush brush2(QColor(255,255,255,255),Qt::SolidPattern);  // the box fill color

    painter->setBrush( brush2);

    QPointF topLeft2 (_drawingOrigenX, _drawingOrigenY);
    QPointF bottomRight2 ( _drawingWidth-shadowThickness, _drawingHeight-shadowThickness);

    QRectF rect2 (topLeft2, bottomRight2);

    painter->drawRoundRect(rect2,10,10);

    // keep the text centered and within the bounds of the box

    //int halfSize = _text.textWidth()/2;
    // _text.setPos( (_width/2) - halfSize, _text.pos().y()   );


}


void StateBoxGraphic::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void StateBoxGraphic::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

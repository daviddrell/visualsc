#include "togglebutton.h"
#include "stateboxgraphic.h"

#include <QApplication>
#include <QDebug>

#define WIDTH 16
#define HEIGHT 16

#define BUFFER_X 10
#define BUFFER_Y 10

ToggleButton::ToggleButton(StateBoxGraphic* parentGraphic, int corner):
    QGraphicsItem(parentGraphic),
    _on(false),
    _width(WIDTH),
    _height(HEIGHT),
    _corner(corner),
    _hovered(false)
{
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);

//    setPos(200,50);
    this->reposition();
    this->setAcceptHoverEvents(true);
    this->installSceneEventFilter(parentGraphic);

    _defaultGraphic = new QPixmap(":/SCGraphicsView/savefileicon_16_16.png");
    _toggleGraphic = new QPixmap(":/SCGraphicsView/savefileicon_depressed_16_16.png");



}

ToggleButton::~ToggleButton()
{
    if(_defaultGraphic)
        delete _defaultGraphic;

    if(_toggleGraphic)
        delete _toggleGraphic;
}

bool ToggleButton::isHovered()
{
    return _hovered;
}

QRectF ToggleButton::boundingRect() const
{
    QRect rect(
            (qreal) 0,
            (qreal) 0,
            _width,
            _height);
    return rect;
}

StateBoxGraphic* ToggleButton::parentAsStateBoxGraphic()
{
    return dynamic_cast<StateBoxGraphic*>(this->parentItem());
}

void ToggleButton::reposition()
{
//    QGraphicsItem* parent = this->parentItem();
    StateBoxGraphic* parent = this->parentAsStateBoxGraphic();
//    QRectF rect(parent->getBufferX(), parent->getBufferY(), parent->getSize().x()- 2*parent->getBufferX(), parent->getSize.y()-2*parent->getBufferY());

    QRectF rect = parent->getBufferedRect();

    switch(_corner)
    {
    case WallCorners::NORTHWEST:
        this->setPos(rect.topLeft() + QPointF(BUFFER_X, BUFFER_Y));
        break;
    case WallCorners::NORTHEAST:
        this->setPos(rect.topRight() + QPointF(-BUFFER_X, BUFFER_Y));
        break;
    case WallCorners::SOUTHEAST:
        this->setPos(rect.bottomRight()+ QPointF(-BUFFER_X, -BUFFER_Y));
        break;
    case WallCorners::SOUTHWEST:
        this->setPos(rect.bottomLeft()+ QPointF(BUFFER_X, -BUFFER_Y));
        break;
    }

    QPointF center(_width/2.0, _height/2.0);
    this->setPos(this->pos() - center);
}

void ToggleButton::handleStateSizeChanged(SizeAttribute * sa)
{
    //    QGraphicsItem* parent = this->parentItem();
        StateBoxGraphic* parent = this->parentAsStateBoxGraphic();
    //    QRectF rect(parent->getBufferX(), parent->getBufferY(), parent->getSize().x()- 2*parent->getBufferX(), parent->getSize.y()-2*parent->getBufferY());

        QRectF rect = parent->getBufferedRect();

        switch(_corner)
        {
        case WallCorners::NORTHWEST:
            this->setPos(rect.topLeft() + QPointF(BUFFER_X, BUFFER_Y));
            break;
        case WallCorners::NORTHEAST:
            this->setPos(rect.topRight() + QPointF(-BUFFER_X, BUFFER_Y));
            break;
        case WallCorners::SOUTHEAST:
            this->setPos(rect.bottomRight()+ QPointF(-BUFFER_X, -BUFFER_Y));
            break;
        case WallCorners::SOUTHWEST:
            this->setPos(rect.bottomLeft()+ QPointF(BUFFER_X, -BUFFER_Y));
            break;
        }

        QPointF center(_width/2.0, _height/2.0);
        this->setPos(this->pos() - center);
}

void ToggleButton::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    qDebug () << "ToggleButton::hoverEnter";
    //QApplication::setOverrideCursor(Qt::ArrowCursor);
//    update();
//    event->setAccepted(false);
    _hovered = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void ToggleButton::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    qDebug () << "ToggleButton::hoverLeave";

    //QApplication::restoreOverrideCursor();
//    update();
//    event->setAccepted(false);
    _hovered = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

//void ToggleButton::mousePressEvent ( QGraphicsSceneMouseEvent *  event)
//{
//    qDebug () << "ToggleButton::mousePress";
//    update();
//    event->setAccepted(false);
//}

//void ToggleButton::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
//{
//    qDebug () << "ToggleButton::mouseRelease";

//    _on = !(_on);
//    update();
//    emit clicked();
//    event->setAccepted(false);
//}

void ToggleButton::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug () << "ToggleButton::mouseDoubleClick";
    _on = !(_on);
    update();
    emit toggled();
    QGraphicsItem::mouseDoubleClickEvent(event);
  //  event->setAccepted(false);
}


void ToggleButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    QPixmap * graphic;
    if(_on)
    {
        graphic = _toggleGraphic;
    }
    else
    {
        graphic = _defaultGraphic;
    }

    painter->drawPixmap(0,0,_width,_height, *graphic);

}

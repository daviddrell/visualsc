#include "togglebutton.h"
#include "stateboxgraphic.h"

#include <QApplication>
#include <QDebug>

#define WIDTH 16
#define HEIGHT 16

#define BUFFER_X 12
#define BUFFER_Y 12

#define THICKNESS 1.67
//#define THICKNESS 1.61803398875

#define ARROW_BUFFER 5

ToggleButton::ToggleButton(StateBoxGraphic* parentGraphic, int corner):
    QGraphicsItem(parentGraphic),
    _on(false),
    _width(WIDTH),
    _height(HEIGHT),
    _corner(corner),
    _hovered(false)
{
    //this->setFlag(QGraphicsItem::ItemIsFocusable, true);

//    setPos(200,50);
    this->reposition();
    this->setAcceptHoverEvents(true);
    //this->installSceneEventFilter(parentGraphic);

//    QImage img = QImage(":/SCGraphicsView/Circled Chevron Down-26.png");
//    QImage img2 = QImage(":/SCGraphicsView/Circled Chevron Left -26.png");


//    _defaultGraphic->fromImage(img.scaled(WIDTH,HEIGHT,Qt::IgnoreAspectRatio, Qt::FastTransformation));

    _defaultGraphic = new QPixmap(":/SCGraphicsView/circledchevrondown.png");
    _toggleGraphic = new QPixmap(":/SCGraphicsView/Circled Chevron Left -26.png");

    _pen.setWidthF(THICKNESS);
    _pen.setStyle(Qt::SolidLine);
    _pen.setColor(Qt::black);
    _pen.setCapStyle(Qt::RoundCap);
    _pen.setJoinStyle(Qt::RoundJoin);
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

    QRectF rect = parent->getContentAreaRect();

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

        QRectF rect = parent->getContentAreaRect();

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

bool ToggleButton::isOn()
{
    return _on;
}

void ToggleButton::toggle()
{
    _on = !(_on);
}

void ToggleButton::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    qDebug () << "ToggleButton::hoverEnter";
    //QApplication::setOverrideCursor(Qt::ArrowCursor);
//    update();
//    event->setAccepted(false);
    _hovered = true;
//    QGraphicsItem::hoverEnterEvent(event);

}

void ToggleButton::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    qDebug () << "ToggleButton::hoverLeave";

    //QApplication::restoreOverrideCursor();
//    update();
//    event->setAccepted(false);
    _hovered = false;
//    QGraphicsItem::hoverLeaveEvent(event);

}


void ToggleButton::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "togglebutton::mousemoveevent";
    this->parentAsStateBoxGraphic()->mouseMoveEvent(event);
}

void ToggleButton::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    qDebug () << "ToggleButton::mouseRelease";
    QApplication::restoreOverrideCursor();
    this->parentAsStateBoxGraphic()->mouseReleaseEvent(event);

}
void ToggleButton::mousePressEvent ( QGraphicsSceneMouseEvent *  event)
{
    qDebug () << "ToggleButton::mousePress";
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    this->parentAsStateBoxGraphic()->mousePressEvent(event);
}



void ToggleButton::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug () << "ToggleButton::mouseDoubleClick";
    _on = !(_on);
    update();
    emit toggled();
}


void ToggleButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    painter->setPen(_pen);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

//    QPixmap * graphic;
//    if(_on)
//    {
//        graphic = _toggleGraphic;
//    }
//    else
//    {
//        graphic = _defaultGraphic;
//    }

//    painter->drawPixmap(0,0,_width,_height, *graphic);


    QRectF totalRect = this->boundingRect();
    QRectF br = totalRect;

    br.setTopLeft(br.topLeft() + QPointF(ARROW_BUFFER, ARROW_BUFFER));
    br.setBottomRight(br.bottomRight() - QPointF(ARROW_BUFFER, ARROW_BUFFER));

    qreal x = br.x();
    qreal y = br.y();

    qreal w = br.width();
    qreal h = br.height();

    QPointF north(x+w/2.0, y);
    QPointF east(x+w, y+w/2.0);
    QPointF south(x+w/2.0, y+h);
    QPointF west(x,y+h/2.0);

    // on means active, and in this case active means minimized <
    if(_on)
    {
        // draw a <
        painter->drawEllipse(totalRect);
        painter->drawLine(north,west);
        painter->drawLine(west,south);


    }
    else
    {
        // draw a V

        painter->drawEllipse(totalRect);
        painter->drawLine(east,south);
        painter->drawLine(south,west);
    }

}

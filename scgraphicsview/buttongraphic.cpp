#include "buttongraphic.h"
#include <QPixmap>
#include "scdatamodel.h"
#include <QPainter>

ButtonGraphic::ButtonGraphic():
        QGraphicsObject(NULL),
        _normalGraphic(NULL),
        _depressedGraphic(NULL)
{
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
}

ButtonGraphic::~ButtonGraphic()
{
    if ( _normalGraphic)    delete  _normalGraphic;
    if ( _hoveredGraphic)   delete  _hoveredGraphic;
    if ( _depressedGraphic) delete  _depressedGraphic;
}

void ButtonGraphic::hoverEnterEvent ( QGraphicsSceneHoverEvent *  )
{
    _mouseState = kHovered;
    update();
}

void ButtonGraphic::hoverLeaveEvent ( QGraphicsSceneHoverEvent *  )
{
    _mouseState = kNormal;
    update();
}

void ButtonGraphic::mousePressEvent ( QGraphicsSceneMouseEvent *  )
{
    _mouseState = kPressed;
    update();
}

void ButtonGraphic::mouseReleaseEvent( QGraphicsSceneMouseEvent *  )
{
    _mouseState = kNormal;
    update();
}


void ButtonGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPixmap * graphic = _normalGraphic;
    if ( _mouseState == kHovered)
    {
        graphic = _hoveredGraphic;
    }
    else if (_mouseState == kPressed )
    {
        graphic = _depressedGraphic;
    }

    if ( graphic)
        painter->drawPixmap(0,0,16,16,*graphic);
}

QRectF ButtonGraphic::boundingRect() const
{
    QRect rect(
            (qreal)this->pos().x(),
            (qreal) this->pos().y(),
            (qreal) 16.0,
            (qreal)16.0);
    return rect;
}

void ButtonGraphic::setNormalGraphic(QString source)
{
    _normalGraphic = new QPixmap(source);
    if (_normalGraphic == NULL )
    {
        QString err = "setNormalGraphic: error loading image[" + source +"] file";
        SCDataModel::singleton()->logError(err);
    }
}

void ButtonGraphic::setHoveredGraphic(QString source)
{
    _hoveredGraphic = new QPixmap(source);
    if (_hoveredGraphic == NULL )
    {
        QString err = "setHoveredGraphic: error loading image[" + source +"] file";
        SCDataModel::singleton()->logError(err);
    }
}

void ButtonGraphic::setDepressedGraphic(QString source)
{
    _depressedGraphic = new QPixmap(source);
    if (_depressedGraphic == NULL )
    {
        QString err = "setDepressedGraphic: error loading image[" + source +"] file";
        SCDataModel::singleton()->logError(err);
    }
}

#include "buttongraphic.h"
#include <QPixmap>
#include "scdatamodel.h"
#include <QPainter>
#include <QDebug>
#include <QApplication>

ButtonGraphic::ButtonGraphic(int x, int y):
        QGraphicsObject(NULL),
        _normalGraphic(NULL),
        _depressedGraphic(NULL)
{
    setPos(x,y);
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
    //qDebug() << "ButtonGraphic HoverEnterEvent";
    _mouseState = kHovered;
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    update();
}

void ButtonGraphic::hoverLeaveEvent ( QGraphicsSceneHoverEvent *  )
{
    _mouseState = kNormal;
    QApplication::restoreOverrideCursor();
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
    emit clicked();
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
    {
        painter->drawPixmap(0,0,16,16,*graphic);
    }
}

QRectF ButtonGraphic::boundingRect() const
{
    QRect rect(
            (qreal)0,
            (qreal) 0,
            (qreal) 16.0,
            (qreal)16.0);
    return rect;
}

void ButtonGraphic::setNormalGraphic(QString source)
{
    _normalGraphic = new QPixmap(source);
    if (_normalGraphic->isNull() )
    {
        QString err = "setNormalGraphic: error loading image[" + source +"] file";
        SCDataModel::singleton()->logError(err);
    }
}

void ButtonGraphic::setHoveredGraphic(QString source)
{
    _hoveredGraphic = new QPixmap(source);
    if (_hoveredGraphic->isNull() )
    {
        QString err = "setHoveredGraphic: error loading image[" + source +"] file";
        SCDataModel::singleton()->logError(err);
    }
}

void ButtonGraphic::setDepressedGraphic(QString source)
{
    _depressedGraphic = new QPixmap(source);
    if (_depressedGraphic->isNull()  )
    {
        QString err = "setDepressedGraphic: error loading image[" + source +"] file";
        SCDataModel::singleton()->logError(err);
    }
}

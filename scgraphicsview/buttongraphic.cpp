#include "buttongraphic.h"
#include <QPixmap>
#include "scdatamodel.h"
#include <QPainter>

ButtonGraphic::ButtonGraphic():
        QGraphicsObject(NULL),
        _normalGraphic(NULL),
        _depressedGraphic(NULL)
{

}

ButtonGraphic::~ButtonGraphic()
{
    if ( _normalGraphic)    delete  _normalGraphic;
    if ( _hoveredGraphic)   delete  _hoveredGraphic;
    if ( _depressedGraphic) delete  _depressedGraphic;

}

void ButtonGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(0,0,32,32,*_normalGraphic);
}

QRectF ButtonGraphic::boundingRect() const
{
    QRect rect(
            (qreal)this->pos().x(),
               (qreal) this->pos().y(),
               (qreal) 32.0,
               (qreal)32.0);
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

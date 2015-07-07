#include "mousecontroller.h"
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QDebug>

//#define MOUSE_DEBUG

MouseController::MouseController() :
    _xPos(0),
    _yPos(0)
{

}

void MouseController::mouseInput(QGraphicsSceneMouseEvent* me)
{
    _xPos = me->scenePos().x();
    _yPos = me->scenePos().y();
}

int MouseController::getX()
{
    return _xPos;
}

int MouseController::getY()
{
    return _yPos;
}

void MouseController::printPos()
{
#ifdef MOUSE_DEBUG
    qDebug() << "Mouse Pos (" << _xPos << ", " << _yPos<<")";
#endif
}

QPoint* MouseController::getPoint()
{
    QPoint* qp = new QPoint(_xPos,_yPos);
    return qp;
}

MouseController::~MouseController()
{

}


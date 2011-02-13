#include "sctransition.h"
#include "scstate.h"

#include <QDebug>

SCTransition::SCTransition(QObject * parent):
        QObject(parent),
        event(),
        cond(),
        target(),
        _attributes()
{
}

SCTransition::~SCTransition()
{
    qDebug()<< "SCTransition destroyed: " + QString::number((long)this);
}


void SCTransition::setTargetState(SCState * st)
{
    _targetState = st;
}

SCState * SCTransition::getTargetState()
{
    return _targetState;
}

void SCTransition::setAttributes(TransitionAttributes & ta)
{
     _attributes.cond = ta.cond;
     _attributes.event = ta.event;
     _attributes.path = ta.path;
     _attributes.target = ta.target;
     _attributes.type = ta.type;
}

void SCTransition::getAttributes(TransitionAttributes & ta)
{
    ta.cond   = _attributes.cond  ;
    ta.event  = _attributes.event ;
    ta.path   = _attributes.path;
    ta.target = _attributes.target ;
    ta.type   = _attributes.type ;
}

void SCTransition::handleLineSelected()
{
    emit selected();
}

void SCTransition::handleLineUnSelected()
{
    emit unselected();
}

void SCTransition::setPath(QList<QPointF>& path)
{
    _attributes.path =  path;

}

void SCTransition::getPath(QList<QPointF>& path)
{
    path = _attributes.path.pathPoints;
}

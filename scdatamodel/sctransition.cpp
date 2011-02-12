#include "sctransition.h"
#include "scstate.h"

#include <QDebug>

SCTransition::SCTransition(QObject * parent):
        QObject(parent),
        event(),
        cond(),
        target(),
        _attributes(),
        _path()
{
}

SCTransition::~SCTransition()
{
    qDebug()<< "SCTransition destroyed: " + QString::number((long)this);
}


void SCTransition::setAttributes(TransitionAttributes & ta)
{
     _attributes.cond = ta.cond;
     _attributes.event = ta.event;
     _attributes.path = ta.path;
     _attributes.target = ta.target;
     _attributes.type = ta.type;
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
    _path =  path;

}

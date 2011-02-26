#include "sctransition.h"
#include "scstate.h"

#include <QDebug>

SCTransition::SCTransition(QObject * parent):
        QObject(parent), attributes(parent, "transition.attributes")
{
    /*
     event
     cond
     target
     type  : "internal" "external"
     position
     path
     */

    TransitionAttributes::TransitionStringAttribute * target = new TransitionAttributes::TransitionStringAttribute (this, "target",QString());
    attributes.addItem(target);

    TransitionAttributes::TransitionStringAttribute * cond = new TransitionAttributes::TransitionStringAttribute (this, "cond",QString());
    attributes.addItem(cond);

    TransitionAttributes::TransitionStringAttribute * type = new TransitionAttributes::TransitionStringAttribute (this, "type",QString("internal"));
    attributes.addItem(type);

    TransitionAttributes::TransitionPositionAttribute * position = new TransitionAttributes::TransitionPositionAttribute (this, "position",QPointF(0,0));
    attributes.addItem(position);

    QList<QPointF> emptyPath;
    TransitionAttributes::TransitionPathAttribute * path = new TransitionAttributes::TransitionPathAttribute (this, QString("path"),emptyPath);
    attributes.addItem(path);


}

SCTransition::~SCTransition()
{
    qDebug()<< "SCTransition destroyed: " + QString::number((long)this);
}


QString SCTransition::getAttributeValue(QString key)
{
    IAttribute * attr = attributes.value(key);
    if ( attr )
    {
        return attr->asString();
    }
    else return QString();
}

void SCTransition::setAttributeValue(QString key, QString value)
{
    IAttribute * attr = attributes.value(key);
    if ( attr )
    {
        attr->setValue(value);
    }
}


void SCTransition::handleLineSelected()
{
    emit selected();
}

void SCTransition::handleLineUnSelected()
{
    emit unselected();
}

#if 0
void SCTransition::setPath(QList<QPointF>& path)
{
    TransitionPathAttribute * path = attributes.value("path");

    path->
        }

void SCTransition::getPath(QList<QPointF>& path)
{
    path = _attributes.path.pathPoints;
}
#endif


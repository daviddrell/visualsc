#ifndef TRANSISTIONATTRIBUTES_H
#define TRANSISTIONATTRIBUTES_H

#include <QList>
#include <QPointF>
#include <QString>


#include "scdatamodel_global.h"

//<path d="M 100 100 L 300 100 L 200 300 z"
//       fill="red" stroke="blue" stroke-width="3" />


class SCDATAMODELSHARED_EXPORT  TransitionPathAttributes
{
public:
    TransitionPathAttributes();
    TransitionPathAttributes(TransitionPathAttributes & pathAttributes );

    QList<QPointF>  path;
};


class TransitionAttributes
{
public:
    TransitionAttributes();

    /*
     event
     cond
     target
     type  : "internal" "external"
     */

    typedef  enum {Internal, External} TRANSISTION_TYPE;

    QString event;
    QString cond;
    QString target;
    TRANSISTION_TYPE type;

    TransitionPathAttributes path;// graphic support
};


#endif // TRANSISTIONATTRIBUTES_H

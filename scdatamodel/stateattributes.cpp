#include "stateattributes.h"

#include <QPoint>
#include <QString>

StateAttributes::StateAttributes() :
       name(),
       size(QPoint(0,0)),
       position(QPoint(0,0)),
       hasBeenSized(false)
{

}

StateAttributes::StateAttributes(StateAttributes & sa) :
           name(),
           size(QPoint(0,0)),
           position(QPoint(0,0)),
           hasBeenSized(false)
{
    size.set(  sa.size.asPoint());
    name.set (sa.name.asString());
    position.set( sa.position.asPoint());
}

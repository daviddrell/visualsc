#include "transitionattributes.h"


TransitionPathAttributes::TransitionPathAttributes():
        pathPoints()
{

}

// copy constructor
TransitionPathAttributes::TransitionPathAttributes( TransitionPathAttributes& pathAttributes ):
         pathPoints()
{
   pathPoints.append(pathAttributes.pathPoints);
}


TransitionAttributes::TransitionAttributes():
        event(),
        cond(),
        target(),
        type(Internal),
        path()
{

}

TransitionPathAttributes& TransitionPathAttributes::operator=( QList<QPointF>& src )
{

       pathPoints.append(src);

      return *this;
}



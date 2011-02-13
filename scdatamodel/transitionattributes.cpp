#include "transitionattributes.h"


TransitionPathAttributes::TransitionPathAttributes():
        path()
{

}

// copy constructor
TransitionPathAttributes::TransitionPathAttributes( TransitionPathAttributes& pathAttributes ):
         path()
{
   path.append(pathAttributes.path);
}


TransitionAttributes::TransitionAttributes():
        event(),
        cond(),
        target(),
        type(Internal),
        path()
{

}




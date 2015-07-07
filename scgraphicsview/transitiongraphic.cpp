#include "transitiongraphic.h"
#include "scstate.h"
#include <QDebug>

TransitionGraphic::TransitionGraphic(StateBoxGraphic *parentGraphic, StateBoxGraphic *targetGraphic, SCTransition * t, KeyController * keys) :
    QGraphicsObject(NULL),
    _transitionDM(t),
    _lineSegments(),
    _elbows(),
    _targetStateGraphic(targetGraphic),
    _keyController(keys)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, false);
    this->setParentItem(parentGraphic);
    //this->setParent(targetGraphic);

    TransitionAttributes::TransitionPathAttribute * p =
            dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (  t->attributes.value("path"));

    QList<QPointF> pointList = p->asQPointFList();




    if(pointList.count() < 2 && targetGraphic != NULL )
    {
        qDebug() << "1";
        // this path is new, anchor each end to the source and target states
        // find the orientation of the parent and target graphics to determine which sides to anchor to.

        int sourceSide=0;
        int targetSide=0;

        getClosestSides( & sourceSide, & targetSide);
        QPointF sourceAnchor = this->parentItem()->mapFromScene(this->parentItemAsStateBoxGraphic()->getSideCenterPointInSceneCoord(sourceSide));
        QPointF targetAnchor = this->parentItemAsStateBoxGraphic()->mapFromScene( _targetStateGraphic->getSideCenterPointInSceneCoord(targetSide));

        SelectableLineSegmentGraphic * segment  = new SelectableLineSegmentGraphic(sourceAnchor,sourceAnchor, targetAnchor, t,this,_keyController);
        segment->setParentItem(this);
        //segment->acceptHoverEvents();
        segment->installSceneEventFilter(this);

        _lineSegments.append(segment);
        connect(segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF))); // if the start point moved, then update
        connect(segment, SIGNAL(updateModel()), this, SLOT(updateModel()));     // update the transition's datamodel if the segment is changed

        segment->setTerminator(true);

    }
    else if ( pointList.count() == 2 )      // straight line from start to end state
    {
qDebug() << "2";


        SelectableLineSegmentGraphic * segment   = new SelectableLineSegmentGraphic(pointList[0], pointList[0], pointList[1], t, this,_keyController);
        segment->setParentItem(this);
       // segment->setParent(parentGraphic);
        _lineSegments.append(segment);

        // connect the parent state-graphic's slots to the new transition graphic's signals

        connect ( segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF)));
        connect(segment, SIGNAL(updateModel()), this, SLOT(updateModel()));

        segment->setTerminator(true);

    }
    else
    {
qDebug() << "3";
        SelectableLineSegmentGraphic * lastSegment=NULL;

        for (int i = 0 ; i < pointList.count() - 1 ; i ++)
        {
            SelectableLineSegmentGraphic * segment   = new SelectableLineSegmentGraphic( pointList[i+0], pointList[i+0], pointList[i+1], t, this,_keyController);
            segment->setParentItem(this);
            _lineSegments.append(segment);
            connect ( segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF)));
            connect(segment, SIGNAL(updateModel()), this, SLOT(updateModel()));

            lastSegment = segment;
        }

        lastSegment->setTerminator(true);

    }

     //printInfo();
}



/**
 * @brief TransitionGraphic::parentItemAsStateBoxGraphic
 * @return returns the parent QGraphicsItem of the transition graphic as a StateBoxGraphic
 */
StateBoxGraphic* TransitionGraphic::parentItemAsStateBoxGraphic()
{
    return static_cast<StateBoxGraphic*>(this->parentItem());
}

/*bool TransitionGraphic::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
{
    switch (event->type() )
    {
    case QEvent::GraphicsSceneMouseMove:
        {
        qDebug() << "Hello ";
        }
        break;
    default:
        // we dont care about the rest of the events
        return false;
        break;
    }

}*/

TransitionGraphic::~TransitionGraphic()
{
    for(int i=0;i < _lineSegments.count(); i++)
    {
        SelectableLineSegmentGraphic * ls =_lineSegments.at(i);
        delete ls;
    }
    _lineSegments.clear();
    for(int i = 0; i < _elbows.count() ; i++)
    {
        ElbowGrabber* elb = _elbows.at(i);
        delete elb;
    }
    _elbows.clear();
}



/**
 * @brief TransitionGraphic::printInfo
 * prints the transition graphic's parent and target states
 */
void TransitionGraphic::printInfo(void)
{
    qDebug() << "Transition graphic from: " << this->parentItemAsStateBoxGraphic()->objectName() << " to: " << _targetStateGraphic->objectName();
}


// create the corner grabbers



/**
 * @brief createNewElbow
 * create a new elbow on the currently hovered line segment.
 * a new line segment and elbow grabber object is created and added to the transition graphic
 */
void TransitionGraphic::createNewElbow()
{

}


void TransitionGraphic::handleKeyPressEvent(int key)
{
   // qDebug() << "Transition Graphic Key Press: " << key;
    if(key==Qt::Key_N)
    {
        qDebug() << "Creating New Elbow at pos: ";
        this->createNewElbow();
    }
}

/**
 * @brief TransitionGraphic::updateModel
 * Updates the transition graphic's data model if it exists
 * converts
 */
void TransitionGraphic::updateModel()
{

    if ( _transitionDM )                                 // check if the data model for this transition exists
    {
        if ( _lineSegments.count() < 1 ) return;        // no line segments

        QList<QPointF> path;

        path.append( _lineSegments[0]->getStart() );    // add the first point

        if (_lineSegments.count() == 1 )                // one line segment, only a start and end point
            path.append( _lineSegments[0]->getEnd() );

        else
        {
            for(int i = 1 ; i < _lineSegments.count(); i++)
            {
                path.append( _lineSegments[i]->getStart() );    // add each line segment's start
            }

            path.append( _lineSegments[_lineSegments.count()-1]->getEnd() );    // add the last line segment's end
        }


        TransitionAttributes::TransitionPathAttribute * pathAttr = dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (_transitionDM->attributes.value("path"));

        pathAttr->setValue(path);   // update the path values of _transitionDM, the data model object for this transition

    }
}


void TransitionGraphic::getClosestSides(int* sourceSide, int* targetSide)
{

    QPointF sourcePos = this->parentItemAsStateBoxGraphic()->getVisibleCenter();
    QPointF targetPos = _targetStateGraphic->getVisibleCenter();

    double deltaX = abs (sourcePos.x() - targetPos.x());
    double deltaY = abs (sourcePos.y() - targetPos.y());

    if ( deltaX > deltaY)
    {
        if ( sourcePos.x() > targetPos.x() )
        {
            *sourceSide = 3;
            *targetSide = 1;
        }
        else
        {
            *sourceSide = 1;
            *targetSide = 3;
        }
    }
    else
    {
        if ( sourcePos.y() > targetPos.y() )
        {
            *sourceSide = 0;
            *targetSide = 2;
        }
        else
        {
            *sourceSide = 2;
            *targetSide = 0;
        }
    }


}

void TransitionGraphic::paint (QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{

}

QRectF TransitionGraphic::boundingRect() const
{
    return QRectF();
}

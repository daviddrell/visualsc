#include "transitiongraphic.h"
#include "scstate.h"
#include <QDebug>

TransitionGraphic::TransitionGraphic(StateBoxGraphic *parentGraphic, StateBoxGraphic *targetGraphic, SCTransition * t, KeyController * keys) :
    QGraphicsObject(NULL),
    _transitionDM(t),
    _lineSegments(),
    _parentStateGraphic(parentGraphic),
    _targetStateGraphic(targetGraphic),
    _keys(keys)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, false);
//    this->setAcceptHoverEvents(true);
//    this->installSceneEventFilter(this);

    TransitionAttributes::TransitionPathAttribute * p =
            dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (  t->attributes.value("path"));

    QList<QPointF> pointList = p->asQPointFList();


    if (  pointList.count() < 2 && targetGraphic != NULL )
    {
        // this path is new, anchor each end to the source and target states
        // find the orientation of the parent and target graphics to determine which sides to anchor to.

        int sourceSide=0;
        int targetSide=0;

        getClosestSides( & sourceSide, & targetSide);
        QPointF sourceAnchor = _parentStateGraphic->mapFromScene( _parentStateGraphic->getSideCenterPointInSceneCoord(sourceSide));
        QPointF targetAnchor = _parentStateGraphic->mapFromScene( _targetStateGraphic->getSideCenterPointInSceneCoord(targetSide));

        SelectableLineSegmentGraphic * segment  = new SelectableLineSegmentGraphic(sourceAnchor,sourceAnchor, targetAnchor, t,this,_keys);
        segment->setParentItem(_parentStateGraphic);
        //segment->acceptHoverEvents();
        segment->installSceneEventFilter(this);

        _lineSegments.append(segment);
        connect(segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF))); // if the start point moved, then update
        connect(segment, SIGNAL(updateModel()), this, SLOT(updateModel()));

        segment->setTerminator(true);

    }
    else if ( pointList.count() == 2 )
    {

        SelectableLineSegmentGraphic * segment   = new SelectableLineSegmentGraphic(pointList[0], pointList[0], pointList[1], t, this,_keys);
        segment->setParentItem(parentGraphic);
        _lineSegments.append(segment);

        // connect the parent state-graphic's slots to the new transition graphic's signals

        connect ( segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF)));
        connect(segment, SIGNAL(updateModel()), this, SLOT(updateModel()));

        segment->setTerminator(true);

    }
    else
    {
        SelectableLineSegmentGraphic * lastSegment=NULL;

        for (int i = 0 ; i < pointList.count() - 1 ; i ++)
        {
            SelectableLineSegmentGraphic * segment   = new SelectableLineSegmentGraphic( pointList[i+0], pointList[i+0], pointList[i+1], t, this,_keys);
            segment->setParentItem(parentGraphic);
            _lineSegments.append(segment);
            connect ( segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF)));
            connect(segment, SIGNAL(updateModel()), this, SLOT(updateModel()));

            lastSegment = segment;
        }

        lastSegment->setTerminator(true);

    }
     //printInfo();
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
}

void TransitionGraphic::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{

    disconnect(_keys, SIGNAL(keyPressed(int)), this, SLOT(handleKeyPressEvent(int)));


}

/**
 * @brief TransitionGraphic::printInfo
 * prints the transition graphic's parent and target states
 */
void TransitionGraphic::printInfo(void)
{
    qDebug() << "Transition graphic from: " << _parentStateGraphic->objectName() << " to: " << _targetStateGraphic->objectName();
}


// create the corner grabbers

void TransitionGraphic::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{

    connect(_keys, SIGNAL(keyPressed(int)), this, SLOT(handleKeyPressEvent(int)));

}


void TransitionGraphic::handleKeyPressEvent(int key)
{
    qDebug() << "Transition Graphic Key Press: " << key;
}

void TransitionGraphic::updateModel ()
{

    if ( _transitionDM )
    {
        if ( _lineSegments.count() < 1 ) return;

        QList<QPointF> path;

        path.append( _lineSegments[0]->getStart() );

        if (_lineSegments.count() == 1 )
        {
            path.append( _lineSegments[0]->getEnd() );
        }
        else
        {
            for(int i = 1 ; i < _lineSegments.count(); i++)
            {
                path.append( _lineSegments[i]->getStart() );
            }

            path.append( _lineSegments[_lineSegments.count()-1]->getEnd() );
        }


        TransitionAttributes::TransitionPathAttribute * pathAttr = dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (_transitionDM->attributes.value("path"));

        pathAttr->setValue(path);

    }
}


void TransitionGraphic::getClosestSides(int* sourceSide, int* targetSide)
{

    QPointF sourcePos = _parentStateGraphic->getVisibleCenter();
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

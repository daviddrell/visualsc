#include "transitiongraphic.h"
#include "scstate.h"
#include <QDebug>

TransitionGraphic::TransitionGraphic(StateBoxGraphic *parentGraphic, StateBoxGraphic *targetGraphic, SCTransition * t) :
    QGraphicsObject(NULL),
    _transitionDM(t),
    _lineSegments(),
    _parentStateGraphic(parentGraphic),
    _targetStateGraphic(targetGraphic)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, false);

    TransitionAttributes::TransitionPathAttribute * p =
            dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (  t->attributes.value("path"));

    QList<QPointF> path = p->asQPointFList();

    if (  path.count() < 2 && targetGraphic != NULL )
    {
        // this path is new, anchor each end to the source and target states
        // find the orientation of the parent and target graphics to determine which sides to anchor to.

        int sourceSide=0;
        int targetSide=0;

        getClosestSides( & sourceSide, & targetSide);

        QPointF sourceAnchor = _parentStateGraphic->mapFromScene( _parentStateGraphic->getSideCenterPointInSceneCoord(sourceSide));
        QPointF targetAnchor = _parentStateGraphic->mapFromScene( _targetStateGraphic->getSideCenterPointInSceneCoord(targetSide));

        SelectableLineSegmentGraphic * segment  = new SelectableLineSegmentGraphic(sourceAnchor,sourceAnchor, targetAnchor, t);
        segment->setParentItem(_parentStateGraphic);
        _lineSegments.append(segment);
        connect ( segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF)));

        qDebug()<<"_parentStateGraphic->zValue() =" +QString::number(_parentStateGraphic->zValue());
        qDebug()<<"_targetStateGraphic->zValue() =" +QString::number(_targetStateGraphic->zValue());

        if ( _parentStateGraphic->zValue() > _targetStateGraphic->zValue())
            segment->setZValue( _parentStateGraphic->zValue() +1 );
        else
            segment->setZValue( _targetStateGraphic->zValue() +1 );

        segment->setTerminator(true);

    }
    else if ( path.count() >= 3 )
    {

        SelectableLineSegmentGraphic * segment   = new SelectableLineSegmentGraphic(path[0], path[0], path[1], t);
        segment->setParentItem(parentGraphic);
        _lineSegments.append(segment);

        // connect the parent state-graphic's slots to the new transition graphic's signals

        connect ( segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF)));

        for (int i = 2 ; i < path.count() - 1 ; i ++)
        {
            SelectableLineSegmentGraphic * segment   = new SelectableLineSegmentGraphic( path[i+0], path[i+0], path[i+1], t);
            segment->setParentItem(parentGraphic);
            _lineSegments.append(segment);
            connect ( segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF)));
        }
    }
}


TransitionGraphic::~TransitionGraphic()
{
    for(int i=0;i < _lineSegments.count(); i++)
    {
        SelectableLineSegmentGraphic * ls =_lineSegments.at(i);
        delete ls;
    }
    _lineSegments.clear();
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

void TransitionGraphic::handleTransitionLineStartMoved(QPointF)
{
}

void TransitionGraphic::paint (QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{

}

QRectF TransitionGraphic::boundingRect() const
{
    return QRectF();
}

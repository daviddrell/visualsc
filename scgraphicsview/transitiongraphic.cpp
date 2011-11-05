#include "transitiongraphic.h"

TransitionGraphic::TransitionGraphic(StateBoxGraphic *parentGraphic, SCTransition * t) :
    QGraphicsObject(NULL),
    _transitionDM(t),
    _lineSegments()
{
    this->setFlag(QGraphicsItem::ItemIsMovable, false);

    TransitionAttributes::TransitionPathAttribute * p =
            dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (  t->attributes.value("path"));

    QList<QPointF> path = p->asQPointFList();

    if (  path.count() < 2  )
    {
        QPointF position = QPointF(10,10);
        SelectableLineSegmentGraphic * segment  = new SelectableLineSegmentGraphic(position,position, QPointF(position.x()  , position.y() + 15 ), t);
        segment->setParentItem(this);
        _lineSegments.append(segment);
    }
    else if ( path.count() >= 3)
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

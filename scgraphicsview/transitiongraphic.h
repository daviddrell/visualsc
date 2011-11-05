#ifndef TRANSITIONGRAPHIC_H
#define TRANSITIONGRAPHIC_H

#include <QGraphicsObject>
#include "sctransition.h"
#include "selectablelinesegmentgraphic.h"
#include <QList>
#include "stateboxgraphic.h"

class TransitionGraphic : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit TransitionGraphic( StateBoxGraphic *parentState, SCTransition *t);
    ~TransitionGraphic();

signals:

public slots:
    void handleTransitionLineStartMoved(QPointF);


protected:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    QRectF boundingRect() const;

    SCTransition  * _transitionDM;
    QList<SelectableLineSegmentGraphic *>  _lineSegments;

};

#endif // TRANSITIONGRAPHIC_H

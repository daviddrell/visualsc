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
    explicit TransitionGraphic( StateBoxGraphic *parentState, StateBoxGraphic *targetState, SCTransition *t);
    ~TransitionGraphic();

signals:

public slots:
    void updateModel ();

protected:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    QRectF boundingRect() const;

private :

        // private methods
    void getClosestSides(int* sourceSide, int* targetSide);

    // private data
    SCTransition  * _transitionDM;
    QList<SelectableLineSegmentGraphic *>  _lineSegments;
    StateBoxGraphic *_parentStateGraphic;
    StateBoxGraphic *_targetStateGraphic;

};

#endif // TRANSITIONGRAPHIC_H

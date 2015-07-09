#ifndef LINESEGMENTGRAPHIC_H
#define LINESEGMENTGRAPHIC_H

#include "keycontroller.h"
#include "elbowgrabber.h"
#include "transitiongraphic.h"

#define CORNER_GRAB_BUFFER 5


class ElbowGrabber;
class TransitionGraphic;

class LineSegmentGraphic : public QObject, public QGraphicsPolygonItem
{
public:
    LineSegmentGraphic();
    LineSegmentGraphic(ElbowGrabber* startPoint, ElbowGrabber* endPoint, TransitionGraphic* parentGraphic, KeyController* keys);
    ~LineSegmentGraphic();
    void setTerminal(bool);

    void enclosePathInSceneCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  );
    void enclosePathInItemCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  );
    void enclosePathInCoordindates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  );
    void enclosePathInElbows();
    ElbowGrabber* getElbow(int i);
    TransitionGraphic* parentItemAsTransitionGraphic();
    void setElbowAt(int index, ElbowGrabber* elb);

private:
    KeyController*  _keyController;
    ElbowGrabber*   _elbows[2];
    QPolygonF       _selectRegion;
    bool            _isTerminal;

    QPen            _pen;

    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events

};

#endif // LINESEGMENTGRAPHIC_H

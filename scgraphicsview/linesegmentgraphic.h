#ifndef LINESEGMENTGRAPHIC_H
#define LINESEGMENTGRAPHIC_H

#include "keycontroller.h"
#include "elbowgrabber.h"
#include "transitiongraphic.h"

class ElbowGrabber;
class TransitionGraphic;

class LineSegmentGraphic : public QObject, public QGraphicsPolygonItem
{
public:
    LineSegmentGraphic();
    LineSegmentGraphic(ElbowGrabber* startPoint, ElbowGrabber* endPoint, TransitionGraphic* parentGraphic, KeyController* keys);
    ~LineSegmentGraphic();
    void setTerminal(bool);

private:
    KeyController*  _keyController;
    ElbowGrabber*   _elbows[2];
    QPolygonF       _selectRegion;
    bool            _isTerminal;

    QPen            _pen;


    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event

};

#endif // LINESEGMENTGRAPHIC_H

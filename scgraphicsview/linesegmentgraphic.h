#ifndef LINESEGMENTGRAPHIC_H
#define LINESEGMENTGRAPHIC_H

#include "keycontroller.h"
#include "elbowgrabber.h"
#include "transitiongraphic.h"

#define CORNER_GRAB_BUFFER 5
#define ELBOW_GUARD_BUFFER 15.0
#define LINE_HOVER_WIDTH  10.0


class ElbowGrabber;
class TransitionGraphic;

class LineSegmentGraphic : public QObject, public QGraphicsPolygonItem
{
public:
    int mouseDownX;
    int mouseDownY;
    QPointF leftElbowOffset;
    QPointF rightElbowOffset;
    LineSegmentGraphic();
    LineSegmentGraphic(ElbowGrabber* startPoint, ElbowGrabber* endPoint, TransitionGraphic* parentGraphic, KeyController* keys);
    ~LineSegmentGraphic();
  //  void setTerminal(bool);


    bool isAnchored();
    void setElbowOffsets();
    void setMouseState(int state);
    int getMouseState();
    void enclosePathInSceneCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  );
    void enclosePathInItemCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  );
    void enclosePathInCoordindates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  );
    void enclosePathInElbows();
    ElbowGrabber* getElbow(int i);
    TransitionGraphic* parentItemAsTransitionGraphic();
    void setElbowAt(int index, ElbowGrabber* elb);
    void forceHoverLeaveEvent();
    void forceHoverEnterEvent();
    void setHovered(bool);
    bool isHovered();

private:
    KeyController*  _keyController;
    ElbowGrabber*   _elbows[2];
    QPolygonF       _selectRegion;
    //bool            _isTerminal;

    QPen            _pen;
    bool            _isHovered;
    int _mouseState;

    QColor          _defaultColor;
    QColor          _hoverColor;



    /*
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events

    */
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event


};

#endif // LINESEGMENTGRAPHIC_H

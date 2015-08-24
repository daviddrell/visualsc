#ifndef ELBOWGRABBER_H
#define ELBOWGRABBER_H

#include <QGraphicsItem>
#include "transitiongraphic.h"
#include "LineSegmentGraphic.h"


class ArrowHeadGraphic;
class TransitionGraphic;
class LineSegmentGraphic;

class ElbowGrabber : public QObject ,  public QGraphicsItem
{
    Q_OBJECT


    enum Zone{FLAT, ANGLED, VERTICAL};
public:
    ElbowGrabber(TransitionGraphic* parentGraphic, KeyController* keys);
    ElbowGrabber(TransitionGraphic* parentGraphic, QPointF point, KeyController* keys);
    ~ElbowGrabber();
    void setAngle(int agle);///< set the rotation angle for corners which are drawn as arrowheads
    int  getCorner(); ///< allows the owner to find out which corner this is
    void setMouseState(int); ///< allows the owner to record the current mouse state
    int  getMouseState(); ///< allows the owner to get the current mouse state
    QPointF getCenterPoint();

    qreal mouseDownX;
    qreal mouseDownY;

    enum {kMouseReleased=0, kMouseDown, kMouseMoving}; ///< define the mouse states

    enum PaintStyle {kBox, kCrossHair, kArrowHead};
    void setPaintStyle(PaintStyle s);///< set box or cross-hairs

    void forceLineHoverLeaveEvent();

    void setXSnap(qreal x);
    void setYSnap(qreal y);
    void setPosSnap(QPointF);
    void setPosSnap(qreal x, qreal y);
//setPosNoSnap(QPointF);
//    void setPosNoSnap(qreal x, qreal y);
    void straightenLines();
    int getZone(qreal);
    int getZone(QPointF);
    qreal distance(ElbowGrabber* one, ElbowGrabber* two);
    qreal distanceX(QPointF, QPointF);
    qreal distanceY(QPointF, QPointF);

    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView

    void forceHoverEnterEvent();
    void forceHoverLeaveEvent();

    void setSegmentAt(int index,LineSegmentGraphic*);
    //void setSegmentTwo(LineSegmentGraphic*);

    LineSegmentGraphic* getSegment(int);

    bool isAnchor();
    void setAnchor(bool);

    bool isTerminal();
    void setTerminal(bool);

    TransitionGraphic* parentAsTransitionGraphic();


    void setSnappedSide(int);
    int getSnappedSide();

    void encloseLinePaths();

signals:
    void anchorMoved(QPointF newPos);   // signal the stateboxgraphic parent and target that the anchors need updating, newPos given in scene scope

private:


    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
   // virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
   // virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events

    // once the hover event handlers are implemented in this class,
    // the mouse events must also be implemented because of
    // some linkage issue - apparrently there is some connection
    // between the hover events and mouseMove/Press/Release
    // events which triggers a vtable issue
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
    virtual void mousePressEvent(QGraphicsSceneDragDropEvent *event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );

    void updateArrowHead();


    QColor _outterborderColor; ///< the hover event handlers will toggle this between red and black
    QPen _outterborderPen; ///< the pen is used to paint the red/black border

    qreal   _width;
    qreal   _height;

    int _snappedSide;// 0,1,2,3  - starting at x=0,y=0 and moving clockwise around the box

    int _mouseButtonState;
    bool _placedOnASquare;

    bool _anchor;

    PaintStyle _paintStyle;
    int        _arrowAngle;

    bool _terminator;

    ArrowHeadGraphic* _arrowHead;
    LineSegmentGraphic* _segments[2];

    KeyController*  _keyController;

    QColor _defaultColor;
    QColor  _hoverColor;
};

#endif // ELBOWGRABBER_H

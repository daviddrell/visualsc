#ifndef TRANSITIONGRAPHIC_H
#define TRANSITIONGRAPHIC_H

#include <QGraphicsObject>
#include "sctransition.h"
#include "selectablelinesegmentgraphic.h"
#include <QList>
#include "stateboxgraphic.h"
#include "keycontroller.h"
#include "elbowgrabber.h"
#include "mousecontroller.h"
#include "linesegmentgraphic.h"

#define TRANSITION_DEFAULT_COLOR Qt::blue
#define TRANSITION_HOVER_COLOR Qt::red
#define TRANSITION_DEFAULT_LINE_STYLE Qt::SolidLine
#define TRANSITION_HOVER_LINE_STYLE Qt::DashLine
#define TRANSITION_DEFAULT_WIDTH 2
#define TRANSITION_HOVER_WIDTH 3


class ElbowGrabber;
class LineSegmentGraphic;

class TransitionGraphic : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit TransitionGraphic( StateBoxGraphic *parentState, StateBoxGraphic *targetState, SCTransition *t, KeyController * keys, MouseController* mouse);
    void printInfo();
    StateBoxGraphic* parentItemAsStateBoxGraphic();
    ~TransitionGraphic();
    void setCurrentlyHoveredSegment(LineSegmentGraphic* seg);
    void clearCurrentlyHoveredSegment();
    bool isCurrentlyHovered();
    LineSegmentGraphic* getCurrentlyHoveredSegment();
    void setGrabbersVisible(bool visible);


signals:
  // void startEndMoved(QPointF newPos);
  // void anchorMoved(QPointF newPos);

public slots:
    void updateModel ();
    void handleParentStateGraphicMoved(QPointF);
    void handleTargetStateGraphicMoved(QPointF);

    void handleParentStateGraphicResized(QRectF,QRectF);
    //void handleParentStateGraphicResized(qreal, qreal, qreal, qreal);
    void handleTargetStateGraphicResized(QRectF, QRectF);



protected:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    QRectF boundingRect() const;
   // virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    //virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events


private :

        // private methods
    void updateElbow(QPointF newPos, ElbowGrabber* elbow);
    void updateLineSegments(ElbowGrabber* elbow);
    void getClosestSides(int* sourceSide, int* targetSide);
    void createCustomPath(QPointF mouseLocation, ElbowGrabber*);

    // private data
    SCTransition  * _transitionDM;  // data model for the transition object
    //QList<SelectableLineSegmentGraphic *>  _lineSegments;
    QList<LineSegmentGraphic *> _lineSegments;
    QList<ElbowGrabber *> _elbows;
    ElbowGrabber*   _anchors[2];
    //StateBoxGraphic *_parentStateGraphic;
    StateBoxGraphic *_targetStateGraphic;
    KeyController * _keyController;
    MouseController* _mouseController;
    LineSegmentGraphic* _hovered;

    virtual bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event ) ;


private slots:
    void handleKeyPressEvent(int key);
    void createNewElbow();

};

#endif // TRANSITIONGRAPHIC_H

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
#include "selectabletextblock.h"
#include <QObject>


//#define TRANSITION_DEFAULT_COLOR Qt::blue
//#define TRANSITION_HOVER_COLOR Qt::red
#define TRANSITION_DEFAULT_LINE_STYLE Qt::SolidLine
#define TRANSITION_HOVER_LINE_STYLE Qt::DashLine
#define TRANSITION_DEFAULT_WIDTH 2
#define TRANSITION_HOVER_WIDTH 3
#define ANCHOR_BUFFER_LENGTH 15

class ElbowGrabber;
class LineSegmentGraphic;

class TransitionGraphic : public QGraphicsObject
{
    Q_OBJECT


public:
    explicit TransitionGraphic( StateBoxGraphic *parentState, StateBoxGraphic *targetState, SCTransition *t, KeyController * keys, MouseController* mouse);
    void printInfo();
    StateBoxGraphic* parentItemAsStateBoxGraphic();
    StateBoxGraphic* getTargetStateGraphic();
    ~TransitionGraphic();

    void setTargetStateGraphic(StateBoxGraphic* sbg);
    void setTextPos(QPointF position);
    void setTextSize(qreal w, qreal h);
    void setCurrentlyHoveredLineSegment(LineSegmentGraphic* seg);
    void clearCurrentlyHoveredLineSegment();
    bool isLineSegmentCurrentlyHovered();
    LineSegmentGraphic* getCurrentlyHoveredLineSegment();

    void setCurrentlyHoveredElbow(ElbowGrabber*);
    void clearCurrentlyHoveredElbow();
    bool isElbowCurrentlyHovered();
    ElbowGrabber* getCurrentlyHoveredElbow();

    void setGrabbersVisible(bool visible);
    ElbowGrabber* getSourceAnchor();
    ElbowGrabber* getSinkAnchor();

    SCTransition* getTransitionModel();


signals:
  // void startEndMoved(QPointF newPos);
  // void anchorMoved(QPointF newPos);
    void clicked(SCTransition*);

public slots:
    void updateModel ();
    void handleParentStateGraphicMoved(QPointF);
    void handleTargetStateGraphicMoved(QPointF);

    void handleParentStateGraphicReleased();
    void handleTargetStateGraphicReleased();

    void handleParentStateGraphicResizedReleased();
    void handleTargetStateGraphicResizedReleased();

    void handleParentStateGraphicResized(QRectF,QRectF, int);
    void handleGrandParentStateGraphicResized(QRectF, QRectF, int);
    //void handleParentStateGraphicResized(qreal, qreal, qreal, qreal);
    void handleTargetStateGraphicResized(QRectF, QRectF, int);
    void handleGrandParentTargetStateGraphicResized(QRectF, QRectF, int);

    void handleAttributeChanged(TransitionStringAttribute* tsa);



protected:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    QRectF boundingRect() const;
   // virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    //virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events


private :

        // private methods

    void bindNeighborElbows(ElbowGrabber* elbow);
    void updateElbow(QPointF newPos, ElbowGrabber* elbow);
    void updateLineSegments(ElbowGrabber* elbow);
    void getClosestSides(int* sourceSide, int* targetSide);
    void createCustomPath(QPointF mouseLocation, ElbowGrabber*);

    // private data
    SCTransition  * _transitionModel;  // data model for the transition object
    //QList<SelectableLineSegmentGraphic *>  _lineSegments;
    QList<LineSegmentGraphic *> _lineSegments;
    QList<ElbowGrabber *> _elbows;
    ElbowGrabber*   _anchors[2];
    //StateBoxGraphic *_parentStateGraphic;
    StateBoxGraphic *_targetStateGraphic;
    KeyController * _keyController;
    MouseController* _mouseController;
    LineSegmentGraphic* _hoveredLine;
    ElbowGrabber* _hoveredElbow;

    //TextBox* _transitionTextBox;

    SelectableTextBlock* _eventText;    ///<  text to go in the title area.


    bool _hasMovedSinceCreatingElbow;
    bool _isCurrentlyDeleting;

    virtual bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event ) ;


private slots:
    void handleElbowKeyPressEvent(int key);
    void handleLineSegmentKeyPressEvent(int key);
    void createNewElbow();
    void deleteHoveredElbow();

};

#endif // TRANSITIONGRAPHIC_H

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

class ElbowGrabber;

class TransitionGraphic : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit TransitionGraphic( StateBoxGraphic *parentState, StateBoxGraphic *targetState, SCTransition *t, KeyController * keys, MouseController* mouse);
    void printInfo();
    StateBoxGraphic* parentItemAsStateBoxGraphic();
    ~TransitionGraphic();
    void setCurrentlyHoveredSegment(SelectableLineSegmentGraphic* seg);
    void clearCurrentlyHoveredSegment();
    bool isCurrentlyHovered();
    SelectableLineSegmentGraphic* getCurrentlyHoveredSegment();

signals:

public slots:
    void updateModel ();



protected:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    QRectF boundingRect() const;
   // virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    //virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events


private :

        // private methods
    void getClosestSides(int* sourceSide, int* targetSide);
    // private data
    SCTransition  * _transitionDM;  // data model for the transition object
    QList<SelectableLineSegmentGraphic *>  _lineSegments;
    QList<ElbowGrabber *> _elbows;
    //StateBoxGraphic *_parentStateGraphic;
    StateBoxGraphic *_targetStateGraphic;
    KeyController * _keyController;
    MouseController* _mouseController;
    SelectableLineSegmentGraphic* _hovered;

   // virtual bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event ) ;


private slots:
    void handleKeyPressEvent(int key);
    void createNewElbow();

};

#endif // TRANSITIONGRAPHIC_H

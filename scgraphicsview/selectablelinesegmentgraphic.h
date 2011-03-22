#ifndef SELECTABLELINESEGMENTGRAPHIC_H
#define SELECTABLELINESEGMENTGRAPHIC_H


#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include "cornergrabber.h"
#include <QGraphicsPolygonItem>
#include <QPainterPath>
class SCTransition;

class SelectableLineSegmentGraphic : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    SelectableLineSegmentGraphic(QPointF position, QPointF start, QPointF end, SCTransition* transition );

signals:
    void selected();
    void unselected();

private:

    // private data

    QPen            _pen;
    QPointF         _dragStart;
    QPointF         _cornerDragStart;
    int             _XcornerGrabBuffer;
    int             _YcornerGrabBuffer;

    QPointF         _lineEnd_0;
    QPointF         _lineEnd_1;
    bool            _cornerGrabbed;
    CornerGrabber*  _corners[2];// 0,1  - starting at x=0,y=0
    QPolygonF       _selectRegion;

     SCTransition*  _transitionModel;


    // private methods


    void updateModel ();

    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events

    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );///< allows the main object to be moved in the scene by capturing the mouse move events
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );

    virtual void mouseMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void mousePressEvent(QGraphicsSceneDragDropEvent *event);
    virtual bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event ) ;

    void setCornerPositions();

    void createCustomPath(QPointF mouseLocation, CornerGrabber*);

    void enclosePath(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  );


};


#endif // SELECTABLELINESEGMENTGRAPHIC_H

#ifndef SELECTABLEBOXGRAPHIC_H
#define SELECTABLEBOXGRAPHIC_H


#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include "cornergrabber.h"

class CornerGrabber;
class IAttribute;

class SelectableBoxGraphic :  public QGraphicsObject
{

public:

    SelectableBoxGraphic(QGraphicsObject*parent);
    ~SelectableBoxGraphic();

    QGraphicsTextItem _title;    ///< sample text to go in the title area.

    void setSize(QPoint size);
    void increaseWidth(int inc);
    QString getPositionAsString();
    QString getSizeAsString();
    void    getSize(QPoint& size);
    void setHighlighted(bool);

private:


    virtual void graphicHasChanged() = 0; ///< subclass must implement this, used to record user changes back to the data model

    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView
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
    void adjustDrawingSize(int x, int y);
    void updateModel();


    // private slots

private slots:


    void handleAttributeChanged(IAttribute *attr);



private:
    //private data

    QColor _outterborderColor; ///< the hover event handlers will toggle this between red and black
    QPen _outterborderPen; ///< the pen is used to paint the red/black border

    QPointF _dragStart;
    int     _gridSpace;
    qreal   _width;
    qreal   _height;

    QPointF _cornerDragStart;

    int _XcornerGrabBuffer;
    int _YcornerGrabBuffer;
    qreal   _drawingWidth;
    qreal   _drawingHeight;
    qreal   _drawingOrigenX;
    qreal   _drawingOrigenY;

    CornerGrabber*  _corners[4];// 0,1,2,3  - starting at x=0,y=0 and moving clockwise around the box

    bool    _isHighlighted;

};

#endif // SELECTABLEBOXGRAPHIC_H

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

/**
  * \class SelectableBoxGraphic
  *
  *  This QGraphicsObject is designed to the base class for all scene objects that will be contained in a box.
  *  For example the StateBoxGraphic and SelectableTextBlock both use this class.

  *  This box can be re-sized and it can be moved. For moving, we capture
  *  the mouse move events and move the box location accordingly.
  *
  *  To resize the box, we place small indicator boxes on the four corners that give the user
  *  a visual cue to grab with the mouse. The user then drags the corner to stretch
  *  or shrink the box.  The corner grabbers are implemented with the CornerGrabber class.
  *  The CornerGrabber class captures the mouse when the mouse is over the corner's area,
  *  but the StateBox object (which owns the corners) captures and processes the mouse
  *  events on behalf of the CornerGrabbers (because the owner wants to be
  *  resized, not the CornerGrabbers themselves). This is accomplished by installed a scene event filter
  *  on the CornerGrabber objects:
          _corners[0]->installSceneEventFilter(this);

  * \ingroup GraphicsView
  */

class SelectableBoxGraphic :  public QGraphicsObject
{

Q_OBJECT
public:

    SelectableBoxGraphic(QGraphicsObject*parent);
    ~SelectableBoxGraphic();

    virtual void setSize(QPointF size); ///< set the size of this object
    void    increaseWidth(int inc);
    QString getPositionAsString();
    QString getSizeAsString();
    QPointF getSize();

    void    setHighlighted(bool);

    enum BoxStyle {kSolidWithShadow, kSolidNoShadow, kTransparent};
    enum ShowBoxStyle { kAlways, kWhenSelected};
    enum DrawBoxLineStyle { kDrawSolid, kDrawDotted};

    void setShowBoxLineStyle(ShowBoxStyle s );///< show box always, or only when selected
    void setDrawBoxLineStyle( DrawBoxLineStyle s);///< if drawing box, draw solid or dotted line
    void setBoxStyle (BoxStyle s); ///< set box style
    void setHoverLineThickness(int t);///< set line thickness when hovered
    QRectF getUsableArea();///< returns a rect relative to the SelecableBoxGraphic that represents the inside margin, or usable area of the box.
    QPointF getVisibleCenter();///< returns a point which is appears to be the center of the box (i.e. does not include drop shadow), may not be the center of boundingRect()
    QPointF getSideCenterPointInSceneCoord(int side); ///< returns the center point on a given side, for anchoring a transition line in the middle of a side


    //bool eventFilter(QObject *, QEvent *);

signals:
    void stateBoxMoved(QPointF);    // this signal activates when the statebox graphic is moved or resized to alert the transition graphic to be updated as to remained anchored to its target state. Although the sink anchor is still a child of the source state graphic, it should behave like a child of its target state.
    void stateBoxResized(QPointF);

protected:

    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event

private:


    virtual void graphicHasChanged() = 0; ///< subclass must implement this, used to record user changes back to the data model

    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView

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


    void paintWithVisibleBox (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);



private:
    //private data

    QPen     _pen ;

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

    bool    _isHighlighted; ///< highlighting is used to indicate visually a group of objects that are related when one of the group is selected
    bool    _isHovered;

    ShowBoxStyle     _showBoxStyle;
    DrawBoxLineStyle _drawBoxLineStyle;
    BoxStyle         _boxStyle;
    int              _hoverLineThickness;


};

#endif // SELECTABLEBOXGRAPHIC_H

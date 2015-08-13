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

#define BOX_BUFFER 12
#define BOX_DEFAULT_PEN_WIDTH 2
#define BOX_HOVER_PEN_WIDTH 3
#define INSIDE_PARENT_BUFFER 7

#define BOX_DRAW_BUFFER 2

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

/* GridLocations
 *
 *  0   1   2
 *  7   8   3
 *  6   5   4
 *
 *  UL  U   UR
 *  L   C   R
 *  DL  D   DR
 *
 */
enum GridLocation{
    UL,
    U,
    UR,
    R,
    DR,
    D,
    DL,
    L,
    C
};


#define CORNER_GRAB_X_BUFFER 7
#define CORNER_GRAB_Y_BUFFER 7

class SelectableBoxGraphic :  public QGraphicsObject
{


Q_OBJECT
public:

    SelectableBoxGraphic(QGraphicsObject*parent);
    SelectableBoxGraphic(QGraphicsObject*parent, bool keepInsideParent);
    ~SelectableBoxGraphic();

    void setCornerPositions();
    SelectableBoxGraphic* parentAsSelectableBoxGraphic();
    void setPosAndUpdateAnchors(QPointF newPos);
    void setSizeAndUpdateAnchors(QPointF size);
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

    QRectF getUsableArea();///< returns a rect relative to the SelecableBoxGraphic that represents the inside margin, or usable area of the box.
    QPointF getVisibleCenter();///< returns a point which is appears to be the center of the box (i.e. does not include drop shadow), may not be the center of boundingRect()
    QPointF getSideCenterPointInSceneCoord(int side); ///< returns the center point on a given side, for anchoring a transition line in the middle of a side
    void getAllChildren(QList<SelectableBoxGraphic*> &stateList);

    void setPenWidth(qreal, qreal);
    void setMinSize(QPoint);
    //bool eventFilter(QObject *, QEvent *);
    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );///< allows the main object to be moved in the scene by capturing the mouse move events
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );
signals:
    void stateBoxMoved(QPointF);    // this signal activates when the statebox graphic is moved or resized to alert the transition graphic to be updated as to remained anchored to its target state. Although the sink anchor is still a child of the source state graphic, it should behave like a child of its target state.
    void stateBoxResized(QRectF oldBox, QRectF newBox, int corner);

    void stateBoxReleased();

protected:

    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event

    QPointF _dragStart;
    int     _gridSpace;
    QPoint _minSize;
    qreal _penHoverWidth;
    qreal _penWidth;
    int _XcornerGrabBuffer;
    int _YcornerGrabBuffer;
    qreal   _drawingWidth;
    qreal   _drawingHeight;
    qreal   _drawingOriginX;
    qreal   _drawingOriginY;
    bool    _isHovered;
    bool    _isHighlighted; ///< highlighting is used to indicate visually a group of objects that are related when one of the group is selected
    ShowBoxStyle     _showBoxStyle;
    DrawBoxLineStyle _drawBoxLineStyle;
    BoxStyle         _boxStyle;
    QPen     _pen ;

    int getGridLocation(QRectF, QPointF);
    bool isBetween(qreal start, qreal end, qreal point);
    void paintWithVisibleBox (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    bool _keepInsideParent;
    qreal   _width;
    qreal   _height;

    QPointF _cornerDragStart;



    CornerGrabber*  _corners[4];// 0,1,2,3  - starting at x=0,y=0 and moving clockwise around the box


    void adjustDrawingSize(int x, int y);

    int getHoveredCorner();
    virtual void mousePressEvent(QGraphicsSceneDragDropEvent *event);


private:


    virtual void graphicHasChanged() = 0; ///< subclass must implement this, used to record user changes back to the data model


    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events


    virtual void mouseMoveEvent(QGraphicsSceneDragDropEvent *event);

    virtual bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event ) ;





private:
    //private data









};

#endif // SELECTABLEBOXGRAPHIC_H

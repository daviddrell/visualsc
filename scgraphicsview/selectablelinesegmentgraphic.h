/*
    visualsc - Visual State Chart Editing for SCXML State Engines
    Copyright (C) 2011  David W. Drell (david@davidwdrell.net) and
    Contributing Authors:


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include "keycontroller.h"
//#include "transitiongraphic.h"

class TransitionGraphic;
class SCTransition;
class IAttribute;


/**
  * \class SelectableLineSegmentGraphic
  * \ingroup GraphicsView
  *
  * \brief this is intened as a prototype for the line path segment that will graphically form a transition path.
  *
  */

class SelectableLineSegmentGraphic : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    SelectableLineSegmentGraphic(QPointF position, QPointF start, QPointF end, SCTransition* transition, TransitionGraphic* parentGraphic, KeyController* keys );
    ~SelectableLineSegmentGraphic();
    void setStartEndPosition(QPointF position);
    void setTerminator(bool);///< if true, this segment's end point is a terminator and should draw an arrow head

    QPointF getStart();
    QPointF getEnd();

signals:
    void selected();
    void unselected();
    void startEndMoved(QPointF newPos);
    void updateModel ();

private:

    // private data

    QPen            _pen;
    QPointF         _dragStart;
    QPointF         _cornerDragStart;
    int             _XcornerGrabBuffer;
    int             _YcornerGrabBuffer;

    QPointF         _lineEnd_0; // visible starting end
    QPointF         _lineEnd_1; // visible finishing end
    bool            _cornerGrabbed;
    CornerGrabber*  _corners[2];// 0,1  - starting at x=0,y=0
    QPolygonF       _selectRegion;

    SCTransition*  _transitionModel;

    TransitionGraphic* _parentGraphic;

    bool           _isTerminal;///< is this the last line segment in a transition graphic?

    KeyController*  _keyController;  // key controller passed from transition graphic


    // private methods


   // void updateModel ();

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

    void enclosePathInSceneCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  );
    void enclosePathInItemCoordiates(qreal lineStartX,qreal lineStartY, qreal lineEndX, qreal lineEndY  );

private slots:

    void handleAttributeChanged(IAttribute *attr);

};






#endif // SELECTABLELINESEGMENTGRAPHIC_H

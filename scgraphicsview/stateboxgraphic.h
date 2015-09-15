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


#ifndef STATEBOX_H
#define STATEBOX_H


#include "selectableboxgraphic.h"
#include "selectabletextblock.h"
#include "fixedtextblock.h"
#include "togglebutton.h"



#include "sizeattribute.h"
class SCState;
class IAttribute;



/*

 ----- 0 -----
 |           |
 |           |
3             1
 |           |
 |           |
 ----- 2 -----

 */






enum WallFace{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

enum WallCorners{
    NORTHWEST,
    NORTHEAST,
    SOUTHEAST,
    SOUTHWEST,
    WallCornersLength
};

/**
  * \class StateBoxGraphic
    \ingroup GraphicsView
    \brief This visually prepresents the state.

  */

class StateBoxGraphic :  public SelectableBoxGraphic
{
    Q_OBJECT

public:
    StateBoxGraphic(QGraphicsObject*parent, SCState * state);
    ~StateBoxGraphic();

   // SelectableTextBlock* TextItem;    ///<  text to go in the title area.

    virtual void graphicHasChanged (); ///< implemented to receive updates from the SelectableBoxGraphic
    int getGridLocation(QPointF mts,QPointF point);
    void setGridSpace(int space);
    //void setTitle(QString t);
    void setSize(QPointF size);
    QString getPositionAsString();
    QString getSizeAsString();
    QPointF getSize();
    void setHighlighted(bool);
    SCState* getStateModel();
    StateBoxGraphic* getHighestLevelParentItemAsStateBoxGraphic();
    QPointF mapToHighestParent(QPointF pos);
    StateBoxGraphic* parentItemAsStateBoxGraphic();

    void getStates(QList<StateBoxGraphic*> &stateList);
    void getAllStates(QList<StateBoxGraphic*> &stateList);
    //void getAllTransitions(QList<TransitionGraphic*> &transList);

    FixedTextBlock* getStateTitle();
    void forceUpdate();

    void setName(QString);
    QString getStateName();

    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );///< allows the main object to be moved in the scene by capturing the mouse move events
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );

    qreal getBufferX();
    qreal getBufferY();
    QRectF getBufferedRect();



signals:
    void resizeState(StateBoxGraphic*);
    void clicked(SCState*);
    void minimized(SCState*);
    void expanded(SCState*);
    void nameChanged(QString);
    void exitActionChanged(QString);
    void entryActionChanged(QString);


protected:
    void paintWithVisibleBox (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

public slots:
    void updateElements();
    void handleTransitionLineStartMoved(QPointF newPos);
    void handleTransitionLineEndMoved(QPointF newPos);
    void handleIsParallelStateChanged(StateString*);
    void handleInitialStateChanged(StateString*);
    void handleFinalStateChanged(StateString*);
    void handleAttributeChanged(SizeAttribute*);
    void handleAttributeChanged(PositionAttribute* pos);
    void handleAttributeChanged(StateName*);
    void handleExitActionChanged(StateString*);
    void handleEntryActionChanged(StateString*);
    void handleTextBlockAttributeChanged(SizeAttribute*);
    void handleTextBlockAttributeChanged(PositionAttribute*);
    void handleTextBlockMoved(QPointF);
    void handleMinimize();

private:

//    bool mouseEventIgnore();

    virtual bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event ) ;
    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );// [virtual protected]

    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events

    QRectF* getBufferedBoxRect(qreal buffer,qreal offset);
    double xDistance(QPointF, QPointF);
    double yDistance(QPointF, QPointF);
    double distance(QPointF,QPointF);
    double distance(qreal,qreal,qreal,qreal);
    int getSmallest(double*, int);
    int findNearestWall(QRectF,QPointF);
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    bool isContained(QRectF);



    // private slots

private slots:

    //void handleAttributeChanged(void *attr);


private:
    int call;
    int returnClosestWallFace(QPointF mts,QPointF newPos);
    int returnClosestWallFace(qreal a, qreal b, qreal c, qreal d);
    //private data

    SCState * _stateModel;

    QPointF _diagLineStart;
    QPointF _diagLineEnd;
    bool    _diagLineDrawIt;



    QPointF _intersection;

    QColor  _finalStateColor;
    QColor  _initialStateColor;

    FixedTextBlock* _stateTitle;
    FixedTextBlock* _entryActionTitle;
    FixedTextBlock* _exitActionTitle;

    ToggleButton* _minimize;

    QPointF _restoreSize;
    QPointF _minimizeSize;

};

#endif // STATEBOX_H

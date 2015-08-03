/*
    visualsc - Visual State Chart Editing for SCXML State Engines
    Copyright (C) 2011  David W. Drell (david@davidwdrell.net) and`
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

#include "stateboxgraphic.h"

#include <QBrush>
#include <QLinearGradient>
#include <QDebug>
#include <QTextDocument>
#include "math.h"
#include "scstate.h"
#include <QTimer>
#include "textblock.h"
#include "selectablelinesegmentgraphic.h"


StateBoxGraphic::StateBoxGraphic(QGraphicsObject * parent,SCState *stateModel):
        SelectableBoxGraphic(parent),
        //TextItem(parent,stateModel->getIDTextBlock()),
        TextItem(new SelectableTextBlock(this, stateModel->getIDTextBlock())),
        _stateModel(stateModel),
        _diagLineStart(),
        _diagLineEnd(),
        _diagLineDrawIt(false),
        _intersection()
{


    // this graphic representation of a state is linked to a state in the model

    setShowBoxLineStyle ( SelectableBoxGraphic::kAlways );
    setDrawBoxLineStyle  ( SelectableBoxGraphic::kDrawSolid );
    //setBoxStyle(SelectableBoxGraphic::kSolidWithShadow );
    setBoxStyle(SelectableBoxGraphic::kSolidNoShadow);
    setMinSize(QPoint(60,60));
    setPenWidth(2.472135954999419,4);
    //TextItem.setPos(25,10);

    //TextItem.setParentItem(this);
    //PositionAttribute* position = dynamic_cast<PositionAttribute*> (_stateModel->attributes.value("position"));
    //qDebug() << "setting position: " << mapFromScene(position->asPointF());
    //qDebug() << "setting position: " << mapToScene(position->asPointF());
    //qDebug() << "setting position: " << mapToParent(position->asPointF());
    //TextItem.setPos(position->asPointF());

    //connect(_stateModel->getIDTextBlock()->getSizeAttr(), SIGNAL(changed(SizeAttribute*)), this, SLOT(handleTextBlockAttributeChanged(SizeAttribute*)));

    //connect(_stateModel->getIDTextBlock()->getPosAttr(), SIGNAL(changed(PositionAttribute*)), this, SLOT(handleTextBlockAttributeChanged(PositionAttribute*)));

    //connect(TextItem, SIGNAL(textBlockMoved(QPointF)), this, SLOT(handleTextBlockMoved(QPointF)));
}


StateBoxGraphic::~StateBoxGraphic()
{
    //qDebug () << "stateboxgraphic deconstructor:";
}

/**
 * @brief StateBoxGraphic::handleTextBlockMoved
 * @param diff
 *
 * SLOT
 *
 * connect in StateBoxGraphic constructor
 *
 * this slot is connected to a signal in mouse move event for this state's text block
 * and will limit the text block to only be able to be moved in the state's area
 *
 *
 *
 * NOT CURRENTLY IN USE
 * moved functionality to selectabletextblock mouse move eevent
 */
void StateBoxGraphic::handleTextBlockMoved(QPointF diff)
{
    /*
    qDebug() << "Sbg::htbm diff " << diff << "\torigin: "<<TextItem->pos();

    QPointF tl = TextItem->pos();

    if(tl.x() < 0)
        diff.setX(0);

    if(tl.y() < 0)
        diff.setY(0);


    TextItem->setPos(diff);
    TextItem->graphicHasChanged();
    */
}

void StateBoxGraphic::handleTextBlockAttributeChanged(SizeAttribute* size)
{
    qDebug() << "SBG::handleTextBLockATtribtueChanged SIZE";
    QPointF sz = size->asPointF();
    QPointF pos = _stateModel->getIDTextBlock()->getPosAttr()->asPointF();

    QRectF rect = QRectF(pos.x(), pos.y(), sz.x(), sz.y());
    bool inside = isContained(rect);

    qDebug() << "Is the text block inside its sbg? " <<inside;
}

void StateBoxGraphic::handleTextBlockAttributeChanged(PositionAttribute* position)
{
    qDebug() << "SBG::handleTextBLockATtribtueChanged POS";
    QPointF sz = _stateModel->getIDTextBlock()->getSizeAttr()->asPointF();
    QPointF pos = position->asPointF();

    QRectF rect = QRectF(pos.x(), pos.y(), sz.x(), sz.y());
    bool inside = isContained(rect);

    qDebug() << "Is the text block inside its sbg? " <<inside;
}

bool StateBoxGraphic::isContained(QRectF rect)
{
    bool ret;
    QPointF topLeft = rect.topLeft();
    QPointF topRight = rect.topRight();
    QPointF bottomRight = rect.bottomRight();
    QPointF bottomLeft = rect.bottomLeft();

    QPointF origin = QPointF(0,0);

    bool tlInside = getGridLocation(origin, topLeft)     == C;
    bool trInside = getGridLocation(origin, topRight)    == C;
    bool brInside = getGridLocation(origin, bottomRight) == C;
    bool blInside = getGridLocation(origin, bottomLeft)  == C;

    ret = tlInside && trInside && brInside && blInside;
    return ret;
}


void StateBoxGraphic::handleIsParallelStateChanged(IAttribute*attr)
{
    if ( attr->asString() == "true")
    {
        setDrawBoxLineStyle(kDrawDotted);
    }
    else
    {
        setDrawBoxLineStyle(kDrawSolid);
    }
}

SCState* StateBoxGraphic::getStateModel()
{
    return _stateModel;
}

double StateBoxGraphic::yDistance(QPointF q1, QPointF q2)
{
    return fabs(q1.y()-q2.y());
}

double StateBoxGraphic::xDistance(QPointF q1, QPointF q2)
{
    return fabs(q1.x()-q2.x());
}

double StateBoxGraphic::distance(QPointF q1, QPointF q2)
{
    return distance(q1.x(),q1.y(),q2.x(),q2.y());
}

double StateBoxGraphic::distance(qreal x1, qreal y1, qreal x2, qreal y2)
{
    qreal a = x1-x2;
    qreal b = y1-y2;
    return sqrt((a*a)+(b*b));
}

int StateBoxGraphic::getSmallest(double* ar, int len)
{
    int min = std::numeric_limits<int>::max();
    int index = 0;
    for(int i = 0; i < len; i++)
    {
        if(ar[i] < min)
        {
            min = ar[i];
            index = i;
        }
    }
    return index;
}

/**
 * @brief StateBoxGraphic::getBufferedBoxRect
 * @param bufferLength
 * @return
 *
 * Returns a modified QRectF of the statebox that has a reduced width and height to match the box shown and ignore the stylistic shadow of the state box
 *
 */
QRectF* StateBoxGraphic::getBufferedBoxRect(qreal bufferLength, qreal offset)
{
    QRectF box = getUsableArea();
    QRectF* ret = new QRectF(box);
    ret->setWidth(ret->width()-bufferLength);
    ret->setHeight(ret->height()-bufferLength);
    ret->setX(ret->x()-offset);
    ret->setY(ret->y()-offset);
    return ret;
}

/**
 * @brief StateBoxGraphic::findNearestWall
 * @param box
 * @param point
 * @return 0 for top wall, 1 for east wall, 2 for south wall, and 3 for west wall
 *
 * Determine which wall of a rectangle a point is closest to and returns 0-3
 */
int StateBoxGraphic::findNearestWall(QRectF box, QPointF point)
{
    qreal w = box.width();
    qreal h = box.height();
   // qreal wmid = box.width()/2.0;
   // qreal hmid = box.height()/2.0;

    QPointF NW(box.x(),box.y());
    QPointF NE(box.x()+w,box.y());
    QPointF SE(box.x()+w,box.y()+h);
    QPointF SW(box.x(),box.y()+h);

    /*
    QPointF N(box.x()+wmid,box.y());
    QPointF E(box.x()+w,box.y()+hmid);
    QPointF S(box.x() + wmid, box.y()+h);
    QPointF W(box.x(),box.y()+hmid);
    */


    double distToCorners[4];        // distance from given point to corner points
    distToCorners[NORTHWEST] = distance(point, NW);
    distToCorners[NORTHEAST] = distance(point, NE);
    distToCorners[SOUTHEAST] = distance(point, SE);
    distToCorners[SOUTHWEST] = distance(point, SW);

    double x,y;

    // find the corner this point is closest to. Once we know this, we select the closer of the two walls that make up that corner.
    switch(getSmallest(distToCorners, WallCornersLength))
    {
    case NORTHWEST: // the closest corner is the northwest corner. check the north and west walls.
        x = xDistance(point,NW);
        y = yDistance(point,NW);

        if(x < y)
            return WEST;
        else
            return NORTH;

    case NORTHEAST:
        x = xDistance(point, NE);
        y = yDistance(point, NE);

        if(x < y)
            return EAST;
        else
            return NORTH;

    case SOUTHEAST:

        x = xDistance(point, SE);
        y = yDistance(point, SE);

        if(x < y)
            return EAST;
        else
            return SOUTH;

    case SOUTHWEST:
        x = xDistance(point, SW);
        y = yDistance(point, SW);

        if(x < y)
            return WEST;
        else
            return SOUTH;

    default:
        break;

    }
}

/**
 * @brief StateBoxGraphic::mapToHighestParent
 * @param pos
 * @return
 *
 * maps a position in this item's coordinates to the scene, using each of its parents coordinates to determine where it is in the scene
 *
 */
QPointF StateBoxGraphic::mapToHighestParent(QPointF pos)
{
    StateBoxGraphic* parent = this->parentItemAsStateBoxGraphic();
    QPointF mthp = pos;
    while(parent)
    {
        //qDebug() <<"pos is " << mthp;
        mthp += parent->pos();
        parent = parent->parentItemAsStateBoxGraphic();

    }
    //qDebug() << "mthp is: " << mthp;
    return mthp;
}

/**
 * @brief StateBoxGraphic::handleTransitionLineStartMoved
 * @param newPos
 * Connected to a source elbow's signal anchorMoved in transition graphics.
 * will readjust the position of the source point of the transition arrow to be anchored to its starting state
 * newPos is in the scene
 *
 */
void StateBoxGraphic::handleTransitionLineStartMoved(QPointF newPos)
{
    // this method keeps the starting position of a line snapped to the outter edge of the box
    ElbowGrabber* elbow = dynamic_cast<ElbowGrabber*> (QObject::sender());
    QPointF mts;

    // first determine if this statebox has any parents
    StateBoxGraphic* gp = this->getHighestLevelParentItemAsStateBoxGraphic();
    if(gp)
    {
        // if this state has a parent, then use its parents to map it to the scene
        mts = this->mapToHighestParent(pos());
    }
    else
    {
        // otherwise this is the highest level parent, and its pos is already in the scene;
        mts = pos();
    }

    //qDebug() << "mts: " << mts;
    qreal x = mts.x();
    qreal y = mts.y();

    qreal w = this->getSize().x();
    qreal h = this->getSize().y();
    qreal nx, ny;

    int wall;

    /* the grid location of a statebox is as follows:
     *         |   |
     ________0_|_1_|_2________
     ________7_|_8_|_3________
             6 | 5 | 4
               |   |
     *
     * Where 8 represents the area of the box and the lines continue infinitely
     * and in clockwise order this enum is: UL U UR R DR D DL L C
     *
     * we snap the anchor where the mouse pointer lies in the grid
     *
     *
     *
     */
    int grid = getGridLocation(mts,newPos);
    qDebug() << "box position: " << mts <<" newPos: "<<newPos<<"the elbow is in grid: " << grid;
    switch (grid) {
    case UL:
        nx = x;
        ny = y;
        elbow->setSnappedSide(NORTH);   // snapped side relates to how to update the elbow for resizing
        break;
    case U:
        nx = newPos.x();
        ny = y;
        elbow->setSnappedSide(NORTH);
        break;

    case UR:
        nx = x+w;
        ny = y;
        elbow->setSnappedSide(NORTH);
        break;
    case R:
        nx = x+w;
        ny = newPos.y();
        elbow->setSnappedSide(EAST);
        break;
    case DR:
        nx = x+w;
        ny = y+h;
        elbow->setSnappedSide(SOUTH);
        break;
    case D:
        nx = newPos.x();
        ny = y+h;
        elbow->setSnappedSide(SOUTH);
        break;
    case DL:
        nx = x;
        ny = y+h;
        elbow->setSnappedSide(SOUTH);
        break;
    case L:
        nx = x;
        ny = newPos.y();
        elbow->setSnappedSide(WEST);
        break;
    case C:
        wall = returnClosestWallFace(mts,newPos);
        elbow->setSnappedSide(wall);
        switch(wall)
        {
        case NORTH:
            nx = newPos.x();
            ny = y;
            break;
        case EAST:
            nx = x+w;
            ny = newPos.y();
            break;

        case SOUTH:
            nx = newPos.x();
            ny = y+h;
            break;
        case WEST:
            nx = x;
            ny = newPos.y();
            break;
        }

        break;
    default:
        break;
    }


    _intersection.setX(nx);
    _intersection.setY(ny);


    if(gp)
    {
        elbow->setPos(gp->mapToItem(this,gp->mapFromScene(_intersection)));
    }
    else
    {
        elbow->setPos(mapFromScene(_intersection));
    }
}



/**
 * @brief StateBoxGraphic::getGridLocation
 * @param mts
 * @param point
 * @return
 *
 *
 *  returns the grid location of a point on this box with scene based coordinates
 *  the entire state box will take up grid location 8
 *
 *   the grid location of a statebox is as follows:
               |   |
     ________0_|_1_|_2________
     ________7_|_8_|_3________
             6 | 5 | 4
               |   |

 *
 *  UL  U   UR
 *  L   C   R
 *  DL  D   DR
 *
 *
 *
 *
 */
int StateBoxGraphic::getGridLocation(QPointF mts,QPointF point)
{
    qreal px = point.x();
    qreal py = point.y();

    //QPointF mts = (pos());

    qreal x = mts.x();
    qreal y = mts.y();

    qreal w = this->getSize().x();
    qreal h = this->getSize().y();

    // above the box
    if(py < y )
    {
        // directly above
        if(isBetween(x,x+w,px))
            return U;

        else if(px<x)
            return UL;

        else
            return UR;
    }

    // below
    else if( py > (y+h) )
    {
        //directly below
        if(isBetween(x,x+w,px))
            return D;
        else if(px < x)
            return DL;
        else
            return DR;
    }

    // directly right of the box
    else if( px > (x+w) && isBetween(y,y+h,py))
    {
        return R;
    }

    // directly left of the box
    else if( px < x && isBetween(y,y+h,py))
    {
        return L;
    }

    else    // otherwise it is in the center of the box
        return C;

}


void StateBoxGraphic::handleAttributeChanged(SizeAttribute* size)
{
    //qDebug() << "StateBoxGraphic::handleAttributeChanged(SizeAttribute*)";
    QPoint sz = size->asPointF().toPoint();
    setSizeAndUpdateAnchors(sz);
}

void StateBoxGraphic::handleAttributeChanged(PositionAttribute* pos)
{
    //qDebug() << "StateBoxGraphic::handleAttributeChanged position attr";
    setPosAndUpdateAnchors(pos->asPointF());
}


/**
 * @brief StateBoxGraphic::returnClosestWallFace
 * @param newPos
 * @return
 *
 * returns the wall closest to the point given, used when the point is inside the rectangle of the state box and we want to know which wall its closest to to set its anchor
 *
 */
int StateBoxGraphic::returnClosestWallFace(QPointF mts,QPointF newPos)
{
    //QPointF mts = (pos());

    qreal x = mts.x();
    qreal y = mts.y();

    qreal w = this->getSize().x();
    qreal h = this->getSize().y();

    qreal dn, dr, dd, dl;
    dn = fabs(y - newPos.y());
    dr = fabs(x+w-newPos.x());
    dl = fabs(x - newPos.x());
    dd = fabs(y+h-newPos.y());

    if(dn <= dr && dn <= dd && dn <= dl)
        return NORTH;

    else if(dr <= dn && dr <= dd && dr <= dl)
        return EAST;

    else if(dd <= dn && dd <= dr && dd <= dl)
        return SOUTH;

    else
        return WEST;
}

int StateBoxGraphic::returnClosestWallFace(qreal a, qreal b, qreal c, qreal d)
{
    if(a < b && a < c && a < d)
        return NORTH;

    else if(b < a && b < c && b < d)
        return EAST;

    else if(c < a && c < b && c < d)
        return SOUTH;

    else
        return WEST;
}

/**
 * @brief StateBoxGraphic::handleTransitionLineEndMoved
 * @param newPos scene scope location of the mouse and where to move the ElbowGrabber
 *
 * This function is connected to terminal anchors in transition graphic and is called in
 * place of a regular setPos to snap the elbow to its anchored state.
 */
void StateBoxGraphic::handleTransitionLineEndMoved(QPointF newPos)
{
    // qDebug() << "StateBoxGraphic::handleTransitionLineEndMoved";
    ElbowGrabber* elbow = dynamic_cast<ElbowGrabber*> (QObject::sender());

    QPointF mts;
    StateBoxGraphic* gp = this->getHighestLevelParentItemAsStateBoxGraphic();
    if(gp)
    {
        // if this state has a parent, then use its parents to map it to the scene
        mts = this->mapToHighestParent(pos());
    }
    else
    {
        // otherwise this is the highest level parent, and its pos is already in the scene;
        mts = pos();
    }


    qreal x = mts.x();
    qreal y = mts.y();

    qreal w = this->getSize().x();
    qreal h = this->getSize().y();
    qreal nx, ny;



    /* the grid location of a statebox is as follows:
     *         |   |
     ________0_|_1_|_2________
     ________7_|_8_|_3________
             6 | 5 | 4
               |   |
     *
     * Where 8 represents the area of the box and the lines continue infinitely
     * and in clockwise order this enum is: UL U UR R DR D DL L C
     *
     * we snap the anchor where the mouse pointer lies in the grid
     *
     *
     *
     */

    int wall;
    int grid = getGridLocation(mts,newPos);
    qDebug() << "box position: " << mts <<" newPos: "<<newPos<<"the elbow is in grid: " << grid;
    switch (grid) {
    case UL:
        nx = x;
        ny = y;
        elbow->setSnappedSide(NORTH);
        break;
    case U:
        nx = newPos.x();
        ny = y;
        elbow->setSnappedSide(NORTH);
        break;

    case UR:
        nx = x+w;
        ny = y;
        elbow->setSnappedSide(NORTH);
        break;
    case R:
        nx = x+w;
        ny = newPos.y();
        elbow->setSnappedSide(EAST);
        break;
    case DR:
        nx = x+w;
        ny = y+h;
        elbow->setSnappedSide(SOUTH);
        break;
    case D:
        nx = newPos.x();
        ny = y+h;
        elbow->setSnappedSide(SOUTH);
        break;
    case DL:
        nx = x;
        ny = y+h;
        elbow->setSnappedSide(SOUTH);
        break;
    case L:
        nx = x;
        ny = newPos.y();
        elbow->setSnappedSide(WEST);
        break;
    case C:
        wall = returnClosestWallFace(mts,newPos);
        elbow->setSnappedSide(wall);
        switch(wall)
        {
        case NORTH:
            nx = newPos.x();
            ny = y;
            break;
        case EAST:
            nx = x+w;
            ny = newPos.y();
            break;

        case SOUTH:
            nx = newPos.x();
            ny = y+h;
            break;
        case WEST:
            nx = x;
            ny = newPos.y();
            break;
        }

        break;
    default:
        break;
    }


    _intersection.setX(nx);
    _intersection.setY(ny);

   // update the elbow to the intersection



    // this is how to handle snapping children states of one state to any other state outside of its parent
    StateBoxGraphic* parentState = elbow->parentAsTransitionGraphic()->parentItemAsStateBoxGraphic();       // find the next highest parent of this state
    QPointF parentOffset = (parentState->pos());                                                            // add its position to the total offset
    while(parentState->parentItem())                                // while this parent still has parents, continue adding each next higher level state's offset
    {
        parentState = dynamic_cast<StateBoxGraphic*>(parentState->parentItem());
        parentOffset+=parentState->pos();
    }

    QPointF elbowBased(_intersection - parentOffset);             // modify the intersection by the total offset
    // qDebug() << "test: " << test;
    elbow->setPos(elbowBased);



}
StateBoxGraphic* StateBoxGraphic::parentItemAsStateBoxGraphic()
{
    if(this==NULL)
        return NULL;
    else
        return dynamic_cast<StateBoxGraphic* >(this->parentItem());
}

/**
 * @brief StateBoxGraphic::getHighestLevelParentItemAsStateBoxGraphic
 * @return
 *
 * this function returns the eldest ancestor state box graphic for any state box graphic
 *
 */
StateBoxGraphic* StateBoxGraphic::getHighestLevelParentItemAsStateBoxGraphic()
{
    // qDebug() << "StateBoxGraphic::getHighestLevelParentItemAsStateBoxGraphic";
    StateBoxGraphic* gp = parentItemAsStateBoxGraphic();
    while(gp->parentItemAsStateBoxGraphic())
    {
        //qDebug() << "counter: ";
        gp = gp->parentItemAsStateBoxGraphic();
       qDebug() << "* GP is: " << gp;
    }

    return gp;
}


void StateBoxGraphic::paintWithVisibleBox (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    int shadowThickness = 5;

    if ( _boxStyle == kSolidWithShadow )
    {

        /*
     The drop shadow effect will be created by drawing a filled, rounded corner rectangle with a gradient fill.
     Then on top of this will be drawn  filled, rounded corner rectangle, filled with a solid color, and offset such that the gradient filled
     box is only visible below for a few pixels on two edges.

     The total box size is _width by _height. So the top box will start at (0,0) and go to (_width-shadowThickness, _height-shadowThickness),
     while the under box will be offset, and start at (shadowThickness+0, shadowThickness+0) and go to  (_width, _height).
       */


        QLinearGradient gradient;
        gradient.setStart(_drawingOrigenX,_drawingOrigenY);
        gradient.setFinalStop( _drawingWidth ,_drawingOrigenY);
        QColor grey1(125,125,125,125);// starting color of the gradient - can play with the starting color and ,point since its not visible anyway

        // grey2 is ending color of the gradient - this is what will show up as the shadow. the last parameter is the alpha blend, its set
        // to 125 allowing a mix of th color and and the background, making more realistic shadow effect.
        QColor grey2(225,225,225,125);


        gradient.setColorAt((qreal)0, grey1 );
        gradient.setColorAt((qreal)1, grey2 );

        QBrush brush(gradient);

        painter->setBrush( brush);

        // for the desired effect, no border will be drawn, and because a brush was set, the drawRoundRect will fill the box with the gradient brush.
        QPen pen;
        pen.setStyle(Qt::NoPen);
        painter->setPen(pen);

        QPointF topLeft (_drawingOrigenX,_drawingOrigenX);
        QPointF bottomRight ( _drawingWidth , _drawingHeight);

        QRectF rect (topLeft, bottomRight);

        painter->drawRoundRect(rect,25,25); // corner radius of 25 pixels
    }

    // draw the top box, the visible one

    if ( _isHighlighted )
        _pen.setColor(Qt::red);
    else
        _pen.setColor(Qt::black);

  /*  if ( _isHovered )
        _pen.setWidth(BOX_HOVER_PEN_WIDTH);
    else
        _pen.setWidth(BOX_DEFAULT_PEN_WIDTH);
*/
    if ( _drawBoxLineStyle == kDrawSolid )
        _pen.setStyle( Qt::SolidLine );
    else
        _pen.setStyle( Qt::DotLine );

    painter->setPen(_pen);


    if ( _boxStyle != kTransparent )
    {
        // QBrush brush2(QColor(187,250,185,255),Qt::SolidPattern);  // the box fill color
         QBrush brush2(QColor(255,255,255,255),Qt::SolidPattern);  // white fill
         painter->setBrush( brush2);
    }


    // draw outter layer box
    // if shadowed box, draw inside, else draw on the outter edge
    QRectF rect2;

    if ( _boxStyle == kSolidWithShadow )
    {
        QPointF topLeft2 (_drawingOrigenX, _drawingOrigenY);
        QPointF bottomRight2 ( _drawingWidth - shadowThickness, _drawingHeight - shadowThickness);

        rect2 = QRectF (topLeft2, bottomRight2);
    }
    else
    {
        QPointF topLeft2 (_drawingOrigenX, _drawingOrigenY);
        QPointF bottomRight2 ( _drawingWidth, _drawingHeight);

        rect2 = QRectF (topLeft2, bottomRight2);
    }

    painter->drawRoundRect(rect2,25,25);

}

void  StateBoxGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if  ( (_showBoxStyle == kAlways) || (( _showBoxStyle == kWhenSelected) && ( _isHovered == true)))
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
        paintWithVisibleBox (painter,0,0);
    }

#if 0  // debug stuff
    if ( _diagLineDrawIt )
    {
        QPen p;
        p.setColor(Qt::blue);
        p.setStyle(Qt::DotLine);
        painter->setPen(p);
        painter->drawLine(_diagLineStart, _diagLineEnd);

        painter->drawEllipse(_intersection, 3,3);
    }
#endif

}


/*
void StateBoxGraphic::handleAttributeChanged(void *attr)
{
    qDebug() << "StateBoxGraphic::handleAttributeChanged: ";

    StateName * name = dynamic_cast<StateName *> ((IAttribute*)attr);
    SizeAttribute * size = dynamic_cast<SizeAttribute *> ( (IAttribute*)attr);
    PositionAttribute * position =dynamic_cast<PositionAttribute*> ((IAttribute*)attr);

    if ( name )
    {
        this->setObjectName(name->asString()); // help debug tracing - id which object this is
        SCTextBlock * tb  = _stateModel->getIDTextBlock();
        tb->setText(name->asString());
        //TextItem.setPlainText(name->asString());
    }
    else if ( size )
    {
        QPoint pt = size->asPointF().toPoint();
        SelectableBoxGraphic::setSize(pt);
    }
    else if ( position)
    {
        QPointF ps = position->asPointF();
        SelectableBoxGraphic::setPos(ps);
    }

    QGraphicsItem *parent = this->parentItem();
    if ( parent)
        parent->update();
    else
        this->update();

}
*/


void StateBoxGraphic::setHighlighted(bool on)
{
    qDebug()<<"StateBoxGraphic::setHighlighted highlight = " + QString(on?"True":"False");
    SelectableBoxGraphic::setHighlighted(  on );
}


QString StateBoxGraphic::getPositionAsString()
{
    return SelectableBoxGraphic::getPositionAsString();
}



QPointF  StateBoxGraphic::getSize()
{
    QPointF size = SelectableBoxGraphic::getSize();
    return size;
}

QString StateBoxGraphic::getSizeAsString()
{
    return SelectableBoxGraphic::getSizeAsString();
}


void StateBoxGraphic::setSize(QPointF size)
{
    return SelectableBoxGraphic::setSize(size);

    if ( _stateModel )
    {
        _stateModel->setSize (size );
    }
}



void StateBoxGraphic::graphicHasChanged ()
{
    //qDebug () << "StateBoxGraphic::graphicsHasChanged";
    if ( _stateModel )
    {
        QPointF ps = this->pos();
        _stateModel->setPosition(ps);
        //qDebug()<<"graphicsHasChanged: "<<_stateModel->objectName() << " position: " << this->pos();

        QPointF sz =this->getSize();
        _stateModel->setSize(sz);
    }
}


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
        TextItem(parent,stateModel->getIDTextBlock()),
        _stateModel(stateModel),
        _diagLineStart(),
        _diagLineEnd(),
        _diagLineDrawIt(false),
        _intersection()
{


    // this graphic representation of a state is linked to a state in the model

    setShowBoxLineStyle ( SelectableBoxGraphic::kAlways );
    setDrawBoxLineStyle  ( SelectableBoxGraphic::kDrawSolid );
    setBoxStyle(SelectableBoxGraphic::kSolidWithShadow );

    /*
    StateName * name = dynamic_cast<StateName *> ( _stateModel->attributes.value("name"));
    connect (name, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(name);

    SizeAttribute * size = dynamic_cast<SizeAttribute *> (  _stateModel->attributes.value("size"));
    connect (size, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(size);

    PositionAttribute * position =dynamic_cast<PositionAttribute*> ( _stateModel->attributes.value("position"));
    connect (position, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)), Qt::QueuedConnection);
    handleAttributeChanged(position);
*/

    TextItem.setPos(25,10);

    TextItem.setParentItem(this);

}


StateBoxGraphic::~StateBoxGraphic()
{

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

/*


    double d[8];

    d[0] = distance(point, NW);
    d[1] = distance(point, N);
    d[2] = distance(point, NE);
    d[3] = distance(point, E);
    d[4] = distance(point, SE);
    d[5] = distance(point, S);
    d[6] = distance(point, SW);
    d[7] = distance(point, W);

    double wallDist[4];

    wallDist[0] = d[0] + d[1] + d[2];
    wallDist[1] = d[2] + d[3] + d[4];
    wallDist[2] = d[4] + d[5] + d[6];
    wallDist[3] = d[6] + d[7] + d[0];




    for(int i = 0; i < 4;i++)
    {
        qDebug() << "wallDist["<<i<<"] = " << wallDist[i];
    }

    return getSmallest(wallDist, 4);
    */
}

/**
 * @brief StateBoxGraphic::handleTransitionLineStartMoved
 * @param newPos
 * Connected to a source elbow's signal anchorMoved in transition graphics.
 * will readjust the position of the source point of the transition arrow to be anchored to its starting state
 *
 *
 */
void StateBoxGraphic::handleTransitionLineStartMoved(QPointF newPos)
{
    //qDebug()<<"htls moved";
    // this method keeps the starting position of a line snapped to the outter edge of the box

    ElbowGrabber* elbow = dynamic_cast<ElbowGrabber*> (QObject::sender());
    QPointF cursorPos = mapFromScene(newPos);

    // find the side which is closest to the newPos
    //QRectF box = getUsableArea();
    QRectF box = *(getBufferedBoxRect(SOURCE_ANCHOR_BUFFER, SOURCE_ANCHOR_POS_BUFFER)); // 10 is the cornerGuard
    int closest = findNearestWall(box, cursorPos);
    QPointF* corners[4];
    corners[NORTHWEST] = new QPointF(box.x(),box.y());
    corners[NORTHEAST] = new QPointF(box.x()+box.width(), box.y());
    corners[SOUTHEAST] = new QPointF(box.x()+box.width(),box.y()+box.height());
    corners[SOUTHWEST] = new QPointF(box.x(),box.y()+box.height());

    qreal x = 0;
    qreal y = 0;
    elbow->setSnappedSide(closest);
    switch(closest)
    {
    case NORTH: //northwest and northeast
        x = cursorPos.x();
        y = box.y();
        if(x >= corners[NORTHEAST]->x())  x = corners[NORTHEAST]->x();
        if(x < corners[NORTHWEST]->x())  x = corners[NORTHWEST]->x();

        break;

    case EAST: // northeast and southeast corners

        x = box.x() + box.width();
        y = cursorPos.y();

        if(y >= corners[SOUTHEAST]->y()) y = corners[SOUTHEAST]->y();
        if(y < corners[NORTHEAST]->y()) y = corners[NORTHEAST]->y();

        break;
    case SOUTH: //southwest and southeast

        x = cursorPos.x();
        y = box.y() + box.height();

        if(x >= corners[SOUTHEAST]->x())  x = corners[SOUTHEAST]->x();
        if(x < corners[SOUTHWEST]->x())  x = corners[SOUTHWEST]->x();

        break;

    case WEST: //northwest and southwest corners

        x = box.x();
        y = cursorPos.y();

        if(y >= corners[SOUTHWEST]->y()) y = corners[SOUTHWEST]->y();
        if(y < corners[NORTHWEST]->y()) y = corners[NORTHWEST]->y();


        break;
    default:


        break;

    }


    _intersection.setX(x);
    _intersection.setY(y);
    elbow->setPos(_intersection);
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
    // qDebug() << "htle moved";
    // this method keeps the starting position of a line snapped to the outter edge of the box

    ElbowGrabber* elbow = dynamic_cast<ElbowGrabber*> (QObject::sender());
    QPointF cursorPos = (newPos);                   // keep the cursor in scene scope

    // find the side which is closest to the newPos
    //QRectF box = getUsableArea();
    // account for the shading and reduce the width and height
    QRectF box = *(getBufferedBoxRect(SINK_ANCHOR_BUFFER, SINK_ANCHOR_POS_BUFFER));


    /*

     ----- 0 -----
     |           |
     |           |
    3             1
     |           |
     |           |
     ----- 2 -----

     */

   // qDebug() << "the box pos is " << box.topLeft();
    QPointF boxTopLeft(box.topLeft());
    QPointF boxInScene = mapToScene(boxTopLeft);
   // qDebug() << "the box in the scene is " << boxInScene;
    QRectF adjustedBox(boxInScene.x(), boxInScene.y(), box.width(), box.height());
    int closest = findNearestWall(adjustedBox, cursorPos);

    QPointF* corners[4];
    corners[NORTHWEST] = new QPointF(adjustedBox.x(),adjustedBox.y());
    corners[NORTHEAST] = new QPointF(adjustedBox.x()+adjustedBox.width(), adjustedBox.y());
    corners[SOUTHEAST] = new QPointF(adjustedBox.x()+adjustedBox.width(),adjustedBox.y()+adjustedBox.height());
    corners[SOUTHWEST] = new QPointF(adjustedBox.x(),adjustedBox.y()+adjustedBox.height());

    qreal x = 0;
    qreal y = 0;
   // qDebug() << "Setting closest wall: " << closest;
    elbow->setSnappedSide(closest);
    switch(closest)
    {    
    case NORTH: //northwest and northeast
        x = cursorPos.x();
        y = adjustedBox.y();
        if(x >= corners[NORTHEAST]->x())  x = corners[NORTHEAST]->x();
        if(x < corners[NORTHWEST]->x())  x = corners[NORTHWEST]->x();

        break;

    case EAST: // northeast and southeast corners

        x = adjustedBox.x() + adjustedBox.width();
        y = cursorPos.y();

        if(y >= corners[SOUTHEAST]->y()) y = corners[SOUTHEAST]->y();
        if(y < corners[NORTHEAST]->y()) y = corners[NORTHEAST]->y();

        break;
    case SOUTH: //southwest and southeast

        x = cursorPos.x();
        y = adjustedBox.y() + adjustedBox.height();

        if(x >= corners[SOUTHEAST]->x())  x = corners[SOUTHEAST]->x();
        if(x < corners[SOUTHWEST]->x())  x = corners[SOUTHWEST]->x();

        break;

    case WEST: //northwest and southwest corners

        x = adjustedBox.x();
        y = cursorPos.y();

        if(y >= corners[SOUTHWEST]->y()) y = corners[SOUTHWEST]->y();
        if(y < corners[NORTHWEST]->y()) y = corners[NORTHWEST]->y();


        break;
    default:

        break;

    }

    _intersection.setX(x);
    _intersection.setY(y);

   // update the elbow to the intersection
   // elbow->setZValue(500);
    /*
   qDebug() << "intersection: " << _intersection;
   StateBoxGraphic* parentState =  elbow->parentAsTransitionGraphic()->parentItemAsStateBoxGraphic();
   QPointF parentOffset = (parentState->pos());
   //QPointF test(_intersection.x() -(parentState->x()), _intersection.y()-(parentState->y()));
   QPointF test(_intersection - parentOffset);
   qDebug() << "test: " << test;
   elbow->setPos(test);*/


    // this is how to handle snapping children states of one state to any other state outside of its parent
    StateBoxGraphic* parentState = elbow->parentAsTransitionGraphic()->parentItemAsStateBoxGraphic();       // find the next highest parent of this state
    QPointF parentOffset = (parentState->pos());                                                            // add its position to the total offset
    while(parentState->parentItem())                                // while this parent still has parents, continue adding each next higher level state's offset
    {
        parentState = dynamic_cast<StateBoxGraphic*>(parentState->parentItem());
        parentOffset+=parentState->pos();
    }

    QPointF test(_intersection - parentOffset);             // modify the intersection by the total offset
    // qDebug() << "test: " << test;
    elbow->setPos(test);                                    // set the elbow's position
}

StateBoxGraphic* StateBoxGraphic::parentItemAsStateBoxGraphic()
{
    if(this==NULL)
        return NULL;
    else
        return dynamic_cast<StateBoxGraphic* >(this->parentItem());
}

void  StateBoxGraphic::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    SelectableBoxGraphic::paint(painter, option, widget);
    //qDebug() << " object name: " << this->objectName()<< " object position: "<<this->getPositionAsString();

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



void StateBoxGraphic::handleAttributeChanged(void *attr)
{

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

    if ( _stateModel )
    {
        QPointF ps = this->pos();
        _stateModel->setPosition(ps);
        //qDebug()<<"graphicsHasChanged: "<<_stateModel->objectName() << " position: " << this->pos();

        QPointF sz =this->getSize();
        _stateModel->setSize(sz);
    }
}



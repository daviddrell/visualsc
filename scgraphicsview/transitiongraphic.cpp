/*
 *  This class represents the transition on the GUI and is a container for anything related to one transition graphic
 *  Elbows
 *  LineSegments
 *  TextBlocks
 *
 * anchor[0]                  anchor[1]
 *      x-------x-x--x-----x-------->
 *
 *
 */

#include "transitiongraphic.h"
#include "scstate.h"
#include "selectableboxgraphic.h"
#include <QDebug>
#include <QApplication>


#define PI                  3.14159265359
#define ANGLE_SNAP_DEGREE   11


TransitionGraphic::TransitionGraphic(StateBoxGraphic *parentGraphic, StateBoxGraphic *targetGraphic, SCTransition * t, KeyController * keys, MouseController* mouse) :
    QGraphicsObject(NULL),
    _transitionModel(t),
    _lineSegments(),
    _elbows(),
    _targetStateGraphic(targetGraphic),
    _keyController(keys),
    _mouseController(mouse),
    _hasMovedSinceCreatingElbow(true),
    _isCurrentlyDeleting(false),
    _eventText(NULL)
{

    this->setFlag(QGraphicsItem::ItemIsMovable, false);
    this->setParentItem(parentGraphic);     // the source state will be this transition graphic's parent item

    TransitionPathAttribute * p = dynamic_cast<TransitionPathAttribute *> (  t->attributes.value("path"));

    QList<QPointF> pointList = p->asQPointFList();
    /*qDebug() << "Printing Point List of size: " <<pointList.count();
    for(int i = 0; i < pointList.count();i++)
        qDebug() << pointList.at(i);
*/

    // populate the transition graphic with elbows.
    // if this is a new transition graphic, then create two elbows
    // otherwise this is a transition graphic being loaded in, so add elbows according to its path list
    if(pointList.count() < 2 && targetGraphic != NULL )
    {
        qDebug() << "new transition Grahpic!";
        // create two new elbows, these are the anchors attached to the source and sink state
        _anchors[0] = new ElbowGrabber(this,_keyController);
        _anchors[0]->installSceneEventFilter(this);
        _anchors[0]->setAcceptHoverEvents(true);
        _elbows.append(_anchors[0]);

        _anchors[1] = new ElbowGrabber(this, _keyController);
        _anchors[1]->installSceneEventFilter(this);
        _anchors[1]->setAcceptHoverEvents(true);
        _elbows.append(_anchors[1]);

        // set the anchor positions by snapping them to their source and sink state boxes
        int sourceSide=0;
        int targetSide=0;

        // get the mid point of the source and sink on the sides that are nearest to each other
        getClosestSides( & sourceSide, & targetSide);
        QPointF sourceAnchor = (this->parentItemAsStateBoxGraphic()->getSideCenterPointInSceneCoord(sourceSide));
        QPointF targetAnchor = (_targetStateGraphic->getSideCenterPointInSceneCoord(targetSide));

        // use the statebox function handleTransitionLineStartMoved & handleTransitionLineEndMoved to update the anchors to this position and snap them to the box border.
       // _anchors[0]->setPos(mapFromScene(sourceAnchor));
       // _anchors[1]->setPos(mapFromScene(targetAnchor));

        LineSegmentGraphic* segment = new LineSegmentGraphic(_anchors[0], _anchors[1], this, _keyController);
        segment->setZValue(_anchors[0]->zValue()-1);            // move the line segment behind its anchors
        segment->setAcceptHoverEvents(true);                    // allow the segment to be hovered
        segment->installSceneEventFilter(this);

        _anchors[0]->setSegmentAt(0, NULL);
        _anchors[0]->setSegmentAt(1, segment);

        _anchors[1]->setSegmentAt(0, segment);
        _anchors[1]->setSegmentAt(1, NULL);

        _anchors[1]->setTerminal(true);                         // set the last elbow grabber as the terminator
        _anchors[1]->setPaintStyle(ElbowGrabber::kArrowHead);   // set the end of the transition to an arrowhead

        _anchors[0]->setAnchor(true);
        _anchors[1]->setAnchor(true);

        _lineSegments.append(segment);

        _anchors[0]->setPos(this->mapFromScene(sourceAnchor));
        _anchors[1]->setPos(this->mapFromScene(targetAnchor));

        // calculate the position of the hover box for the line segment
        segment->enclosePathInElbows();
        this->updateModel();
    }
    else // load the point list and create the elbows and line segments
    {
        //qDebug() << "loading the point list";
        // add every point as an elbow
        for(int i = 0; i < pointList.count(); i++)
        {
            ElbowGrabber* elb;
            elb = new ElbowGrabber(this, pointList.at(i), _keyController);
            elb->installSceneEventFilter(this);                 // transition graphic will handle elbow events
            elb->setAcceptHoverEvents(true);                    // allow the elbow to be hovered

            _elbows.append(elb);
        }


        // create a line segment in between every elbow
        for(int i = 0; i < _elbows.count()-1; i++)
        {
            //qDebug()<<"i: "<<i;
            ElbowGrabber* elbOne = _elbows.at(i);
            ElbowGrabber* elbTwo = _elbows.at(i+1);

            LineSegmentGraphic* segment = new LineSegmentGraphic(elbOne, elbTwo, this, _keyController);
            segment->setZValue(elbOne->zValue()-1);            // move the line segment behind its anchors

            segment->installSceneEventFilter(this);
            segment->setAcceptHoverEvents(true);                    // allow the line segment to be hovered


           // connect(segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF)));
           // connect(segment, SIGNAL(updateModel()), this, SLOT(updateModel()));


            if(i==0 || i== _elbows.count()-2)   // anchor elbows at the end and beginning
            {
                if(i==0){                       // source anchor
                    //qDebug() << "source anchor";
                    elbOne->setSegmentAt(0, NULL);
                    elbOne->setSegmentAt(1, segment);

                    elbTwo->setSegmentAt(0, segment);
                }
                if(i==_elbows.count()-2){      // sink anchor
                    //qDebug() << "sink anchor";
                    elbTwo->setSegmentAt(0, segment);
                    elbTwo->setSegmentAt(1, NULL);

                    elbOne->setSegmentAt(1, segment);
                }
            }
            else                                // standard elbow that connects two line segments
            {
                //qDebug() << "normal elbow";
                elbOne->setSegmentAt(1, segment);
                elbTwo->setSegmentAt(0, segment);
            }


            // make the elbows invisible by default
            //elbOne->setVisible(false);
            //elbOne->setVisible(false);


            _lineSegments.append(segment);  // add the line segments



        }

        // set first and last elbows as the anchors and the last elbow as the terminal elbow
        _anchors[0] = _elbows.at(0);                    //anchors will be the saved as the first and last points following a path from the start to end states
        _anchors[1] = _elbows.at(_elbows.count()-1);

        _anchors[1]->setTerminal(true);                         // set the last elbow grabber as the terminator
        _anchors[1]->setPaintStyle(ElbowGrabber::kArrowHead);   // set the end of the transition to an arrowhead

        // these elbows are anchors
        _anchors[0]->setAnchor(true);
        _anchors[1]->setAnchor(true);

        //_anchors[1]->setAcceptHoverEvents(true);    // enable the sink anchor to be hovered

        /* moved to transition connect
        connect(_anchors[0],SIGNAL(anchorMoved(QPointF)),parentGraphic,SLOT(handleTransitionLineStartMoved(QPointF)));  // state box will handle snapping the source elbow/anchor to its border instead of standard movement
        connect(_anchors[1],SIGNAL(anchorMoved(QPointF)),_targetStateGraphic,SLOT(handleTransitionLineEndMoved(QPointF)));  // state box will handle snapping the source elbow/anchor to its border instead of standard movement
        //qDebug() << "hooking anchor to state graphic: " << _targetStateGraphic->objectName();

        // do this to set closest wall from default
        emit _anchors[0]->anchorMoved(mapToScene(_anchors[0]->pos()));
        emit _anchors[1]->anchorMoved(mapToScene(_anchors[1]->pos()));
        */

        //TextItem.setPos(25,10);
        //TextItem.setParentItem(_anchors[1]);

        // set the event text to be tied to the source anchor
        //_eventText->setParentItem(_anchors[0]);

        //_eventText->setPos(0,0);


    } // end of loading a transition from file

/*
    _transitionTextBox = new TextBox(_anchors[0], "Hello World");
    _transitionTextBox->setVisible(false);
    _transitionTextBox->setPos(100,100);
    */

    _eventText = new SelectableTextBlock((QGraphicsObject*) _anchors[0], t->getEventTextBlock());

}   // end of constructor

SCTransition* TransitionGraphic::getTransitionModel()
{
    return _transitionModel;
}


ElbowGrabber* TransitionGraphic::getSourceAnchor()
{
    return _anchors[0];
}

ElbowGrabber* TransitionGraphic::getSinkAnchor()
{
    return _anchors[1];
}

void TransitionGraphic::setTargetStateGraphic(StateBoxGraphic *sbg)
{
    _targetStateGraphic = sbg;
}

/**
 * @brief TransitionGraphic::setGrabbersVisible
 * @param visible
 * calls setVisible for all elbows in the transition graphic to the bool value
 */
void TransitionGraphic::setGrabbersVisible(bool visible)
{
    //TODO move corner grabbers from line segment to higher level transition graphic
    for(int i = 0; i < _elbows.count(); i++)
        _elbows.at(i)->setVisible(visible);
}

StateBoxGraphic* TransitionGraphic::getTargetStateGraphic()
{
    return _targetStateGraphic;
}

/**
 * @brief TransitionGraphic::parentItemAsStateBoxGraphic
 * @return returns the parent QGraphicsItem of the transition graphic as a StateBoxGraphic
 */
StateBoxGraphic* TransitionGraphic::parentItemAsStateBoxGraphic()
{
    return static_cast<StateBoxGraphic*>(this->parentItem());
}

/**
 * @brief TransitionGraphic::updateElbow
 * @param newPos coordinates of the new position to move the mouse, given in scene scope
 * @param elbow elbow being manipulated
 * Called from the transition graphics scene event filter if a mouse press then mouse move was detected.
 * this function wlil update the elbow positions on the gui and then update the model once the mouse is released
 * additionally, anchor elbows will snap to the edges of their connected states
 */
void TransitionGraphic::updateElbow(QPointF newPos, ElbowGrabber *elbow)
{
    //elbow->setPos(newPos);

    // check if this elbow is an anchor
    if(elbow->isAnchor())
    {
        emit elbow->anchorMoved(newPos);    // call the handlestart/endmoved based on if this anchor is at the source or end state
    }
    // change the elbow's coordinates to the specified x and y
    else
    {
        elbow->setPos(mapFromScene(newPos));
    }

    // update the polygon of the segments
    updateLineSegments(elbow);
    this->updateModel();
}

/**
 * @brief TransitionGraphic::updateLineSegments
 * @param elbow
 *
 * called from update elbow
 * updates the position and polygon of the line segments linked to an elbow
 */
void TransitionGraphic::updateLineSegments(ElbowGrabber* elbow)
{
    LineSegmentGraphic* one = elbow->getSegment(0);
    LineSegmentGraphic* two = elbow->getSegment(1);

    if(one) // check if object exists
    {
        one->enclosePathInElbows();
    }
    if(two) // check if object exists
    {
        two->enclosePathInElbows();
    }
}


bool TransitionGraphic::horizontallyAligned(ElbowGrabber *one, ElbowGrabber *two)
{
    return (one->y() == two->y());
}

bool TransitionGraphic::verticallyAligned(ElbowGrabber *one, ElbowGrabber *two)
{
    return (one->x() == two->x());
}


/**
 * @brief TransitionGraphic::sceneEventFilter
 * @param watched   object that caused the event
 * @param event     action (mouse)
 * @return
 *
 * The scene event filter of transition graphic will update(change coordinates/redraw) the child elbow that was manipulated in the graphics view
 * and subsequently update the datamodel by calling TransitionGraphic::updateModel() when the mouse is released
 *
 */
bool TransitionGraphic::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
{
    ElbowGrabber * elbow = dynamic_cast<ElbowGrabber *>(watched);
    LineSegmentGraphic * line = dynamic_cast<LineSegmentGraphic *>(watched);
    QGraphicsSceneMouseEvent * mevent = dynamic_cast <QGraphicsSceneMouseEvent*>(event);
    //QGraphicsSceneHoverEvent * hevent = dynamic_cast <QGraphicsSceneHoverEvent*>(event);

    if(elbow)
    {
        //qDebug() <<  "elbow event!\t" << event->type();


            switch(event->type())
            {
            case QEvent::GraphicsSceneHoverEnter:
                qDebug()<<"GraphicsSceneHoverEnter: " << event->type();
                elbow->forceHoverEnterEvent();
                break;

            case QEvent::GraphicsSceneHoverLeave:
                qDebug()<<"GraphicsSceneHoverLeave: " <<event->type();
                elbow->forceHoverLeaveEvent();
                break;


            case QEvent::GraphicsSceneMousePress:
            {
                emit clicked(this->_transitionModel);
                //qDebug() << "mouse press";
                elbow->setMouseState(ElbowGrabber::kMouseDown);
                QApplication::setOverrideCursor(Qt::ClosedHandCursor);

                QPointF scenePosition =  elbow->mapToScene(mevent->pos());
                elbow->mouseDownY = scenePosition.y();
                elbow->mouseDownX = scenePosition.x();



                //_cornerGrabbed = true;
                //qDebug() << "Corner Position: " << elbow->mouseDownX<<" ," << elbow->mouseDownY;

                LineSegmentGraphic* segOne = elbow->getSegment(0);
                LineSegmentGraphic* segTwo = elbow->getSegment(1);
                if(segOne && segOne->isHovered())
                {
                    segOne->forceHoverLeaveEvent(); // a mouse press should switch focus to elbows, so force the segments for this elbow to be unhovered
                }
                if(segTwo && segTwo->isHovered())
                {

                    segTwo->forceHoverLeaveEvent();
                }
                elbow->forceHoverEnterEvent(); // old elbow hover events will trigger for mouse press and releases

            }
                break;

            case QEvent::GraphicsSceneMouseMove:
            {
                //qDebug() << "mouse moving";
                elbow->setMouseState(ElbowGrabber::kMouseMoving );
            }
                break;

            case QEvent::GraphicsSceneMouseRelease:
            {
                //qDebug() << "mouse release";
                //_cornerGrabbed = false;

                elbow->setMouseState(ElbowGrabber::kMouseReleased);
                elbow->straightenLines();
                QApplication::restoreOverrideCursor();

                //straightenLines(elbow);

                updateModel();                  // update the transition datamodel for this path
                //elbow->forceHoverLeaveEvent();  // old elbow hover events will trigger for mouse press and releases
            }
                break;

            default:
                // we dont care about the rest of the events
                return false;
                break;
            }

            if ( elbow->getMouseState() == ElbowGrabber::kMouseMoving && mevent)
            {
                //qDebug()<< "mevent: " << mevent->pos() << "scene mapped mevent: " << elbow->mapToScene(mevent->pos());
                // give the scene scope position of the mouse and update the elbow that triggered the scene event
                //this->updateElbow(elbow->mapToScene(mevent->pos()), elbow);
                //this->updateElbow(this->mapFromItem(elbow, mevent->pos()), elbow);
                //this->update();
                updateElbow(elbow->mapToScene(mevent->pos()),elbow);


            }
            return true;     // this is needed

    }
    else if(line)
    {
        //qDebug () << "line event!\t\t" <<event->type();


        switch(event->type())
        {
        case QEvent::GraphicsSceneHoverEnter:
            //qDebug()<<"GraphicsSceneHoverEnter: " << event->type();
            line->forceHoverEnterEvent();
            break;

        case QEvent::GraphicsSceneHoverLeave:
            //qDebug()<<"GraphicsSceneHoverLeave: " <<event->type();
            line->forceHoverLeaveEvent();
            _hasMovedSinceCreatingElbow = true;     // elbows can be created again after unhovering the segment
            break;

        case QEvent::GraphicsSceneMousePress:
        {
            emit clicked(this->_transitionModel);
           // qDebug() << "line mouse press";
            line->setMouseState(ElbowGrabber::kMouseDown);
            QApplication::setOverrideCursor(Qt::ClosedHandCursor);

            QPointF scenePosition = mapToScene(mevent->pos());
            line->mouseDownY = scenePosition.y();
            line->mouseDownX = scenePosition.x();
            line->setElbowOffsets();



            //_cornerGrabbed = true;
            //qDebug() << "line Corner Position: " << line->mouseDownX<<" ," << line->mouseDownY;

            // force hover event?? line

        }
            break;

        case QEvent::GraphicsSceneMouseMove:
        {
            //qDebug() << "line mouse moving";
            line->setMouseState(ElbowGrabber::kMouseMoving );
        }
            break;

        case QEvent::GraphicsSceneMouseRelease:
        {
            //qDebug() << "mouse release";
            //_cornerGrabbed = false;

            line->setMouseState(ElbowGrabber::kMouseReleased);

            line->getElbow(0)->straightenLines();
            line->getElbow(1)->straightenLines();

            QApplication::restoreOverrideCursor();


            updateModel();                  // update the transition datamodel for this path
            //line->forceHoverLeaveEvent();  // old elbow hover events will trigger for mouse press and releases
        }
            break;

        default:
            // we dont care about the rest of the events
            return false;
            break;
        }

        // update the segment and its elbows and its elbows' segments if it is being dragged
        if ( line->getMouseState() == ElbowGrabber::kMouseMoving && mevent)
        {
            ElbowGrabber* left = line->getElbow(0);
            ElbowGrabber* right = line->getElbow(1);
            QPointF mouseInScene = mapToScene(mevent->pos());

            qreal lx = left->pos().x();
            qreal rx = right->pos().x();
            qreal ly = left->y();
            qreal ry = right->y();

            if(!line->isAnchored())
            {
                // give the scene scope position of the mouse and update the elbow that triggered the scene event


                // update the position relative to the mouse event's position
                left->setPos(mapFromScene(mouseInScene) - line->leftElbowOffset);
                right->setPos(mapFromScene(mouseInScene) - line->rightElbowOffset);


                // if this line is horizontally aligned, then only y is allowed to change
                if(horizontallyAligned(left,right))
                {
                    left->setX(lx);
                    right->setX(rx);
                }
                // if this line is vertically aligned, then only x is allowed to change
                else if(verticallyAligned(left,right))
                {
                    left->setY(ly);
                    right->setY(ry);
                }

                bool rightAngleMode = false;
                if(rightAngleMode)
                {
                    bindNeighborElbows(left);
                    bindNeighborElbows(right);
                }

                // update the three line segments that touch these two elbows
                line->enclosePathInElbows();
                left->getSegment(0)->enclosePathInElbows();
                right->getSegment(1)->enclosePathInElbows();



                // update the data model
                this->update();


            }
            else     // an anchored line segment is being dragged
            {
                if(left->isAnchor() && right->isAnchor())    // check if both elbows of this line are anchors
                {
                    // then there is only one line in this transition
                    emit left->anchorMoved((mouseInScene) - line->leftElbowOffset);
                    emit right->anchorMoved(mouseInScene - line->rightElbowOffset);
                    line->enclosePathInElbows();
                    this->update();
                }
                else if(line->getElbow(0)->isAnchor())   // is this a source anchor?
                {


                    // left is an source anchor



                    // snap the source anchor to the state and set the other elbow's position
                    emit left->anchorMoved((mouseInScene) - line->leftElbowOffset);
                    right->setPos(mapFromScene(mouseInScene) - line->rightElbowOffset);

                    // horizontal line = non anchor can only move up and down
                    if(horizontallyAligned(left,right))
                    {
//                        left->setX(lx);
                        right->setX(rx);
                    }

                    // vertical line = non anchor can only move left and right
                    else if(verticallyAligned(left,right))
                    {
//                        left->setY(ly);
                        right->setY(ry);
                    }


                    // update the two line segments that are affected
                    line->enclosePathInElbows();
                    right->getSegment(1)->enclosePathInElbows();

                    // update the datamodel
                    this->update();
                }
                else        // this is a sink anchor
                {


                    // snap the sink anchor to the state and set the other elbow's position
                    left->setPos(mapFromScene(mouseInScene) - line->leftElbowOffset);
                    emit right->anchorMoved(mouseInScene - line->rightElbowOffset);

                    // horizontal line = non anchor can only move up and down
                    if(horizontallyAligned(left,right))
                    {
                        left->setX(lx);
//                        right->setX(rx);
                    }

                    // vertical line = non anchor can only move left and right
                    else if(verticallyAligned(left,right))
                    {
                        left->setY(ly);
//                        right->setY(ry);
                    }



                    // update the two line segments that are affected
                    line->enclosePathInElbows();
                    left->getSegment(0)->enclosePathInElbows();

                    // update the datamodel
                    this->update();
                }
            }
        }
        return true;     // this is needed
    }

    return false; // we don't care about the rest of the events
}

void TransitionGraphic::bindNeighborElbows(ElbowGrabber* elbow)
{
    if(elbow->isAnchor())
    {

    }
    else    // not an anchor elbow
    {
        ElbowGrabber* left = elbow->getSegment(0)->getElbow(0);
        qreal xDist = fabs(left->x() - elbow->x());
        qreal yDist = fabs(left->y() - elbow->y());

        if(xDist < yDist)
        {
            left->setX(elbow->x());
        }
        else
        {
            left->setY(elbow->y());
        }

        if(left->isAnchor())
            emit left->anchorMoved(left->pos());

        ElbowGrabber* right = elbow->getSegment(1)->getElbow(1);
        qreal rxDist = fabs(right->x() - elbow->x());
        qreal ryDist = fabs(right->y() - elbow->y());

        if(rxDist < ryDist)
        {
            right->setX(elbow->x());
        }
        else
        {
            right->setY(elbow->y());
        }

        if(right->isAnchor())
            emit right->anchorMoved(right->pos());

    }
}

void TransitionGraphic::createCustomPath(QPointF mouseLocation, ElbowGrabber* elbow)
{//TODO

    // update all line segments connected to this elbow

    qDebug() << "Elbow position: " << elbow->x() << ", "<< elbow->y();

}

TransitionGraphic::~TransitionGraphic()
{

    for(int i=0;i < _lineSegments.count(); i++)
    {
        LineSegmentGraphic * ls =_lineSegments.at(i);
        delete ls;
    }
    _lineSegments.clear();
    for(int i = 0; i < _elbows.count() ; i++)
    {
        ElbowGrabber* elb = _elbows.at(i);
        delete elb;
    }
    _elbows.clear();


// Delete causes an exception
   //qDebug() << "eventText" << _eventText->objectName() << _eventText->isEnabled();
   //delete _eventText;


}

/**
 * @brief TransitionGraphic::handleGrandParentStateGraphicResized
 * @param oldBox
 * @param newBox
 * @param corner
 *
 * connect in scgraphicsview.cpp
 *
 * connects all grandparents of a state box graphic to update elbows to stay in the same spot if a parent of the statebox is resized
 *
 */
void TransitionGraphic::handleGrandParentStateGraphicResized(QRectF oldBox, QRectF newBox, int corner)
{
    QPointF mtsTL = mapToScene(oldBox.topLeft());
    QPointF nmtsTL = mapToScene(newBox.topLeft());
    QPointF diff = mtsTL - nmtsTL;
    qreal dx = diff.x();
    qreal dy = diff.y();

    // make other elbows stay in place
    // and update their line segment hover boxes
    for(int i = 1 ; i < _elbows.count();i++)
    {
        _elbows[i]->setPos(QPointF(_elbows[i]->x()+dx, _elbows[i]->y()+dy));
        _elbows[i]->getSegment(0)->enclosePathInElbows();
    }

    this->updateModel();
}


/**
 * @brief TransitionGraphic::handleParentStateGraphicResized
 * @param oldBox
 * @param newBox
 * @param corner
 *
 * will update the position of every anchor when a transition graphic's parent state box is being resized
 * oldBox and newBox represent the before and after rectangles of the state box
 *
 * SLOT
 * connect in SCGraphicsView
 *
 * connect(sbg, SIGNAL(stateBoxResized), transitionGraphic, SLOT(handleparentstategraphic resized))
 *
 */
void TransitionGraphic::handleParentStateGraphicResized(QRectF oldBox, QRectF newBox, int corner)
{
    qreal newWidth = newBox.width();
    qreal newHeight = newBox.height();
    qreal oldWidth = oldBox.width();
    qreal oldHeight = oldBox.height();
    qreal scaleX = newWidth/oldWidth;
    qreal scaleY = newHeight/oldHeight;


    //qDebug() << "handleParentStateGraphicResized: x, y: " << scaleX << ", "<<scaleY;
    //_anchors[0]->setPos(_anchors[0]->pos()-point);
    //_anchors[0]->setPos(_anchors[0]->pos()*scale);

    switch(_anchors[0]->getSnappedSide())
    {
    // the anchor only needs to modify its x position
    case NORTH:
        _anchors[0]->setPos(QPointF(_anchors[0]->x()*scaleX, _anchors[0]->y()));
        break;
    case SOUTH:
        _anchors[0]->setPos(QPointF(_anchors[0]->x(), _anchors[0]->y()*scaleY));
        _anchors[0]->setPos(QPointF(_anchors[0]->x()*scaleX, _anchors[0]->y()));
        break;

     // modify x and y by scale
     case EAST:
        _anchors[0]->setPos(QPointF(_anchors[0]->x()*scaleX, _anchors[0]->y()));
        _anchors[0]->setPos(QPointF(_anchors[0]->x(), _anchors[0]->y()*scaleY));
        break;

        // the anchor only needs to modify its y position
     case WEST:
        _anchors[0]->setPos(QPointF(_anchors[0]->x(), _anchors[0]->y()*scaleY));
        break;
    }

    QPointF mtsTL = mapToScene(oldBox.topLeft());
    QPointF nmtsTL = mapToScene(newBox.topLeft());
    QPointF diff = mtsTL - nmtsTL;
    qreal dx = diff.x();
    qreal dy = diff.y();

    // make other elbows stay in place
    // and update their line segment hover boxes
    for(int i = 1 ; i < _elbows.count();i++)
    {
        _elbows[i]->setPos(QPointF(_elbows[i]->x()+dx, _elbows[i]->y()+dy));
        _elbows[i]->getSegment(0)->enclosePathInElbows();
    }

    this->updateModel();
}


/**
 * @brief TransitionGraphic::handleGrandParentTargetStateGraphicResized
 * @param oldBox
 * @param newBox
 * @param corner
 *essentially does the same thing as handleParentStateGraphicMoved for anchor elbows but with the offset adjustments inverted
 *
 */
void TransitionGraphic::handleGrandParentTargetStateGraphicResized(QRectF oldBox, QRectF newBox, int corner)
{
    qreal xDiff = (newBox.x() - oldBox.x());
    qreal yDiff = (newBox.y() - oldBox.y());

    QPointF point = QPointF(xDiff,yDiff);
    QPointF location = _anchors[1]->pos();
    location+=point;
    _anchors[1]->setPos(location);
    updateLineSegments(_anchors[1]);
    this->updateModel();
}

/**
 * @brief TransitionGraphic::handleTargetStateGraphicResized
 * @param oldBox
 * @param newBox
 * @param corner
 *
 * Called by the target statebox when it is resized to reposition the sink anchor and keep it snapped to the edge
 *
 * uses the old box and new box of the state being changed to calculate relative position offsets to update the sink anchor's position
 *
 */
void TransitionGraphic::handleTargetStateGraphicResized(QRectF oldBox, QRectF newBox, int corner)
{
    // old box and new box are scene mapped.

    //qDebug() << "Box:\t" << oldBox << "\t-->\t" << newBox;

    qreal buffer = 0;
    qreal xScale = (newBox.width()-buffer)/(oldBox.width()-buffer);
    qreal yScale = (newBox.height()-buffer)/(oldBox.height()-buffer);

    qreal xDiff = (newBox.x() - oldBox.x());
    qreal yDiff = (newBox.y() - oldBox.y());

    qreal widthDiff = newBox.width() - oldBox.width();
    qreal heightDiff = newBox.height() - oldBox.height();

    QPointF anchorOnBox;
    qreal newDistToOrigin, dx, dy, updatedNewDistToOrigin;


    // map the anchor
    anchorOnBox = this->mapToItem(_targetStateGraphic, _anchors[1]->pos());

    int side = _anchors[1]->getSnappedSide();

    if(side == NORTH || side == SOUTH)
    {
        if(corner == NORTHEAST)
        {
            // get the delta scaling and multiply it by the anchor's x pos
            dx = (xScale-1.0)*(anchorOnBox.x());        // xScale-1.0 = (newWidth - oldWidth) / oldWidth = percentage change

            if(side==SOUTH)
                dy = 0;
            else
                dy = yDiff;

            QPointF location = _anchors[1]->pos();
            QPointF point = QPointF(dx, dy);

            location+=point;
            _anchors[1]->setPos(location);
        }
        else if (corner == NORTHWEST)
        {
            newDistToOrigin = newBox.width() - anchorOnBox.x();     // find the distance from the anchor x position to the anchored corner
            updatedNewDistToOrigin = xScale * newDistToOrigin;      // find the new anchor x distance based on the scaling
            dx = updatedNewDistToOrigin - newDistToOrigin;          // the delta x is how much the anchor needs to move between these points

            if(side==SOUTH)
                 dy = 0;
            else
                 dy = yDiff;

            QPointF location = _anchors[1]->pos();
            QPointF point = QPointF(-1.0*dx, dy);

            location+=point;

            _anchors[1]->setPos(location);
        }
        else if(corner == SOUTHWEST)
        {
            newDistToOrigin = newBox.width() - anchorOnBox.x();     // find the distance from the anchor x position to the anchored corner
            updatedNewDistToOrigin = xScale * newDistToOrigin;      // find the new anchor x distance based on the scaling
            dx = updatedNewDistToOrigin - newDistToOrigin;          // the delta x is how much the anchor needs to move between these points

            if(side==SOUTH)
                 dy = heightDiff;
            else
                 dy = 0;

            QPointF location = _anchors[1]->pos();
            QPointF point = QPointF(-1.0*dx, dy);

            location+=point;

            _anchors[1]->setPos(location);
        }
        else if(corner == SOUTHEAST)
        {
            // get the delta scaling and multiply it by the anchor's x pos
            dx = (xScale-1.0)*(anchorOnBox.x());        // xScale-1.0 = (newWidth - oldWidth) / oldWidth = percentage change

            if(side==SOUTH)
                dy = heightDiff;
            else
                dy = 0;

            QPointF location = _anchors[1]->pos();
            QPointF point = QPointF(dx, dy);

            location+=point;
            _anchors[1]->setPos(location);
        }
    }
    else if(side==EAST||side==WEST)
    {
        if(corner == NORTHEAST)
        {
            newDistToOrigin = newBox.height() - anchorOnBox.y();     // find the distance from the anchor x position to the anchored corner
            updatedNewDistToOrigin = yScale * newDistToOrigin;      // find the new anchor x distance based on the scaling
            dy = updatedNewDistToOrigin - newDistToOrigin;          // the delta x is how much the anchor needs to move between these points

            if(side==WEST)
                 dx = 0;
            else
                 dx = widthDiff;

            QPointF location = _anchors[1]->pos();
            QPointF point = QPointF(dx, -1.0*dy);

            location+=point;

            _anchors[1]->setPos(location);
        }
        else if (corner == NORTHWEST)
        {
            newDistToOrigin = newBox.height() - anchorOnBox.y();     // find the distance from the anchor x position to the anchored corner
            updatedNewDistToOrigin = yScale * newDistToOrigin;      // find the new anchor x distance based on the scaling
            dy = updatedNewDistToOrigin - newDistToOrigin;          // the delta x is how much the anchor needs to move between these points

            if(side==WEST)
                 dx = xDiff;
            else
                 dx = 0;

            QPointF location = _anchors[1]->pos();
            QPointF point = QPointF(dx, -1.0*dy);

            location+=point;

            _anchors[1]->setPos(location);
        }
        else if(corner == SOUTHWEST)
        {
            // get the delta scaling and multiply it by the anchor's x pos
            dy = (yScale-1.0)*(anchorOnBox.y());        // xScale-1.0 = (newWidth - oldWidth) / oldWidth = percentage change

            if(side==WEST)
                dx = xDiff;
            else    // east
                dx = 0;

            QPointF location = _anchors[1]->pos();
            QPointF point = QPointF(dx, dy);

            location+=point;
            _anchors[1]->setPos(location);
        }
        else if(corner == SOUTHEAST)
        {
            // get the delta scaling and multiply it by the anchor's x pos
            dy = (yScale-1.0)*(anchorOnBox.y());        // xScale-1.0 = (newWidth - oldWidth) / oldWidth = percentage change

            if(side==WEST)
                dx = 0;
            else    // east
                dx = widthDiff;

            QPointF location = _anchors[1]->pos();
            QPointF point = QPointF(dx, dy);

            location+=point;
            _anchors[1]->setPos(location);
        }
    }
    else
    {
        qDebug() <<"ERROR transitiongraphic::handletargetstategraphicresize unexpected side: "<<side;
    }

    // update the line segment hover box too
    _anchors[1]->getSegment(0)->enclosePathInElbows();
    this->updateModel();
}

/**
 * @brief TransitionGraphic::handleStateGraphicMoved
 * @param point
 *
 * connect in scgraphicsview.cpp
 *
 * update all elbows except for the source anchor so that every elbow appears to be static
 *
 * this function is called when the parent state is moved
 */
void TransitionGraphic::handleParentStateGraphicMoved(QPointF offset)
{
//    QPointF location = _anchors[1]->pos();
//    location-= offset;
//    _anchors[1]->setPos(location);
//    _anchors[1]->getSegment(0)->enclosePathInElbows();
//    this->updateModel();

    for(int i = 1 ; i < _elbows.count(); i++)
    {
        QPointF location = _elbows[i]->pos();
        location-=offset;
        _elbows[i]->setPos(location);
        _elbows[i]->getSegment(0)->enclosePathInElbows();
    }
    this->updateModel();
}

/**
 * @brief TransitionGraphic::handleParentStateGraphicReleased
 *
 * SLOT
 *
 * connect in SCGraphicsView::connectTransition
 *
 *
 *
 */
void TransitionGraphic::handleParentStateGraphicReleased()
{
    qDebug() << "TransitionGraphic::handleParentStateGraphicReleased";
    for(int i = 0 ; i < _elbows.size()-1; i++)
    {
        _elbows.at(i)->straightenLines();
    }
//    _anchors[0]->straightenLines();
    this->updateModel();
}


void TransitionGraphic::handleTargetStateGraphicReleased()
{
    _anchors[1]->straightenLines();
    this->updateModel();
}

void TransitionGraphic::handleParentStateGraphicResizedReleased()
{
    for(int i = 0 ; i < _elbows.size()-1; i++)
    {
        _elbows.at(i)->straightenLines();
    }
//    _anchors[0]->straightenLines();
    this->updateModel();
}

void TransitionGraphic::handleTargetStateGraphicResizedReleased()
{
    _anchors[1]->straightenLines();
    this->updateModel();
}


/**
 * @brief TransitionGraphic::handleTargetStateGraphicMoved
 * @param point
 * connect in scgraphicsview.cpp
 *
 * when the parent or target stateboxgraphic is moved, the anchor associated with the target graphic will be updated
 * automatically.
 *
 * in addition, if the target state box is moved, all elbows except the source elbow will be repositioned with the target.
 *
 * this function is for when the target State is dragged around
 */
void TransitionGraphic::handleTargetStateGraphicMoved(QPointF point)
{
    // point will be the vector

//    for(int i = 1; i < this->_elbows.size(); i++)
//    {
//        ElbowGrabber* elb = _elbows.at(i);
//        QPointF location = elb->pos();
//        location+=point;
//        elb->setPos(location);
//        updateLineSegments(elb);
//    }
//    this->updateModel();

    QPointF location = _anchors[1]->pos();
    location+=point;
    _anchors[1]->setPos(location);
    updateLineSegments(_anchors[1]);
    this->updateModel();
}


/**
 * @brief setCurrentlyHoveredSegment
 */
void TransitionGraphic::setCurrentlyHoveredLineSegment(LineSegmentGraphic* seg)
{
    _hoveredLine = seg;
}

void TransitionGraphic::clearCurrentlyHoveredLineSegment()
{
    _hoveredLine = NULL;
}

bool TransitionGraphic::isLineSegmentCurrentlyHovered()
{
    return (_hoveredLine!=NULL);
}

LineSegmentGraphic* TransitionGraphic::getCurrentlyHoveredLineSegment()
{
    return _hoveredLine;
}

void TransitionGraphic::setCurrentlyHoveredElbow(ElbowGrabber * elb)
{
    _hoveredElbow = elb;
}
void TransitionGraphic::clearCurrentlyHoveredElbow()
{
    _hoveredElbow = NULL;
}
bool TransitionGraphic::isElbowCurrentlyHovered()
{
    return (_hoveredElbow!=NULL);
}
ElbowGrabber* TransitionGraphic::getCurrentlyHoveredElbow()
{
    return _hoveredElbow;
}


/**
 * @brief TransitionGraphic::printInfo
 * prints the transition graphic's parent and target states
 */
void TransitionGraphic::printInfo(void)
{
    qDebug() << "Transition graphic from: " << this->parentItemAsStateBoxGraphic()->objectName() << " to: " << _targetStateGraphic->objectName();
}

void TransitionGraphic::deleteHoveredElbow()
{

    if(_isCurrentlyDeleting)
    {
        qDebug() << "Halt. an elbow is currently being deleted.";
        return;
    }



    /*
                    delete these
                    v  v
     O------O-------X~~~~~~~O-------O

     O------O------->       ]O-------O
     hook up the left line to point to the next elbow
     hook up the right elbow to point to the left line

     */
    // delete this elbow and delete the elbow's second segment
    ElbowGrabber* elb = this->getCurrentlyHoveredElbow();

    if(elb && !(elb->isAnchor()))  // check the elbow exists and that is not an anchor elbow
    {

        _isCurrentlyDeleting = true;


        LineSegmentGraphic* deadLine = elb->getSegment(1);
        LineSegmentGraphic* leftLine = elb->getSegment(0);

        if(!deadLine)
            return;

        ElbowGrabber* rightElb = deadLine->getElbow(1);

        /*
        if(!deadLine || !leftLine)
        {
            qDebug() << "Error: elbow deletion object does not exist!";
            return;
        }
        if(!rightElb)
        {
            qDebug() << "Error: elbow deletion object does not exist!";
            return;
        }
        */

        leftLine->setElbowAt(1, rightElb);
        rightElb->setSegmentAt(0, leftLine);

        int index = _elbows.indexOf(elb);
        _elbows.removeAt(index);

        index = _lineSegments.indexOf(deadLine);
        _lineSegments.removeAt(index);

        leftLine->enclosePathInElbows();    // redraw the line segment's hover box

        elb->forceHoverLeaveEvent();
        delete elb;
        delete deadLine;

        this->updateModel();

        _isCurrentlyDeleting = false;
    }


}

/**
 * @brief TransitionGraphic::createNewElbow
 * create a new elbow on the currently hovered line segment, based on where the mouse is currently
 * a new line segment and elbow grabber object is created and added to the transition graphic
 */
void TransitionGraphic::createNewElbow()
{
    //static bool isCreating = false;
    LineSegmentGraphic* splitLine = this->getCurrentlyHoveredLineSegment();  // segment that is being split
    if(splitLine && splitLine->isHovered() && _hasMovedSinceCreatingElbow)
    {
        _hasMovedSinceCreatingElbow = false;        // prevents creating two elbows ontop of each other
        //isCreating = true;


    //_hovered->printInfo();
    ElbowGrabber* elbMid = new ElbowGrabber(this, this->mapFromScene(_mouseController->getX(),_mouseController->getY()), _keyController);  // maps the scene based mouse coordinates to create a new elbow at the cursor location
    elbMid->installSceneEventFilter(this);
    elbMid->setAcceptHoverEvents(true);

    //elb->setPos(this->_mouseController->getX(),_mouseController->getY());


    ElbowGrabber* elbEnd = splitLine->getElbow(1);

    // update the split line's end elbow to the new one
    splitLine->setElbowAt(1, elbMid);
    splitLine->enclosePathInElbows();

    // create the new line segment that starts from the new elbow and goes to the old end
    LineSegmentGraphic* newLine = new LineSegmentGraphic(elbMid, elbEnd, this, _keyController);
    newLine->setZValue(elbMid->zValue()-1); // set the line segment behind its elbow
    newLine->installSceneEventFilter(this);
    newLine->setAcceptHoverEvents(true);


    // update the old elbows sockets
    //  start       mid        end
    // [  A x]----[x B y]----[y C  ]
    elbEnd->setSegmentAt(0, newLine);
    elbMid->setSegmentAt(0, splitLine);
    elbMid->setSegmentAt(1, newLine);

    // keep the elbow placements in order
    _elbows.insert(_elbows.indexOf(elbEnd), elbMid);    // insert the new elbow before the end elbow



    // keep the line segments in order
   /* if(_lineSegments.count()==1)
        _lineSegments.append(newLine);
    else
        _lineSegments.insert(_lineSegments.indexOf(splitLine)+1,newLine);*/

    _lineSegments.append(newLine);
    //isCreating = false;

    this->updateModel();
    }
}

/**
 * @brief TransitionGraphic::handleKeyPressEvent
 * @param key
 * connection made when a line segment of this transition graphic is hovered
 */
void TransitionGraphic::handleLineSegmentKeyPressEvent(int key)
{
    qDebug() << "Transition Graphic Key Press: " << key;
    if(key==Qt::Key_N)
    {
        qDebug() << "Creating New Elbow at pos: ";
        this->createNewElbow();
    }

// TODO elbow deletion

}

void TransitionGraphic::handleElbowKeyPressEvent(int key)
{
    if(key == Qt::Key_D)
    {
        qDebug() << "Deleting Currently Hovered Elbow";
        this->deleteHoveredElbow();
    }
}


/**
 * @brief TransitionGraphic::updateModel
 * Updates the transition graphic's data model if it exists
 *
 */
void TransitionGraphic::updateModel()
{



    if ( _transitionModel )                                 // check if the data model for this transition exists
    {
        // use elbows to create the path attribute
        QList<QPointF> path;
        for( int  i = 0; i < _elbows.count(); i++)
        {
            path.append(_elbows.at(i)->pos());

        }
        TransitionPathAttribute * pathAttr = dynamic_cast<TransitionPathAttribute *> (_transitionModel->attributes.value("path"));

        pathAttr->setValue(path);   // update the path values of _transitionModel, the data model object for this transition
    }
}



void TransitionGraphic::setTextPos(QPointF position)
{
    _eventText->setPos(position);
}

void TransitionGraphic::setTextSize(qreal w, qreal h)
{
    QPoint size(w,h);
    _eventText->setSize(size);
}

/**
 * @brief TransitionGraphic::handleAttributeChanged
 * @param tsa
 * SLOT
 * connect in SCGraphicsView::connectTransition
 *
 * when the data model is updated, change the event text in the graphics view
 */
void TransitionGraphic::handleAttributeChanged(TransitionStringAttribute *tsa)
{
    _eventText->setText(tsa->asString());
}

void TransitionGraphic::getClosestSides(int* sourceSide, int* targetSide)
{

    QPointF sourcePos = (this->parentItemAsStateBoxGraphic()->getVisibleCenter());
    QPointF targetPos = (_targetStateGraphic->getVisibleCenter());

    //qDebug() << "source Pos " << sourcePos << "target Pos " << targetPos;

    double deltaX = fabs (sourcePos.x() - targetPos.x());
    double deltaY = fabs (sourcePos.y() - targetPos.y());

    //qDebug() << "dx " << deltaX << "dy " << deltaY;

    if ( deltaX > deltaY)
    {
        if ( sourcePos.x() > targetPos.x() )
        {      
            *sourceSide = WEST;
            *targetSide = EAST;
        }
        else
        {
            *sourceSide = EAST;
            *targetSide = WEST;
        }
    }
    else
    {
        if ( sourcePos.y() > targetPos.y() )
        {
            *sourceSide = NORTH;
            *targetSide = SOUTH;
        }
        else
        {
            *sourceSide = SOUTH;
            *targetSide = NORTH;
        }
    }
}

void TransitionGraphic::paint (QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{

}

QRectF TransitionGraphic::boundingRect() const
{
    return QRectF();
}

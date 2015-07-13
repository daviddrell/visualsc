#include "transitiongraphic.h"
#include "scstate.h"
#include <QDebug>

TransitionGraphic::TransitionGraphic(StateBoxGraphic *parentGraphic, StateBoxGraphic *targetGraphic, SCTransition * t, KeyController * keys, MouseController* mouse) :
    QGraphicsObject(NULL),
    _transitionDM(t),
    _lineSegments(),
    _elbows(),
    _targetStateGraphic(targetGraphic),
    _keyController(keys),
    _mouseController(mouse)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, false);
    this->setParentItem(parentGraphic);     // the source state will be this transition graphic's parent item
    //this->acceptHoverEvents();            // a Transition graphic has no dimensions, its invididual elbows and line segments will have hover events
    //this->setParent(targetGraphic);

    TransitionAttributes::TransitionPathAttribute * p =
            dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (  t->attributes.value("path"));

    QList<QPointF> pointList = p->asQPointFList();
  /*  qDebug() << "Printing Point List of size: " <<pointList.count();
    for(int i = 0; i < pointList.count();i++)
        qDebug() << pointList.at(i);

*/
    // populate the transition graphic with elbows.
    // if this is a new transition graphic, then create two elbows
    // otherwise this is a transition graphic being loaded in, so add elbows according to its path list
    if(pointList.count() < 2 && targetGraphic != NULL )
    {
        // create two new elbows, these are the anchors attached to the source and sink state
        _anchors[0] = new ElbowGrabber(this);
        _anchors[0]->installSceneEventFilter(this);
        _elbows.append(_anchors[0]);

        _anchors[1] = new ElbowGrabber(this);
        _anchors[1]->installSceneEventFilter(this);
        _elbows.append(_anchors[1]);

        LineSegmentGraphic* segment = new LineSegmentGraphic(_anchors[0], _anchors[1], this, _keyController);
        segment->setAcceptHoverEvents(true);                    // allow the elbow to be hovered


        _anchors[0]->setSegmentOne(NULL);
        _anchors[0]->setSegmentTwo(segment);

        _anchors[1]->setSegmentOne(segment);
        _anchors[1]->setSegmentTwo(NULL);

        _anchors[1]->setTerminal(true);                         // set the last elbow grabber as the terminator
        _anchors[1]->setPaintStyle(ElbowGrabber::kArrowHead);   // set the end of the transition to an arrowhead

        _anchors[0]->setAnchor(true);
        _anchors[1]->setAnchor(true);

        connect(_anchors[0],SIGNAL(anchorMoved(QPointF)),parentGraphic,SLOT(handleTransitionLineStartMoved(QPointF)));  // state box will handle snapping the source elbow/anchor to its border instead of standard movement
        connect(_anchors[1],SIGNAL(anchorMoved(QPointF)),_targetStateGraphic,SLOT(handleTransitionLineEndMoved(QPointF)));  // state box will handle snapping the source elbow/anchor to its border instead of standard movement

        //qDebug() << "hooking anchor to state graphic: " << _targetStateGraphic->objectName();


        // set the anchor positions by snapping them to their source and sink state boxes
        int sourceSide=0;
        int targetSide=0;

        // get the mid point of the source and sink on the sides that are nearest to each other
        getClosestSides( & sourceSide, & targetSide);
        QPointF sourceAnchor = (this->parentItemAsStateBoxGraphic()->getSideCenterPointInSceneCoord(sourceSide));
        QPointF targetAnchor = (_targetStateGraphic->getSideCenterPointInSceneCoord(targetSide));

        // use the statebox function handleTransitionLineStartMoved & handleTransitionLineEndMoved to update the anchors to this position and snap them to the box border.
        emit _anchors[0]->anchorMoved(sourceAnchor);
        emit _anchors[1]->anchorMoved(targetAnchor);


        //elbOne->setVisible(false);
       // elbOne->setVisible(false);


        _lineSegments.append(segment);
    }
    else // load the point list and create the elbows and line segments
    {
        // add every point as an elbow
        for(int i = 0; i < pointList.count(); i++)
        {
            ElbowGrabber* elb;
            elb = new ElbowGrabber(this, pointList.at(i));
            elb->installSceneEventFilter(this);                 // transition graphic will handle elbow events
           // elb->setAcceptHoverEvents(true);                    // allow the elbow to be hovered

            _elbows.append(elb);
        }


        // create a line segment in between every elbow
        for(int i = 0; i < _elbows.count()-1; i++)
        {
            ElbowGrabber* elbOne = _elbows.at(i);
            ElbowGrabber* elbTwo = _elbows.at(i+1);

            LineSegmentGraphic* segment = new LineSegmentGraphic(elbOne, elbTwo, this, _keyController);
            segment->setAcceptHoverEvents(true);                    // allow the line segment to be hovered
            //segment->installSceneEventFilter(this);

           // connect(segment, SIGNAL(startEndMoved(QPointF)), parentGraphic, SLOT(handleTransitionLineStartMoved(QPointF)));
           // connect(segment, SIGNAL(updateModel()), this, SLOT(updateModel()));


            if(i==0 || i== _elbows.count()-2)   // anchor elbows at the end and beginning
            {
                if(i==0){                       // source anchor
                    elbOne->setSegmentOne(NULL);
                    elbOne->setSegmentTwo(segment);
                }
                if(i==_elbows.count()-2){      // sink anchor
                    elbTwo->setSegmentOne(segment);
                    elbTwo->setSegmentTwo(NULL);
                }
            }
            else                                // standard elbow that connects two line segments
            {
                elbOne->setSegmentTwo(segment);
                elbTwo->setSegmentOne(segment);
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

        connect(_anchors[0],SIGNAL(anchorMoved(QPointF)),parentGraphic,SLOT(handleTransitionLineStartMoved(QPointF)));  // state box will handle snapping the source elbow/anchor to its border instead of standard movement
        connect(_anchors[1],SIGNAL(anchorMoved(QPointF)),_targetStateGraphic,SLOT(handleTransitionLineEndMoved(QPointF)));  // state box will handle snapping the source elbow/anchor to its border instead of standard movement
        //qDebug() << "hooking anchor to state graphic: " << _targetStateGraphic->objectName();
    }
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
        elbow->setPos(mapFromScene(newPos));

    // update the polygon of the segments
    updateLineSegments(elbow);
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
    if(one) // check if object exists
    {
        // update start and end?
        one->enclosePathInElbows();
        //qDebug() << "updated one";
    }
    LineSegmentGraphic* two = elbow->getSegment(1);
    if(two) // check if object exists
    {
        // update start and end?
        two->enclosePathInElbows();
        //qDebug() << "updated two";
    }
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
    // qDebug() << "transition graphic scene event filter";
    // qDebug() << " QEvent == " + QString::number(event->type()) << " " << count++;
    // qDebug() << "QGraphicsItem Scene Event " << watched->
    ElbowGrabber * elbow = dynamic_cast<ElbowGrabber *>(watched);
    if ( elbow == NULL) return false; // not expected to get here

    QGraphicsSceneMouseEvent * mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if ( mevent == NULL)
    {
        // this is not one of the mouse events we are interrested in
        return false;
    }

    switch (event->type() )
    {
        // if the mouse went down, record the x,y coords of the press, record it inside the corner object
    case QEvent::GraphicsSceneMousePress:
        {
        //qDebug() << "mouse press";
            elbow->setMouseState(ElbowGrabber::kMouseDown);

            QPointF scenePosition =  elbow->mapToScene(mevent->pos());
            elbow->mouseDownY = scenePosition.y();
            elbow->mouseDownX = scenePosition.x();



            //_cornerGrabbed = true;
            qDebug() << "Corner Position: " << elbow->mouseDownX<<" ," << elbow->mouseDownY;

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


    case QEvent::GraphicsSceneMouseRelease:
        {
            //qDebug() << "mouse release";
            //_cornerGrabbed = false;

            elbow->setMouseState(ElbowGrabber::kMouseReleased);
            updateModel();                  // update the transition datamodel for this path
            elbow->forceHoverLeaveEvent();  // old elbow hover events will trigger for mouse press and releases
        }
        break;


    case QEvent::GraphicsSceneMouseMove:
        {
       //     qDebug() << "mouse moving";
            elbow->setMouseState(ElbowGrabber::kMouseMoving );
        }
        break;


    default:
        // we dont care about the rest of the events
        return false;
        break;
    }


    if ( elbow->getMouseState() == ElbowGrabber::kMouseMoving )
    {
        // give the scene scope position of the mouse and update the elbow that triggered the scene event
        this->updateElbow(elbow->mapToScene(mevent->pos()), elbow);
        //this->updateElbow(this->mapFromItem(elbow, mevent->pos()), elbow);
        //this->update();
    }

    return true;// true => do not send event to watched - we are finished with this event

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
}

void TransitionGraphic::handleParentStateGraphicResized(QRectF oldBox, QRectF newBox)
{
    int buffer = 12;
    qreal newWidth = newBox.width()-buffer;
    qreal newHeight = newBox.height()-buffer;
    qreal oldWidth = oldBox.width()-buffer;
    qreal oldHeight = oldBox.height()-buffer;
    qreal scaleX = newWidth/oldWidth;
    qreal scaleY = newHeight/oldHeight;


    qDebug() << "handleParentStateGraphicResized: x, y: " << scaleX << ", "<<scaleY;
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

     // the anchor only needs to modify its y position
     case EAST:
        _anchors[0]->setPos(QPointF(_anchors[0]->x()*scaleX, _anchors[0]->y()));
        _anchors[0]->setPos(QPointF(_anchors[0]->x(), _anchors[0]->y()*scaleY));
        break;
     case WEST:
        _anchors[0]->setPos(QPointF(_anchors[0]->x(), _anchors[0]->y()*scaleY));
        break;
    }

    //QPointF diff = mapToScene(oldBox - newBox);

    QPointF mtsTL = mapToScene(oldBox.topLeft());
    QPointF nmtsTL = mapToScene(newBox.topLeft());
  //  qDebug() << "old xy: " << oldBox.x()<< ", " << oldBox.y();
//qDebug() << "new xy: " << newBox.x()<< ", " << newBox.y();
qDebug() << "old "<<mtsTL;
qDebug() << "new "<<nmtsTL;
QPointF diff = mtsTL - nmtsTL;
qreal dx = diff.x();
qreal dy = diff.y();
    switch(_anchors[1]->getSnappedSide())
    {
    // the anchor only needs to modify its x position
    case NORTH:
    case SOUTH:
       // _anchors[1]->setPos(QPointF(_anchors[1]->x()+dx, _anchors[1]->y()));
        break;

     // the anchor only needs to modify its y position
     case EAST:
     case WEST:
       // _anchors[1]->setPos(QPointF(_anchors[1]->x(), _anchors[1]->y()+dy));
        break;
    }

 _anchors[1]->setPos(QPointF(_anchors[1]->x()+dx, _anchors[1]->y()+dy));
   // _anchors[0]->setPos(QPointF(_anchors[0]->x()*scaleX, _anchors[0]->y()*scaleY));
}

void TransitionGraphic::handleTargetStateGraphicResized(qreal scaleX, qreal scaleY)
{
   // qDebug() << "handleTargetStateGraphicResized: " << scale;
    _anchors[1]->setPos(_anchors[1]->pos()*scaleX);
}

/**
 * @brief TransitionGraphic::handleStateGraphicMoved
 * @param point
 *
 * connect in scgraphicsview.cpp
 *
 * when the parent or target stateboxgraphic is moved, the anchor associated with the target graphic will be updated
 * automatically.
 *
 * only one anchor needs to be updated because all children of a state are already updated automatically.
 *
 * this function is called when the parent state is moved
 */
void TransitionGraphic::handleParentStateGraphicMoved(QPointF point)
{

    // point will be the coorindate of the statebox after it moved.
  //  QPointF difference =
  //  qDebug()<<"the source state graphic has moved. must update the transition anchor" << point;
    // point will be the difference.
    //emit _anchors[1]->anchorMoved(_anchors[1]->mapToScene(_anchors[1]->pos()) + point);
    QPointF location = _anchors[1]->pos();
    location-=point;
    _anchors[1]->setPos(location);
    updateLineSegments(_anchors[1]);
}

/**
 * @brief TransitionGraphic::handleTargetStateGraphicMoved
 * @param point
 * connect in scgraphicsview.cpp
 *
 * when the parent or target stateboxgraphic is moved, the anchor associated with the target graphic will be updated
 * automatically.
 *
 * only one anchor needs to be updated because all children of a state are already updated automatically.
 *
 * this function is for when the target State is dragged around
 */
void TransitionGraphic::handleTargetStateGraphicMoved(QPointF point)
{

    // point will be the coorindate of the statebox after it moved.
  //  QPointF difference =
  //  qDebug()<<"the target state graphic has moved. must update the transition anchor" << point;


    // point will be the difference.


    //emit _anchors[1]->anchorMoved(_anchors[1]->mapToScene(_anchors[1]->pos()) + point);
    QPointF location = _anchors[1]->pos();
    location+=point;
    _anchors[1]->setPos(location);
    updateLineSegments(_anchors[1]);
}


/**
 * @brief setCurrentlyHoveredSegment
 */
void TransitionGraphic::setCurrentlyHoveredSegment(LineSegmentGraphic* seg)
{
    _hovered = seg;
}

void TransitionGraphic::clearCurrentlyHoveredSegment()
{
    _hovered = NULL;
}

bool TransitionGraphic::isCurrentlyHovered()
{
    return (_hovered!=NULL);
}

LineSegmentGraphic* TransitionGraphic::getCurrentlyHoveredSegment()
{
    return _hovered;
}

/**
 * @brief TransitionGraphic::printInfo
 * prints the transition graphic's parent and target states
 */
void TransitionGraphic::printInfo(void)
{
    qDebug() << "Transition graphic from: " << this->parentItemAsStateBoxGraphic()->objectName() << " to: " << _targetStateGraphic->objectName();
}

/**
 * @brief TransitionGraphic::createNewElbow
 * create a new elbow on the currently hovered line segment, based on where the mouse is currently
 * a new line segment and elbow grabber object is created and added to the transition graphic
 */
void TransitionGraphic::createNewElbow()
{
    //_hovered->printInfo();
    ElbowGrabber* elbMid = new ElbowGrabber(this, this->mapFromScene(_mouseController->getX(),_mouseController->getY()));  // maps the scene based mouse coordinates to create a new elbow at the cursor location
    elbMid->installSceneEventFilter(this);
    elbMid->setAcceptHoverEvents(true);

    //elb->setPos(this->_mouseController->getX(),_mouseController->getY());


    LineSegmentGraphic* splitLine = this->getCurrentlyHoveredSegment();  // segment that is being split
    ElbowGrabber* elbEnd = splitLine->getElbow(1);

    // update the split line's end elbow to the new one
    splitLine->setElbowAt(1, elbMid);

    // create the new line segment that starts from the new elbow and goes to the old end
    LineSegmentGraphic* newLine = new LineSegmentGraphic(elbMid, elbEnd, this, _keyController);
    newLine->setAcceptHoverEvents(true);


    // update the old elbows sockets
    //  start       mid        end
    // [  A x]----[x B y]----[y C  ]
    elbEnd->setSegmentOne(newLine);
    elbMid->setSegmentOne(splitLine);
    elbMid->setSegmentTwo(newLine);

    // keep the elbow placements in order
    _elbows.insert(_elbows.indexOf(elbEnd), elbMid);    // insert the new elbow before the end elbow



    // keep the line segments in order
   /* if(_lineSegments.count()==1)
        _lineSegments.append(newLine);
    else
        _lineSegments.insert(_lineSegments.indexOf(splitLine)+1,newLine);*/

    _lineSegments.append(newLine);
}

/**
 * @brief TransitionGraphic::handleKeyPressEvent
 * @param key
 * connection made when a line segment of this transition graphic is hovered
 */
void TransitionGraphic::handleKeyPressEvent(int key)
{
    qDebug() << "Transition Graphic Key Press: " << key;
    if(key==Qt::Key_N)
    {
        qDebug() << "Creating New Elbow at pos: ";
        this->createNewElbow();
    }
}

/**
 * @brief TransitionGraphic::updateModel
 * Updates the transition graphic's data model if it exists
 * converts
 */
void TransitionGraphic::updateModel()
{

    if ( _transitionDM )                                 // check if the data model for this transition exists
    {
        // use elbows to create the path attribute
        QList<QPointF> path;
        for( int  i = 0; i < _elbows.count(); i++)
            path.append(_elbows.at(i)->pos());

        TransitionAttributes::TransitionPathAttribute * pathAttr = dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (_transitionDM->attributes.value("path"));

        pathAttr->setValue(path);   // update the path values of _transitionDM, the data model object for this transition
    }
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
            *sourceSide = 3;
            *targetSide = 1;
        }
        else
        {
            *sourceSide = 1;
            *targetSide = 3;
        }
    }
    else
    {
        if ( sourcePos.y() > targetPos.y() )
        {
            *sourceSide = 0;
            *targetSide = 2;
        }
        else
        {
            *sourceSide = 2;
            *targetSide = 0;
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

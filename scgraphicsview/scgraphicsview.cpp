#include "scgraphicsview.h"
#include "sctransition.h"
#include "transitionattributes.h"
#include "selectablelinesegmentgraphic.h"
#include <QList>


SCGraphicsView::SCGraphicsView(QWidget *parentWidget, SCDataModel * dm) :
        QWidget (parentWidget),
        _scene(),
        _view(parentWidget),
        _dm(dm),
        _mapStateToGraphic()
{

    connect (_dm, SIGNAL(newStateSignal(SCState*)), this, SLOT(handleNewState(SCState*)));
    connect (_dm, SIGNAL(newTransitionSignal(SCTransition*)), this, SLOT(handleNewTransition(SCTransition*)));

    _view.setScene(& _scene);

    _view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    _view.show();
}

QGraphicsView * SCGraphicsView::getQGraphicsView()
{
    return & _view;
}


void SCGraphicsView::increaseSizeOfAllAncestors (SCState * state)
{
    SCState * parentState = dynamic_cast<SCState *>(state->parent());

    if ( parentState )
    {
        StateBoxGraphic * parentGraphic =   _mapStateToGraphic[parentState];

        if ( parentGraphic )
        {
            // has the size been set on the parent?
//            if ( ! parentState->hasBeenSized() )
//            {
//                // make sure the parent is big enough to hold all the states

//                QPoint sz;
//                parentGraphic->getSize(sz);
//                sz.setX( sz.x() + 120 );
//                parentGraphic->setSize( sz );

//            }

//            increaseSizeOfAllAncestors (parentState);
        }
    }

}


SCState * SCGraphicsView::lookUpTargetState(QString target)
{

//    QList<SCState *> states;
//    _dm->getAllStates(states);

//    QList<SCState *>::iterator i;
//    for (i = states.begin(); i != states.end(); ++i)
//    {
//        SCState *st = *i;
//        StateAttributes attr;
//        st->getAttributes(attr);
//        if (attr.name.asString() == target)
//            return st;
//    }

    return NULL;
}

void SCGraphicsView::handleNewTransition (SCTransition *t)
{
     // create a transition graphic

    SelectableLineSegmentGraphic * transGraphic  = 0;


    TransitionAttributes::TransitionPositionAttribute * pos =
            dynamic_cast<TransitionAttributes::TransitionPositionAttribute *> (  t->attributes.value("position"));
    QPointF position = pos->asQPointF();

    TransitionAttributes::TransitionPathAttribute * p =
            dynamic_cast<TransitionAttributes::TransitionPathAttribute *> (  t->attributes.value("path"));


    QList<QPointF> path = p->asQPointFList();

    if (  path.count() < 2  )
    {

        position = QPointF(10,10);

        transGraphic  = new SelectableLineSegmentGraphic(position,position, QPointF(position.x()  , position.y() + 15 ));
    }
    else if ( path.count() >= 2)
    {

        transGraphic  = new SelectableLineSegmentGraphic(path[0],
                                                         path[0],
                                                         path[1]);

    }


    // get the parent state graphic

    SCState *parentState = dynamic_cast<SCState *>(t->parent());

    StateBoxGraphic * parentGraphic =   _mapStateToGraphic[parentState];

    transGraphic->setZValue( parentGraphic->zValue() + 1 );
    transGraphic->setParentItem(parentGraphic);

}


void SCGraphicsView::handleNewState (SCState *newState)
{

    SCState * parentState = 0;
    StateBoxGraphic * parentGraphic =0;
    StateBoxGraphic * stateGraphic = 0;

    parentState = dynamic_cast<SCState *>(newState->parent());

    if (_mapStateToGraphic.contains(parentState))
    {
        parentGraphic =   _mapStateToGraphic[parentState];
    }

    // after the reference has been set through the construction
    // of the stateboxgraphic, the graphic and the model
    // will be linked internally. e.g. setting the size off the graphic
    // will set the size of the model and vice versa

    stateGraphic = new StateBoxGraphic( parentGraphic, newState);

    // quick look up of graphics from state model references

    _mapStateToGraphic.insert(newState, stateGraphic);


    if ( ! parentGraphic )
    {
        _scene.addItem(stateGraphic);
    }

    // make the parent bigger to hold this state
    // and since its bigger, make its parent bigger.....
    increaseSizeOfAllAncestors (newState);

}

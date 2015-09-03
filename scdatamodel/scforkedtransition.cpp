#include "scforkedtransition.h"
#include "scstate.h"

SCForkedTransition::SCForkedTransition():
    _eventTextBlock(new SCTextBlock()),
    _targetState(NULL)
{


    this->initialize();



}

void SCForkedTransition::initialize()
{
    // initialize the attributes
    // strings are empty
    TransitionStringAttribute * target = new TransitionStringAttribute (this, "target",QString());
    attributes.addItem(target);


    TransitionStringAttribute * tuid = new TransitionStringAttribute (this, "targetUid",QString());
    attributes.addItem(tuid);

    QUuid u=QUuid::createUuid();
    TransitionStringAttribute* uid = new TransitionStringAttribute(this, "forkUid", u.toString());
    attributes.addItem(uid);

    TransitionStringAttribute * comments = new TransitionStringAttribute (this, "comments",QString());
    attributes.addItem(comments);

    QList<QPointF> emptyPath;
    TransitionPathAttribute * path = new TransitionPathAttribute (this, QString("targetPath"),emptyPath);
    attributes.addItem(path);

    TransitionStringAttribute* event = new TransitionStringAttribute(this, "event", "fork event");
    attributes.addItem(event);
    this->setObjectName(event->asString());
}

QString SCForkedTransition::getEventName()
{
    return attributes.value("event")->asString();
}

QList<SCTransitionBranch*>* SCForkedTransition::getSourceBranches()
{
    return &_sourceBranches;
}

SCForkedTransition::~SCForkedTransition()
{

}

SCTextBlock* SCForkedTransition::getEventTextBlock()
{
    return _eventTextBlock;
}

SCState* SCForkedTransition::targetState()
{
    return _targetState;
}

/**
 * @brief SCForkedTransition::addSourceState
 * @param source
 * @return
 *
 * creates a new transition branch, given a SCState
 *
 */
SCTransitionBranch* SCForkedTransition::addSourceState(SCState *source)
{
    SCTransitionBranch* tb = new SCTransitionBranch(this, source);
    _sourceBranches.append(tb);
    return tb;
}

void SCForkedTransition::setTargetState(SCState *target)
{
    _targetState = target;
}


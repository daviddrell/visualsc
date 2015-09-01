#include "sctransitionbranch.h"
#include "scforkedtransition.h"
#include "scstate.h"

SCTransitionBranch::SCTransitionBranch(SCForkedTransition* group, SCState* source):
    SCItem(source),
    _group(group)

{
    _eventTextBlock = group->getEventTextBlock();
    initialize();
}

SCTransitionBranch::~SCTransitionBranch()
{

}

void SCTransitionBranch::initialize()
{
    // initialize the attributes
    TransitionStringAttribute * sourceUid = new TransitionStringAttribute (this, QString("sourceUid"),QString());
    attributes.addItem(sourceUid);

    QList<QPointF> emptyPath;
    TransitionPathAttribute * path = new TransitionPathAttribute (this, QString("path"),emptyPath);
    attributes.addItem(path);

    TransitionStringAttribute* connectToFinished = new TransitionStringAttribute(this, "connectToFinished", "false");
    attributes.addItem(connectToFinished);

//    this->addParentAttributes();
}

QString SCTransitionBranch::getEventName()
{
    return _group->attributes.value("event")->asString();
//    return attributes.value("event")->asString();
}

void SCTransitionBranch::addParentAttributes()
{
    QMapIterator<QString, IAttribute*> i(_group->attributes);
    while(i.hasNext())
    {
        attributes.addItem(i.value());
    }
}

SCState* SCTransitionBranch::parentAsSCState()
{
    return dynamic_cast<SCState*>(this->parent());
}

void SCTransitionBranch::deleteSafely()
{
    // unhook the transition from its parent and target before deleting it
    // detach from sink
    // detach from source

    emit markedForDeletion(this);
    this->deleteLater();
}

IAttributeContainer* SCTransitionBranch::getAttributes()
{
    return & attributes;
}

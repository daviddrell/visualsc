#include "sctransitionbranch.h"
#include "scforkedtransition.h"
#include "scstate.h"
#include <QDebug>

SCTransitionBranch::SCTransitionBranch(SCForkedTransition* group, SCState* source):
    SCItem(source),
    _group(group)

{
    _eventTextBlock = group->getEventTextBlock();
    initialize();
    this->setObjectName(group->getEventName());
}

SCTransitionBranch::~SCTransitionBranch()
{
    this->removeParentAttributes();
}

void SCTransitionBranch::initialize()
{
    qDebug () << "SCTransitionBranch::initialize()";
    // initialize the attributes
    TransitionStringAttribute * sourceUid = new TransitionStringAttribute (this, QString("sourceUid"),QString());
    attributes.addItem(sourceUid);

    QList<QPointF> emptyPath;
    TransitionPathAttribute * path = new TransitionPathAttribute (this, QString("path"),emptyPath);
    attributes.addItem(path);

    TransitionStringAttribute* connectToFinished = new TransitionStringAttribute(this, "connectToFinished", "false");
    attributes.addItem(connectToFinished);

    this->addParentAttributes();
}

QString SCTransitionBranch::getEventName()
{
//    return _group->attributes.value("event")->asString();
    return this->attributes.value("event")->asString();
}

/**
 * @brief SCTransitionBranch::setEventName
 * @param name
 * @return
 *
 * called by scformview when the event name is changed in the table
 *
 */
void SCTransitionBranch::setEventName(QString name)
{
    this->setObjectName(name);
    attributes.value("event")->setValue(name);
}

void SCTransitionBranch::removeParentAttributes()
{
    qDebug() << "SCTransitionBranch::removeParentAttributes()";
    QMapIterator<QString, IAttribute*> i(_group->attributes);
    while(i.hasNext())
    {
        QString attr = i.next().key();
        qDebug() << "removing key " << attr << " from branch attribute list";
        this->attributes.remove(attr);
    }

}

void SCTransitionBranch::addParentAttributes()
{
    qDebug() << "SCTransitionBranch::addParentAttributes()";
    QMapIterator<QString, IAttribute*> i(_group->attributes);
    while(i.hasNext())
    {
        IAttribute* attr = i.next().value();
        qDebug() << "attempting to add attribute: " << attr->key();
        this->attributes.addItem(attr);
    }
}

TransitionStringAttribute* SCTransitionBranch::getTransStringAttr(QString key)
{
    return dynamic_cast<TransitionStringAttribute*>(this->attributes.value(key));
}

SCState* SCTransitionBranch::targetState()
{
    return _group->targetState();
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

SCTextBlock* SCTransitionBranch::getEventTextBlock()
{
    return _eventTextBlock;
}

IAttributeContainer* SCTransitionBranch::getAttributes()
{
    return & attributes;
}

void SCTransitionBranch::handleTargetStateNameChanged(StateName * sn)
{
//    if(_group->attributes.value("target")->asString() != sn->asString())
//    {
//        qDebug() << "changing group target! " << sn->asString();
//        _group->attributes.value("target")->setValue(sn->asString());
//    }


    if(this->attributes.value("event")->asString() != sn->asString())
    {
        qDebug() << "changing target name for trans branch to " << sn->asString();
        this->attributes.value("event")->setValue(sn->asString());
    }
}

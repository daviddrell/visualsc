#include "fvitem.h"

FVItem::FVItem(SCItem* item, int type, CustomTreeWidgetItem* widget):
    _item(item),
    _type(type),
    _treeWidget(widget)
{

}

FVItem::FVItem(CustomTreeWidgetItem * widget):
    _treeWidget(widget)
{

}

FVItem::FVItem(SCItem * item, int type):
    _item(item),
    _type(type)
{

}



FVItem::~FVItem()
{

}

CustomTreeWidgetItem* FVItem::getTreeWidget()
{
    return _treeWidget;
}

/*
FVItem& FVItem::operator=(const SCState* state)
{
    _item = state;
}

FVItem& FVItem::operator= (const FVItem* item)
{
    _item = item;
}

*/


SCState* FVItem::getState()
{
    return dynamic_cast<SCState*>(_item);
}

SCTransition* FVItem::getTransition()
{
    return dynamic_cast<SCTransition*>(_item);
}

SCTransitionBranch* FVItem::getTransitionBranch()
{
    return dynamic_cast<SCTransitionBranch*>(_item);
}

SCItem* FVItem::getItem()
{
    return _item;
}

IAttributeContainer* FVItem::getAttributes()
{
    if(isState())
    {
        return &getState()->attributes;
    }
    else if(isTransition())
    {
        return &getTransition()->attributes;
    }
    else if(isTransitionBranch())
    {
        return &getTransitionBranch()->attributes;
    }
    else
    {
        qDebug() << "FVItem::getAttributes ERROR unexpected type";
        return NULL;
    }
}

IAttributeContainer* FVItem::getTextBlockAttributes()
{
    if(isState())
    {
        return &getState()->getIDTextBlock()->attributes;
    }
    else if(isTransition())
    {
        return &getTransition()->getEventTextBlock()->attributes;
    }
    else if(isTransitionBranch())
    {
        return &getTransitionBranch()->getEventTextBlock()->attributes;
    }
    else
    {
        qDebug() << "FVItem::getTextBlockAttributes ERROR unexpected type";
        return NULL;
    }
}

QString FVItem::getType()
{

    if(isState())
    {
        return "state";
    }
    else if(isTransition())
    {
        return "transition";
    }
    else if(isTransitionBranch())
    {
        return "transitionbranch";
    }

    else
    {
        qDebug() << "FVItem::getType() ERROR unknown type";
        return "unknown";
    }
}

int FVItem::getTypeInt()
{
    return _type;
}

QString FVItem::getTitle()
{
    if(isState())
    {
        return getState()->getStateNameAttr()->asString();
    }
    else if(isTransition())
    {
        return getTransition()->getTransStringAttr("event")->asString();
    }
    else if(isTransitionBranch())
    {
        return getTransitionBranch()->getTransStringAttr("event")->asString();
    }
    else
    {
        qDebug() << "FVItem::getTitle() ERROR unknown type";
        return "unknown";
    }
}



bool FVItem::isState()
{
    return (_type==FVItem::STATE);
}

bool FVItem::isTransition()
{
    return (_type==FVItem::TRANSITION);
}

bool FVItem::isTransitionBranch()
{
    return (_type == FVItem::TRANSITIONBRANCH);
}

void FVItem::setItem(SCItem* item)
{
    _item = item;
}

void FVItem::setTreeWidget(CustomTreeWidgetItem * widget)
{
    _treeWidget = widget;
}

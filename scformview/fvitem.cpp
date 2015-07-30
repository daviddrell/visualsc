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

SCItem* FVItem::getItem()
{
    return _item;
}


bool FVItem::isState()
{
    return (_type==FVItem::STATE);
}

bool FVItem::isTransition()
{
    return (_type==FVItem::TRANSITION);
}

void FVItem::setItem(SCItem* item)
{
    _item = item;
}

void FVItem::setTreeWidget(CustomTreeWidgetItem * widget)
{
    _treeWidget = widget;
}

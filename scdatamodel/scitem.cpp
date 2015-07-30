#include "scitem.h"
#include "textblock.h"

SCItem::SCItem() : QObject()
{
}

SCItem::SCItem(QObject* parent) : QObject(parent)
{
}
/*
SCTextBlock* SCItem::getTextBlock(QString textBlockName)
{
    return NULL;
}
*/

void SCItem::deleteSafely()
{
    emit markedForDeletion(this);
    this->deleteLater();
}

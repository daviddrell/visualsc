#include "scitem.h"
#include "textblock.h"
#include <QDebug>

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
    qDebug () << "emit markedForDeletion() in SCItem::deleteSafely()";
    emit markedForDeletion(this);
    this->deleteLater();
}

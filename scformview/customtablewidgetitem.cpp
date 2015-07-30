#include "customtablewidgetitem.h"
#include <QDebug>

CustomTableWidgetItem::CustomTableWidgetItem(QString text):
    QTableWidgetItem(text),
    QObject()
{

}

CustomTableWidgetItem::~CustomTableWidgetItem()
{

}

void CustomTableWidgetItem::handleAttributeChanged(SizeAttribute* sz)
{
    setText(sz->asString());
}

void CustomTableWidgetItem::handleAttributeChanged(PositionAttribute* po)
{
    setText(po->asString());
}

void CustomTableWidgetItem::handleAttributeChanged(StateName * sn)
{
    qDebug() << "CustomTableWidgetItem::handleAttributeChanged";
    setText(sn->asString());
}


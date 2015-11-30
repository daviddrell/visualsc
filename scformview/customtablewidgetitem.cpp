#include "customtablewidgetitem.h"
#include <QDebug>

CustomTableWidgetItem::CustomTableWidgetItem(QString text):
    QTableWidgetItem(text)
{

}

CustomTableWidgetItem::~CustomTableWidgetItem()
{

}

void CustomTableWidgetItem::handleAttributeChanged(IAttribute*attr)
{
    setText(attr->asString());
}


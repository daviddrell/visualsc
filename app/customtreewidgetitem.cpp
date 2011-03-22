#include "customtreewidgetitem.h"
#include <QTreeWidgetItem>

CustomTreeWidgetItem::CustomTreeWidgetItem(CustomTreeWidgetItem * parent) : QTreeWidgetItem(parent)
{

}

void CustomTreeWidgetItem::setData(QObject* d)
{
    _stateOrTransitionItem = d;
}

QObject * CustomTreeWidgetItem::data()
{
    return  _stateOrTransitionItem ;

}

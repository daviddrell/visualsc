#ifndef CUSTOMTREEWIDGETITEM_H
#define CUSTOMTREEWIDGETITEM_H

#include <QTreeWidgetItem>
class QObject;

class CustomTreeWidgetItem : public QTreeWidgetItem
{
public:
    CustomTreeWidgetItem(CustomTreeWidgetItem * parent=NULL);

    void setData(QObject* d);
    QObject * data();

private:
    QObject * _stateOrTransitionItem;
};

#endif // CUSTOMTREEWIDGETITEM_H

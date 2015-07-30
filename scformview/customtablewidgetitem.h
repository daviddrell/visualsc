#ifndef CUSTOMTABLEWIDGETITEM_H
#define CUSTOMTABLEWIDGETITEM_H

#include <QTableWidgetItem>
#include <QObject>
#include "iattribute.h"
#include "stateattributes.h"
#include "transitionattributes.h"

class IAttribute;


class CustomTableWidgetItem : public QObject, public QTableWidgetItem
{
    Q_OBJECT

public:
    CustomTableWidgetItem(QString);
    ~CustomTableWidgetItem();

public slots:
    void handleAttributeChanged(SizeAttribute*);
    void handleAttributeChanged(PositionAttribute*);
    void handleAttributeChanged(StateName*);
    void handleAttributeChanged(TransitionStringAttribute*);
};

#endif // CUSTOMTABLEWIDGETITEM_H

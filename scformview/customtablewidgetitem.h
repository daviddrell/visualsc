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
    //void setText(const QString &text);

signals:
    void textChanged(QString);

public slots:
    void handleAttributeChanged(SizeAttribute*);
    void handleAttributeChanged(PositionAttribute*);
    void handleAttributeChanged(StateName*);
    void handleAttributeChanged(TransitionStringAttribute*);
    void handleAttributeChanged(StateString*);
};

#endif // CUSTOMTABLEWIDGETITEM_H

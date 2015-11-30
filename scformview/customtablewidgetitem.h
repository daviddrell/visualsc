#ifndef CUSTOMTABLEWIDGETITEM_H
#define CUSTOMTABLEWIDGETITEM_H

#include <QTableWidgetItem>
#include <QObject>
#include "iattribute.h"
#include "stateattributes.h"
#include "transitionattributes.h"
#include "textblockattributes.h"

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
    void handleAttributeChanged(IAttribute*);

};

#endif // CUSTOMTABLEWIDGETITEM_H

#ifndef SCITEM_H
#define SCITEM_H
#include "iattribute.h"

class SCItem : public QObject
{
public:
    SCItem();
    SCItem(QObject *parent);

    virtual IAttributeContainer * getAttributes() = 0;
};

#endif // SCITEM_H

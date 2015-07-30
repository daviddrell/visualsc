#ifndef SCITEM_H
#define SCITEM_H
#include "iattribute.h"
//#include "textblock.h"

class SCItem : public QObject
{
    Q_OBJECT
public:
    SCItem();
    SCItem(QObject *parent);


   // SCTextBlock* getTextBlock(QString textBlockName);
    virtual IAttributeContainer * getAttributes() = 0;
    void deleteSafely();

signals:
    void markedForDeletion(QObject*);
};

#endif // SCITEM_H

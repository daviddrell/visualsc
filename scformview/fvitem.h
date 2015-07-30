#ifndef FVITEM_H
#define FVITEM_H

#include <QObject>
#include "scstate.h"
#include "sctransition.h"
#include "customtreewidgetitem.h"
#include "scitem.h"


class FVItem : public QObject
{
public:
    enum FVItemType { STATE,TRANSITION };

    FVItem(SCItem*,int, CustomTreeWidgetItem*);
    FVItem(CustomTreeWidgetItem*);
    FVItem(SCItem*,int);
    ~FVItem();

    CustomTreeWidgetItem* getTreeWidget();

    //FVItem& operator= (const SCState* state);
   //FVItem& operator= (const FVItem* item);

    void setItem(SCItem*);
    void setTreeWidget(CustomTreeWidgetItem*);

    bool isState();
    bool isTransition();

    SCState* getState();
    SCTransition* getTransition();
    SCItem* getItem();



protected:
    CustomTreeWidgetItem* _treeWidget;


private:
    SCItem* _item;
    int _type;



};

#endif // FVITEM_H

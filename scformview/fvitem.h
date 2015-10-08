#ifndef FVITEM_H
#define FVITEM_H

#include <QObject>
#include <QDebug>
#include "scstate.h"
#include "sctransition.h"
#include "customtreewidgetitem.h"
#include "scitem.h"
#include "sctransitionbranch.h"


class FVItem : public QObject
{
public:
    enum FVItemType {STATE,TRANSITION,TRANSITIONBRANCH };

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
    bool isTransitionBranch();

    int getTypeInt();

    SCState* getState();
    SCTransition* getTransition();
    SCTransitionBranch* getTransitionBranch();
    SCItem* getItem();

    QString getType();
    QString getTitle();

    IAttributeContainer* getAttributes();
    IAttributeContainer* getTextBlockAttributes();


protected:
    CustomTreeWidgetItem* _treeWidget;


private:
    SCItem* _item;
    int _type;



};

#endif // FVITEM_H

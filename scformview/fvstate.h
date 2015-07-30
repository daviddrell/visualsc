#ifndef FVSTATE_H
#define FVSTATE_H

#include <QObject>
#include "scstate.h"
#include "sctransition.h"
#include "customtreewidgetitem.h"
#include "fvitem.h"

class FVState : public FVItem
{
public:
    FVState(SCState* state, CustomTreeWidgetItem* stateTreeItem);
    ~FVState();

   // FVState& operator=(const SCState& state);
   // FVState& operator=(const FVState& state);

    SCState* getState();
    void setState(SCState* state);



private:
    SCState* _state;
    //CustomTreeWidgetItem* _treeWidget;



};

#endif // FVSTATE_H

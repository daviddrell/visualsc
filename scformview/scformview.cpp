/*
    visualsc - Visual State Chart Editing for SCXML State Engines
    Copyright (C) 2011  David W. Drell (david@davidwdrell.net) and
    Contributing Authors:


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "scformview.h"
#include "fvstate.h"
#include "customtreewidgetitem.h"
#include "stateselectionwindow.h"
#include "textblock.h"
#include "scitem.h"
#include <QHBoxLayout>
#include <QTableWidget>
#include <QVariant>
#include <QtGui>
#include <QLabel>
#include <QList>
#include <QVariant>
#include <QToolBar>
#include <QMessageBox>
#include <QAction>
#include <QMenuBar>
#include <QFontComboBox>
#include <QToolButton>
#include <QButtonGroup>
#include <QSignalMapper>
#include <QTreeWidgetItemIterator>
#include <QHash>
#include <QDebug>
#include <QApplication>
#include <QHeaderView>
#include <QFileDialog>


#define PROPERTY_TABLE_WIDTH_1  108     // smallest value while still having clearance for the longest attribute name connectToFinished
#define PROPERTY_TABLE_WIDTH_2  170

#define WINDOW_WIDTH    618
#define WINDOW_HEIGHT   1000


//#define TREEVIEW_COLOR_ENABLE
#define BOX_RED_COLOR_JUMP      67
#define BOX_GREEN_COLOR_JUMP    23
#define BOX_BLUE_COLOR_JUMP     7
#define BOX_RED_OFFSET          0
#define BOX_GREEN_OFFSET        0
#define BOX_BLUE_OFFSET         1

#define POP_UP_X                160
#define POP_UP_Y                200


SCFormView::SCFormView(QWidget *parent, SCDataModel *dataModel) :
        QMainWindow(parent, Qt::Window),
        _dm(dataModel),
        _currentlySelected(NULL)//,
        //_previouslySelected(NULL)
{
this->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    createActions();
    createMenus();
    createToolbars();

    // central widget and outter layout

    QHBoxLayout *layout = new QHBoxLayout;  // main layout, horizontal
        layout->setContentsMargins(6,0,6,6);
    QWidget *widget = new QWidget;
//    widget->setContentsMargins(6,6,6,6);
    widget->setLayout(layout);
    setCentralWidget(widget);

//     this->addToolBar(editToolBar);
//    editToolBar->setMovable(false);


    //  start tree view

    stateChartTreeView = new QTreeWidget();
    stateChartTreeView->setColumnCount(1);
    //connect(stateChartTreeView, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(handleTreeViewItemClicked(QTreeWidgetItem*,int)));
    connect(stateChartTreeView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(handleTreeViewItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    stateChartTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection); // enable multiple selection through ctrl+click and shift+click

    QVBoxLayout *treeLayout = new QVBoxLayout();
    //selectedChartItem = new QLabel();

    treeLayout->addWidget(editToolBar);             // add the edit tool bar to the tree layout
    //treeLayout->addWidget(selectedChartItem);

    treeLayout->addWidget (stateChartTreeView );    // first item added to layout
    layout->addLayout(treeLayout);


    QVBoxLayout *propertyLayout = new QVBoxLayout;     // property table is a vertical layout
    layout->addLayout(propertyLayout );                // second item added to horizontal layout


    //propertyLayout->addWidget( selectedChartItem);     // sec
    propertyLayout->addWidget( propertyToolBar);


    //  property table



    propertyTable = new QTableWidget();
    propertyTable->setColumnCount(2);
    propertyTable->setColumnWidth(0,PROPERTY_TABLE_WIDTH_1);
    propertyTable->horizontalHeader()->setStretchLastSection(true);
    //propertyTable->setColumnWidth(1,PROPERTY_TABLE_WIDTH_2);
//    propertyTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Preuba"));

    propertyTable->setHorizontalHeaderLabels(QString("HEADER 1;HEADER 2;HEADER 3").split(";"));

    propertyLayout->addWidget( propertyTable );


    //QLabel textBlockPropertyLabel("Text Block Properties");
    //propertyLayout->addItem(textBlockPropertyLabel);
    // textBlockPropertyTable

    textBlockPropertyTable = new QTableWidget();
    textBlockPropertyTable->setColumnCount(2);
    textBlockPropertyTable->setColumnWidth(0,PROPERTY_TABLE_WIDTH_1);
    textBlockPropertyTable->horizontalHeader()->setStretchLastSection(true);
    //textBlockPropertyTable->setColumnWidth(1,180);
    //textBlockPropertyTable->columnViewportPosition(1);

    propertyLayout->addWidget(textBlockPropertyTable);

    setWindowTitle(tr("Tree Statechart Editor"));
    setUnifiedTitleAndToolBarOnMac(true);

   // QList<SCState*> states;
   // states.append( _dm->getTopState());

    connect (_dm, SIGNAL(newStateSignal(SCState*)), this, SLOT(handleNewState(SCState*)));

    // transitionsReadyToConnect is a special signal for the data model handling new transitions from when reading an scxml. The graphics view relies on two different signals because when it is creating graphics, we can only connect transitions if their target is known. But this is not the case for the formview, where both signals result in the same function
    connect (_dm, SIGNAL(transitionsReadyToConnect(SCTransition*)), this, SLOT(handleNewTransition(SCTransition*)));        // data model emits this signal for transitions when reading an scxml and all states and transitions have been loaded into the DM
    connect (_dm, SIGNAL(insertNewTransitionSignal(SCTransition*)), this, SLOT(handleNewTransition(SCTransition*)));        // data model emits this signal for when a user inserts a new transition

    // create an fv item for part of the forkedtransition
    connect(_dm, SIGNAL(insertNewTransitionSignal(SCForkedTransition*)), this, SLOT(handleNewTransition(SCForkedTransition*)));

    connect(_dm, SIGNAL(newRootMachine(SCState*)), this, SLOT(handleNewRootMachine(SCState*)));

    // TODO make a handle new textblock for scformview


    initTree();
}



void SCFormView::import()
{
    if(!_currentlySelected->isState())
    {
        sendMessage("Error","Please select a state to import into");
        return;
    }
    qDebug() << "import!";
    QString prevFilePath=QDir::homePath();
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Import SCXML Input File"), prevFilePath, tr("SCXML Files (*.scxml)"));
    _dm->importFile(_currentlySelected->getState(),fileName);
}

/**
 * @brief SCFormView::highlightRootItem
 *
 * used to reselect the root machine item
 * called when open file is called;
 */
void SCFormView::highlightRootItem()
{
    _currentlySelected = _topState;
    stateChartTreeView->setCurrentItem(_currentlySelected->getTreeWidget());
}


void SCFormView::highlightItem(SCItem* item)
{
    FVItem* fvitem = _items.value(item);
    _currentlySelected = fvitem;
    stateChartTreeView->setCurrentItem(_currentlySelected->getTreeWidget());
}

void SCFormView::clearSelectedItems()
{
    QList<QTreeWidgetItem*> selected = stateChartTreeView->selectedItems();
    for(int i =0 ; i < selected.size(); i++)
    {
        selected.at(i)->setSelected(false);
    }
}

/**
 * @brief SCFormView::highlightPreviousItem
 *
 * called in the slot functions for handleState/Transition/TextBlock deleted
 *
 * when a tree object is deleted, set the highlighted object to be the last highlighted object, or the root machine if nothing is found
 *
 */
void SCFormView::highlightPreviousItem()
{

    SCState* st = dynamic_cast<SCState*>(_currentlySelected);
    SCTransition* trans = dynamic_cast<SCTransition*>(_currentlySelected);

    if(st && st->getParentState())
    {
        //_currentlySelected = _previouslySelected = st->getParentState();
        _currentlySelected->setItem(st->getParentState());
    }
    else if(trans)
    {
        //_currentlySelected = _previouslySelected = trans->parentSCState();
        _currentlySelected->setItem(trans->parentSCState());
    }
    else
    {
        qDebug() << "SCFormView::highlightPreviousItem Unsure of what is currently selected, so defaulting to root machine.";
        //_currentlySelected = _previouslySelected = _dm->getTopState();
        _currentlySelected->setItem(_dm->getTopState());
    }

    CustomTreeWidgetItem* twid = findItem(_currentlySelected);
    twid->setSelected(true); // rehighlight the item that was highlighted
    handleTreeViewItemClicked((QTreeWidgetItem*)twid, 0);
}


/**
 * @brief SCFormView::initTree
 *
 *
 * this is the initial loadtree that will load the tree view widgets and set connections simultaenously
 *
 */
void SCFormView::initTree()
{
     //_currentlySelected = _previouslySelected = _dm->getTopState();

    //_topState = new FVItem(_dm->getTopState(), FVItem::STATE);

    QList<SCState*> states;
    states.append( _dm->getTopState());
    loadTreeState(NULL, states, true);

    highlightRootItem();

    /*
    CustomTreeWidgetItem* twid = findItem(_currentlySelected);
    twid->setSelected(true); // rehighlight the item that was highlighted
    handleTreeViewItemClicked((QTreeWidgetItem*)twid, 0);*/
}

/**
 * @brief SCFormView::findItem
 * @param object
 * @return
 *
 * Pass a SCState or SCTransition
 * returns the corresponding CustomTreeWidgetItem that belongs to it
 *
 * Note: whenever the tree changes, every CustomTreeWidgetItem is re-made
 *
 */
CustomTreeWidgetItem* SCFormView::findItem(QObject * object)
{
    static int numTimesCalled=0;
    //SCItem* item = dynamic_cast<SCItem*>(object);
    /*
    if(item)
    {
        qDebug() << "SCFormView::findItem Item Found from a QObject. Call Times: " << numTimesCalled++;
        //return _itemToTreeWidget.value(item);
        return ;
    }*/

    FVItem* item = dynamic_cast<FVItem*>(object);

    if(item)
    {
         qDebug() << "SCFormView::findItem returning item's tree widget";
        return item->getTreeWidget();
    }
    else
    {
        qDebug() << "SCFormView::findItem ERROR could not find tree widget for given object";
    }

    return NULL;
}

/**
 * @brief SCFormView::handleMakeTransitionConnections
 * @param trans
 *
 * SLOT
 * connect in SCFormView
 * connect (_dm, SIGNAL(transitionsReadyToConnect(SCTransition*)), this, SLOT(handleMakeTransitionConnections(SCTransition*)));

 * called for scxml reader
 * when all states and transitions are done being read, then we can safely create this transition
 *
 */
void SCFormView::handleMakeTransitionConnections(SCTransition* trans)
{
    handleNewTransition(trans);
}

/**
 * @brief SCFormView::handleNewTransition
 * @param t
 *
 *  SLOT
 *
 * called when the dm emits a new transition signal
 *
 * creates a new transition in the tree view
 *
 */
void SCFormView::handleNewTransition(SCTransition* tr)
{
    //qDebug()<<"SCFormView::handleNewTransition";

    // the parent state's widget will be the parent of this transition's widget
    SCState* parentState = tr->parentSCState();
    FVItem*parentfv = _items.value(parentState);

    // create the tree widget and fv item associated with this state
    CustomTreeWidgetItem* item = new CustomTreeWidgetItem(parentfv->getTreeWidget());
    FVItem* fvItem = new FVItem(tr, FVItem::TRANSITION, item);

    // link the tree widget to the fv item, and set its text and icon
    item->setData(fvItem);
    item->setText(0, tr->getEventName());
    item->setIcon(0,QIcon(":/SCFormView/diagonalarrowhollow.png"));

    // link the SCTransiton and FVItem
    _items.insert(tr, fvItem);

    // hook up the transition to handlers
    connectTransition(tr);                              // deletesafely handler in formview
    connectTransition(tr, fvItem->getTreeWidget());     // connect the tree widget to the transition
}

void SCFormView::handleNewTransition(SCForkedTransition * ft)
{
    qDebug() << "SCFormView::handleNewTransition forked transitions";

    QList<SCTransitionBranch*>* branches = ft->getSourceBranches();
    foreach(SCTransitionBranch* br, *branches)
    {
        // the parent state's widget will be the parent of this transition branch's widget
        SCState* parentState = br->parentAsSCState();
        FVItem* parentFv = _items.value(parentState);

        // create the tree widget and fvitem associated with this state
        CustomTreeWidgetItem* item = new CustomTreeWidgetItem(parentFv->getTreeWidget());
        FVItem* fvItem = new FVItem(br, FVItem::TRANSITIONBRANCH, item);

        // link the tree widget to the fv item, and set its text and icon
        item->setData(fvItem);
        item->setText(0, br->getEventName());
        item->setIcon(0, QIcon(":/SCFormView/diagonalarrowhollow.png"));

        // link the SCTransitionBranch and FVItem
        _items.insert(br, fvItem);

        // hook up the transition to handlers
        connectTransition(br);                              // deletesafely handler in formview
        connectTransition(br, fvItem->getTreeWidget());     // connect the tree widget to the transition
    }

}

/**
 * @brief SCFormView::handleNewState
 * @param s
 *
 * connected to the signal newStateSignal in SCDataModel
 *
 * will create a new state in the formview
 *
 */
void SCFormView::handleNewState(SCState* st)
{
    // parent of this widget will be the state's parent's widget
    SCState* parentState = st->getParentState();
    FVItem* parentfv = _items.value(parentState);

    // create the tree widget and fv item associated with this state
    CustomTreeWidgetItem* item = new CustomTreeWidgetItem(parentfv->getTreeWidget());
    FVItem* fvItem = new FVItem(st, FVItem::STATE, item);

    // link the tree widget to the fv item, expand the widget, set its text and icon
    item->setData(fvItem);
    item->setExpanded(true);
    item->setText(0, st->getName());
    item->setIcon(0,QIcon(":/SCFormView/cardboardbox.png"));

#ifdef TREEVIEW_COLOR_ENABLE
    int r = 255 - (((st->getLevel()-BOX_RED_OFFSET) * BOX_RED_COLOR_JUMP ) % 255);
    int g = 255 - (((st->getLevel()-BOX_GREEN_OFFSET) * BOX_GREEN_COLOR_JUMP ) % 255);
    int b = 255 - (((st->getLevel()-BOX_BLUE_OFFSET) * BOX_BLUE_COLOR_JUMP ) % 255);

    item->setBackgroundColor(0, QColor(r,g,b,255));
#endif

    // link the SCState and FVItem
    _items.insert(st,fvItem);

    // hook up the state to handlers in the formview
    connectState(st);           // deleteSafely handler
    connectState(st, fvItem->getTreeWidget());   // tree item handler
}



/**
 * @brief SCFormView::updateStateName
 * @param state
 * @param name
 *
 * Called by handlePropertyCellChanged
 * this function updates the name of a state in the tree and property table
 *
 */
void SCFormView::updateStateName(SCState* state, QString name)
{
    // update the tree and table if necessary
    QTreeWidgetItem* treeItem = findItem(state);
    treeItem->setText(0, name);  // update the tree

    // update any tree transitions that target this state
    QList<SCTransition*> ins = state->getTransitionsIn();
    //qDebug() << "got into name " << "ins count: " << ins.count();
    for(int i = 0; i < ins.count(); i++)
    {
        //qDebug() << "inbound transition to state: " << st->attributes.value("name")->asString() << " have target " << ins.at(i)->attributes.value("target")->asString();
        ins.at(i)->setAttributeValue("target",name);
    }

    // update the property table if the state is selected
    if(isCurrentlySelectedEqualTo(state))
    {
        propertyTableSetText(propertyTableIndexOf("name"), name);
    }
}

void SCFormView::deleteTreeItem(SCItem* item)
{
    CustomTreeWidgetItem* treeWidget = findItem(item);

    stateChartTreeView->removeItemWidget(treeWidget, 0);
    //delete treeWidget;
}


void SCFormView::handleTextBlockPropertyCellChanged(int r, int c)
{
    qDebug() << "SCFormView::handleTextBlockPropertyCellChanged";

    // only process changes from the second column
    if(c!=1)
        return;

    SCTextBlock* tb;
    if(_currentlySelected->isState())
    {
        tb = _currentlySelected->getState()->getIDTextBlock();
    }
    else if(_currentlySelected->isTransition())
    {
        tb = _currentlySelected->getTransition()->getEventTextBlock();
    }
    else
    {
        qDebug() << "UNKNOWN type given";
        return;
    }

    QString key = textBlockPropertyTable->item(r,0)->text();
    QString value = textBlockPropertyTable->item(r,1)->text();
    //qDebug() << "key is : " << key;
    if(key == "size")
    {

        bool failed = false;
        QStringList xy = value.split(",");

        if(xy.count()==2)
        {
            QString xStr = xy.at(0);
            QString yStr = xy.at(1);
            bool xConfirm;
            bool yConfirm;

            qreal x = xStr.toDouble(&xConfirm);
            qreal y = yStr.toDouble(&yConfirm);

            if(xConfirm && yConfirm && x>0 && y>0)
            {
               // qDebug() << "xy: " << x<< " " << y;
                QPointF point(x,y);

                SizeAttribute* size = tb->getSizeAttr();
                size->setValue(point);
                //qDebug() << "SETTING SIZE OF TEXTBLOCK!!";
            }
            else
                failed = true;
        }
        else
            failed = true;

        // improper input handler
        if(failed)
        {
            textBlockPropertyTableSetText(textBlockPropertyTableIndexOf("size"), tb->getSizeAttr()->asString());
            sendMessage("Invalid Size","Please enter width and height \"w,h\"");
        }


    }
    else if(key =="position")
    {
        bool failed = false;
        QStringList xy = value.split(",");

        if(xy.count()==2)
        {
            QString xStr = xy.at(0);
            QString yStr = xy.at(1);
            bool xConfirm;
            bool yConfirm;

            qreal x = xStr.toDouble(&xConfirm);
            qreal y = yStr.toDouble(&yConfirm);

            if(xConfirm && yConfirm)
            {
                //qDebug() << "xy: " << x<< " " << y;
                QPointF point(x,y);

                tb->getPosAttr()->setValue(point);
                //qDebug()<<"SETTING POS OF TEXgtLBOCK!";

            }
            else
                failed = true;
        }
        else
            failed = true;

        if(failed)
        {
            textBlockPropertyTableSetText(textBlockPropertyTableIndexOf("position"), tb->getPosAttr()->asString());
            sendMessage("Invalid Position","Please enter coordinates: \"x,y\"");
        }
    }
}

/**
 * @brief SCFormView::handlePropertyCellChanged
 * @param r
 * @param c
 *
 * SLOT
 * connect in SCFormView
 * connect(propertyTable, cellChanged, SCFormView, handlePropertyCellChanged)
 *
 * When a cell in the propertyTable is changed, some attributes should update themselves as well
 *
 * will
 * update the attribute in SCItem
 * update the selectable textBox text
 * update the tree view
 *
 *
 */
void SCFormView::handlePropertyCellChanged(int r, int c)
{
    //qDebug() << "SCFormView::handle PropertyCellChanged type is " << _currentlySelected->getType();

    // only process the second column
    if ( c != 1 ) return;

    // first determine item type, then determine which property type was changed
    if(_currentlySelected->isState())
    {
        //qDebug() << "staate";
        SCState* st = _currentlySelected->getState();    
        QString key = propertyTable->item(r,0)->text();
        QString value = propertyTable->item(r,1)->text();

        if(key == "name")
        {
            // update the data model

            st->setStateName(value);
        }
        else if (key == "position")
        {
            bool failed = false;
            QStringList xy = value.split(",");

            if(xy.count()==2)
            {
                QString xStr = xy.at(0);
                QString yStr = xy.at(1);
                bool xConfirm;
                bool yConfirm;

                qreal x = xStr.toDouble(&xConfirm);
                qreal y = yStr.toDouble(&yConfirm);

                if(xConfirm && yConfirm)
                {
                    //qDebug() << "xy: " << x<< " " << y;
                    QPointF point(x,y);

                    //emit st->positionChangedInFormView(st, point);
                    st->setPosition(point);
                }
                else
                    failed = true;
            }
            else
                failed = true;

            if(failed)
            {
                propertyTableSetText(propertyTableIndexOf("position"), st->attributes.value("position")->asString());
                sendMessage("Invalid Position","Please enter coordinates: \"x,y\"");
            }
        }
        else if(key == "size")
        {
            bool failed = false;
            QStringList xy = value.split(",");

            if(xy.count()==2)
            {
                QString xStr = xy.at(0);
                QString yStr = xy.at(1);
                bool xConfirm;
                bool yConfirm;

                qreal x = xStr.toDouble(&xConfirm);
                qreal y = yStr.toDouble(&yConfirm);

                if(xConfirm && yConfirm && x>0 && y>0)
                {
                    //qDebug() << "xy: " << x<< " " << y;
                    QPointF point(x,y);

                    //emit st->sizeChangedInFormView(st, point);
                    st->setSize(point);
                }
                else
                    failed = true;
            }
            else
                failed = true;

            // improper input handler
            if(failed)
            {
                propertyTableSetText(propertyTableIndexOf("size"), st->attributes.value("size")->asString());
                sendMessage("Invalid Size","Please enter width and height \"w,h\"");
            }
        }
        else if(key == "initialState")
        {
            SCState* parentSt = st->parentAsSCState();
            bool failed = false;
            if(value == "false")
            {
                if(st->parentAsSCState() == NULL)
                {
                    failed = true;
                }
                else
                    st->setInitial(value);
            }
            else if(!parentSt)
            {
                sendMessage("Error","The Root Machine cannot be an initial state.");
                failed = true;
            }
            else if(parentSt->isParallel())
            {
                sendMessage("Error","Parallel states have no initial top-level states.");
                failed = true;
            }
            else if(parentSt->hasAnInitialState())
            {
                sendMessage("Error","State Machine \""+parentSt->objectName()+"\" already has an initial state \""+parentSt->getInitialStateMember()->objectName()+"\"");
                failed = true;
            }
            else
            {
                st->setInitial(value);
            }

            if(failed)
            {
                propertyTableSetText(propertyTableIndexOf("initialState"), st->attributes.value("initialState")->asString());
            }
        }
        else    // some other property was updated
        {
            st->attributes.value(key)->setValue(value);
        }

    }
    else if(_currentlySelected->isTransition())
    {
        //SCTransition* trans= dynamic_cast<SCTransition*>(_currentlySelected);
        SCTransition* trans = _currentlySelected->getTransition();
        // update the datamodel, tree, and graphics view box
        QString key = propertyTable->item(r,0)->text();
        QString value = propertyTable->item(r,1)->text();
        if(key == "event")
        {
            // update the tree
            //updateTransitionEvent(trans, value);

            // update the data model and graphics view
           qDebug() << "emit trans->eventChangedInFormView(trans, value);";
            //emit trans->eventChangedInFormView(trans, value);
           trans->setEventName(value);

            //updateTransitionEvent(trans, value);
            //handleItemNameChangedInDataModel(trans, value);
        }
        else    // some other property was updated
        {
            trans->attributes.value(key)->setValue(value);
        }
    }
   /* else if(_currentlySelected->isMachine())
    {
        //SCState* st = dynamic_cast<SCState*>(_currentlySelected);
        SCState* st = _currentlySelected->getState();
        qDebug() << "got into state machine*!@";

        QString key = propertyTable->item(r,0)->text();
        QString value = propertyTable->item(r,1)->text();
        if(key == "name")
        {
            // update the state name in the form view and the transitions that target it
            // update the tree and table if necessary
            st->setStateName(value);

            // now signal the datamodel and graphicsview that the state name was changed here
            //emit st->nameChangedInFormView(st, value);

            // then signal that a state was change in the form view (this is done in SCDataModel already with SCState::setText)

        }
    }*/
}

/**
 * @brief SCFormView::updateTransitionEvent
 * @param trans
 * @param eventName
 *
 * called by handleCellPropertyChanged for the "event" attribute of a transition
 *
 * this updates the tree and property table
 *
 */
void SCFormView::updateTransitionEvent(SCTransition* trans, QString eventName)
{
    qDebug() << "transition name was changed";
    if(trans)
    {
        // update the data model
        // trans->attributes.value("event")->setValue(eventName);

        // update the tree
        QTreeWidgetItem* treeItem = findItem(trans);
        treeItem->setText(0, eventName);

        // update the property table (if necessary)
        if(isCurrentlySelectedEqualTo(trans))
        {
            propertyTableSetText(propertyTableIndexOf("event"), eventName);
        }

        // update the selectable text box
        // trans->setText(eventName);
    }
}



void SCFormView::handleTransitionDeleted(QObject *t)
{

    SCTransition* trans = (SCTransition*) t;


    // get the FVItem and Tree Widget associated with the transition
    FVItem* fvItem = _items.take(trans);

    if(fvItem==NULL)
    {
        qDebug()<< "SCFormView::handleTransitionDeleted did not have a transition to delete";
        return;
    }
    qDebug () << "deleting fromview item for transiton: " << trans->attributes.value("event")->asString();

    CustomTreeWidgetItem* fvTreeItem = fvItem->getTreeWidget();

    // deselect the tree item
    //fvTreeItem->setSelected(false);




    // delete the tree widget and the fv item pointers
    delete fvTreeItem;
    delete fvItem;

    // unselect any selected item;
    QList<QTreeWidgetItem*> selected = stateChartTreeView->selectedItems();
    qDebug() << "there are " << selected.size()<<" selected items.";
    //stateTreeItem->setSelected(false);

    for(int i = 0; i < selected.size();i++)
    {
     selected.at(i)->setSelected(false);
    }

    // reset the currently selected state to the top state;
    _currentlySelected = _topState;
    _topState->getTreeWidget()->setSelected(true);

}

/**
 * @brief SCFormView::handleStateDeleted
 * @param s
 *
 * SLOT
 *  connect(SCState, SIGNAL(destroyed(QObject*)), SCFormView, SLOT(handleStateDeleted(QObject*)), Qt::QueuedConnection);
 *
 */

void SCFormView::handleStateDeleted(QObject *s)
{
    SCState* state = (SCState*) s;

    qDebug () << "deleting form view item for state: ";
//    qDebug () << "deleting form view item for state: " <<state->objectName();

    // get the FVItem and TreeWidget associated with the state
    FVItem* fvItem = _items.take(state);

    if(fvItem==NULL)
    {
        qDebug() << "SCFormView::handleStateDeleted did not have a state to delete";
        return;
    }
    CustomTreeWidgetItem* fvTreeItem = fvItem->getTreeWidget();

    // deselect the tree item
    //fvTreeItem->setSelected(false);



    // delete the tree widget and the fv item pointers
    delete fvTreeItem;
    delete fvItem;

    // reselect the top state as the selected item
 //   highlightRootItem();
}

QObject* SCFormView::getNeighborState(QObject*s)
{
    QObject *prev=0;

    QList<SCState*> states;
    _dm->getAllStates(states);
    int c = states.count();

    for(int i = 0; i < c; i++)
    {
        SCState * st = states.at(i);
        if ( s == st)
            return prev;
        prev = st;
    }

    return prev;
}

void SCFormView::handleItemExpand(SCState * st)
{
    FVItem* fv = _items.value(st);
    fv->getTreeWidget()->setExpanded(true);
}

void SCFormView::handleItemMinimize(SCState * st)
{
    FVItem* fv = _items.value(st);
    fv->getTreeWidget()->setExpanded(false);
}

void SCFormView::handleItemClicked(SCState* st)
{
    this->highlightItem(st);
}

void SCFormView::handleItemClicked(SCTransition* tr)
{
    this->highlightItem(tr);
}

void SCFormView::handleItemClicked(SCTransitionBranch* br)
{
    this->highlightItem(br);
}

void SCFormView::connectTextBlock(SCTextBlock * textBlock, CustomTableWidgetItem *tableItem, QString key)
{
    if(key=="size")
    {
        connect(textBlock->getSizeAttr(), SIGNAL(changed(SizeAttribute*)), tableItem, SLOT(handleAttributeChanged(SizeAttribute*)));
    }
    else if(key == "position")
    {
        connect(textBlock->getPosAttr(), SIGNAL(changed(PositionAttribute*)), tableItem, SLOT(handleAttributeChanged(PositionAttribute*)));
    }
}

/**
 * @brief SCFormView::connectState
 * @param state
 * @param treeItem
 *
 *  this function sets up connections for a sc states' attributes changing to also update the tree view
 *
 *
 */
void SCFormView::connectState(SCState *state, CustomTreeWidgetItem* treeItem)
{
    connect(state->getStateNameAttr(), SIGNAL(changed(StateName*)), treeItem, SLOT(handleAttributeChanged(StateName*)));

}




/**
 * @brief SCFormView::connectState
 * @param st
 *
 * sets up the connect call for the given state
 * to alter the form view when the state is manipulated
 *
 *
 */
void SCFormView::connectState(SCState* st)
{
    // SCState connects
    connect(st, SIGNAL(markedForDeletion(QObject*)), this, SLOT(handleStateDeleted(QObject*)), Qt::QueuedConnection);
    connect(st, SIGNAL(changedParent(SCState*,SCState*)), this, SLOT(handleChangedParent(SCState*,SCState*)));

    // when a state emits the clicked signal, select it in form view
    connect(st, SIGNAL(clicked(SCState*)), this, SLOT(handleItemClicked(SCState*)));
    connect(st, SIGNAL(minimized(SCState*)), this, SLOT(handleItemMinimize(SCState*)));
    connect(st, SIGNAL(expanded(SCState*)), this, SLOT(handleItemExpand(SCState*)));
}

/**
 * @brief SCFormView::connectTransition
 * @param trans
 *
 * set the connection for transition signals to scformview slots
 * when a file is opened, this is only called AFTER the datamodel loads EVERY state and transition in because transitions will require knowledge of their target state, which may not exist at the time of their creation.
 *
 */
void SCFormView::connectTransition(SCTransition* trans)
{
    //qDebug() << "SCFormView::connectTransition for " << trans->attributes.value("event")->asString();
    connect(trans, SIGNAL(markedForDeletion(QObject*)), this, SLOT(handleTransitionDeleted(QObject*)));
    connect(trans, SIGNAL(changedTarget(SCTransition*,SCState*)), this, SLOT(handleChangedTransitionTarget(SCTransition*,SCState*)));

    // when the target's name changes, also change the transition's attribute
    StateName* targetStateName = trans->targetState()->getStateNameAttr();
    connect(targetStateName, SIGNAL(changed(StateName*)), trans, SLOT(handleTargetStateNameChanged(StateName*)));

    // highlight the transition when it is clicked
    connect(trans, SIGNAL(clicked(SCTransition*)), this,SLOT(handleItemClicked(SCTransition*)));
}

void SCFormView::connectTransition(SCTransitionBranch * br)
{
    connect(br, SIGNAL(markedForDeletion(QObject*)), this, SLOT(handleTransitionBranchDeleted(QObject*)));


    // when the target state's name changes, also change the transition attribute
    StateName* targetStateName = br->targetState()->getStateNameAttr();
    connect(targetStateName, SIGNAL(changed(StateName*)), br, SLOT(handleTargetStateNameChanged(StateName*)));

    // highlight the branch when it is clicked
    connect(br, SIGNAL(clicked(SCTransitionBranch*)), this, SLOT(handleItemClicked(SCTransitionBranch*)));

}

void SCFormView::handleTransitionBranchDeleted(QObject *)
{

}



/**
 * @brief SCFormView::replantTree
 * Clears the tree view and property tables of the formview and reloads it using the datamodel
 *
 * called whenever the data model changes.
 *
 */
void SCFormView::replantTree()
{
    stateChartTreeView->clear();
    propertyTable->clear();
    textBlockPropertyTable->clear();

    _itemToTreeWidget.clear();
    _itemToTextBlock.clear();
    _treeWidgetToTextBlock.clear();

    qDeleteAll(_items); // delete the FVItem pointers
    _items.clear();

    QList<SCState*> states;
    states.append( _dm->getTopState());
    loadTreeState(NULL, states, false);
}

/**
 * @brief SCFormView::loadTreeState
 * @param parentItem
 * @param states
 * @param connect   boolean value, true will call connect functions for the state and its children
 *
 * Loads a state into the tree and all of its children states and children transitions
 *
 * when called with a NULL parentItem, the state is treated as the root machine
 *
 */
void SCFormView::loadTreeState(CustomTreeWidgetItem * parentItem, QList<SCState*> & states, bool connect)
{
    SCState* st;
    CustomTreeWidgetItem* item;
    for(int i = 0; i < states.count(); i++)
    {
        st = states.at(i);
        if(!st)
            continue;

        if(connect)
            connectState(st);

        // create a new tree widget item for this state

        // if this is the root machine, then do not give it a parent
        if(parentItem==NULL)
        {
            item = new CustomTreeWidgetItem();
            stateChartTreeView->addTopLevelItem((QTreeWidgetItem*)item);

        }
        else
            item = new CustomTreeWidgetItem(parentItem);

        // by default, open up the tree item
        item->setExpanded(true);

        // set the data, text, and icon of the tree item
        //item->setData(st);
        item->setText(0, st->attributes.value("name")->asString());
        item->setIcon(0,QIcon(":/SCFormView/cardboardbox.png"));

        // link the SCState item and this tree widget item using a QHash
        SCItem* stateToItem = st;
        _itemToTreeWidget.insert(stateToItem, item);
        FVItem* fvItem = new FVItem(st, FVItem::STATE, item);
        _items.insert(st,fvItem);
        item->setData(fvItem);

        // if this is the root machine, then set the top state and connect the state to its tree widget handlers
        if(parentItem == NULL)
        {
            _topState = fvItem;
            connectState(_topState->getState(), _topState->getTreeWidget());   // tree item handler
            //connectState(_topState->getState(), _topState->getTreeWidget(), "")
        }

        // get the state's text block
        SCTextBlock  * textBlock = st->getIDTextBlock();
        _itemToTextBlock.insert(st,textBlock);
        loadTreeTextBlock (item, textBlock);

        // get all substates of this state
        QList<SCState*> subStates;
        st->getStates(subStates);
        loadTreeState (item, subStates, connect);

        // get all transitions in this state
        QList<SCTransition*> transitions;
        st->getTransitions(transitions);
        loadTreeTransition (item, transitions, connect);
    }
}

/**
 * @brief SCFormView::loadTreeTextBlock
 * @param parentItem
 * @param textBlock
 *
 *
 * loads a textblock that belongs to either a state or transition into the hash table that link the two
 * this hash table will be used when the state or transition is selected and its text block properties will be loaded into the secondary property table
 *
 */
void SCFormView::loadTreeTextBlock(CustomTreeWidgetItem * parentItem , SCTextBlock* textBlock)
{
    _treeWidgetToTextBlock.insert(parentItem, textBlock);
}

/**
 * @brief SCFormView::loadTreeTransition
 * @param parentItem    parent tree widget item that must be a state
 * @param transitions
 * @param connect
 *
 * loads a list of transitions into the tree view, these are out transitions from the source state parentItem
 *
 *
 */
void SCFormView::loadTreeTransition(CustomTreeWidgetItem * parentItem , QList<SCTransition*> &transitions, bool connect)
{
    SCTransition* tr;
    CustomTreeWidgetItem* item;
    for(int i = 0; i < transitions.count();i++)
    {
        tr = transitions.at(i);
        if(!tr)
            continue;

        if(connect)
            connectTransition(tr);

       item = new CustomTreeWidgetItem(parentItem);
       //item->setData(tr);

       item->setText(0, tr->attributes.value("event")->asString());
       item->setIcon(0,QIcon(":/SCFormView/diagonalarrowhollow.png"));

       // connect the transition and tree widget item to the hashtable
       _itemToTreeWidget.insert(tr, item);
       FVItem* fvItem  =new FVItem(tr, FVItem::TRANSITION, item);
       _items.insert(tr,fvItem);
       item->setData(fvItem);

       // load the event text block for the transiton
       SCTextBlock  * textBlock = tr->getEventTextBlock();
       _itemToTextBlock.insert(tr,textBlock);
       loadTreeTextBlock(item, textBlock);
    }
}

/**
 * @brief SCFormView::connectTransition
 * @param trans
 * @param tableItem
 * @param attributeKey
 *
 * connects transition string attributes to table items so when the attribute changes, the table will also update
 */
void SCFormView::connectTransition(SCTransition * trans, CustomTableWidgetItem * tableItem, QString attributeKey)
{

    if(attributeKey == "event")
    {
        TransitionStringAttribute* tsa = trans->getTransStringAttr(attributeKey);
        connect(tsa, SIGNAL(changed(TransitionStringAttribute*)), tableItem, SLOT(handleAttributeChanged(TransitionStringAttribute*)));
    }
    else if(attributeKey == "target")
    {
        TransitionStringAttribute* tar = trans->getTransStringAttr(attributeKey);
        connect(tar, SIGNAL(changed(TransitionStringAttribute*)), tableItem, SLOT(handleAttributeChanged(TransitionStringAttribute*)));
    }
}



void SCFormView::connectTransition(SCTransition *trans, CustomTreeWidgetItem* treeItem)
{
    connect(trans->getTransStringAttr("event"), SIGNAL(changed(TransitionStringAttribute*)), treeItem, SLOT(handleAttributeChanged(TransitionStringAttribute*)));
}




void SCFormView::connectTransition(SCTransitionBranch *br, CustomTableWidgetItem * tableItem, QString attributeKey)
{
    if(attributeKey == "event")
    {
        TransitionStringAttribute* tsa = br->getTransStringAttr(attributeKey);
        connect(tsa, SIGNAL(changed(TransitionStringAttribute*)), tableItem, SLOT(handleAttributeChanged(TransitionStringAttribute*)));
    }
    else if (attributeKey == "target")
    {
        TransitionStringAttribute* tar = br->getTransStringAttr(attributeKey);
        connect(tar, SIGNAL(changed(TransitionStringAttribute*)), tableItem, SLOT(handleAttributeChanged(TransitionStringAttribute*)));
    }

}

void SCFormView::connectTransition(SCTransitionBranch* br, CustomTreeWidgetItem* treeItem)
{
    connect(br->getTransStringAttr("event"), SIGNAL(changed(TransitionStringAttribute*)), treeItem, SLOT(handleAttributeChanged(TransitionStringAttribute*)));
}

/**
 * @brief SCFormView::connectStateAttribute
 * @param state
 * @param tableItem
 * @param attributeKey
 *
 * called when a property table for the state is loaded
 * disconnects are not needed because when the property table is out of scope, the table widgets are destroyed and recreated
 *
 * helper function that connects changes in the data model to update the properties of a state in the property table.
 *
 *
 */
void SCFormView::connectState(SCState* state, CustomTableWidgetItem* tableItem, QString attributeKey)
{
    if(attributeKey == "size")
    {
        SizeAttribute* size = state->getSizeAttr();
        connect(size, SIGNAL(changed(SizeAttribute*)), tableItem, SLOT(handleAttributeChanged(SizeAttribute*)));
    }
    else if(attributeKey == "position")
    {
        PositionAttribute* pos = state->getPosAttr();
        connect(pos, SIGNAL(changed(PositionAttribute*)), tableItem, SLOT(handleAttributeChanged(PositionAttribute*)));
    }
    else if(attributeKey == "name")
    {
        StateName* name = state->getStateNameAttr();
        connect(name, SIGNAL(changed(StateName*)), tableItem, SLOT(handleAttributeChanged(StateName*)));

    }
    else    // this is a string attribute, so connect it
    {
        StateString * ss = state->getStringAttr(attributeKey);
        connect(ss, SIGNAL(changed(StateString*)), tableItem, SLOT(handleAttributeChanged(StateString*)));
    }
//    else if(attributeKey == "uid")
//    {
//        StateString* uid = state->getStringAttr("uid");
//        connect(uid, SIGNAL(changed(StateString*)), tableItem, SLOT(handleAttributeChanged(StateString*)));
//    }
//    else if(attributeKey == "entryAction")
//    {
//        StateString* ea = state->getStringAttr("entryAction");
//        connect(ea, SIGNAL(changed(StateString*)), tableItem, SLOT(handleAttributeChanged(StateString*)));
//    }
//    else if(attributeKey == "exitAction")
//    {
//        StateString* ea = state->getStringAttr("exitAction");
//        connect(ea, SIGNAL(changed(StateString*)), tableItem, SLOT(handleAttributeChanged(StateString*)));
//    }


}



void SCFormView::loadPropertyTable(FVItem* item)
{
    if(item->isState())
    {
        loadPropertyTable(item->getState());
    }
    else if (item->isTransition())
    {
        loadPropertyTable(item->getTransition());
    }
    else if (item->isTransitionBranch())
    {
        loadPropertyTable(item->getTransitionBranch());
    }
    propertyTable->setHorizontalHeaderLabels(QString("Attribute;Value").split(";"));

    // this does update the header labels for some reason
//    textBlockPropertyTable->setHorizontalHeaderLabels(QString("Attribute;Value").split(";"));
}

void SCFormView::loadPropertyTable(SCTransition* trans)
{
    IAttributeContainer* atts = trans->getAttributes();
    int row = 0;
    QMapIterator<QString,IAttribute*> i(*atts);

    int numRows = atts->size() - trans->doNotPrintSize();
    propertyTable->setRowCount(numRows);

    // load the event property first
    IAttribute* attr = atts->value("event");

    CustomTableWidgetItem * propName = new CustomTableWidgetItem("event");
    propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

    CustomTableWidgetItem * propValue = new CustomTableWidgetItem(attr->asString());
    propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));
    QFont bold("Arial", 8, QFont::Bold);
    propValue->setFont(bold);

    propertyTable->setItem(row, 0, propName);
    propertyTable->setItem(row++, 1, propValue);

    // connect this to the table widget item
    connectTransition(_currentlySelected->getTransition(), propValue, "event");

    // load the target property second
    attr = atts->value("target");

    propName = new CustomTableWidgetItem("target");
    propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

    propValue = new CustomTableWidgetItem(attr->asString());
    propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

    propertyTable->setItem(row, 0, propName);
    propertyTable->setItem(row++, 1, propValue);

    // connect this to the table widget item
    connectTransition(_currentlySelected->getTransition(), propValue, "target");

    while (i.hasNext())
    {
        QString key  = i.next().key();

        // only add the property if it's not in the do not display list
        if(trans->doNotPrint(key))
        {

        }
        else if(key == "event"||key=="target")
        {


        }
        else
        {



            IAttribute* attr = atts->value(key);

            CustomTableWidgetItem * propName = new CustomTableWidgetItem(key);
            propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

            CustomTableWidgetItem * propValue = new CustomTableWidgetItem(attr->asString());
            propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

            propertyTable->setItem(row, 0, propName);
            propertyTable->setItem(row++, 1, propValue);


            connectTransition(_currentlySelected->getTransition(), propValue, key);

         }
    }
}

void SCFormView::loadPropertyTable(SCTransitionBranch * trans)
{
    IAttributeContainer* atts = trans->getAttributes();
    int row = 0;
    QMapIterator<QString,IAttribute*> i(*atts);

//    int numRows = atts->size() - trans->doNotPrintSize();
    int numRows = atts->size();
    propertyTable->setRowCount(numRows);

    // load the event property first
    IAttribute* attr = atts->value("event");

    CustomTableWidgetItem * propName = new CustomTableWidgetItem("event");
    propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

    CustomTableWidgetItem * propValue = new CustomTableWidgetItem(attr->asString());
    propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));
    QFont bold("Arial", 8, QFont::Bold);
    propValue->setFont(bold);

    propertyTable->setItem(row, 0, propName);
    propertyTable->setItem(row++, 1, propValue);

    // connect this to the table widget item
    connectTransition(_currentlySelected->getTransitionBranch(), propValue, "event");

    // load the target property second
    attr = atts->value("target");

    propName = new CustomTableWidgetItem("target");
    propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

    propValue = new CustomTableWidgetItem(attr->asString());
    propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

    propertyTable->setItem(row, 0, propName);
    propertyTable->setItem(row++, 1, propValue);

    // connect this to the table widget item
    connectTransition(_currentlySelected->getTransitionBranch(), propValue, "target");

    while (i.hasNext())
    {
        QString key  = i.next().key();

        // only add the property if it's not in the do not display list

//        if(trans->doNotPrint(key))
//        {

//        }
//        else

        if(key == "event"||key=="target")
        {


        }
        else
        {



            IAttribute* attr = atts->value(key);

            CustomTableWidgetItem * propName = new CustomTableWidgetItem(key);
            propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

            CustomTableWidgetItem * propValue = new CustomTableWidgetItem(attr->asString());
            propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

            propertyTable->setItem(row, 0, propName);
            propertyTable->setItem(row++, 1, propValue);


            connectTransition(_currentlySelected->getTransitionBranch(), propValue, key);

         }
    }
}

/**
 * @brief SCFormView::loadPropertyTable
 * @param state     SCState to load property table
 *
 * loads the property table with the properties of the passed state.
 * additionally will call connectState on the state so that the property table is updated as the data model is for certain properties
 *
 */
void SCFormView::loadPropertyTable(SCState* state)
{
    IAttributeContainer* atts = state->getAttributes();
    int row = 0;
    QMapIterator<QString,IAttribute*> i(*atts);

    int numRows = atts->count() - state->doNotPrintSize();
    propertyTable->setRowCount(numRows);

    // add the name to the property table first

    {
        IAttribute* attr = atts->value("name");
        CustomTableWidgetItem * propName = new CustomTableWidgetItem("name");
        propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

        CustomTableWidgetItem * propValue = new CustomTableWidgetItem(attr->asString());
        propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));
        QFont bold("Arial", 8, QFont::Bold);
        propValue->setFont(bold);

        propertyTable->setItem(row, 0, propName);
        propertyTable->setItem(row++, 1, propValue);

        connectState(_currentlySelected->getState(), propValue, "name");
    }

    while (i.hasNext())
    {
        QString key  = i.next().key();

        if(state->doNotPrint(key))
        {

        }
        else if(key == "name")
        {

        }
        else
        {
            IAttribute* attr = atts->value(key);
            CustomTableWidgetItem * propName = new CustomTableWidgetItem(key);
            propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

            CustomTableWidgetItem * propValue = new CustomTableWidgetItem(attr->asString());
            propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

            propertyTable->setItem(row, 0, propName);
            propertyTable->setItem(row++, 1, propValue);

            connectState(_currentlySelected->getState(), propValue, key);
        }
    }


}

/**
 * @brief SCFormView::setAttributeConnections
 * @param atts
 * @param shouldConnect
 *
 * loads the property table with the given attributes of a state or transition
 *
 */
void SCFormView::setAttributeConnections(IAttributeContainer * atts, bool shouldConnect)
{
    if(!(atts)) // return if atts is null
    {
        //qDebug() << "Atts is null";
        return;
    }

    if(shouldConnect)
    {

        //qDebug() << "connecting state " << _currentlySelected->getState()->getStateNameAttr()->asString();

        int row = 0;
        QMapIterator<QString,IAttribute*> i(*atts);
        while (i.hasNext())
        {
            QString key  = i.next().key();
            IAttribute* attr = atts->value(key)  ;

            //connect ( attr, SIGNAL(changed(IAttribute*)), this, SLOT(handlePropertyChanged(IAttribute*)));

            CustomTableWidgetItem * propName = new CustomTableWidgetItem(key);

            propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

            CustomTableWidgetItem * propValue = new CustomTableWidgetItem(attr->asString());

            propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

            propertyTable->setItem(row, 0, propName);
            propertyTable->setItem(row++, 1, propValue);

            if(_currentlySelected->isState())
            {

                connectState(_currentlySelected->getState(), propValue, key);
            }

            if(_currentlySelected->isTransition())
            {
                connectTransition(_currentlySelected->getTransition(), propValue, key);
            }
        }
    } else {

        QMapIterator<QString,IAttribute*> i(*atts);
        while (i.hasNext())
        {

            //QString key  = i.next().key();
            //IAttribute* attr = atts->value(key)  ;
            //qDebug() <<  "deleting attribute connection :" << attr->asString();
            //disconnect ( attr, SIGNAL(changed(IAttribute*)), this, SLOT(handlePropertyChanged(IAttribute*)));
        }
    }
}

/**
 * @brief SCFormView::setTextBlockAttributeConnections
 * @param atts
 * @param connect
 *
 *
 * loads the textblock properties given in the attributes list into the secondary property table
 *
 *
 */
void SCFormView::setTextBlockAttributeConnections(IAttributeContainer* atts, bool connect)
{
    if(!atts)
        return;

    if(connect)
    {
        int row = 0;

        // insert the name at the top


        QMapIterator<QString,IAttribute*> i(*atts);
        while (i.hasNext())
        {
            QString key  = i.next().key();

            IAttribute* attr = atts->value(key)  ;

            //connect ( attr, SIGNAL(changed(IAttribute*)), this, SLOT(handlePropertyChanged(IAttribute*)));

            CustomTableWidgetItem * propName = new CustomTableWidgetItem(key);

            propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

            CustomTableWidgetItem * propValue = new CustomTableWidgetItem(attr->asString());

            propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

            textBlockPropertyTable->setItem(row, 0, propName);
            textBlockPropertyTable->setItem(row++, 1, propValue);
            if(_currentlySelected->isState())
            {
                connectTextBlock(_currentlySelected->getState()->getIDTextBlock(), propValue, key);
            }
            else if(_currentlySelected->isTransition())
            {
                connectTextBlock(_currentlySelected->getTransition()->getEventTextBlock(), propValue, key);
            }
        }
    }
    else
    {

        QMapIterator<QString,IAttribute*> i(*atts);
        while (i.hasNext())
        {

            //QString key  = i.next().key();
            //IAttribute* attr = atts->value(key)  ;
            //qDebug() <<  "deleting attribute connection :" << attr->asString();
            //disconnect ( attr, SIGNAL(changed(IAttribute*)), this, SLOT(handlePropertyChanged(IAttribute*)));
        }
    }

}



/**
 * @brief SCFormView::clearPropertyTable
 * @param del
 *
 * //disconnects all tree form handles
 * disconnects all property table handles
 *
 * called in handleTreeViewItemClicked
 *
 */
void SCFormView::clearPropertyTable()
{
    for (int r =0; r <propertyTable->rowCount(); r++ )
    {
        QTableWidgetItem* item = propertyTable->itemAt(r,0);
        delete item;
        item = propertyTable->itemAt(r,1);
        delete item;
    }
    propertyTable->clear();
    disconnect(propertyTable, SIGNAL(cellChanged(int,int)), this, SLOT(handlePropertyCellChanged(int,int)));
}

/**
 * @brief SCFormView::clearTextBlockPropertyTable
 *
 * disconnects and clears the text block property table
 *
 * called in handleTreeViewItemClicked
 *
 */
void SCFormView::clearTextBlockPropertyTable()
{
    for(int r = 0; r < textBlockPropertyTable->rowCount(); r++)
    {
        QTableWidgetItem* item = textBlockPropertyTable->itemAt(r,0);
        delete item;
        item = textBlockPropertyTable->itemAt(r,1);
        delete item;
    }
    textBlockPropertyTable->clear();
    disconnect(textBlockPropertyTable, SIGNAL(cellChanged(int,int)),this, SLOT(handleTextBlockPropertyCellChanged(int,int)));
}

/**
 * @brief SCFormView::handleTreeViewItemChanged
 * @param current
 * @param previous
 *
 * whenever a new item is selected in the tree view, this function is called
 * to set up the tool bars and property tables
 *
 */
void SCFormView::handleTreeViewItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
//    qDebug() << "SCFormView::handleTreeViewItemChanged";
    CustomTreeWidgetItem * item = dynamic_cast<CustomTreeWidgetItem*>(current);

    if(item)
    {
        _currentlySelected = dynamic_cast<FVItem*>(item->data());

        // clear the table, delete the old table items
        clearPropertyTable();

        // load the new attributes
        IAttributeContainer * currentAttributes =  _currentlySelected->getAttributes();

        // load the property table with the current state's attributes
        loadPropertyTable(_currentlySelected);

        // watch for user changes to the attributes
        connect(propertyTable, SIGNAL(cellChanged(int,int)), this, SLOT(handlePropertyCellChanged(int,int)));

        // clear the textblock property table
        clearTextBlockPropertyTable();

        // reload the textBlockPropertyTable
        currentAttributes = _currentlySelected->getTextBlockAttributes();

        // set up the text block property table with the attributes of the text block
        textBlockPropertyTable->setRowCount(currentAttributes->count());
        setTextBlockAttributeConnections(currentAttributes, true);

        // watch for user changes to the attributes from the property table
        connect(textBlockPropertyTable, SIGNAL(cellChanged(int,int)), this, SLOT(handleTextBlockPropertyCellChanged(int,int)));

        // depending on the selected item, change some of the buttons
        changeEditToolBar(_currentlySelected->getTypeInt());
    }
    else
    {
        qDebug() << "ERROR SCFormView::handleTreeViewItemChanged Item does not exist!";
    }
}

/**
 * @brief SCFormView::handleTreeViewItemClicked
 * Updates the attribute information for the currently highlight tree object
 * First clears the table of all attributes then
 * loads each attribute of the highlighted tree object
 * Disconnects handlePropertyCellChanged
 * connects handlePropertyCellChanged
 *
 * DEPRECATED FUNCTION, replaced by handleTreeViewItemChanged, which triggers on arrow key press in addition to mouse clicks
 *
 * @param qitem
 */
void SCFormView::handleTreeViewItemClicked(QTreeWidgetItem* qitem,int ){

    CustomTreeWidgetItem * item = dynamic_cast<CustomTreeWidgetItem*>(qitem);

    // display the type : State or Transition
    // display the name of the item
    // load its attributes in the table

    _currentlySelected = dynamic_cast<FVItem*>(item->data());
    // load the Title

    //QString selectedItemTitle = _currentlySelected->getType()  + " : " +  getCurrentlySelectedTitle();
    //selectedChartItem->setText(selectedItemTitle );

    /*
    if (_currentlySelected->getType() == "TextBlock" )
    {
        textToolBar->setEnabled(true);
    }
    else
    {
        textToolBar->setEnabled(false);
    }
*/
    // clear the table, delete the old table items
    clearPropertyTable();

    // load the new attributes
    IAttributeContainer * currentAttributes =  _currentlySelected->getAttributes();

    // load the property table with the current state's attributes
    loadPropertyTable(_currentlySelected);

    // watch for user changes to the attributes
    connect(propertyTable, SIGNAL(cellChanged(int,int)), this, SLOT(handlePropertyCellChanged(int,int)));

    // clear the textblock property table
    clearTextBlockPropertyTable();

    // reload the textBlockPropertyTable
    currentAttributes = _currentlySelected->getTextBlockAttributes();

    // set up the text block property table with the attributes of the text block
    textBlockPropertyTable->setRowCount(currentAttributes->count());
    setTextBlockAttributeConnections(currentAttributes, true);

    // watch for user changes to the attributes from the property table
    connect(textBlockPropertyTable, SIGNAL(cellChanged(int,int)), this, SLOT(handleTextBlockPropertyCellChanged(int,int)));

    // depending on the selected item, change some of the buttons
    changeEditToolBar(_currentlySelected->getTypeInt());
}

/**
 * @brief SCFormView::clearAndLoadPropertyTable
 * @param state
 *
 * clears the proprety table and reloads it with the properties of the given SCItem
 *
 */
void SCFormView::clearAndLoadPropertyTable(SCItem* oldItem,SCItem* item)
{
    // get the attributes of this item
    IAttributeContainer* atts = item->getAttributes();

    // delete all the rows of the property table and disconnect cell changed to handlePropertyChanged
    //clearPropertyTable();

    // set the number of rows to the number of attributes to load
    propertyTable->setRowCount(atts->count());

    int row = 0;
    QMapIterator<QString,IAttribute*> i(*atts);
    while (i.hasNext())
    {
        QString key  = i.next().key();
        IAttribute* attr = atts->value(key)  ;

        //connect ( attr, SIGNAL(changed(IAttribute*)), this, SLOT(handlePropertyChanged(IAttribute*)));

        CustomTableWidgetItem * propName = new CustomTableWidgetItem(key);

        propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

        CustomTableWidgetItem * propValue = new CustomTableWidgetItem(attr->asString());

        propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

        propertyTable->setItem(row, 0, propName);
        propertyTable->setItem(row++, 1, propValue);


    }
    connect(propertyTable, SIGNAL(cellChanged(int,int)), this, SLOT(handlePropertyCellChanged(int,int)));


}

/**
 * @brief SCFormView::reloadPropertyTable
 * reloads the property table with the currentAttributes
 */
void SCFormView::reloadPropertyTable()
{
    IAttributeContainer * currentAttributes = _currentlySelected->getAttributes();
   // clearPropertyTable();                   // clear out the table

    propertyTable->setRowCount(currentAttributes->count());     // load the table to have a set number of elements
    setAttributeConnections(currentAttributes, false);      // set up the attribute connections but do not call connect
}




/**
 * @brief SCFormView::handlePropertyChanged
 * When a property change is detected from the FormView's cell or the GraphicsView, update the attribute value
 * @param attr
 *
 *
 * // TODO marked for deletion, currently not used. replaced this function with multiple functions depending on SCItem and the property changed as it was too generic and we needed to be able to identify which SCItem actually changed its property
 *
 */
void SCFormView::handlePropertyChanged(IAttribute *attr)
{
    // user changed something, walk the table to find the attribute that the user changed,
    // and update the data model

    QString parentName;
    QString currentTreeItemName;

    if((attr->parent()))
        parentName = attr->parent()->objectName();
    else
        parentName = "";



    currentTreeItemName = propertyTable->item(0,1)->text();


    //qDebug()<< "handlePropertyChanged: "<<attr->asString() << " parent: " << parentName << " currentTreeItem: " << currentTreeItemName;
    //qDebug()<< propertyTable->parent()->objectName();

        for (int r =0; r <propertyTable->rowCount(); r++ )
        {
            QString txt = propertyTable->item(r,0)->text();
            QString key = attr->key() ;


            if (  txt == key )
            {
                propertyTable->item(r,1)->setText( attr->asString() );

                if ( key =="name")
                {
                    // the user changed the name
                    // selectedChartItem->setText( attr->asString()  );


                    // TODO bug when child(0) is not the textblock, there is a problem.
                    // reload the tree
                    stateChartTreeView->currentItem()->setText( 0, attr->asString() );
                    stateChartTreeView->currentItem()->child(0)->setText( 0, attr->asString() );


                }
            }

        }

}


void SCFormView::backgroundButtonGroupClicked(QAbstractButton *)
{

}


void SCFormView::buttonGroupClicked(int )
{

}



/**
 * @brief SCFormView::itemPromptTextBox
 *
 * connected to the QAction button insertTextBox
 *
 * will create a pop up for the user to name their new text box
 * and call itemInsertTextBox with the string input
 *
 */
void SCFormView::itemPromptTextBox()
{
    if(_currentlySelected->getType().isEmpty())
    {
        this->sendMessage("Error","Please select a tree object");
        return;
    }

    // prompt a user for a text box name
    bool ok;
    QString input = promptTextInput("New Text Box", "Text Box Title","", &ok);

    if(input.isEmpty()) // should be allowed to do this actually.
    {

    }

    if(ok)
    {
        itemInsertTextBlock(dynamic_cast<SCItem*>(_currentlySelected), input);
    }
}

/**
 * @brief SCFormView::itemInsertTextBox
 * @param item
 * @param name
 *
 * creates a new text block and adds it to the QList of textblocks that belongs to item
 * and adds it to the tree
 */
void SCFormView::itemInsertTextBlock(SCItem *item, const QString name)
{
    qDebug() << "item inesrt text box called ";
    // add the text block to the data model
    if(!_dm->insertNewTextBlock(item, name)){
        qDebug() << "failed here";
        return;
    }

    // insert the new text box into the tree and sc graphics view


    // loadTree (item, item->getTextBlock());

    SCTransition* trans = dynamic_cast<SCTransition*>(item);
    SCState* state = dynamic_cast<SCState*>(item);
    if(state)
    {

    }
    else if(trans)
    {
        /*
        qDebug() << "trans texblock object name "<<trans->getTextBlock(name)->objectName();
        //loadTree(item, trans->getTextBlock(name));
        connect(trans->getTextBlock(name), SIGNAL(destroyed(QObject*)), this, SLOT(handleTextBlockDeleted(QObject*)), Qt::QueuedConnection);

        // add the textblock to the tree view and reload the tree.
        emit trans->transitionAddTextBlock(trans->getTextBlock(name));
        this->reloadTree();

       // add the textblock to the sc graphics view
*/


    }

}

/**
   sendMessage

   will create a pop up message
   can set the window title and the message using the arguments

   has an OK button to dismiss the pop up
 */

void SCFormView::sendMessage(QString title, QString message)
{
    // TODO check if anything is highlighted
    QMessageBox msgBox;

    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    //msgBox.setDetailedText(message);
    //msgBox.setInformativeText(message);
    //msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint);
//    msgBox.move(160,200);
    QPoint offset(POP_UP_X, POP_UP_Y);
    msgBox.move(this->pos()+offset);
    msgBox.adjustSize();
    int ret = msgBox.exec();

    switch (ret)
    {
    case QMessageBox::Ok:
        // ok was pressed;
        break;

    default:
        // should never be reached
        break;
    }
}

/**
   promptTextInput

   Opens a popup with a text input field

   The arguments will set the pop up's window title, the message, and set the intial input text field's text

   returns the input text by the user or (not tested) an empty string if nothing is returned

 */
const QString SCFormView::promptTextInput(QString windowTitle, QString message, QString defaultText, bool* ok)
{

    QString mText;
    QInputDialog *inp = new QInputDialog(this);
    inp->setLabelText(message);
    inp->setWindowTitle(windowTitle);
    inp->setTextEchoMode(QLineEdit::Normal);
    inp->setTextValue(defaultText);
    //inp->resize(500,309);
    inp->adjustSize();
    QPoint offset(POP_UP_X, POP_UP_Y);
    inp->move(this->pos() + offset);
    if(inp->exec() == QDialog::Accepted){
        mText = inp->textValue();
        *ok = true;
    }
    return mText;
}

/**
 * @brief SCFormView::itemPromptProperty
 *
 * connected to the QAction insertProperty
 *
 * prompts the user for a property name to add to the currently highlighted item when the button is pressed
 *
 *
 * button pressed > itemPromptProperty() for the name > itemInsertProperty() to update the datamodel and the table
 *
 *
 */
void SCFormView::itemPromptProperty()
{
    qDebug() << "SCFormView::itemPropmtProperty()";
    // check if there is a highlighted tree object to add a property to
    if(_currentlySelected->getType().isEmpty())
    {
        this->sendMessage("Error","Please select a tree object");
        return;
    }


    // prompt the user for a property name
    bool ok;
    QString input = this->promptTextInput("New Property", "Insert "+_currentlySelected->getType()+" property", "", &ok);

    // check if the property name has a space (not supported by scxml reading)
    if(input.contains(' '))
    {
        sendMessage("Error", "Property Name cannot contain spaces");
        return;
    }
    else if(input.isEmpty())
    {
        sendMessage("Error", "Property Name cannot be empty");
        return;
    }

    // check if the input is valid
    if(ok && !input.isEmpty())
    {
        qDebug() << "inserting property1";
        // the user inputted something so insert this as a new property
        itemInsertProperty((_currentlySelected),input);
    }
}

/**
 * @brief SCFormView::itemInsertProperty
 * @param item
 * @param propertyName
 * Creates a new property for the highlighted tree object with given property name

   will update the datamodel for both SCTransitions and SCStates

   will also update the current property table, adding the new property to the top of the table
 */
void SCFormView::itemInsertProperty(FVItem* item, QString propertyName)
{
    QString itemType = _currentlySelected->getType();

    qDebug() << "item Type " << itemType;
    if(!_dm->insertNewProperty(item->getItem(), propertyName))
        return; // failed to insert, so halt here.

    // insert the new table item
    CustomTableWidgetItem * propName = new CustomTableWidgetItem(propertyName);
    propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

    CustomTableWidgetItem * propValue = new CustomTableWidgetItem("");
    propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

    // insert the new property at the top of the table
    propertyTable->insertRow(0);
    propertyTable->setItem(0, 0, propName);
    propertyTable->setItem(0, 1, propValue);
}


/**
   itemDeleteSelectedProperty()

  Deletes the currently selected propertyTable property if one is currently selected
  will remove it from the scform propertyTable and from the dataModel

 */
void SCFormView::itemDeleteSelectedProperty()
{
    // check for the highlighted row
    QString propertyName;
    QTableWidgetItem* marked = NULL;
    marked = propertyTable->currentItem();

    if(!marked)
    {
        sendMessage("Error", "Please select a property");
        return;
    }

    propertyName = propertyTable->item(marked->row(),0)->text();    // get the property name regardless of the column

    qDebug() <<"propertyName : " << propertyName;

    //propertyName = propertyTable->selectedItems().at(0);



    //propertyTable->getSelectedItem();
    qDebug() << "deleting";



    SCItem* item = _currentlySelected->getItem();



    if(!_dm->deleteProperty(item, propertyName))
    {
        sendMessage("Error", "Cannot delete core property");
        qDebug() << "failed to delete!";
        return;
    }

    propertyTable->removeRow(marked->row());    // remove the deleted property from the property table


}

/**
 * @brief SCFormView::deleteItem
 *
 * function called by the QAction Delete Item
 *
 * will prompt a delete confirm for the currently selected item
 *
 */
void SCFormView::deleteItem()
{
    // TODO check if anything is highlighted
    QMessageBox msgBox;

    if(_currentlySelected->isState() && _topState == _currentlySelected)
    {
        sendMessage("Error", "Cannot delete the root machine");
        return;
    }

    msgBox.setWindowTitle(" ");

    if(_currentlySelected->isState())
    {
        msgBox.setText("Deleting \"" +  _currentlySelected->getState()->objectName()+"\"");

    }
    else if(_currentlySelected->isTransition())
    {
        msgBox.setText("Deleting \"" +_currentlySelected->getTransition()->objectName()+"\"");
    }
    else
    {
        msgBox.setText("Deleting Selected Item");
    }

    msgBox.setInformativeText("Are you sure?");

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setWindowFlags( Qt::WindowStaysOnTopHint);
    QPoint offset(POP_UP_X, POP_UP_Y);
    msgBox.move(this->pos() + offset);
    int ret = msgBox.exec();

    switch (ret)
    {
    case QMessageBox::Ok:
        deleteItem(_currentlySelected->getItem());
        break;
    case QMessageBox::Cancel:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}

/**
 * @brief SCFormView::deleteItem
 * @param item
 *
 * called when the QAction deleteAction is triggered
 *
 *
 */
void SCFormView::deleteItem(QObject * item)
{

    _dm->deleteItem(item);
   // highlightRootItem();
}

void SCFormView::pointerGroupClicked(int)
{

}


void SCFormView::reselectTarget()
{
    if(_currentlySelected->isTransition())
    {
        //SCTransition* trans = _currentlySelected->getTransition();

        // create a new state tree window to select a state from
        _targetStateSelectionWindow = new StateSelectionWindow(this, _dm);

        // Pass the target and the target name to handleStateSelectionWindowStateSelected
        connect( _targetStateSelectionWindow, SIGNAL(stateSelected(SCState*)), this, SLOT(handleReselectTransitionTarget(SCState*)));

        _targetStateSelectionWindow->show();
    }
}

void SCFormView::reselectParent()
{
    if(_currentlySelected->isState())
    {
        SCState* st = _currentlySelected->getState();
        if(st == NULL)
        {
            sendMessage("Error","Click a state");
            return;
        }

        if(st->parent() == NULL)
        {
            sendMessage("Error","Cannot reselect parent for the root machine");
            return;
        }

        // create a new state tree window to select a state from
        _targetStateSelectionWindow = new StateSelectionWindow(this, _dm);

        // Pass the target and the target name to handleStateSelectionWindowStateSelected
        connect( _targetStateSelectionWindow, SIGNAL(stateSelected(SCState*)), this, SLOT(handleReselectParent(SCState*)));

        _targetStateSelectionWindow->show();


    }
    else if (_currentlySelected->isTransition())
    {
        //SCTransition* trans = _currentlySelected->getTransition();

        // create a new state tree window to select a state from
        _targetStateSelectionWindow = new StateSelectionWindow(this, _dm);

        // Pass the target and the target name to handleStateSelectionWindowStateSelected
        connect( _targetStateSelectionWindow, SIGNAL(stateSelected(SCState*)), this, SLOT(handleReselectTransitionParent(SCState*)));

        _targetStateSelectionWindow->show();
    }
    else
    {
        sendMessage("Error","Click a transition or state first");
    }




}

void SCFormView::handleReselectTransitionParent(SCState * source)
{
    _targetStateSelectionWindow->close();
    delete _targetStateSelectionWindow;
    _targetStateSelectionWindow = NULL;

    SCTransition* trans = _currentlySelected->getTransition();
    SCState* target = trans->targetState();
    QString eventName = trans->getEventName();

    _dm->deleteItem(trans);

    SCTransition* newTrans = _dm->insertNewTransition(source, target);
    newTrans->setEventName(eventName);

    highlightRootItem();
}

void SCFormView::handleReselectTransitionTarget(SCState * target)
{
    qDebug() << "SCFormView::handleReselectTransitionTarget";

    _targetStateSelectionWindow->close();
    delete _targetStateSelectionWindow;
    _targetStateSelectionWindow = NULL;

    //_currentlySelected->getTransition()->setTargetState(target);

    //TransitionAttributes * ta = new TransitionAttributes(_currentlySelected->getTransition()->attributes);

    //_dm->handleMakeANewTransition(dynamic_cast<TransitionAttributes*>(_currentlySelected->getTransition()->getAttributes()));

    SCTransition* trans = _currentlySelected->getTransition();
    SCState* source = trans->parentSCState();
    QString eventName = trans->getEventName();

    _dm->deleteItem(trans);

    SCTransition* newTrans = _dm->insertNewTransition(source, target);
    newTrans->setEventName(eventName);

    highlightRootItem();

}

void SCFormView::handleChangedTransitionTarget(SCTransition * trans, SCState * newTarget)
{
    qDebug() << "SCFormView::handleChangedTransitionTarget";

}

void SCFormView::handleReselectParent(SCState * target)
{
    qDebug() << "SCFormView::handleReselectParent";
    _currentlySelected->getState()->reselectParent(target);
    _targetStateSelectionWindow->close();
    delete _targetStateSelectionWindow;
    _targetStateSelectionWindow = NULL;
}

/**
 * @brief SCFormView::handleChangedParent
 * @param state
 * @param newParent
 *
 * state changed parent to newParent
 *
 * deletes all out transitions and recreates them using their old properties
 *
 *
 *
 */
void SCFormView::handleChangedParent(SCState* state,SCState* newParent)
{
    qDebug() << "SCFormView::handleChangedParent";
    QTreeWidgetItem* stateWidget            = _items.value(state)->getTreeWidget();
    QTreeWidgetItem* currentParentWidget    = _items.value(state->getParentState())->getTreeWidget();
    QTreeWidgetItem* newParentWidget        = _items.value(newParent)->getTreeWidget();

    //stateWidget->setParent();

    //_items.value(state)->getTreeWidget()->setParent(_items.value(newParent)->getTreeWidget());
    //_items.value(newParent)->getTreeWidget()->addChild(_items.value(state)->getTreeWidget());
    currentParentWidget->removeChild(stateWidget);
    newParentWidget->addChild(stateWidget);
    stateWidget->setExpanded(true);


    QList<SCTransition*> transitions;
    state->getAllTransitions(transitions);
    // these are the sink anchors that belong to out transitions of the state
    for(int i = 0; i < transitions.size(); i++)
    {
        SCTransition* trans = transitions.at(i);
        QString eventName = trans->getEventName();
        QString pathStr = trans->getPathAttr()->asString();
        qDebug() << "pathStr: " << pathStr;
        _dm->deleteItem(trans);
        _dm->insertNewTransition(state, trans->targetState(), eventName, pathStr);
    }


    highlightRootItem();
}

/**
 * @brief SCFormView::insertTransition
 *
 * Part 1 of inserting a new transition is to open up a new target state selection window
 *
 * the currently selected state is the source
 * the chosen state from the state selection window is the target
 *
 * will next call
 * handleStateSelectionWindowStateSelected and is passed the target state and its name
 *
 */
void SCFormView::insertTransition()
{

    //SCState * st = dynamic_cast<SCState *> (_currentlySelected);
    SCState* st = _currentlySelected->getState();
    if ( st == NULL ) return;

    if ( st->parent() == NULL)
    {
        /*
        QMessageBox msgBox;
        msgBox.setText("cannot add state from root machine");
        msgBox.exec();*/
        sendMessage("Error", "Cannot add transition from root machine");
        return;
    }

    // need to force the user to select a target state for this new transition

    // create a new state tree window to select a state from
    _targetStateSelectionWindow = new StateSelectionWindow(this, _dm);

    // Pass the target and the target name to handleStateSelectionWindowStateSelected
    connect( _targetStateSelectionWindow, SIGNAL(stateSelected(SCState*)), this, SLOT(handleStateSelectionWindowStateSelected(SCState*)));

    _targetStateSelectionWindow->show();






}

/**
 * @brief SCFormView::handleStateSelectionWindowStateSelected
 * @param targetName
 *
 * SLOT
 * connect is in insertTransition
 *
 * connect(StateSelectionWindow, stateSelected, scformview, handleStateSelectionWindowStateSelected)
 *
 *
 *
 *
 */

//#define FORKED_ENABLE

void SCFormView::handleStateSelectionWindowStateSelected(SCState* target)
{
    QList<QTreeWidgetItem*> selected = stateChartTreeView->selectedItems();

    // check if one or multiple states are selected
    if(selected.size()==1)
    {
        // user has clicked on a new state, create the transition with this target state
        SCState* st= _currentlySelected->getState();
        _dm->insertNewTransition(st, target);


    }
    else    // multiple source states
    {
#ifdef FORKED_ENABLE
        QList<SCState*> states;
        // add all the scstates to a listn
        for(int i = 0; i < selected.size(); i++)
        {
            FVItem* fvItem = dynamic_cast<FVItem*>(dynamic_cast<CustomTreeWidgetItem*>(selected.at(i))->data());
            qDebug() << "i: " << i<<"\t" <<fvItem->getState()->objectName();
            states.append(fvItem->getState());
        }

        // insert the transition
        _dm->insertNewTransition(states,target);
#endif


#ifndef FORKED_ENABLE
        // user has clicked on a new state, create the transition with this target state
        SCState* st= _currentlySelected->getState();
        _dm->insertNewTransition(st, target);
#endif

    }

    // delete the selection window
    _targetStateSelectionWindow->close();
    delete _targetStateSelectionWindow;
    _targetStateSelectionWindow = NULL;

}

/**
 * @brief SCFormView::updateCurrentlySelected
 *
 * sets currently selected to the first item in the tree view's selected list
 *
 * DEPRECATED FUNCTION, no longer needed due to changing how items are selected
 *
 */
void SCFormView::updateCurrentlySelected()
{
    QList<QTreeWidgetItem*> selected = stateChartTreeView->selectedItems();
    CustomTreeWidgetItem* ti = dynamic_cast<CustomTreeWidgetItem*>(selected.at(0));
    _currentlySelected = dynamic_cast<FVItem*> (ti->data());
}

/**
 * @brief SCFormView::insertState
 *
 * SLOT
 * connect in SCFormView
 * connect(QAction insertStateAction, SIGNAL(triggered()), SCFormView, SLOT(insertState()));
 *
 * when the insert state button is pushed, the datamodel will create a new SCState
 */
void SCFormView::insertState()
{
    //SCState * st  = dynamic_cast<SCState*>(_currentlySelected);

    //updateCurrentlySelected();

    SCState* st = _currentlySelected->getState();
    if ( st == NULL ) return;




    qDebug() << "inserting new state into parent = " + _currentlySelected->getState()->objectName();

    _dm->insertNewState(st);

    // SCState connects
}

void SCFormView::bringToFront()
{
    if(_currentlySelected->isState())
    {
        emit _currentlySelected->getState()->bringToFront(_currentlySelected->getState());
    }
}

void SCFormView::sendToBack()
{
    if(_currentlySelected->isState())
    {
        emit _currentlySelected->getState()->sendToBack(_currentlySelected->getState());
    }
}

/**
 * @brief SCFormView::findItem
 * @param item
 * @return
 *
 * uses the QHash<SCItem*, CustomTreeWidgetItem*> to find a CustomTreeWidgetItem* based on the given SCTransition*
 * These QHash entries are inserted in loadTree
 */
CustomTreeWidgetItem* SCFormView::findItem(SCTransition* item)
{

   // CustomTreeWidgetItem* find = _itemToTreeWidget.value(dynamic_cast<SCItem*>(item));

    CustomTreeWidgetItem* find = _items.value(dynamic_cast<SCItem*>(item))->getTreeWidget();
    return find;
}

/**
 * @brief SCFormView::findItem
 * @param item
 * @return
 *
 * uses the QHash<SCItem*, CustomTreeWidgetItem*> to find a CustomTreeWidgetItem* based on the given SCState*
 * These QHash entries are inserted in loadTree
 *
 */
CustomTreeWidgetItem* SCFormView::findItem(SCState* item)
{
    /*
    CustomTreeWidgetItem* find = _itemToTreeWidget.value(dynamic_cast<SCItem*>(item));
    if(find)
         qDebug() << "findItem for SCItem to CustomTreeWidgetItem: " << find->text(0);
    else
        qDebug() << "could not find item for item: ";*/


    CustomTreeWidgetItem* find = _items.value(dynamic_cast<SCItem*>(item))->getTreeWidget();
    return find;
}

CustomTreeWidgetItem* SCFormView::findItem(SCItem* item)
{
    return _items.value(item)->getTreeWidget();
}


/**
 * @brief SCFormView::handleItemSizeChangedInDataModel
 * @param state
 * @param size
 *
 * SLOT
 * connect in SCFormView
 * connect(SCState, SIGNAL(sizeChangedInDataModel() ,  SCFormView, handleItemSizeChangedInDataModel)
 *
 * when the data model updates its size attribute, update the property table too
 *
 */
void SCFormView::handleItemSizeChangedInDataModel(SCState* state, QPointF size)
{
    qDebug() << "handleItemSizeChangedInDataModel state pos: " << state->attributes.value("size")->asString() << " size: " << size;

    if(state)
    {
        // state size was changed in data model

        // update the property table to the new size
        SizeAttribute * sz = dynamic_cast<SizeAttribute *> (state->attributes.value("size"));
        if(isCurrentlySelectedEqualTo(state))
        {
            for(int i = 0 ; i < propertyTable->rowCount();i++)
            {
                if(propertyTable->item(i,0)->text() == "size")
                {
                    propertyTable->item(i,1)->setText(sz->asString());
                    break;
                }
            }
        }
    }
}

/**
 * @brief SCFormView::handleItemPositionChangedInDataModel
 * @param state
 *
 * SLOT
 * connect in SCFormView::insertState and SCFormView::loadTree (for states)
 * connect(SCState, SIGNAL(positionChangedInDataModel(SCState*)), SCFormView, SLOT(handleItemPositionChangedInDataModel(SCState*)));
 *
 *
 * when SCState sets its position, it will also emit positionChanged to update it in the formview
 *
 */
void SCFormView::handleItemPositionChangedInDataModel(SCState* state, QPointF point)
{
    qDebug() << "handleItemPositionChangedInDataModel state pos: " << state->attributes.value("position")->asString() << " point: " << point;

    if(state)
    {
        // state position was changed in data model

        // update the property table to the new position
        PositionAttribute * pos = dynamic_cast<PositionAttribute *> (state->attributes.value("position"));
        qDebug() << "state position: updated the datamodel! "<< pos->asPointF();

        if(isCurrentlySelectedEqualTo(state))
        {
            for(int i = 0 ; i < propertyTable->rowCount();i++)
            {
                if(propertyTable->item(i,0)->text() == "position")
                {
                    propertyTable->item(i,1)->setText(pos->asString());
                    break;
                }
            }
        }

    }
}

int SCFormView::propertyTableIndexOf(QString propertyName)
{
   for(int i = 0 ;  i < propertyTable->rowCount(); i++)
   {
       if(propertyTable->item(i,0)->text() == propertyName)
       {
           //propertyTable->item(i,1)->setText(pos->asString());
           return i;
       }
   }
   return -1;
}
int SCFormView::textBlockPropertyTableIndexOf(QString propertyName)
{
    for(int i = 0 ;  i < textBlockPropertyTable->rowCount(); i++)
    {
        if(textBlockPropertyTable->item(i,0)->text() == propertyName)
        {
            //propertyTable->item(i,1)->setText(pos->asString());
            return i;
        }
    }
    return -1;
}

void SCFormView::propertyTableSetText(int index, QString text)
{
    propertyTable->item(index,1)->setText(text);
}

void SCFormView::textBlockPropertyTableSetText(int index, QString text)
{
    textBlockPropertyTable->item(index,1)->setText(text);
}


/**
 * @brief SCFormView::handleItemPositionChangedInDataModel
 * @param tb
 * @param point
 *
 * SLOT
 * connect in SCFormView
 *
 *
 */
void SCFormView::handleItemPositionChangedInDataModel(SCTextBlock* tb, QPointF point)
{
    //qDebug() << "SCFormView:: handle tb position changed in data model";

    SCState* state = dynamic_cast<SCState*>(_currentlySelected);

    SCTransition* trans = dynamic_cast<SCTransition*>(_currentlySelected);

    if(state)
    {
        qDebug() << "SCFormView:: updating state textblock position";
        // check that this textBlock is the currently selected one's
        if(state->getIDTextBlock() == tb)
        {
            // update the textblock property table
            textBlockPropertyTableSetText(textBlockPropertyTableIndexOf("position"), state->getIDTextBlock()->attributes.value("position")->asString());
        }
    }
    else if(trans)
    {
        qDebug() << "SCFormView:: updating transition textblock position";
        if(trans->getEventTextBlock() == tb)
        {
            textBlockPropertyTableSetText(textBlockPropertyTableIndexOf("position"), trans->getEventTextBlock()->attributes.value("position")->asString());
        }
    }
    else
    {
        qDebug() << "SCFormView: ERROR: cannot update textblock " << tb->objectName();
    }
}

/**
 * @brief SCFormView::handleItemSizeChangedInDataModel
 * @param tb
 * @param size
 *
 * SLOT
 * connect in SCFormView
 *
 * when the selectabletextblock for the textbox in the graphics view calls graphicsHasChanged, this function will update the size value shown in the textblock propery table
 *
 */
void SCFormView::handleItemSizeChangedInDataModel(SCTextBlock *tb, QPointF size)
{
    //qDebug() << "SCFormView:: handle tb size changed in data model";
    SCState* state = dynamic_cast<SCState*>(_currentlySelected);
    SCTransition* trans = dynamic_cast<SCTransition*>(_currentlySelected);

    if(state)
    {
        qDebug() << "SCFormView:: updating state textblock size";

        // check that this textBlock is the currently selected one's
        if(state->getIDTextBlock() == tb)
        {
            // update the textblock property table
            textBlockPropertyTableSetText(textBlockPropertyTableIndexOf("size"), state->getIDTextBlock()->attributes.value("size")->asString());
        }
    }
    else if(trans)
    {
        qDebug() << "SCFormView:: updating transition textblock size";
        if(trans->getEventTextBlock() == tb)
        {
            textBlockPropertyTableSetText(textBlockPropertyTableIndexOf("size"), trans->getEventTextBlock()->attributes.value("size")->asString());
        }
    }
    else
    {
        qDebug() << "SCFormView: ERROR: cannot update textblock " << tb->objectName();
    }
}


/**
 * @brief SCFormView::handleItemNameChangedInDataModel
 * @param trans
 * @param eventName
 *
 * SLOT
 * connect in SCFormView
 * connect(SCTransition, SIGNAL(eventChangedInDataModel(SCTransition*,QString)),SCFormView,SLOT(handleItemNameChangedInDataModel(SCTransition*,QString)));
 *
 * updates the tree and property table when the event name of transition changes in the data model
 */
void SCFormView::handleItemNameChangedInDataModel(SCTransition* trans, QString eventName)
{
    qDebug() << "SCFormView::handleItemNameChangedInDataModel transition event name was changed in the data model";

    if(trans)
    {
        // update the tree
        QTreeWidgetItem* treeItem = findItem(trans);
        treeItem->setText(0, eventName);

        // update the property table (if necessary)
        if(isCurrentlySelectedEqualTo(trans))
        {
            for(int i = 0 ; i < propertyTable->rowCount();i++)
            {
                if(propertyTable->item(i,0)->text() == "event")
                {
                    propertyTable->item(i,1)->setText(eventName);
                    break;
                }
            }
        }

        /*
        // update the selectable text box
        trans->setText(eventName);
        */
    }
}


/**
 * @brief SCFormView::handleItemNameChangedInDataModel
 * @param state
 * @param name
 *
 *
 * connects the SCState item to update the tree/property table text when it changes name
 * connect(SCState, nameChanged, SCFormView, handleItemNameChangedInDataModel)
 *
 *
 * called when a state's name is changed to update:
 *
 * SCState's attribute's name
 * tree view name
 * property name (if currently selected)
 * and transition target names
 *
 */
void SCFormView::handleItemNameChangedInDataModel(SCState* state, QString name)
{
    qDebug() << "handleItemNameChangedInDataModel change to  " << name;

    // reload the name in the property table (if needed) and in the tree form
    if(state)
    {
        // State name was changed in the datamodel, so update the tree and property table

        QTreeWidgetItem* treeItem = findItem(state);
        treeItem->setText(0, name);  // update the tree

        if(isCurrentlySelectedEqualTo(state))  // update the property table
        {
            for(int i = 0 ; i < propertyTable->rowCount(); i++)
            {

                if(propertyTable->item(i,0)->text() == "name")
                {
                    propertyTable->item(i,1)->setText(name);
                    break;
                }
            }
        }

        // update any transitions that target this state

        QList<SCTransition*> ins = state->getTransitionsIn();
        //qDebug() << "got into name " << "ins count: " << ins.count();
        for(int i = 0; i < ins.count(); i++)
        {
            //qDebug() << "inbound transition to state: " << st->attributes.value("name")->asString() << " have target " << ins.at(i)->attributes.value("target")->asString();
            ins.at(i)->setAttributeValue("target",name);
            if(isCurrentlySelectedEqualTo(ins.at(i)))
            {
                for(int i = 0 ; i < propertyTable->rowCount(); i++)
                {

                    if(propertyTable->item(i,0)->text() == "target")
                    {
                        propertyTable->item(i,1)->setText(name);
                        break;
                    }
                }
            }
        }


    }
}

bool SCFormView::isCurrentlySelectedEqualTo(SCItem* item)
{
    SCItem* cs = dynamic_cast<SCItem*>(_currentlySelected);
    if(item == cs)
        return true;
    return false;
}



void SCFormView::textInserted(QGraphicsTextItem *)
{

}


void SCFormView::currentFontChanged(const QFont &)
{

}


void SCFormView::fontSizeChanged(const QString &)
{

}


void SCFormView::sceneScaleChanged(const QString &)
{

}


void SCFormView::textColorChanged()
{

}


void SCFormView::itemColorChanged()
{

}


void SCFormView::lineColorChanged()
{

}


void SCFormView::textButtonTriggered()
{

}


void SCFormView::fillButtonTriggered()
{

}





void SCFormView::lineButtonTriggered()
{

}


void SCFormView::handleFontChange()
{

}




void SCFormView::about()
{

//    QString text = QString("\nDeveloped by: David Drell & Matt Zhan\nhttps://github.com/daviddrell/visualsc");
//    QString er = tr("<b>VisualSC</b>  is a Statechart Editor which read/writes from/to SCXML %1\nsurname: %2\ndata: %3\ntest:%4").arg("wat");
//    QMessageBox ab;
//    ab.setWindowTitle("About Visual Statechart");
//    ab.setText(er);
//    ab.exec();

    //QMessageBox::about(this, tr("About Visual State Chart"), "");

    /*QMessageBox::about(this, "About Visual Statechart",
                       "https://github.com/daviddrell/visualsc"
                       "\n"
                       "VisualSC is a Statechart Editor which reads/writes from/to SCXML\n"
                       "\n"
                       "This is an open source project by David Drell & Matt Zhan"
                       ""


                       "");*/

    sendMessage("About Visualstate Chart","https://github.com/daviddrell/visualsc"
                                          "\n"
                                          "VisualSC is a Statechart Editor which reads/writes from/to SCXML\n"
                                          "\n"
                                          "This is an open source project by David Drell & Matt Zhan"
                                          "\n\n"
                                          "Icons obtained from https://icons8.com/"


                                          "");



}

void SCFormView::viewKeybinds()
{
    sendMessage("Keybinds", tr("N   Creates a new elbow joint for a hovered transition\nD   Deletes a hovered elbow joint"));






}

void SCFormView::setProject(SMProject *pj)
{
    _project = pj;
}



void SCFormView::handleNewRootMachine(SCState*)
{
    qDebug() << "SCFormView::handleNewRootMachine";
    this->initTree();
}

SCState* SCFormView::getCurrentlySelectedState()
{
    if(!_currentlySelected->isState())
    {
        sendMessage("Error", "Please select a state");
        return NULL;
    }

    return _currentlySelected->getState();
}

/**
 * @brief SCFormView::createActions
 *
 * new all actions, set their text tips, set their icons, and connect them to their functions
 */
void SCFormView::createActions()
{

    insertStateAction = new QAction(QIcon(":/SCFormView/cardboardboxnew.png"), tr("Insert State"), this);
    insertStateAction->setShortcut(tr("Ctrl+Q"));
    insertStateAction->setStatusTip(tr("Insert State"));
    connect(insertStateAction, SIGNAL(triggered()), this, SLOT(insertState()));


    insertTransitionAction = new QAction(QIcon(":/SCFormView/rightarrowhollownew.png"), tr("Insert Transition"), this);
    insertTransitionAction->setShortcut(tr("Ctrl+W"));
    insertTransitionAction->setStatusTip(tr("Insert Transition"));
    connect(insertTransitionAction, SIGNAL(triggered()), this, SLOT(insertTransition()));


    deleteAction = new QAction(QIcon(":/SCFormView/cancelw10.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    reselectParentAction = new QAction(QIcon(":/SCFormView/family.png"), tr("Reset State's Parent"), this);
    reselectParentAction->setShortcut(tr("Reset State's Parent"));
    reselectParentAction->setStatusTip(tr("Reset State's Parent"));
    connect(reselectParentAction, SIGNAL(triggered()), this, SLOT(reselectParent()));

    reselectTargetAction = new QAction(QIcon(":/SCFormView/reselect.png"), tr("Reset Transition's Target"), this);
    reselectTargetAction->setShortcut(tr("Reset Transition's Target"));
    reselectTargetAction->setStatusTip(tr("Reset Transition's Target"));
    connect(reselectTargetAction, SIGNAL(triggered()), this, SLOT(reselectTarget()));

    toFrontAction = new QAction(QIcon(":/SCFormView/framein.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));


    sendBackAction = new QAction(QIcon(":/SCFormView/frameout.png"),
                                 tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+B"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));


    // file menu items
    newAction = new QAction(QIcon(":/SCFormView/newfile.png"),tr("N&ew"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setStatusTip(tr("Create a new State Machine from scratch"));
    //connect(newAction, SIGNAL(triggered()),  this->_project, SLOT(handleNewClick()));
connect(newAction, SIGNAL(triggered()),  this, SIGNAL(newClick()));

    openAction = new QAction(QIcon(":/SCFormView/openfolder.png"),tr("O&pen"), this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(tr("Open an .SCXML file"));
    //connect(openAction, SIGNAL(triggered()), this->_project, SLOT(handleFileOpenClick()));
    connect(openAction, SIGNAL(triggered()), this, SIGNAL(openClick()));


    saveImageAction = new QAction(QIcon(":/SCFormView/picture.png"), tr ("S&ave Image"), this);
    saveImageAction->setShortcut(tr("Ctrl+P"));
    saveImageAction->setStatusTip(tr("Save to .png"));
    connect(saveImageAction, SIGNAL(triggered()), this, SIGNAL(saveImageClick()));

    saveAction = new QAction(QIcon(":/SCFormView/save.png"),tr("S&ave"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Save this State Machine to an .SCXML"));
    connect(saveAction, SIGNAL(triggered()), this, SIGNAL(saveClick()));
//    connect(saveAction, SIGNAL(triggered()),  this->_project, SLOT(handleFileSaveClick()));

    saveAsAction = new QAction(QIcon(":/SCFormView/save.png"),tr("S&ave As"), this);
    saveAsAction->setShortcut(tr("Ctrl+Shift+S"));
    saveAsAction->setStatusTip(tr("Save this State Machine as an .SCXML"));
    connect(saveAsAction, SIGNAL(triggered()), this, SIGNAL(saveAsClick()));

    exportAction = new QAction(QIcon(":/SCFormView/export.png"),tr("E&xport to Code"), this);
    exportAction->setShortcut(tr("Ctrl+E"));
    exportAction->setStatusTip(tr("Export to .cpp & .h"));
//    connect(exportAction, SIGNAL(triggered()),  this->_project, SLOT(handleFileExportClick()));
    connect(exportAction, SIGNAL(triggered()), this, SIGNAL(exportClick()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit VisualSC Editor"));


    importAction = new QAction(QIcon(":/SCFormView/import.png"),tr("I&mport .SCXML"), this);
    importAction->setShortcut(tr("Ctrl+I"));
    importAction->setStatusTip(tr("Import an SCXML state machine into the selected State"));
    connect(importAction, SIGNAL(triggered()), this, SIGNAL(importClick()));
    //connect(importAction, SIGNAL(triggered()), this, SLOT(import()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/SCFormView/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleBoldChanged()));

    italicAction = new QAction(QIcon(":/SCFormView/italic.png"),   tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/SCFormView/underline.png"),  tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(QIcon(":/SCFormView/infow8.png"),tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+H"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    viewKeybindsAction = new QAction(tr("Keybinds"), this);
    viewKeybindsAction->setShortcut(tr("Ctrl+K"));
    connect(viewKeybindsAction, SIGNAL(triggered()), this, SLOT(viewKeybinds()));


    insertTextBox = new QAction(QIcon(":/SCFormView/textbox.png"),
                               tr("Insert Textbox"), this);
    insertTextBox->setShortcut(tr("Ctrl+T"));
    insertTextBox->setStatusTip(tr("Insert a text box for the selected item"));
    connect(insertTextBox, SIGNAL(triggered()), this, SLOT(itemPromptTextBox()));

    insertProperty = new QAction(QIcon(":/SCFormView/plusw10.png"), tr("Add Attribute"),this);
    insertProperty->setStatusTip(tr("Insert a property for the selected item"));
    connect(insertProperty, SIGNAL(triggered()), this, SLOT(itemPromptProperty()));

    deleteProperty = new QAction(QIcon(":/SCFormView/minusw10.png"), tr("Remove Attribute"), this);
    deleteProperty->setStatusTip(tr("Insert a property for the selected item"));
    connect(deleteProperty, SIGNAL(triggered()), this, SLOT(itemDeleteSelectedProperty()));

    attributeInfoAction = new QAction(QIcon(":/SCFormView/infow8.png"), tr("About Attributes"), this);
    attributeInfoAction->setStatusTip(tr("About Attributes"));
    connect(attributeInfoAction, SIGNAL(triggered()), this, SLOT(aboutAttributes()));

}

void SCFormView::aboutAttributes()
{
    if(_currentlySelected->isState())
    {
        sendMessage("State Attribute Descriptions",
                    "name\t\tState's Name\n"
                    "entryAction\tComma separated list of entry events e.g. \"action1,action2,action3\"\n"
                    "exitAction\tComma separated list of exit events\n"
                    "finalState\t(true/false) Final state of the parent state machine [OPTIONAL]\n"
                    "initialState\t(true/false) Initial state of a state machine [REQUIRED unless parallel]\n"
                    "parallelState\t(true/false) True = direct children states are concurrently run\n"
                    "position\tPosition in parent's coordinate system\n"
                    "size\t\tSize in pixels\n"
                    "type\t\tfor state machine recognition\n"
                    "uid\t\tUnique ID used in .SCXML and code generation"
                    );

    }
    else if(_currentlySelected->isTransition())
    {
        sendMessage("Transition Attribute Descriptions",
                    "event\t\t\tTransition's Event Name\n"
                    "target\t\t\tTransition's Target State\n"
                    "comments\t\tAdds a comment to the addTransition call in constructor\n"
                    "connectToFinished\t(true/false) Code generator constructor will call \n\t\t\taddTransition for source state's finished() signal\n"
                    "path\t\t\tElbow positions"
                    );
    }
}

void SCFormView::handleBoldChanged()
{
    SCTextBlock * tb = SCDataModel::getAsTextBlock(_currentlySelected);
    if ( ! tb ) return;

    IAttribute* attr = tb->attributes.value("font-bold")  ;

    if ( ! attr ) return;

    if ( boldAction->isChecked() )
        attr->setValue("true");
    else
        attr->setValue("false");

}

void SCFormView::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(importAction);
    fileMenu->addAction(exportAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveImageAction);
    //fileMenu->addAction(exitAction);


    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(insertTransitionAction);
    itemMenu->addAction(insertStateAction);
    itemMenu->addSeparator();
    itemMenu->addAction(reselectParentAction);
    itemMenu->addAction(reselectTargetAction);
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
    aboutMenu->addAction(viewKeybindsAction);
}

/**
 * @brief SCFormView::changeEditToolBar
 * @param type
 *
 * change the edit tool bars to a configuration based on what is currently selected.
 *
 */
void SCFormView::changeEditToolBar(int type)
{
    switch(type)
    {
    case FVItem::STATE:
        reselectParentAction->setText(tr("Reset State's Parent"));
        reselectParentAction->setShortcut(tr("Reset State's Parent"));
        reselectParentAction->setStatusTip(tr("Reset State's Parent"));

        reselectTargetAction->setIcon(QIcon(":/SCFormView/reselectgrey.png"));
        reselectTargetAction->setEnabled(false);

        insertTransitionAction->setEnabled(true);
        insertStateAction->setEnabled(true);

        break;

    case FVItem::TRANSITION:
        reselectParentAction->setText(tr("Reset Transition's Parent"));
        reselectParentAction->setShortcut(tr("Reset Transition's Parent"));
        reselectParentAction->setStatusTip(tr("Reset Transition's Parent"));

        reselectTargetAction->setIcon(QIcon(":/SCFormView/reselect.png"));
        reselectTargetAction->setEnabled(true);

        insertTransitionAction->setEnabled(false);
        insertStateAction->setEnabled(false);
        break;
    }
}

void SCFormView::createToolbars()
{

    propertyToolBar = new QToolBar("Property",this);
    propertyToolBar->addAction(insertProperty);
    propertyToolBar->addAction(deleteProperty);
   // propertyToolBar->addAction(insertTextBox);
    propertyToolBar->addSeparator();
    propertyToolBar->addAction(attributeInfoAction);

    //editToolBar = addToolBar(tr("Edit"));
    editToolBar = new QToolBar("Edit", this);
    editToolBar->addAction(insertStateAction);
    editToolBar->addAction(insertTransitionAction);
    editToolBar->addSeparator();
    editToolBar->addAction(reselectParentAction);
    editToolBar->addAction(reselectTargetAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);


    //editToolBar->addAction(insertTextBox);
    //editToolBar->addAction(insertProperty);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()),
                                                 Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(
            ":/SCFormView/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));




    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()),
                                                 Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
            ":/SCFormView/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));



    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()),
                                                 Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
            ":/SCFormView/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

#ifdef ENABLE_TEXT_TOOL_BAR
    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
    textToolBar->setEnabled(false);


    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/SCFormView/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/SCFormView/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(0));
    pointerTypeGroup->addButton(linePointerButton,
                                int(1));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
#endif

}

QMenu *SCFormView::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
            << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()),
                this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}


QIcon SCFormView::createColorToolButtonIcon(const QString &imageFile,
                                                  QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}


QIcon SCFormView::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}

#include "stateselectionwindow.h"
#include "customtreewidgetitem.h"
#include "scdatamodel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

StateSelectionWindow::StateSelectionWindow(QWidget *parent, SCDataModel * dm) :
        QMainWindow(parent, Qt::WindowStaysOnTopHint),
        _dm(dm),
        _currentlySelected(NULL)
{

    // setup the GUI base
    _baseLayout = new QVBoxLayout;
    _centralWidget = new QWidget;
    _centralWidget->setLayout(_baseLayout);
    setCentralWidget(_centralWidget);

    // add the done button in the top row
    _doneButton = new QPushButton();
    _doneButton->setText("Done");
    connect (_doneButton,SIGNAL(clicked()), this, SLOT(handleDoneButtonPushed()));
    _row0 = new QHBoxLayout();
    _row0->addWidget(_doneButton);
    _baseLayout->addLayout(_row0);

    // add the selected State labels:
    _TargetLabel = new QLabel();
    _TargetLabelLabel = new QLabel();
    _TargetLabelLabel->setText("Target State: ");
    _TargetLabel->setFrameStyle(QFrame::Panel | QFrame::Plain);
    _TargetLabel->setLineWidth(1);
    _row1 = new QHBoxLayout();
    _row1->addWidget(_TargetLabelLabel);
    _row1->addWidget(_TargetLabel);
    _baseLayout->addLayout(_row1);

    // add the State Tree

    _targetStateSelectionTree = new QTreeWidget();
    //_targetStateSelectionTree->setExpanded(0,1);
    connect ( _targetStateSelectionTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(handleTreeViewItemClicked(QTreeWidgetItem*,int)));
    _row2 = new QHBoxLayout();
    _row2->addWidget(_targetStateSelectionTree);
    _baseLayout->addLayout(_row2);

    QList<SCState*> states;
    states.append( _dm->getTopState());


    loadTree (NULL, states);
    this->setWindowTitle("Select A Target State");
    this->resize(402,650);
    this->move(315,45);

}



void StateSelectionWindow::handleDoneButtonPushed()
{

    if ( _currentlySelected == NULL ) return;

    SCState * st  = dynamic_cast<SCState*>(_currentlySelected);
    qDebug() << "the currently selected state is: " << st->attributes.value("name")->asString();
    emit stateSelected(st);
}


// recursively walk through the state tree and build the tree view
// NOT CURRENTLY IN USE
// deprecated function, replaced by loadTreeState
void StateSelectionWindow::loadTree ( CustomTreeWidgetItem * parentItem , QList<SCState*> & states)
{
    int c = states.count();
    for(int i = 0; i < c; i++)
    {
        SCState * st = states.at(i);

        if ( !  st ) continue;

        CustomTreeWidgetItem * item=0;

        if (parentItem == 0)
        {
            item = new CustomTreeWidgetItem();
            _targetStateSelectionTree->addTopLevelItem((QTreeWidgetItem*)item);
        }
        else
        {
            item = new CustomTreeWidgetItem(parentItem);
        }


        item->setData(st);
        item->setText(0, st->attributes.value("name")->asString());
        item->setIcon(0, QIcon(":/SCFormView/cardboardbox.png"));
        item->setExpanded(true);

        // get all substates of this state
        QList<SCState*> subStates;
        st->getStates(subStates);
        loadTree (item, subStates);
    }
}

void StateSelectionWindow::handleTreeViewItemClicked(QTreeWidgetItem* qitem,int )
{

    CustomTreeWidgetItem * item = dynamic_cast<CustomTreeWidgetItem*>(qitem);

    //SCFormView::setSelectedTreeItem(item->data());
    _currentlySelected =  item->data();

    SCState * st  = dynamic_cast<SCState*>(_currentlySelected);
    _TargetLabel->setText( st->attributes.value("name")->asString());
}

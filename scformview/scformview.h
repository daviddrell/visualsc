
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

#ifndef SCFORMVIEW_H
#define SCFORMVIEW_H

#include "scformview_global.h"
#include <QMainWindow>
#include "scdatamodel.h"
#include <QList>
#include <QModelIndex>
#include <QSignalMapper>
#include <QInputDialog>
#include <QHash>
#include <QObject>
#include <QTableWidgetItem>
#include "scitem.h"
#include "fvitem.h"
#include "customtablewidgetitem.h"

class StateSelectionWindow;


QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
class QTreeWidget;
class QTreeWidgetItem;
class CustomTreeWidgetItem;
class QTableWidget;
class QLabel;
class SCTextBlock;
class SCItem;

QT_END_NAMESPACE


/**
    \defgroup FormView

    The FormView provides a standard windows form for viewing and editing the state chart. It includs a tree widget for hierarchical navigation
    of the state chart. It also is the primary means of inserting and deleting chart elements. The size and position of chart
    elements can be edited in the form's attribute table, but graphical attributes are best edited with the graphicsview.
*/

/** \class SCFormView
  *
  * \brief This form window allows users to navigate the state machine using a tree widget, and to edit state and transition attributes
  * \ingroup FormView
  */

class SCFORMVIEWSHARED_EXPORT SCFormView : public QMainWindow
{
    Q_OBJECT
public:
    explicit SCFormView(QWidget *parent = 0, SCDataModel * dm=0);
    void reset();
    void highlightRootItem();
    void highlightPreviousItem();

private slots:

    void handleAttributeChanged(SizeAttribute*);

    void handleMakeTransitionConnections(SCTransition* trans);
    void backgroundButtonGroupClicked(QAbstractButton *button);
    void buttonGroupClicked(int id);
    void deleteItem();
    void pointerGroupClicked(int id);
    void bringToFront();
    void sendToBack();
    void insertTransition();
    void insertState();
    void textInserted(QGraphicsTextItem *item);
    void currentFontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);
    void sceneScaleChanged(const QString &scale);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void handleBoldChanged();

    void about();

    void handleTreeViewItemClicked(QTreeWidgetItem*,int);
    void handleNewTransition(SCTransition*);
    void handleTransitionDeleted(QObject *);
    void handleStateDeleted(QObject*);
    void handleNewState(SCState*);
    void handleTextBlockDeleted(QObject*);
    void handlePropertyChanged(IAttribute * attr);
    void handlePropertyCellChanged(int, int);
    void handleStateSelectionWindowStateSelected(SCState *st, QString name);

    void setSelectedTreeItem(QObject *);

    void sendMessage(QString title, QString message);
    const QString promptTextInput(QString windowTitle, QString message, QString defaultText, bool* ok);

    void itemPromptTextBox();
    void itemPromptProperty();
    void itemDeleteSelectedProperty();



    void handleItemNameChangedInDataModel(SCState*, QString);
    void handleItemPositionChangedInDataModel(SCState*, QPointF);
    void handleItemSizeChangedInDataModel(SCState* state, QPointF size);

    void handleItemPositionChangedInDataModel(SCTextBlock*, QPointF);
    void handleItemSizeChangedInDataModel(SCTextBlock*, QPointF);


     void handleItemNameChangedInDataModel(SCTransition*, QString);

private:

    void deleteTreeItem(SCItem*);

    int propertyTableIndexOf(QString propertyName);
    int textBlockPropertyTableIndexOf(QString propertyName);
    void propertyTableSetText(int index, QString text);
    void textBlockPropertyTableSetText(int index, QString text);

    void updateTransitionEvent(SCTransition*, QString);
    void updateStateName(SCState*, QString);
    bool isCurrentlySelectedEqualTo(SCItem*);
    void replantTree();
    void initTree();

    void connectState(SCState*);
    void connectState(SCState *, CustomTableWidgetItem*);
    void connectState(SCState*, CustomTableWidgetItem*, QString);


    void connectTransition(SCTransition*);


    IAttributeContainer * getCurrentlySelectedAttributes();
    IAttributeContainer * getPreviouslySelectedAttributes();

    IAttributeContainer * getCurrentlySelectedTextBlockAttributes();
    IAttributeContainer * getPreviouslySelectedTextBlockAttributes();

    QString               getCurrentlySelectedTitle();
    QString               getCurrentlySelectedType();
    QObject*              getNeighborState(QObject*s);

    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();
    QWidget *createBackgroundCellWidget(const QString &text, const QString &image);

    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);
    void loadTreeState ( CustomTreeWidgetItem * parentItem , QList<SCState*> & states, bool connect);
    void loadTreeTransition ( CustomTreeWidgetItem * parentItem , QList<SCTransition*> & transitions, bool connect);
    void loadTreeTextBlock ( CustomTreeWidgetItem * parentItem , SCTextBlock* textBlock);

    void deleteItem(QObject * item);
    void setAttributeConnections(IAttributeContainer * atts, bool shouldConnect);
    void setTextBlockAttributeConnections(IAttributeContainer* atts, bool connect);
    void clearPropertyTable();
    void clearTextBlockPropertyTable();
    void reloadPropertyTable();

    void itemInsertProperty(SCItem* item, const QString name);
    void itemInsertTextBlock(SCItem* item, const QString name);

    void clearAndLoadPropertyTable(SCItem*,SCItem*);

    CustomTreeWidgetItem* findItem(SCState*);
    CustomTreeWidgetItem* findItem(SCTransition* item);
    CustomTreeWidgetItem* findItem(SCItem* item);
    CustomTreeWidgetItem* findItem(QObject * object);



    QTreeWidget    *stateChartTreeView;
    QTableWidget   *propertyTable;
    QTableWidget   *textBlockPropertyTable;
    QLabel         *selectedChartItem;

    QGraphicsView *view;

    QAction *exitAction;
    QAction *addAction;
    QAction *deleteAction;

    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *insertTransitionAction;
    QAction *insertStateAction;
    QAction *aboutAction;


    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *aboutMenu;

    QToolBar *textToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;
    QToolBar *propertyToolBar;

    QComboBox *sceneScaleCombo;
    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;


    QToolButton *addPropertyToolButton;

    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;

    SCDataModel * _dm;
    //QObject     * _currentlySelected;
    //QObject     * _previouslySelected;

    FVItem* _currentlySelected;
    FVItem* _topState;
    //FVItem* _previouslySelected;

    StateSelectionWindow * _targetStateSelectionWindow;


    QAction *insertTextBox; // will allow user to add a text box to any highlighted tree item
    QAction *insertProperty; // allows user to add own properties
    QAction *deleteProperty;

    QHash<SCItem*, CustomTreeWidgetItem*> _itemToTreeWidget;
    QHash<SCItem*, SCTextBlock*> _itemToTextBlock;
    QHash<CustomTreeWidgetItem*, SCTextBlock*> _treeWidgetToTextBlock;
    QHash<SCItem*, FVItem*> _items;
    //QList<FVItem*> _items;



};



#endif // SCFORMVIEW_H

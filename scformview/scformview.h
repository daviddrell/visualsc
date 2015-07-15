
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


private slots:
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
private:

    IAttributeContainer * getCurrentlySelectedAttributes();
    IAttributeContainer * getPreviouslySelectedAttributes();
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
    void loadTree ( CustomTreeWidgetItem * parentItem , QList<SCState*> & states);
    void loadTree ( CustomTreeWidgetItem * parentItem , QList<SCTransition*> & transitions);
    void loadTree ( CustomTreeWidgetItem * parentItem , SCTextBlock* textBlock);

    void deleteItem(QObject * item);
    void setAttributeConnections(IAttributeContainer * atts, bool shouldConnect);
    void clearPropertyTable();
    QTreeWidget    *stateChartTreeView;
    QTableWidget   *propertyTable;
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
    QObject     * _currentlySelected;
    QObject     * _previouslySelected;
    StateSelectionWindow * _targetStateSelectionWindow;




};


#endif // SCFORMVIEW_H

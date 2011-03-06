#ifndef FORMEDITORWINDOW_H
#define FORMEDITORWINDOW_H

#include <QMainWindow>
#include "scdatamodel.h"
#include <QList>
#include <QModelIndex>

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
class QTableWidget;
class QLabel;
QT_END_NAMESPACE




class FormEditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FormEditorWindow(QWidget *parent = 0, SCDataModel * dm=0);


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

    void about();

    void handleTreeViewItemClicked(QTreeWidgetItem*,int);
    void handleNewTransition(SCTransition*);

private:
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();
    QWidget *createBackgroundCellWidget(const QString &text, const QString &image);

    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);
    void loadTree ( QTreeWidgetItem * parentItem , QList<SCState*> & states);

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
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;

    SCDataModel * dm;
    QObject     * _currentlySelected;

private slots:
    void handlePropertyChanged(IAttribute * attr);
    void handlePropertyCellChanged(int, int);

};

#endif // FORMEDITORWINDOW_H

#ifndef STATESELECTIONWINDOW_H
#define STATESELECTIONWINDOW_H
#include "scformview_global.h"
#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QMainWindow>
#endif

class SCDataModel;
class SCState;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;


class QTreeWidget;
class CustomTreeWidgetItem;
class QTreeWidgetItem;
#include <QList>

/** \class StateSelectionWindow
  *
  * \brief This widget is used to display a list of available states, so that the user can select a state as a transition target
  * \ingroup FormView
  */
class SCFORMVIEWSHARED_EXPORT StateSelectionWindow  : public QMainWindow
{
    Q_OBJECT

public:
    StateSelectionWindow(QWidget *parent = 0, SCDataModel * dm=0);

signals:
    void stateSelected(SCState *state, QString name);
    void stateSelected(SCState* state);

private: // methods

    void loadTree ( CustomTreeWidgetItem * parentItem , QList<SCState*> & states);

private slots:

    void handleTreeViewItemClicked(QTreeWidgetItem*,int);
    void handleDoneButtonPushed();

private : //data

    SCDataModel * _dm;
    QTreeWidget * _targetStateSelectionTree;
    QVBoxLayout * _baseLayout;
    QHBoxLayout * _row0;
    QHBoxLayout * _row1;
    QHBoxLayout * _row2;
    QLabel      * _TargetLabelLabel;
    QLabel      * _TargetLabel;
    QWidget     * _centralWidget;
    QPushButton * _doneButton;
    QObject     * _currentlySelected;

};

#endif // STATESELECTIONWINDOW_H

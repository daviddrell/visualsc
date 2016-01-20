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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "smproject.h"
#include <QString>
#include "textformattoolbar.h"
#include <QButtonGroup>
#include <QRadioButton>
#include <QTabWidget>

class SCFormView;
class QSettings;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // void keyPressEvent(QKeyEvent * e);

signals:
    void reset();
    void open(QString);
    void scaleChanged(qreal);
    void gridToggled(bool);

private:
    Ui::MainWindow *ui;
    void addToolbarSpacer(QToolBar *toolbar);
    void sendMessage(QString title, QString message);
    SMProject *_project;
    QString _currentFolder;
    QString _currentFileFullPath;
    QString _currentExportFullPath;
    QString _currentImageFullPath;

    QString _settingsFileName;

    SCFormView *_formEditorWindow;
    TextFormatToolBar *_textFormatToolBar;
    void delay();

    void loadSettings();
    void saveSettings();
    void createSettings();
    void setupProject(bool insertRootState);
    void createFontBar();


    qreal _scale;
    qreal clamp(qreal val, qreal min, qreal max);
    void scale(qreal scale);

    bool _gridEnable;

    QComboBox* _fontBox;
    QComboBox* _fontSizeBox;


    QAction* _boldAction;

//    QButtonGroup _fontSelection;
    QRadioButton* _stateFontRadioButton;
    QRadioButton* _transitionFontRadioButton;
    QRadioButton* _selectedRadioButton;
    QRadioButton* _pStateRadioButton;
    QRadioButton* _qStateRadioButton;
    QButtonGroup* _stateTypeGroup;

    QTabWidget*   _tabWidget;
    QMap<int,SMProject*> _tabbedProjects; ///< key is the tab index
    int                  _tabIndex;

private slots:
    void handleRootStateNameChanged(IAttribute*);
    void handleMessage(QString);
    void handleFileOpenClick();
    void handleFileSaveClick();
    void handleExportCodeClick();
    void handleNewClick();
    void handleReadInputFileDone(bool,QStringList);
    void handleItemClicked();
    void handleBoldToggled(bool toggle);
    void handleNewSubStateTab(SCState*subState);
    void handleTabClosing(int);
    void handleFontRadioChanged();
    void on_actionImport_triggered();

    void on_actionShortcuts_triggered();
    void on_actionSave_As_triggered();
    void on_actionNew_triggered();
    void on_actionZoomOut_triggered();
    void on_actionZoomIn_triggered();
    void on_actionSaveImage_triggered();
    void on_actionGrid_toggled(bool arg1);


    void handleChangeFont(QString);
//    void handleSetProgramFont(QFont*);
    void handleSetProgramFontFamily(IAttribute*);
    void handleSetProgramFontSize(IAttribute*);
    void handleSetProgramFontBold(IAttribute*);

};

#endif // MAINWINDOW_H

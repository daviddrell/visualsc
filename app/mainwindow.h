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

private:
    Ui::MainWindow *ui;

    SMProject *_project;
    QSettings *_settings;
    static QString _keyLastFilePath;
    SCFormView *_formEditorWindow;
    TextFormatToolBar *_textFormatToolBar;

private slots:
    void handleFileOpenClick();
    void handleFileSaveClick();
    void handleNewClick();
    void handleReadInputFileDone(bool,QStringList);
};

#endif // MAINWINDOW_H

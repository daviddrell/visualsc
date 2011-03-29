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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include "formeditorwindow.h"

// adding comments to get the git repository

QString MainWindow::_keyLastFilePath = QString("lastFilePath");

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        _project(0),
        _settings(0),
        _formEditorWindow(0)
{
    QCoreApplication::setOrganizationName("David W Drell");
    QCoreApplication::setOrganizationDomain("davidwdrell.net");
    QCoreApplication::setApplicationName("Visual State Chart Editor");
    _settings  = new QSettings(this);

    ui->setupUi(this);

    connect ( ui->actionOpen, SIGNAL(triggered()), this, SLOT(handleFileOpenClick()));
    connect ( ui->actionSave, SIGNAL(triggered()), this, SLOT(handleFileSaveClick()));
    connect ( ui->actionNew, SIGNAL(triggered()), this, SLOT(handleNewClick()));


}

MainWindow::~MainWindow()
{
    delete _settings;
    delete _project;
    delete ui;
}

void MainWindow::handleFileOpenClick()
{
    QString prevFilePath=QDir::homePath();
    QString fileName ;

    if ( _settings->contains(_keyLastFilePath))
    {
        prevFilePath = _settings->value(_keyLastFilePath).toString();
    }

    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open SCXML Input File"), prevFilePath, tr("SCXML Files (*.scxml)"));



    _settings->setValue(_keyLastFilePath, fileName);


    _project = new SMProject(  ui->centralWidget );

    ui->gridLayout->addWidget( _project->getQGraphicsView() );


    connect (_project, SIGNAL(readInputFileCompleted(bool,QStringList)), this, SLOT(handleReadInputFileDone(bool,QStringList)) );

    _project->readInputFile(fileName);


}


void MainWindow::handleReadInputFileDone(bool ,QStringList)
{

    if ( _formEditorWindow )
    {
        delete _formEditorWindow;
    }
    _formEditorWindow = new FormEditorWindow(0, _project->getDM());
    _formEditorWindow->show();

}

void MainWindow::handleFileSaveClick()
{
    if ( _project == NULL) return;

    QString prevFilePath=QDir::homePath();
    QString fileName ;

    if ( _settings->contains(_keyLastFilePath))
    {
        prevFilePath = _settings->value(_keyLastFilePath).toString();
    }

    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Open SCXML Input File"), prevFilePath, tr("SCXML Files (*.scxml)"));

    _settings->setValue(_keyLastFilePath, fileName);

    _project->save(fileName);
}



void MainWindow::handleNewClick()
{

    if ( _project)
    {
        delete _project;
        _project = NULL;
    }

    _project = new SMProject(  ui->centralWidget );

    ui->gridLayout->addWidget( _project->getQGraphicsView() );

    _formEditorWindow = new FormEditorWindow(0, _project->getDM());
    _formEditorWindow->show();



}

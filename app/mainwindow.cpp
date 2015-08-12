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
#include <QDebug>
#include <QKeyEvent>
#include "scformview.h"
#include <QTime>
#include <QMessageBox>

#define POP_UP_X    160
#define POP_UP_Y    200


// adding comments to get the git repository

QString MainWindow::_keyLastFilePath = QString("lastFilePath");
QString MainWindow::_codeLastFilePath =QString("codeLastFilePath");
QString MainWindow::_lastImportFilePath = QString("lastImportFilePath");
QString MainWindow::_currentFolder = QString("");
QString MainWindow::_currentFileFullPath = QString("");
QString MainWindow::_currentExportFullPath = QString("");

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        _project(0),
        _settings(0),
        _formEditorWindow(0),
        _textFormatToolBar(NULL),
        _savedOnce(false)
{
    QCoreApplication::setOrganizationName("David W Drell");
    QCoreApplication::setOrganizationDomain("davidwdrell.net");
    QCoreApplication::setApplicationName("Visual Statechart Editor");
    _settings  = new QSettings(this);

    ui->setupUi(this);

    connect ( ui->actionOpen, SIGNAL(triggered()), this, SLOT(handleFileOpenClick()));
    connect ( ui->actionSave, SIGNAL(triggered()), this, SLOT(handleFileSaveClick()));
    connect ( ui->actionExportCode, SIGNAL(triggered()), this, SLOT(handleExportCodeClick()));
    connect ( ui->actionNew, SIGNAL(triggered()), this, SLOT(handleNewClick()));


#ifdef ENABLE_TEXT_TOOL_BAR
    _textFormatToolBar = new TextFormatToolBar();


    this->addToolBar(_textFormatToolBar);
#endif
    this->setWindowTitle("Visual Statechart Editor");

    this->resize(1272,1000);
    this->move(633,0);

/*
    _project = new SMProject(  ui->centralWidget );

    _project->initNewSM();

    _project->setObjectName("TEST");

    ui->gridLayout->addWidget( _project->getQGraphicsView() );



   _formEditorWindow = new SCFormView(0, _project->getDM());
   _formEditorWindow->show();

*/



// uncomment this macro to autoload a file
//#define AUTO_LOAD_FILE

#ifdef AUTO_LOAD_FILE

#define DEFAULT_FILE "C:/visualsc/xmlfiles/statesequencemachine.scxml"
    QString fileName =DEFAULT_FILE;
    _settings->setValue(_keyLastFilePath, fileName);
    /*
    _project = new SMProject(  ui->centralWidget );
    connect (_project, SIGNAL(readInputFileCompleted(bool,QStringList)), this, SLOT(handleReadInputFileDone(bool,QStringList)) );
    _project->readInputFile(fileName);
    */

    _project = new SMProject(  ui->centralWidget );
    connect( this, SIGNAL(reset()), _project->getDM(), SLOT(handleReset()));
   // _project->initNewSM(); moved to constructor
    ui->gridLayout->addWidget( _project->getQGraphicsView() );
    _formEditorWindow = new SCFormView(0, _project->getDM());
    _formEditorWindow->show();


    // open the file
    _project->getDM()->openFile(fileName);

    // reselect the new root machine tree widget in the tree view
    _formEditorWindow->highlightRootItem();

#endif

#ifndef AUTO_LOAD_FILE
    _project = new SMProject(  ui->centralWidget );
    ui->gridLayout->addWidget( _project->getQGraphicsView() );
    _formEditorWindow = new SCFormView(0, _project->getDM());

    connect(_formEditorWindow, SIGNAL(newClick()), this, SLOT(handleNewClick()));
    connect(_formEditorWindow, SIGNAL(openClick()), this, SLOT(handleFileOpenClick()));
    connect(_formEditorWindow, SIGNAL(saveClick()), this, SLOT(handleFileSaveClick()));
    connect(_formEditorWindow, SIGNAL(saveAsClick()), this, SLOT(on_actionSave_As_triggered()));
    connect(_formEditorWindow, SIGNAL(importClick()), this, SLOT(on_actionImport_triggered()));
    connect(_formEditorWindow, SIGNAL(exportClick()), this, SLOT(handleExportCodeClick()));

    _formEditorWindow->show();



    connect(this, SIGNAL(reset()), _project->getDM(), SLOT(handleReset()));
    connect(this, SIGNAL(reset()), _formEditorWindow, SLOT(handleReset()));
    connect(this, SIGNAL(open(QString)), _project->getDM(), SLOT(handleOpen(QString)));

#endif

}

MainWindow::~MainWindow()
{
    delete _settings;
    delete _project;
    delete ui;
}

void MainWindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    QList<SCState*> states;
    _project->getDM()->getAllStates(states);
    qDebug()<<"delay num states: "  <<states.size();
}

void MainWindow::handleNewClick()
{
    // alert the graphics view and form view to reset
    emit reset();

    // reset the current save and export files.
    _currentFileFullPath = "";
    _currentExportFullPath = "";
}

void MainWindow::handleFileOpenClick()
{
    if(_currentFolder.isEmpty())
        _currentFolder = QDir::currentPath();


    QString newFileFullPath = QFileDialog::getOpenFileName(this, tr("Open SCXML Input File"), _currentFolder, tr("SCXML Files (*.scxml)"));


    // If open was canceled, then stop this function from continuing
    if(newFileFullPath.isEmpty())
        return;


    _currentFileFullPath = newFileFullPath;
    _currentFolder = QFileInfo(_currentFileFullPath).path();


    emit reset();
    emit open(_currentFileFullPath);
}


void MainWindow::handleReadInputFileDone(bool ,QStringList)
{

    if ( _formEditorWindow )
    {
        delete _formEditorWindow;
    }
    _formEditorWindow = new SCFormView(0, _project->getDM());
    _formEditorWindow->show();

    ui->gridLayout->addWidget( _project->getQGraphicsView() );


}

void MainWindow::handleFileSaveClick()
{
    if ( _project == NULL) return;

    if(_currentFileFullPath.isEmpty())
    {
        if(_currentFolder.isEmpty())
            _currentFolder = QDir::currentPath();


        _currentFileFullPath = QFileDialog::getSaveFileName(this, tr("Save as .SCXML File"), _currentFolder, tr("SCXML Files (*.scxml)"));
        _currentFolder = QFileInfo(_currentFileFullPath).path();
        qDebug() << "the file folder is " << _currentFolder;
        qDebug() << "the file path is " << _currentFileFullPath;
        _project->save(_currentFileFullPath);
    }
    else
    {
        qDebug() << "Straight Saving...";
        _project->save(_currentFileFullPath);
    }
}

void MainWindow::handleExportCodeClick()
{
    if(_project==NULL)
        return;

    // check if we have exported the file before
    if(_currentExportFullPath.isEmpty())
    {
        // let the user select a place to put the file.
        if(_currentFolder.isEmpty())
            _currentFolder = QDir::currentPath();


        _currentExportFullPath = QFileDialog::getSaveFileName(this, tr("Save as .SCXML File"), _currentFolder, tr("SCXML Files (*.scxml)"));
        _currentFolder = QFileInfo(_currentExportFullPath).path();
        _project->exportToCode(_currentExportFullPath);
    }
    else
    {
        qDebug() << "Straight to export...";
        _project->exportToCode(_currentExportFullPath);
    }



    if(_currentFolder.isEmpty())
        _currentFolder = QDir::currentPath();


//    //fileName = QFileDialog::getSaveFileName(this, tr("Save as .cpp and .h Files"), prevFilePath, tr("C++ and Header (*.cpp)"));

//    //_settings->setValue(_codeLastFilePath, fileName);
//    //_project->save(fileName);
//    qDebug() << "export to code file name: " << fileName;
//    _project->exportToCode(fileName);
////    sendMessage("Saved");
}




void MainWindow::on_actionImport_triggered()
{
    SCState* current = _formEditorWindow->getCurrentlySelectedState();

    if(!current)
    {
        return;
    }

    if(_currentFolder.isEmpty())
        _currentFolder = QDir::currentPath();

    QString importFileName = QFileDialog::getOpenFileName(this, tr("Import .SCXML File"), _currentFolder, tr("SCXML Files (*.scxml)"));
    _currentFolder = QFileInfo(importFileName).path();
    _project->getDM()->importFile(current,importFileName);
}

void MainWindow::on_actionShortcuts_triggered()
{
    sendMessage("Shortcuts","State Box\n"
                            "Double Click - Resize Box Automatically\n"
                            "\n"
                            "Text Boxes\n"
                            "Double Click - Edit Text\n"
                            "\n"
                            "Transitions\n"
                            "N - (hovered) Create Elbow\n"
                            "D - (hovered) Delete Elbow\n");
}


void MainWindow::sendMessage(QString title, QString message)
{

    QMessageBox msgBox;

    msgBox.setWindowTitle(title);
    msgBox.setText(message);

    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint);
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

void MainWindow::on_actionSave_As_triggered()
{

    if(_currentFolder.isEmpty())
    {
        _currentFolder = QDir::currentPath();
    }

    _currentFileFullPath = QFileDialog::getSaveFileName(this, tr("Save as .SCXML File"), _currentFolder, tr("SCXML Files (*.scxml)"));
    _currentFolder = QFileInfo(_currentFileFullPath).path();
    qDebug() << "the file folder is " << _currentFolder;
    qDebug() << "the file path is " << _currentFileFullPath;
    _project->save(_currentFileFullPath);

}

void MainWindow::on_actionNew_triggered()
{

}

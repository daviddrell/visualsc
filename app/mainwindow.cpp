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


// adding comments to get the git repository

QString MainWindow::_keyLastFilePath = QString("lastFilePath");
QString MainWindow::_codeLastFilePath =QString("codeLastFilePath");

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        _project(0),
        _settings(0),
        _formEditorWindow(0),
        _textFormatToolBar(NULL)
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
    // _project->initNewSM(); moved to constructor
    ui->gridLayout->addWidget( _project->getQGraphicsView() );
    _formEditorWindow = new SCFormView(0, _project->getDM());
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
    emit reset();

    // all states still exist
  //  QList<SCState*> states;
  //  _project->getDM()->getAllStates(states);
   // _formEditorWindow->highlightRootItem();

#if 0
    _project->getDM()->reset(); // clean out the data model except for the root machine, this will cause the graphicsview and formview to reset as well
    _formEditorWindow->highlightRootItem(); // select the root machine in the tree view
  //  _formEditorWindow->reset();


    if(false)
    {
        if ( _project)
        {
            delete _project;
            _project = NULL;
        }
        _project = new SMProject(  ui->centralWidget );
        //_project->initNewSM(); moved to constructor

        ui->gridLayout->addWidget( _project->getQGraphicsView() );
        //_formEditorWindow = new SCFormView(0, _project->getDM());
        //_formEditorWindow->show();
    }
#endif
}

void MainWindow::handleFileOpenClick()
{


    QString prevFilePath=QDir::homePath();
    QString fileName;

    if( _settings->contains(_keyLastFilePath))
    {
        prevFilePath = _settings->value(_keyLastFilePath).toString();
    }

    fileName = QFileDialog::getOpenFileName(this, tr("Open SCXML Input File"), prevFilePath, tr("SCXML Files (*.scxml)"));
    _settings->setValue(_keyLastFilePath, fileName);

    emit reset();

    QList<SCState*> states;
    _project->getDM()->getAllStates(states);

    while(states.size()!=0)
    {
        for(int i = 0; i < states.size(); i++)
        {
            qDebug() << "state was not deleted: "<<states.at(i)->objectName();
        }

        delay();

        states.clear();
        _project->getDM()->getAllStates(states);
    }

    emit open(fileName);


#if 0
   // clear out the data model
    _project->getDM()->reset();
    _formEditorWindow->highlightRootItem(); // select the root machine in the tree view
    QString prevFilePath=QDir::homePath();
    QString fileName;

    if( _settings->contains(_keyLastFilePath))
    {
        prevFilePath = _settings->value(_keyLastFilePath).toString();
    }

    fileName = QFileDialog::getOpenFileName(this, tr("Open SCXML Input File"), prevFilePath, tr("SCXML Files (*.scxml)"));
    _settings->setValue(_keyLastFilePath, fileName);

    // reset the datamodel and user the reader to load the new scxml


    //_formEditorWindow->reset();
    //this->handleNewClick();

    // open the file
    QList<SCState*> states;
    _project->getDM()->getAllStates(states);

    while(states.size()!=0)
    {
        // clear out the data model
        // _project->getDM()->reset();
        //qDebug() << "\n";
        for(int i = 0; i < states.size(); i++)
        {
            //states.at(i)->deleteLater();
            qDebug() << "state was not deleted: "<<states.at(i)->objectName();
        }
    }
    //_project->getDM()->openFile(fileName);


  //  _formEditorWindow->reset();


#endif







    if(false)
    {
    if ( _project)
    {
        delete _project;
        _project = NULL;
    }
    QString prevFilePath=QDir::homePath();
    QString fileName;

    if( _settings->contains(_keyLastFilePath))
    {
        prevFilePath = _settings->value(_keyLastFilePath).toString();
    }

    fileName = QFileDialog::getOpenFileName(this, tr("Open SCXML Input File"), prevFilePath, tr("SCXML Files (*.scxml)"));
    _settings->setValue(_keyLastFilePath, fileName);
    _project = new SMProject(  ui->centralWidget );
    connect (_project, SIGNAL(readInputFileCompleted(bool,QStringList)), this, SLOT(handleReadInputFileDone(bool,QStringList)) );
    _project->readInputFile(fileName);
    }
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

    QString prevFilePath=QDir::homePath();
    QString fileName ;

    if ( _settings->contains(_keyLastFilePath))
    {
        prevFilePath = _settings->value(_keyLastFilePath).toString();
    }

    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Save as SCXML File"), prevFilePath, tr("SCXML Files (*.scxml)"));

    _settings->setValue(_keyLastFilePath, fileName);
    qDebug() << "the file name is " << fileName;
    _project->save(fileName);
}

void MainWindow::handleExportCodeClick()
{
    if(_project==NULL)
        return;

    if ( _project == NULL) return;

    QString prevFilePath=QDir::homePath();
    QString fileName ;

    if ( _settings->contains(_codeLastFilePath))
    {
        prevFilePath = _settings->value(_codeLastFilePath).toString();
    }

    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Save as .cpp and .h Files"), prevFilePath, tr("C++ and Header (*.cpp)"));

    _settings->setValue(_codeLastFilePath, fileName);
    //_project->save(fileName);
    qDebug() << "export to code file name: " << fileName;
    _project->exportToCode(fileName);
}




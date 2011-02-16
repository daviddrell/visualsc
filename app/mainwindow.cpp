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

    _project->readInputFile(fileName);


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

 //   project = new SMProject(ui->graphicsView);

}

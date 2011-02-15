#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QSettings>

// adding comments to get the git repository

QString MainWindow::_keyLastFilePath = QString("lastFilePath");

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("David W Drell");
    QCoreApplication::setOrganizationDomain("davidwdrell.net");
    QCoreApplication::setApplicationName("Visual State Chart Editor");
    settings  = new QSettings(this);

    ui->setupUi(this);

    connect ( ui->actionOpen, SIGNAL(triggered()), this, SLOT(handleFileOpenClick()));
    connect ( ui->actionSave, SIGNAL(triggered()), this, SLOT(handleFileSaveClick()));
    connect ( ui->actionNew, SIGNAL(triggered()), this, SLOT(handleNewClick()));


}

MainWindow::~MainWindow()
{
    delete settings;
    delete project;
    delete ui;
}

void MainWindow::handleFileOpenClick()
{
    QString prevFilePath=QDir::homePath();
    QString fileName ;

    if ( settings->contains(_keyLastFilePath))
    {
        prevFilePath = settings->value(_keyLastFilePath).toString();
    }

    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open SCXML Input File"), prevFilePath, tr("SCXML Files (*.scxml)"));



    settings->setValue(_keyLastFilePath, fileName);

//    project = new SMProject(ui->graphicsView);

    project = new SMProject(  ui->centralWidget );


    ui->gridLayout->addWidget( project->getQGraphicsView() );

    project->readInputFile(fileName);
}


void MainWindow::handleFileSaveClick()
{
    if ( project == NULL) return;

    QString prevFilePath=QDir::homePath();
    QString fileName ;

    if ( settings->contains(_keyLastFilePath))
    {
        prevFilePath = settings->value(_keyLastFilePath).toString();
    }

    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Open SCXML Input File"), prevFilePath, tr("SCXML Files (*.scxml)"));

    settings->setValue(_keyLastFilePath, fileName);

    project->save(fileName);
}



void MainWindow::handleNewClick()
{

    if ( project)
    {
        delete project;
        project = NULL;
    }

 //   project = new SMProject(ui->graphicsView);

}

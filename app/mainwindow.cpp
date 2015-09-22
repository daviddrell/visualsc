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
//#include "scgraphicsview.h"
//#include "customgraphicsscene.h"

#define POP_UP_X    160
#define POP_UP_Y    200

#define SCALE_STEP 0.161803398875
#define SCALE_MIN   0.1
#define SCALE_MAX   3

#define DEFAULT_FONT_SIZE 10

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        _project(0),
        //_settings(0),
        _formEditorWindow(0),
        _textFormatToolBar(NULL),
        _scale(1),
        _gridEnable(false)
{



    QCoreApplication::setOrganizationName("David W Drell");
    QCoreApplication::setOrganizationDomain("davidwdrell.net");
    QCoreApplication::setApplicationName("Visual Statechart Editor");






    // Designer code
    ui->setupUi(this);

    // Create the Fonts List
//    QList<QString> fontList;
    QStringList fontList;
    QStringList fontSizeList;
    QFontDatabase qfd;
    foreach(const QString &family, qfd.families())
    {
        //   qDebug() << family;
        fontList.append(family);
    }

    // create a list of font sizes based on the first font
    if(fontList.size() > 0)
    {
        foreach(int point, qfd.smoothSizes(fontList.at(0), qfd.styles(fontList.at(0)).at(0)))
        {
            fontSizeList.append(QString::number(point));
        }
    }

    // add combobox
    ui->mainToolBar->addSeparator();

    _fontBox = new QComboBox();
    _fontBox->setEditable(true);
    _fontBox->addItems(fontList);
    ui->mainToolBar->addWidget(_fontBox);

    int fontIndex = _fontBox->findText("arial", Qt::MatchFixedString);

    // set default to arial, if it exists
    if(fontIndex>-1)
    {
        _fontBox->setCurrentIndex(fontIndex);
    }

//    this->addToolbarSpacer(ui->mainToolBar);
//    ui->mainToolBar->setStyleSheet("spacing: 20px");

    // add font size combox box
    _fontSizeBox = new QComboBox();
    _fontSizeBox->setEditable(true);
    _fontSizeBox->addItems(fontSizeList);
    ui->mainToolBar->addWidget(_fontSizeBox);

    int fontSizeIndex = _fontSizeBox->findText(QString::number(DEFAULT_FONT_SIZE),0);
    if(fontSizeIndex>-1)
    {
        _fontSizeBox->setCurrentIndex(fontSizeIndex);
    }



    // radio buttons

//    _fontSelection.setParent(this);
    _selectedRadioButton = new QRadioButton("Selected", this);
    _stateFontRadioButton = new QRadioButton("States", this);
    _transitionFontRadioButton = new QRadioButton("Transitions", this);

    ui->mainToolBar->addWidget(_selectedRadioButton);
    ui->mainToolBar->addWidget(_stateFontRadioButton);
    ui->mainToolBar->addWidget(_transitionFontRadioButton);

    _selectedRadioButton->toggle();

    // custom action connects
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
    connect(_formEditorWindow, SIGNAL(saveImageClick()), this, SLOT(on_actionSaveImage_triggered()));
    connect(_formEditorWindow, SIGNAL(saveClick()), this, SLOT(handleFileSaveClick()));
    connect(_formEditorWindow, SIGNAL(saveAsClick()), this, SLOT(on_actionSave_As_triggered()));
    connect(_formEditorWindow, SIGNAL(importClick()), this, SLOT(on_actionImport_triggered()));
    connect(_formEditorWindow, SIGNAL(exportClick()), this, SLOT(handleExportCodeClick()));

    _formEditorWindow->show();


#endif

    // connects for the mainwindow to other modules


    // if the data model sends an error message, the main window will display it with a pop up
    connect(_project->getDM(), SIGNAL(message(QString)), this, SLOT(handleMessage(QString)));

    // data model has special needs when doing a full reset
    connect(this, SIGNAL(reset()), _project->getDM(), SLOT(handleReset()));

    // no special protocol needed for formview. regular item deletion and creation is good enough
//    connect(this, SIGNAL(reset()), _formEditorWindow, SLOT(handleReset()));
    connect(this, SIGNAL(open(QString)), _project->getDM(), SLOT(handleOpen(QString)));

    // when the grid action is toggled, the graphics view's scene will change its background
    connect(this, SIGNAL(gridToggled(bool)), _project->getSCGraphicsView()->getCustomGraphicsScene(), SLOT(handleGridToggled(bool)));


    // mainwindow component connects

    // when the font combo box is activated, the data model will change the font family attribute for all items
    connect(_fontBox, SIGNAL(activated(QString)), this, SLOT(handleChangeFont(QString)));
    connect(_fontSizeBox, SIGNAL(activated(QString)), this, SLOT(handleChangeFont(QString)));

    // when a datamodel item is clicked, change the font box to match its attributes

    // when the data model signals set program font, set the program font
    connect(_project->getDM(), SIGNAL(setProgramFont(QFont*)), this, SLOT(handleSetProgramFont(QFont*)));


    // load settings from settings.ini if it exists, otherwise create a settings.ini file
    _settingsFileName = QDir::currentPath()+"/"+"settings.ini";
    qDebug () << "settings file " << _settingsFileName;
    loadSettings();
}

MainWindow::~MainWindow()
{
    //delete _settings;
    delete _project;
    delete ui;
}

/**
 * @brief MainWindow::handleSetProgramFont
 * @param font
 *
 * SLOT
 *
 * this is triggered by SCDataModel when the mainwindow font boxes should update
 * for each valid font attribute given, update the corresponding combo box
 *
 */
void MainWindow::handleSetProgramFont(QFont* font)
{
    qDebug() << "mainwindow::handlesetprogramfont";
    // set font, if it exists
    int fontIndex = _fontBox->findText(font->family(), Qt::MatchFixedString);
    if(fontIndex>-1)
    {
        _fontBox->setCurrentIndex(fontIndex);
    }

    int fontSizeIndex = _fontSizeBox->findText(QString::number(font->pointSize()),0);
    if(fontSizeIndex>-1)
    {
        _fontSizeBox->setCurrentIndex(fontSizeIndex);
    }
}

/**
 * @brief MainWindow::addToolbarSpacer
 * @param toolbar
 *
 * Test code found online to implement a custom spacer in the toolbar
 */
void MainWindow::addToolbarSpacer(QToolBar *toolbar)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *spacerLayout = new QHBoxLayout;
    QSpacerItem *spacer =
            new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Minimum);
    spacerLayout->addSpacerItem(spacer);
    widget->setLayout(spacerLayout);
    toolbar->addWidget(widget);
}

/**
 * @brief MainWindow::handleChangeFont
 *
 * SLOT
 * connected to both font family and size combo boxes.
 * whenever a combo box is activated, create a font from the current values
 * based on the radio toggle, update the corresponding item(s) font
 *
 */
void MainWindow::handleChangeFont(QString)
{
    qDebug() << "mainwindow::handlechangefont";
    QString fontFam = _fontBox->currentText();
    QString fontSizeStr = _fontSizeBox->currentText();

    bool ok;

    int size = fontSizeStr.toInt(&ok);
    if(!ok)
    {
        sendMessage("Error","Cannot change font size to "+ fontSizeStr);
        return;
    }

    QFont font(fontFam,size);

    if(_selectedRadioButton->isChecked())
    {
        SCState* st = _formEditorWindow->getCurrentlySelectedState();
        SCTransition* trans = _formEditorWindow->getCurrentlySelectedTransition();

        if(st)
        {
            st->setFont(&font);
        }
        else if(trans)
        {
            trans->setFont(&font);
        }
        else
        {

        }

    }
    else if(_stateFontRadioButton->isChecked())
    {
        _project->getDM()->handleStateFontChanged(&font);
    }
    else if(_transitionFontRadioButton->isChecked())
    {
        _project->getDM()->handleTransitionFontChanged(&font);
    }
    else
    {
        qDebug() << "ERROR NEITHER state or transition radio button selected for font";
    }
}



void MainWindow::handleMessage(QString msg)
{
    this->sendMessage("!", msg);
}

/**
 * @brief MainWindow::saveSettings
 * save settings to settings.ini
 */
void MainWindow::saveSettings()
{
    QSettings settings(_settingsFileName, QSettings::IniFormat);
    settings.setValue("workingDirectory",_currentFolder);
    settings.setValue("gridEnable",_gridEnable);

    settings.sync();
    qDebug ()<< "saveSettings " << settings.fileName();
}


/**
 * @brief MainWindow::loadSettings
 * load settings from settings.ini
 */
void MainWindow::loadSettings()
{
    QSettings settings(_settingsFileName, QSettings::IniFormat);

    if(settings.childKeys().size()==0)
    {
        qDebug() << "Could not find settings.ini ... creating new settings.ini file.";
        createSettings();
        return;
    }

    const QStringList keys = settings.childKeys();
    foreach(const QString &key, keys)
    {
        qDebug() << "loading setting: " <<key <<"\t with value: "<<settings.value(key);
        if(key=="workingDirectory")
            _currentFolder = settings.value(key).toString();
        else if(key == "gridEnable")
        {
            if(settings.value(key) == "true")
            {
                _gridEnable = true;
                ui->actionGrid->setChecked(true);
                emit gridToggled(true);
            }
        }
    }
}

/**
 * @brief MainWindow::createSettings
 * there was no settings file, so create one
 */
void MainWindow::createSettings()
{
    if(_currentFolder.isEmpty())
        _currentFolder = QDir::currentPath();

    QSettings settings(_settingsFileName, QSettings::IniFormat);
    settings.setValue("workingDirectory",_currentFolder);
    settings.sync();
    qDebug() << "creating settings in " << _settingsFileName;
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

    this->setWindowTitle("Visual Statechart Editor");
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

    this->setWindowTitle(_currentFileFullPath);

    emit reset();
    emit open(_currentFileFullPath);

    saveSettings();
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

    // check if there is a current file, save it immediately, otherwise browse for one
    if(_currentFileFullPath.isEmpty())
    {
        if(_currentFolder.isEmpty())
            _currentFolder = QDir::currentPath();

        QString saveName = QFileDialog::getSaveFileName(this, tr("Save as .SCXML File"), _currentFolder, tr("SCXML Files (*.scxml)"));

        // check if the saveName is valid
        if(saveName.isEmpty())
        {

        }
        else
        {
            _currentFileFullPath = saveName;
            _currentFolder = QFileInfo(_currentFileFullPath).path();
            _project->save(_currentFileFullPath);
            this->setWindowTitle(_currentFileFullPath);
            saveSettings();
        }
    }
    else
    {
        _project->save(_currentFileFullPath);
        this->setWindowTitle(_currentFileFullPath);
        saveSettings();
    }
}

void MainWindow::on_actionSave_As_triggered()
{

    if(_project==NULL) return;

    if(_currentFolder.isEmpty())
    {
        _currentFolder = QDir::currentPath();
    }

    QString saveName;
    saveName = QFileDialog::getSaveFileName(this, tr("Save as .SCXML File"), _currentFolder, tr("SCXML Files (*.scxml)"));


    if(saveName.isEmpty())
    {

    }
    else
    {
        _currentFileFullPath = saveName;
        _currentFolder = QFileInfo(_currentFileFullPath).path();
        qDebug() << "the file folder is " << _currentFolder;
        qDebug() << "the file path is " << _currentFileFullPath;
        _project->save(_currentFileFullPath);
        this->setWindowTitle(_currentFileFullPath);
        saveSettings();
    }
}

void MainWindow::handleExportCodeClick()
{

    if(_project==NULL)
        return;

    // return if the the check on the datamodel came out wrong
    if(!_project->getDM()->checkDataModel())
    {
        this->setWindowTitle("Exported failed: please correct errors");
        return;
    }

    bool exportStatus;

    // check if we have exported the file before
    if(_currentExportFullPath.isEmpty())
    {
        // let the user select a place to put the file.
        if(_currentFolder.isEmpty())
            _currentFolder = QDir::currentPath();


        QString classNameLower = _project->getDM()->getCFileName();
        QString defaultSave = _currentFolder + QString("\\"+classNameLower);
        QString exportName = QFileDialog::getSaveFileName(this, tr("Save as .cpp and .h Files"), defaultSave, tr("C++ and Header (*.cpp)"));

        // check if the exportname is valid
        if(exportName.isEmpty())
        {

        }
        else
        {
            _currentExportFullPath = exportName;
            _currentFolder = QFileInfo(_currentExportFullPath).path();
            exportStatus = _project->exportToCode(_currentExportFullPath);
        }
    }
    else
    {
        exportStatus = _project->exportToCode(_currentExportFullPath);
    }

    if(exportStatus)
    {
        this->setWindowTitle("Exported to "+_currentExportFullPath);
        saveSettings();
    }
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

    if(importFileName.isEmpty())
    {

    }
    else
    {
        _currentFolder = QFileInfo(importFileName).path();
        _project->getDM()->importFile(current,importFileName);

        this->setWindowTitle("Imported file "+importFileName+" into state: " + current->getName());
        saveSettings();
    }


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


qreal MainWindow::clamp(qreal val, qreal min, qreal max)
{
    if(val < min)
        return min;
    if(val > max)
        return max;

    return val;
}

void MainWindow::on_actionNew_triggered()
{

}



void MainWindow::on_actionZoomOut_triggered()
{
    // do not zoom out if it will take it below the minimum
    if ((_scale - SCALE_STEP) < SCALE_MIN)
        return;

    scale(-1.0*SCALE_STEP);
}

void MainWindow::on_actionZoomIn_triggered()
{
    // do not zoom if it another step will take it above the max.
    if((_scale + SCALE_STEP) > SCALE_MAX)
        return;

    scale(SCALE_STEP);
}

/**
 * @brief MainWindow::scale
 * @param scale
 *
 * Pass a step size to scale by.
 *
 * E.g. going from 1.0 to 0.9, Pass step -0.1
 *
 * calls QGraphicsView::scale( (0.9)/(1.0) ) which sets the scale from 1.0 to 0.9
 *
 *
 */
void MainWindow::scale(qreal step)
{
    qreal mult = (_scale+step)/(_scale);
    _project->getQGraphicsView()->scale(mult, mult);
    _scale *= mult;
    qDebug() << "setting scale to : " << _scale;
    emit scaleChanged(_scale);  // alert the graphicsView that the scale changed
}

void MainWindow::on_actionSaveImage_triggered()
{
    if ( _project == NULL) return;

    // check if there is a current file, save it immediately, otherwise browse for one
//    if(_currentImageFullPath.isEmpty())
    {
        if(_currentFolder.isEmpty())
            _currentFolder = QDir::currentPath();

        QString defaultSave = _currentFolder + QString("\\"+_project->getDM()->getTopState()->getName());
        QString saveName = QFileDialog::getSaveFileName(this, tr("Save as .png"), defaultSave, tr(".png (*.png)"));

        // check if the saveName is valid
        if(saveName.isEmpty())
        {

        }
        else
        {
            _currentImageFullPath = saveName;
            _currentFolder = QFileInfo(_currentImageFullPath).path();
            _project->getSCGraphicsView()->saveImage(_currentImageFullPath);
            this->setWindowTitle(_currentImageFullPath);
            saveSettings();
        }
    }
//    else
//    {
//        _project->getSCGraphicsView()->saveImage(_currentImageFullPath);
//        this->setWindowTitle("Saved .png "+_currentImageFullPath);
//        saveSettings();
//    }
}

void MainWindow::on_actionGrid_toggled(bool arg1)
{
    qDebug() << "grid toggle: " << arg1;
    _gridEnable = arg1;
    emit gridToggled(arg1);
    saveSettings();
}

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
#include <QDesktopWidget>
#include "scitem.h"
#include "datamodellist.h"


#define POP_UP_X    160
#define POP_UP_Y    200

//#define SCALE_STEP 0.161803398875
#define SCALE_STEP 0.15
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
        _gridEnable(false),
        _tabWidget(NULL)
{

    QCoreApplication::setOrganizationName("David W Drell");
    QCoreApplication::setOrganizationDomain("davidwdrell.net");
    QCoreApplication::setApplicationName("Visual Statechart Editor");

    // Designer code
    ui->setupUi(this);

    _tabWidget = new QTabWidget();
    ui->gridLayout->addWidget(_tabWidget);

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


    // add font widgets
    createFontBar();
    // when the font combo box is activated, the data model will change the font family attribute for all items
    connect(_fontBox, SIGNAL(activated(QString)), this, SLOT(handleChangeFont(QString)));
    connect(_fontSizeBox, SIGNAL(activated(QString)), this, SLOT(handleChangeFont(QString)));


    // load settings from settings.ini if it exists, otherwise create a settings.ini file
    _settingsFileName = QDir::currentPath()+"/"+"settings.ini";
//    qDebug () << "settings file " << _settingsFileName;
    loadSettings();


    // resize the window
    // based on the current resolution, set the formview and graphicsview sizes
    // the height will be the same between the two
    // the width of the formview will take up 1/3 of the screen
    // the width of the graphicsview will take up 2/3 of the screen

#define W7_BORDER   7.5                 // each of the windows 7 window borders are 7.5 pixels.

    QRect desktop = QApplication::desktop()->availableGeometry();
    qreal sysW = desktop.width();
    qreal sysH = desktop.height();

    sysW -= 2*(W7_BORDER);              // account for window border
    qreal gW = sysW*2/3;
    qreal fW = sysW*1/3;
    fW -= W7_BORDER;                    // space between windows
    gW -= W7_BORDER;
    qreal gH = sysH - 37.5;             // 30 pixels + 7.5 pixels


    qDebug() << "fw: " <<fW << " gw: "<< gW <<" gH: "<<gH;

    this->resize(gW,gH);
    this->move( fW + W7_BORDER+ W7_BORDER,0);

}

MainWindow::~MainWindow()
{
    //delete _settings;
    delete _project;
    delete ui;
}

void MainWindow::handleSetProgramFontFamily(IAttribute * ffa)
{
    int fontIndex = _fontBox->findText(ffa->asString(), Qt::MatchFixedString);
    if(fontIndex>-1)
    {
        _fontBox->setCurrentIndex(fontIndex);
    }
}

void MainWindow::handleSetProgramFontSize(IAttribute * attr)
{
    FontSizeAttribute* fsa = dynamic_cast<FontSizeAttribute*>(attr);
    if ( fsa == NULL) return;
    int sizeIndex = _fontSizeBox->findText(QString::number(fsa->asInt()),0);
    if(sizeIndex > -1)
    {
        _fontSizeBox->setCurrentIndex(sizeIndex);
    }
}

void MainWindow::handleSetProgramFontBold(IAttribute * attr)
{
    FontBoldAttribute* fba = dynamic_cast<FontBoldAttribute*>(attr);
    if ( fba == NULL ) return;
    _boldAction->setChecked(fba->asBool());
}

void MainWindow::createFontBar()
{
    // Create the Fonts List
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

    // bold button
    _boldAction = new QAction(tr("Bold"), this);
    _boldAction->setCheckable(true);
    QPixmap pixmap(":/SCFormView/Bold-32.png");
    _boldAction->setIcon(QIcon(pixmap));
    _boldAction->setShortcut(tr("Ctrl+B"));
    connect(_boldAction, SIGNAL(triggered(bool)), this, SLOT(handleBoldToggled(bool)));

    ui->mainToolBar->addAction(_boldAction);

    ui->mainToolBar->addSeparator();

    // radio buttons
    _selectedRadioButton = new QRadioButton("Selected", this);
    _stateFontRadioButton = new QRadioButton("States", this);
    _transitionFontRadioButton = new QRadioButton("Transitions", this);
    _pStateRadioButton= new QRadioButton("PState Code", this);
    _qStateRadioButton= new QRadioButton("QState Code", this);


    connect(_pStateRadioButton,SIGNAL(clicked()),this,SLOT(handlePStateQStateChanged()));
    connect(_qStateRadioButton,SIGNAL(clicked()),this,SLOT(handlePStateQStateChanged()));

    // when a group radio button is selected, uncheck bold (this is to make it easier to trigger making all bold simultaneously)
    connect(_stateFontRadioButton, SIGNAL(clicked()), this, SLOT(handleFontRadioChanged()));
    connect(_transitionFontRadioButton, SIGNAL(clicked()), this, SLOT(handleFontRadioChanged()));

    ui->mainToolBar->addWidget(_selectedRadioButton);
    ui->mainToolBar->addWidget(_stateFontRadioButton);
    ui->mainToolBar->addWidget(_transitionFontRadioButton);
    ui->mainToolBar->addWidget(_pStateRadioButton);
    ui->mainToolBar->addWidget(_qStateRadioButton);

    _selectedRadioButton->toggle();
    ui->mainToolBar->addSeparator();

    _pStateRadioButton->setChecked(false);
    _qStateRadioButton->setChecked(true);

}

void MainWindow::handleBoldToggled(bool toggle)
{
    qDebug() << "bold: " << toggle;

    QFont font;

    if(toggle)
    {
       font = QFont("",1,QFont::Bold);
    }
    else
    {
       font = QFont("",1);
    }

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

    }

}

/**
 * @brief MainWindow::handleItemClicked
 * @param item
 *
 * SLOT
 *
 * when the datamodel emits the itemclicked signal, the mainwindow handles it here
 *
 */
void MainWindow::handleItemClicked()
{
    if(_selectedRadioButton->isChecked())
    {

    }
    else
    {
        _selectedRadioButton->toggle();
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
    font.setBold(_boldAction->isChecked());

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

void MainWindow::handlePStateQStateChanged()
{
    if ( QObject::sender() == _qStateRadioButton)
    {
        if ( _qStateRadioButton->isChecked())
            _pStateRadioButton->setChecked(false);
        else
            _pStateRadioButton->setChecked(true);
    }
    else if ( QObject::sender() == _pStateRadioButton)
    {
        if ( _pStateRadioButton->isChecked())
            _qStateRadioButton->setChecked(false);
        else
            _qStateRadioButton->setChecked(true);
    }
}

/**
 * @brief MainWindow::handleFontRadioChanged
 *
 * SLOT
 *
 * when a radio is triggered that represents a group of items, change the bold button to not checked
 */
void MainWindow::handleFontRadioChanged()
{
//    _boldAction->setChecked(false);
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
 * if no settings are found, create a settings.ini file
 *
 * .exe must have write privilege
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

    qDebug() << "====================" <<"\tloading settings\t" << "====================";

    const QStringList keys = settings.childKeys();
    foreach(const QString &key, keys)
    {
        qDebug() << "" <<key <<"\t:\t"<<settings.value(key);
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

    qDebug() << "====================" <<"\tdone loading settings\t" << "====================";
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

/**
 * @brief MainWindow::delay
 * deprecated function
 */
void MainWindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    QList<SCState*> states;
    _project->getDM()->getAllStates(states);
    qDebug()<<"delay num states: "  <<states.size();
}

/**
 * @brief MainWindow::handleNewClick
 *
 * SLOT
 *
 * when a new project is clicked, reset the program
 */
void MainWindow::handleNewClick()
{
    // alert the data model, graphics view, and form view to reset
    emit reset();

    // reset the current save and export files.
    _currentFileFullPath = "";
    _currentExportFullPath = "";

    this->setWindowTitle("Visual Statechart Editor");
}

void MainWindow::handleNewSubStateTab(SCState*subState)
{
    SMProject * project;

    // create the project
    SCDataModel * dm = new SCDataModel();
    project = new SMProject( dm, _tabWidget );

    // connects for the mainwindow to other modules

    // if the data model sends an error message, the main window will display it with a pop up
    connect(project->getDM(), SIGNAL(message(QString)), this, SLOT(handleMessage(QString)));

    // when the grid action is toggled, the graphics view's scene will change its background
    connect(this, SIGNAL(gridToggled(bool)), project->getSCGraphicsView()->getCustomGraphicsScene(), SLOT(handleGridToggled(bool)));

    // when the data model signals set program font, set the program font
    connect(project->getDM(), SIGNAL(setProgramFontFamily(IAttribute*)), this, SLOT(handleSetProgramFontFamily(IAttribute*)));
    connect(project->getDM(),SIGNAL(setProgramFontSize(IAttribute*)), this, SLOT(handleSetProgramFontSize(IAttribute*)));
    connect(project->getDM(), SIGNAL(setProgramFontBold(IAttribute*)), this, SLOT(handleSetProgramFontBold(IAttribute*)));

    // when the data model emits a clicked signal, change the radio button selection
    connect(project->getDM(), SIGNAL(itemClicked()), this, SLOT(handleItemClicked()));

    project->getDM()->open(subState);
    SCState* topState = project->getDM()->getTopState();

    _tabWidget->addTab( project->getQGraphicsView(),topState->getName());

    IAttribute* nameAttr = topState->attributes["name"];

    // follow name changes in root state
    connect(nameAttr,SIGNAL(changed(IAttribute*)),this,SLOT(handleRootStateNameChanged(IAttribute*)));

    _tabWidget->setTabText(1, nameAttr->asString());

}

void MainWindow::handleFileOpenClick()
{
    if(_currentFolder.isEmpty())
        _currentFolder = QDir::currentPath();


    QString newFileFullPath = QFileDialog::getOpenFileName(this, tr("Open SCXML Input File"), _currentFolder, tr("SCXML Files (*.scxml)"));


    // If open was canceled, then stop this function from continuing
    if(newFileFullPath.isEmpty())
        return;

    if ( _project != NULL)
    {
         _project->getDM()->handleReset();
         delete _project;
         _project =  NULL;
    }

    _currentFileFullPath = newFileFullPath;
    _currentFolder = QFileInfo(_currentFileFullPath).path();

    this->setWindowTitle(_currentFileFullPath);

    // create the project
    SCDataModel * dm = new SCDataModel();

    // this is the root dm
    DataModelList::singleton()->setRoot(dm);

    _project = new SMProject( dm, ui->centralWidget );

    _tabWidget->addTab( _project->getQGraphicsView(),"name");

    _formEditorWindow = new SCFormView(0, _project->getDM());


    // main window actions
    connect(_formEditorWindow, SIGNAL(newClick()), this, SLOT(handleNewClick()));
    connect(_formEditorWindow, SIGNAL(openClick()), this, SLOT(handleFileOpenClick()));
    connect(_formEditorWindow, SIGNAL(saveImageClick()), this, SLOT(on_actionSaveImage_triggered()));
    connect(_formEditorWindow, SIGNAL(saveClick()), this, SLOT(handleFileSaveClick()));
    connect(_formEditorWindow, SIGNAL(saveAsClick()), this, SLOT(on_actionSave_As_triggered()));
    connect(_formEditorWindow, SIGNAL(importClick()), this, SLOT(on_actionImport_triggered()));
    connect(_formEditorWindow, SIGNAL(exportClick()), this, SLOT(handleExportCodeClick()));

    _formEditorWindow->show();


    // connects for the mainwindow to other modules

    // if the data model sends an error message, the main window will display it with a pop up
    connect(_project->getDM(), SIGNAL(message(QString)), this, SLOT(handleMessage(QString)));

    // when the grid action is toggled, the graphics view's scene will change its background
    connect(this, SIGNAL(gridToggled(bool)), _project->getSCGraphicsView()->getCustomGraphicsScene(), SLOT(handleGridToggled(bool)));

    // when the data model signals set program font, set the program font
    connect(_project->getDM(), SIGNAL(setProgramFontFamily(IAttribute*)), this, SLOT(handleSetProgramFontFamily(IAttribute*)));
    connect(_project->getDM(),SIGNAL(setProgramFontSize(IAttribute*)), this, SLOT(handleSetProgramFontSize(IAttribute*)));
    connect(_project->getDM(), SIGNAL(setProgramFontBold(IAttribute*)), this, SLOT(handleSetProgramFontBold(IAttribute*)));
    connect(_project->getDM(),SIGNAL(newSubStateTabRequested(SCState*)),this,SLOT(handleNewSubStateTab(SCState*)));

    // when the data model emits a clicked signal, change the radio button selection
    connect(_project->getDM(), SIGNAL(itemClicked()), this, SLOT(handleItemClicked()));

    _project->getDM()->handleOpen(_currentFileFullPath);

    SCState* rootState = _project->getDM()->getTopState();
    IAttribute* nameAttr = rootState->attributes["name"];

    // follow name changes in root state
    connect(nameAttr,SIGNAL(changed(IAttribute*)),this,SLOT(handleRootStateNameChanged(IAttribute*)));

    _tabWidget->setTabText(0, nameAttr->asString());

    saveSettings();
}

void MainWindow::handleRootStateNameChanged(IAttribute*nameAttr)
{
    _tabWidget->setTabText(0, nameAttr->asString());
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

    SCDataModel::STATE_CODE_MODEL model;
    if ( _pStateRadioButton->isChecked())
        model = SCDataModel::PSTATE;
    else
        model = SCDataModel::QSTATE;

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
            exportStatus = _project->exportToCode(_currentExportFullPath,model);
        }
    }
    else
    {
        exportStatus = _project->exportToCode(_currentExportFullPath,model);
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
        sendMessage("!","Please select the state to be imported into");
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

    QString output = QString::number(_scale*100) +"%";
    this->setWindowTitle(output);
//    qDebug() << "setting scale to : " << _scale;
//    emit scaleChanged(_scale);  // alert the graphicsView that the scale changed


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

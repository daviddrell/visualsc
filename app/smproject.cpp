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

#include "smproject.h"
#include <QGraphicsScene>
#include <QList>
#include <QFile>
#include <QDebug>


SMProject::SMProject(QWidget *parentWidget): QObject(parentWidget),
        _file(),
        _dm(SCDataModel::singleton()),
        _graphicsView( NULL ),
        _parentWidget(parentWidget)
{
        _dm->initializeEmptyStateMachine();
        _dm->connectDataModel();
}

SMProject::~SMProject()
{
    close();
    if (_graphicsView) delete _graphicsView;
}


SCDataModel * SMProject::getDM()
{
    return _dm;
}

SCGraphicsView* SMProject::getSCGraphicsView()
{
    return _graphicsView;
}

QGraphicsView * SMProject::getQGraphicsView()
{
    if ( _graphicsView == NULL )
    {
        // this is new empty project as opposed to a projected loaded from a file
        _graphicsView = new SCGraphicsView(_parentWidget,  _dm);
    }

    return  _graphicsView->getQGraphicsView();
}

// deprecated function
void SMProject::initNewSM()
{
    _dm->initializeEmptyStateMachine();
}

/**
 * @brief SMProject::readInputFile
 * @param file
 *
 * Not currently in use due to multiple connects with multiple open calls
 *
 */
void SMProject::readInputFile(QString file)
{
    connect( _dm, SIGNAL(openCompleted(bool,QStringList)), this, SLOT(handleOpenCompleted(bool,QStringList)));

    _dm->open(file);
}


void SMProject::handleOpenCompleted(bool result ,QStringList messages)
{
    _graphicsView = new SCGraphicsView(_parentWidget,  _dm);
    emit readInputFileCompleted ( result, messages);
}

void SMProject::save(QString fileName)
{
    QString errorMsg;
    _dm->save(fileName, errorMsg);
}

bool SMProject::exportToCode(QString fileName)
{
    QString errorMessage;
    return _dm->exportToCode(fileName, errorMessage);
}

void SMProject::close()
{


}

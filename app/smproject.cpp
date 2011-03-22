#include "smproject.h"
#include <QGraphicsScene>
#include <QList>
#include <QFile>


#include <QDebug>


SMProject::SMProject(QWidget *parentWidget): QObject(parentWidget),
        _pd(),_file(), _dm(this),_graphicsView( parentWidget, & _dm)
{



}

SMProject::~SMProject()
{
    close();
}

SCDataModel * SMProject::getDM()
{
    return &_dm;
}

QGraphicsView * SMProject::getQGraphicsView()
{
    return  _graphicsView.getQGraphicsView();
}

void SMProject::readInputFile(QString file)
{
    connect(& _dm, SIGNAL(openCompleted(bool,QStringList)), this, SLOT(handleOpenCompleted(bool,QStringList)));

    _dm.open(file);

}


void SMProject::handleOpenCompleted(bool result ,QStringList messages)
{
    emit readInputFileCompleted ( result, messages);
}

void SMProject::save(QString fileName)
{
    QString errorMsg;
    _dm.save(fileName, errorMsg);
}


void SMProject::close()
{


}

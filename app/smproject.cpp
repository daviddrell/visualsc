#include "smproject.h"
#include <QGraphicsScene>
#include <QList>
#include <QFile>


#include <QDebug>


SMProject::SMProject(QWidget *parentWidget): QObject(parentWidget),
        _pd(),_file(), _dm(this),_graphicsView( parentWidget, & _dm),
        _editController(this, &_dm)
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
     _dm.open(file);
//    _editController.openFile(file);
}

void SMProject::save(QString fileName)
{
    QString errorMsg;
    _dm.save(fileName, errorMsg);
}


void SMProject::close()
{


}

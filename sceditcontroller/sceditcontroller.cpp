#include "sceditcontroller.h"


SCEditController::SCEditController(QObject * parent, SCDataModel *dm) :
        QObject (parent),
        _dm(dm)
{
}


void SCEditController::openFile(QString file)
{
    _dm->open(file);
}




#ifndef SCEDITCONTROLLER_H
#define SCEDITCONTROLLER_H

#include "scdatamodel.h"
#include "sceditcontroller_global.h"

#include <QObject>
#include <QString>

class SCEDITCONTROLLERSHARED_EXPORT SCEditController : QObject
{
      Q_OBJECT


public:
    SCEditController(QObject * parent, SCDataModel *dm);

    void openFile(QString file);

signals:

private slots:

private:

    SCDataModel *_dm;

};

#endif // SCEDITCONTROLLER_H

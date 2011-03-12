#ifndef SCTRANSISTION_H
#define SCTRANSISTION_H

#include "transitionattributes.h"
#include <QObject>
#include <QList>
#include <QPointF>
class SCState;

#include "scdatamodel_global.h"
class QXmlStreamWriter;


class SCDATAMODELSHARED_EXPORT  SCTransition : public QObject
{
    Q_OBJECT

public:
    SCTransition(QObject * parent);

    ~SCTransition();

    void    setAttributeValue(QString key, QString value);
    QString getAttributeValue(QString key);
    void    writeSCVXML(QXmlStreamWriter & sw);

     //public data members

    TransitionAttributes attributes;

signals:
    void selected();
    void unselected();

private slots:
    void handleLineSelected();
    void handleLineUnSelected();

private:

    SCState *       _targetState;

};


#endif // SCTRANSISTION_H

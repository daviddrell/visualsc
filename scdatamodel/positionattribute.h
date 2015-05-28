#ifndef POSITIONATTRIBUTE_H
#define POSITIONATTRIBUTE_H

#include <QList>
#include <QPointF>
#include <QString>
#include <QColor>
#include "iattribute.h"
#include "scdatamodel_global.h"
#include <QMetaType>


class  SCDATAMODELSHARED_EXPORT PositionAttribute:  public IAttribute
{
public:
    PositionAttribute();

    PositionAttribute(const PositionAttribute& pa);

    PositionAttribute(QObject*  parent,QString key,QPointF p);

    PositionAttribute(QObject*  parent,QString key,QString s);

    ~PositionAttribute();

    PositionAttribute& operator=( PositionAttribute& pa );

    virtual QString asString();
    QPointF asPointF() const;

    virtual void setValue(const QString position);
    void setValue(int x, int y);
    void setValue(const QPointF p);
private :
    QPointF _position;

};



Q_DECLARE_METATYPE(PositionAttribute);

#endif // POSITIONATTRIBUTE_H

#ifndef IDATTRIBUTE_H
#define IDATTRIBUTE_H

#include <QString>
#include "iattribute.h"
#include "scdatamodel_global.h"
#include <QMetaType>


class  SCDATAMODELSHARED_EXPORT IDAttribute:  public IAttribute
{
public:
    IDAttribute();

    IDAttribute(QObject*  parent,QString key,QPointF p);

    IDAttribute(QObject*  parent,QString key,QString s);

    ~IDAttribute();

    IDAttribute& operator=( IDAttribute& sa );

    QString asString();

    void    setValue(const QString id);

private :
    QString _id;

};


Q_DECLARE_METATYPE(IDAttribute);

#endif // IDATTRIBUTE_H

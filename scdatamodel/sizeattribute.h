#ifndef SIZEATTRIBUTE_H
#define SIZEATTRIBUTE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QPoint>
#include "iattribute.h"
#include "scdatamodel_global.h"

class  SCDATAMODELSHARED_EXPORT SizeAttribute: public IAttribute
{
public:

    SizeAttribute();
    SizeAttribute(const SizeAttribute& sizeAttribute);
    SizeAttribute(QObject*  parent, QString key, QPointF s);
    SizeAttribute(QObject*  parent, QString key, QString s);

    ~SizeAttribute();

    SizeAttribute& operator=( SizeAttribute& sa );

    QString asString();
    QPointF asPointF() const;

    void    setValue(QString size);
    void    setValue(const int w,const int h);
    void    setValue(const QPointF s);
private :
        QPointF _size;
};


Q_DECLARE_METATYPE(SizeAttribute);


#endif // SIZEATTRIBUTE_H

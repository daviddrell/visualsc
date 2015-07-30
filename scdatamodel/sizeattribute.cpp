#include "sizeattribute.h"
#include <QDebug>

SizeAttribute::SizeAttribute():IAttribute(), _size()
{
    qRegisterMetaType<SizeAttribute>("SizeAttribute");
}

SizeAttribute::SizeAttribute(QObject*  parent, QString key, QPointF s):IAttribute(parent, key), _size(s)
{
    qRegisterMetaType<SizeAttribute>("SizeAttribute");
}

SizeAttribute::SizeAttribute(QObject*  parent, QString key, QString s):IAttribute(parent, key), _size()
{
    qRegisterMetaType<SizeAttribute>("SizeAttribute");
    setValue(s);
}


SizeAttribute::SizeAttribute(const SizeAttribute& sa):IAttribute(sa), _size(sa.asPointF())
{
    qRegisterMetaType<SizeAttribute>("SizeAttribute");
}

SizeAttribute::~SizeAttribute(){}


SizeAttribute& SizeAttribute::operator=( SizeAttribute& sa )
{
    IAttribute::operator =(sa);
    _size = sa._size;
    return *this;
}


QString SizeAttribute::asString()
{
    return (QString::number(_size.x()) + QString(",") +  QString::number(_size.y()));
}

QPointF SizeAttribute::asPointF() const
{
    return (_size);
}

void  SizeAttribute::setValue(const QString size)
{
    QStringList sl = size.split(",");
    if ( sl.length() == 2)
    {
        QPointF nSize(sl[0].toInt(),sl[1].toInt());

        if ( nSize != _size )
        {
            _size = nSize;
            emit changed(this);
        }
    }
}

void  SizeAttribute::setValue(const int w,const int h)
{

    QPointF nSize(w,h);

    if ( nSize != _size )
    {
        _size = nSize;
        qDebug() << "SizeAttribute::emitting changed";
        emit changed(this);
    }
}

void SizeAttribute::setValue(const QPointF nSize)
{

    if ( nSize != _size )
    {
        _size = nSize;
        qDebug() << "SizeAttribute::emitting changed";
        emit changed(this);
    }
}


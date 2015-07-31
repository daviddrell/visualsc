
#include "positionattribute.h"
#include <QDebug>

PositionAttribute::PositionAttribute() :IAttribute( ), _position()
{
    qRegisterMetaType<PositionAttribute>("PositionAttribute");
}

PositionAttribute::PositionAttribute(const PositionAttribute& pa) :IAttribute( pa), _position(pa.asPointF())
{
    qRegisterMetaType<PositionAttribute>("PositionAttribute");
}

PositionAttribute::PositionAttribute(QObject*  parent,QString key,QPointF p) :IAttribute(parent, key), _position(p)
{
    qRegisterMetaType<PositionAttribute>("PositionAttribute");
}

PositionAttribute::PositionAttribute(QObject*  parent,QString key,QString s) :IAttribute(parent, key), _position()
{
    qRegisterMetaType<PositionAttribute>("PositionAttribute");
    setValue(s);
}

PositionAttribute::~PositionAttribute(){}

PositionAttribute& PositionAttribute::operator=( PositionAttribute& sa )
                                                                                        {
    _position = sa._position;
    return *this;
}

QString PositionAttribute::asString()
{
    return (QString::number(_position.x()) + QString(",") +  QString::number(_position.y()));
}

QPointF PositionAttribute::asPointF() const
{
    return (_position);
}

void PositionAttribute::setValue(const QString position)
{
    QStringList sl = position.split(",");
    if ( sl.length() != 2)
    {
        return;
    }

    QPointF nPosition(sl[0].toInt(),sl[1].toInt());

    if ( nPosition != _position )
    {
        _position = nPosition;
        emit changed(this);
    }
}

void PositionAttribute::setValue(int w, int h)
{
    QPointF nPosition( w,  h);

    if ( nPosition != _position )
    {
        _position = nPosition;
        emit changed(this);
    }
}

void PositionAttribute::setValue(const QPointF nPosition)
{
    if ( nPosition != _position )
    {
        _position = nPosition;
        qDebug() << "pos attr emit changed";
        emit changed(this);
    }
}





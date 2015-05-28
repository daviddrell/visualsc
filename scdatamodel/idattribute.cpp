#include "idattribute.h"


IDAttribute::IDAttribute() :IAttribute( ), _id()
{
    qRegisterMetaType<IDAttribute>("IDAttribute");
}


IDAttribute::IDAttribute(QObject*  parent,QString key,QString s) :IAttribute(parent, key), _id()
{
    qRegisterMetaType<IDAttribute>("IDAttribute");
    setValue(s);
}

IDAttribute& IDAttribute::operator=( IDAttribute& sa )
{
    _id = sa._id;
    return *this;
}

IDAttribute::~IDAttribute()
{
}

QString IDAttribute::asString()
{
    return _id;
}


void IDAttribute::setValue(const QString id)
{

    if ( id != _id )
    {
        _id = id;
        emit changed(this);
    }
}


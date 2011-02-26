#include "stateattributes.h"

StateAttributes::StateAttributes() :
        IAttributeContainer()
{
    qRegisterMetaType<StateAttributes>("StateAttributes");
}

StateAttributes::StateAttributes(QObject * parent, QString key) :
        IAttributeContainer(parent, key)
{
    qRegisterMetaType<StateAttributes>("StateAttributes");
}

StateAttributes::StateAttributes(const StateAttributes& sa): IAttributeContainer(sa)
{
    qRegisterMetaType<StateAttributes>("StateAttributes");
}

StateAttributes& StateAttributes::operator=( StateAttributes& sa )
{
    setAttributes(sa);
    return *this;
}


StateAttributes::AttributeMajorType StateAttributes::getMajorType()
{
    return IAttributeContainer::aMajorType_State ;
}

StateAttributes::~StateAttributes()
{ }




  //  NAME



StateAttributes::StateName::StateName(QObject*  parent,QString key,QString n):IAttribute(parent, key),_name(n)
{
    qRegisterMetaType<StateAttributes>("StateAttributes::StateName");
}

StateAttributes::StateName::StateName():IAttribute(),_name()
{
    qRegisterMetaType<StateAttributes>("StateAttributes::StateName");
}

StateAttributes::StateName::~StateName()
{
    qRegisterMetaType<StateAttributes>("StateAttributes::StateName");
}

StateAttributes::StateName::StateName& StateAttributes::StateName::operator=( StateName& sa )
{
    _name = sa._name;
    return *this;
}

QString StateAttributes::StateName::asString()
{
    return _name;
}

void    StateAttributes::StateName::setValue(const QString name)
{
    if ( name != _name )
    {
        _name = name;
        emit changed(this );
    }
}

int StateAttributes::StateName::getType()
{
    return QVariant::String;
}



//  SIZE




StateAttributes::StateSize::StateSize():IAttribute(), _size()
{
    qRegisterMetaType<StateAttributes>("StateAttributes::StateSize");
}
StateAttributes::StateSize::StateSize(QObject*  parent, QString key, QPoint s):IAttribute(parent, key), _size(s)
{
    qRegisterMetaType<StateAttributes>("StateAttributes::StateSize");
}

StateAttributes::StateSize::StateSize(QObject*  parent, QString key, QString s):IAttribute(parent, key), _size()
{
    qRegisterMetaType<StateAttributes>("StateAttributes::StateSize");
    setValue(s);
}

StateAttributes::StateSize::~StateSize(){}

StateAttributes::StateSize& StateAttributes::StateSize::operator=( StateSize& sa )
{
    _size = sa._size;
    return *this;
}

int StateAttributes::StateSize::getType()
{
    return QVariant::PointF;
}

QString StateAttributes::StateSize::asString()
{
    return (QString::number(_size.x()) + QString(",") +  QString::number(_size.y()));
}

QPointF StateAttributes::StateSize::asPointF() const
{
    return (_size);
}

void    StateAttributes::StateSize::setValue(QString size)
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

void    StateAttributes::StateSize::setValue(const int w,const int h)
{

    QPointF nSize(w,h);

    if ( nSize != _size )
    {
        _size = nSize;
        emit changed(this);
    }
}

void    StateAttributes::StateSize::setValue(const QPointF nSize)
{

    if ( nSize != _size )
    {
        _size = nSize;
        emit changed(this);
    }
}



// POSITION



StateAttributes::StatePosition::StatePosition() :IAttribute( ), _position()
{
    qRegisterMetaType<StateAttributes>("StateAttributes::StatePosition");
}

StateAttributes::StatePosition::StatePosition(QObject*  parent,QString key,QPointF p) :IAttribute(parent, key), _position(p)
{
    qRegisterMetaType<StateAttributes>("StateAttributes::StatePosition");
}

StateAttributes::StatePosition::StatePosition(QObject*  parent,QString key,QString s) :IAttribute(parent, key), _position()
{
    qRegisterMetaType<StateAttributes>("StateAttributes::StatePosition");
    setValue(s);
}

StateAttributes::StatePosition::~StatePosition(){}

StateAttributes::StatePosition::StatePosition& StateAttributes::StatePosition::operator=( StatePosition& sa )
                                                                                        {
    _position = sa._position;
    return *this;
}

int StateAttributes::StatePosition::getType() {return QVariant::PointF;}

QString StateAttributes::StatePosition::asString()
{
    return (QString::number(_position.x()) + QString(",") +  QString::number(_position.y()));
}

QPointF StateAttributes::StatePosition::asPointF() const
{
    return (_position);
}

void    StateAttributes::StatePosition::setValue(const QString position)
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

void    StateAttributes::StatePosition::setValue(int w, int h)
{   
    QPointF nPosition( w,  h);

    if ( nPosition != _position )
    {
        _position = nPosition;
        emit changed(this);
    }
}

void    StateAttributes::StatePosition::setValue(const QPointF nPosition)
{ 
    if ( nPosition != _position )
    {
        _position = nPosition;
        emit changed(this);
    }
}



// String


StateAttributes::StateString::StateString(QObject*  parent,QString key,QString s) :IAttribute(parent, key), _value(s)
{
     qRegisterMetaType<StateAttributes>("StateAttributes::StateString");
}
StateAttributes::StateString::StateString() :IAttribute(), _value()
{
     qRegisterMetaType<StateAttributes>("StateAttributes::StateString");
}
StateAttributes::StateString::~StateString()
{
     qRegisterMetaType<StateAttributes>("StateAttributes::StateString");
}

StateAttributes::StateString::StateString& StateAttributes::StateString::operator=( StateString& sa )
{
    _value = sa._value;
    return *this;
}

int StateAttributes::StateString::getType()
{
    return QVariant::String;
}

QString StateAttributes::StateString::asString()
{
    return _value;
}


void    StateAttributes::StateString::setValue(const QString value)
{
    _value= value;

    if ( value != _value )
    {
        _value = value;
        emit changed(this);
    }
}



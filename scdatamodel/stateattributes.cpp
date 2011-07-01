/*
    visualsc - Visual State Chart Editing for SCXML State Engines
    Copyright (C) 2011  David W. Drell (david@davidwdrell.net) and
    Contributing Authors:


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "stateattributes.h"

StateAttributes::StateAttributes() :
        IAttributeContainer()
{
}

StateAttributes::StateAttributes(QObject * parent, QString key) :
        IAttributeContainer(parent, key)
{
}

StateAttributes::StateAttributes(const StateAttributes& sa): IAttributeContainer(sa)
{
}

StateAttributes& StateAttributes::operator=( StateAttributes& sa )
{
    setAttributes(sa);
    return *this;
}


StateAttributes::~StateAttributes()
{ }



void StateAttributes::setAttributes(const IAttributeContainer& sourceAttrList)
{
    // if the dest container has a matching key,
    // update the value to be the source value.
    // else, add the source to the container
    //
    // remove from the dest list any attributes that are not in the souce list

    QMapIterator<QString,IAttribute*> i(sourceAttrList);
    while (i.hasNext())
    {
        QString key  = i.next().key();
        IAttribute* sourceAttr = sourceAttrList.value(key)  ;
        IAttribute* destAttr = this->value( key ) ;
        if ( destAttr )
        {
            destAttr->setValue( sourceAttr->asString());
        }
        else
        {
            IAttribute* newAttr=NULL;

            StateAttributes::StateName *nm ;
            StateAttributes::StatePosition *ps;
            StateAttributes::StateSize *sz;
             StateAttributes::StateString *str ;

            if ( (  nm = dynamic_cast<StateAttributes::StateName *>(sourceAttr) ) != NULL )
            {
                StateAttributes::StateName * newNm = new StateAttributes::StateName (*nm);
                addItem(newNm);
                newAttr= newNm;
            }
            else if ( ( ps = dynamic_cast<StateAttributes::StatePosition *>(sourceAttr) ) != NULL )
            {
                StateAttributes::StatePosition * newPs = new StateAttributes::StatePosition (*ps);
                addItem(newPs);
                newAttr = newPs;
            }
            else if ( ( sz = dynamic_cast<StateAttributes::StateSize *>(sourceAttr) ) != NULL )
            {
                StateAttributes::StateSize * newSz = new StateAttributes::StateSize (*sz);
                addItem(newSz);
                newAttr = newSz;
            }
            else if ( (  str = dynamic_cast<StateAttributes::StateString *>(sourceAttr) ) != NULL )
            {
                StateAttributes::StateString * newStr = new StateAttributes::StateString (*str);
                addItem(newStr);
                newAttr = newStr;
            }

            if ( newAttr ) emit attributeAdded(newAttr);
        }
    }

    // now delete local attributes that are not contained in the source list

   QMapIterator<QString,IAttribute*> j(sourceAttrList);

    while (j.hasNext())
    {
        QString key  = j.next().key();

        if ( !sourceAttrList.contains(key) )
        {
            IAttribute* attr = this->value(key);
            this->remove(key);
            emit attributeDeleted(attr);
            delete attr;
        }
    }
}



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
}

StateAttributes::StateString::StateString() :IAttribute(), _value()
{
}

StateAttributes::StateString::~StateString()
{
}

StateAttributes::StateString::StateString& StateAttributes::StateString::operator=( StateString& sa )
{
    _value = sa._value;
    return *this;
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



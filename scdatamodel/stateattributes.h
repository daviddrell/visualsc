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

#ifndef STATEATTRIBUTES_H
#define STATEATTRIBUTES_H

#include <QString>
#include <QStringList>
#include <QPoint>

#include "iattribute.h"

#include "scdatamodel_global.h"
#include <QPoint>
#include <QString>
#include <QMetaType>


class  SCDATAMODELSHARED_EXPORT StateAttributes : public IAttributeContainer
{
public:
    StateAttributes();

    StateAttributes(QObject * parent, QString key);

    StateAttributes(const StateAttributes& sa);

    StateAttributes& operator=( StateAttributes& sa );

    virtual AttributeMajorType getMajorType();

    virtual void setAttributes(const IAttributeContainer& sourceAttrList);

    ~StateAttributes() ;

      //  NAME


    class SCDATAMODELSHARED_EXPORT StateName :  public IAttribute
    {
    public:

        StateName(QObject*  parent,QString key,QString n);
        StateName();
        ~StateName();

        StateName& operator=( StateName& sa );

        QString asString();
        void    setValue(const QString name);
        int getType() ;

    private :
            QString _name;
    };



    //  SIZE


    class  SCDATAMODELSHARED_EXPORT StateSize: public IAttribute
    {
    public:

        StateSize();
        StateSize(QObject*  parent, QString key, QPoint s);
        StateSize(QObject*  parent, QString key, QString s);

        ~StateSize();

        StateSize& operator=( StateSize& sa );


        int getType() ;

        QString asString();
        QPointF asPointF() const;

        void    setValue(QString size);
        void    setValue(const int w,const int h);
        void    setValue(const QPointF s);
    private :
            QPointF _size;
    };



    // POSITION


    class  SCDATAMODELSHARED_EXPORT StatePosition:  public IAttribute
    {
    public:
        StatePosition();

        StatePosition(QObject*  parent,QString key,QPointF p);

        StatePosition(QObject*  parent,QString key,QString s);

        ~StatePosition();

        StatePosition& operator=( StatePosition& sa );

        int getType();

        QString asString();
        QPointF asPointF() const;

        void    setValue(const QString position);
        void    setValue(int w, int h);
        void    setValue(const QPointF p);
    private :
            QPointF _position;

    };

    class  SCDATAMODELSHARED_EXPORT StateString:  public IAttribute
    {
    public:
        StateString(QObject*  parent,QString key,QString s);
        StateString() ;
        ~StateString() ;

        StateString& operator=( StateString& sa );

        int getType();

        QString asString();


        void    setValue(const QString value);

        private :
            QString _value;

    };

};


#endif // STATEATTRIBUTES_H

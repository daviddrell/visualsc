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
#include "sizeattribute.h"
#include "positionattribute.h"
#include "scdatamodel_global.h"


/**
 * \class StateAttributes
 *
 * \ingroup DataModel
 * \ingroup Attributes
 *
 */

class  SCDATAMODELSHARED_EXPORT StateAttributes : public IAttributeContainer
{
    Q_OBJECT

public:
    StateAttributes();

    StateAttributes(QObject * parent, QString key);

    StateAttributes(const StateAttributes& sa);

    StateAttributes& operator=( StateAttributes& sa );

    virtual void setAttributes(const IAttributeContainer& sourceAttrList);

    ~StateAttributes() ;

};

class SCDATAMODELSHARED_EXPORT StateName :  public IAttribute
{
    Q_OBJECT

public:

    StateName(QObject*  parent,QString key,QString n);
    StateName();
    ~StateName();

    StateName& operator=( StateName& sa );

    QString asString();
    void    setValue(const QString name);

private :
    QString _name;
};



class  SCDATAMODELSHARED_EXPORT StateString:  public IAttribute
{
    Q_OBJECT

public:
    StateString(QObject*  parent,QString key,QString s);
    StateString() ;
    ~StateString() ;

    StateString& operator=( StateString& sa );

    QString asString();

    void    setValue(const QString value);

private :
    QString _value;

};

#endif // STATEATTRIBUTES_H

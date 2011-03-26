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

#ifndef IATTRIBUTE_H
#define IATTRIBUTE_H

#include <QString>
#include <QVariant>
#include <QMap>
#include <QMapIterator>

class IAttribute: public QObject
{
    Q_OBJECT

public:

    IAttribute() ;

    IAttribute(QObject *parent, QString initkey);

    IAttribute(const IAttribute& a);

    enum AttributeType {
        aType_Name,
        aType_Position,
        aType_Size,
        aType_Path
    };

    virtual QString asString()=0;
    virtual void setValue(const QString value)  = 0 ;
    virtual int getType() = 0;

    virtual QString key() const;

signals:
    void changed( IAttribute* attr);

private:
    QString _key;

};


// container - a Map of IAttribute-subclasses

class IAttributeContainer : public QObject, public QMap<QString,IAttribute*>
{

    Q_OBJECT

public:

    enum AttributeMajorType { aMajorType_State, aMajorType_Transition};

    IAttributeContainer(QObject *parent, QString key);

    IAttributeContainer(const IAttributeContainer& c);

    IAttributeContainer();

    ~IAttributeContainer();

    virtual AttributeMajorType getMajorType() = 0;

    virtual QString key() ;

    void addItem( IAttribute* attr);


    /**
      * \fn setAttributes
      * \abstract sets the values of the abtribute list to the same values as the passed-in list
      */

    virtual void setAttributes(const IAttributeContainer& sourceAttrList) = 0;

    /**
      * \fn operator=
      * \abstract sets the values of the abtribute list to the same values as the passed-in list, equivalent to setAttributes
      */
    IAttributeContainer& operator=( const IAttributeContainer& copyFrom );

signals:

    void attributeAdded(IAttribute *a);
    void attributeDeleted(IAttribute *a);

private:


    QString _containerName;


};

#endif // IATTRIBUTE_H

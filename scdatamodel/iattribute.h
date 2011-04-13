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

/**
 *  \defgroup Attributes
 *
 * Attributes are intented to be contained in IAttributeContainer-s.
 * Attributes should must subclass IAttribute and implement asString(), setValue() and getType().
 * New Attribute sub-types should be added to the AttributeType enum.
 *
 * Generally, a view can query attribute containers for attributes, then query attributes
 * for their values using asString(), without checking the type.
 *
 * Below is an example of generic attribute handling from the FormView, which loads the attribute
 * table view when a state or transition is clicked in the tree-view.
 *
 * <b> Notes on State and Transition Identifiers</b>

  SCXML does not require a state have an id. But here we definitely need a consistent
     method of refering to a state. So 'name' is used as the definitive identifier for states in this model.
     When the model is saved back to an SCXML file, the name is pushed as the 'id' field. So all states in SCXML
     files produced by this program will end up with an id.

  In SCXML, Transitions are identified by their target states with the 'target' attribute.
     In order to genericise attribute handling, this program identifies transitions with the 'name' key (same as for state
     identifiers). Thus internally, all states and transitions have 'name's. When saved back to an SCXML file,
     the transition name is written as the 'target' attribute.


 \code
    // get an interator to the IAttributeContainer which is define as Map of key strings and IAttribute pointers.
    //  the key strings are the attribute id's or names, for example "name", "path", "size", "pos", "target".

    QMapIterator<QString,IAttribute*> i(*attributes);

    while (i.hasNext())
    {
        QString key  = i.next().key(); // get this key for this attribute, e.g. 'name', 'path', 'size', etc

        IAttribute* attr = attributes->value(key)  ;// get the map value, which is the IAttribute pointer

        // all IAttributes have a changed signal, so connect to it so we can be notified if another view changes
        // the value while we are displaying it here in this view, so we can update our view

        connect ( attr, SIGNAL(changed(IAttribute*)), this, SLOT(handlePropertyChanged(IAttribute*)));

        // create a table entry for this attribute

        QTableWidgetItem * propName = new QTableWidgetItem(key);

        propName->setFlags( (propName->flags() & (~Qt::ItemIsEditable)) | ((Qt::ItemIsEnabled)));

        // load the table entry with the attribute's value as a string

        QTableWidgetItem * propValue = new QTableWidgetItem(attr->asString());

        propValue->setFlags(propValue->flags() | (Qt::ItemIsEditable) | (Qt::ItemIsEnabled));

        // the table has two columns, one with the property (attribute) key and the other with the value

        propertyTable->setItem(row, 0, propName);
        propertyTable->setItem(row++, 1, propValue);

    }
 \endcode

 *
 *
 */

/**
 * \class IAttribute
 *
 * \brief This is the base abstract class for state and transition attributes.
 *
 * \ingroup Attributes
 *
 * Attributes are intented to be contained in IAttributeContainer-s.
 * Attributes should must subclass IAttribute and implement asString(), setValue() and getType().
 * New Attribute sub-types should be added to the AttributeType enum.
 *
 * Generally, a view can query attribute containers for attributes, then query attributes
 * for their values using asString(), without checking the type.
 *
 */

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


/**
 * \class IAttributeContainer
 *
 * \brief This is the base abstract class for state and transition attribute containers.
 *
 * \ingroup Attributes
 *
 * Attributes are intented to be contained in IAttributeContainer-s.
 *
 * \sa Attributes
 *
 */


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

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
   \defgroup Attributes

   \ingroup DataModel

 Attributes are intented to be contained in IAttributeContainer-s.

 Elements of a state chart are states and state transitions. Each of these elements have attributes.
 In addition, states and transitions can have sub elements such as commentary text. Each sub element may
 have attributes.

 Examples of attributes are a state's ID, state type (initial, final, etc), a state's graphical position, size.

 The data model holds a reference to the top-level state (class SCState). The top level state is also known
 as the state-machine. The top level state holds an attribute container
 for its attributes as well as a QList of substates and a QList of transitions which are sourced from that state.

 Attributes must subclass IAttribute and implement asString(), setValue().

 Generally, a view can query attribute containers for attributes, then query attributes
 for their values using asString().

 Below is an example of generic attribute handling from the FormView, which loads the attribute
 table view when a state or transition is clicked in the tree-view.

 <b> Notes on State and Transition Identifiers</b>


 \code
    // get an interator to the IAttributeContainer which is define as Map of key strings and IAttribute pointers.
    //  the key strings are the attribute id's or names, for example "name", "path", "size", "pos", "target".

    QMapIterator<QString,IAttribute*> i(*attributes);

    while (i.hasNext())
    {
        QString key  = i.next().key(); // get the key for this attribute, e.g. 'name', 'path', 'size', etc

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

 */

/**
 * \class IAttribute
 *
 * \brief This is the base  class for state and transition attributes.
 *
 * \ingroup Attributes
 * \ingroup DataModel
 *
 * Attributes are intented to be contained in IAttributeContainer-s.
 * Attributes must subclass IAttribute and implement asString(), setValue().
 * New Attribute sub-types should be added to the AttributeType enum.
 *
 * Generally, a view can query attribute containers for attributes, then query attributes
 * for their values using asString(), without checking the type.
 *
 * An subclass of IAttribute may override the the setValue() method to provide native type parameters,
 * for example, from the TransitionPositionAttribute subclass:
 \code
        virtual void    setValue(const QString position)  ;
        void            setValue(const int w,const int h);
        void            setValue(const QPointF s);

 \endcode
 */

class IAttribute: public QObject
{
    Q_OBJECT

public:

    IAttribute() ;

    IAttribute(QObject *parent, QString initkey);

    IAttribute(const IAttribute& a);

    IAttribute& operator=(const IAttribute& a);

    virtual QString asString();
    virtual void setValue(const QString value) ;
    virtual QString key() const;

signals:
    void changed( IAttribute* attr);
    void error(QString err);

private:
    QString _key;

};

class GenericAttribute: public IAttribute
{
public:

    GenericAttribute();

    GenericAttribute(QObject *parent, QString initkey, QString initValue);

    GenericAttribute(const GenericAttribute& a);

    GenericAttribute& operator=(const GenericAttribute& a);

    virtual QString asString();
    virtual void setValue(const QString value);

private:
    QString _value;

};

/**
 * \class IAttributeContainer
 *
 * \brief This is the base abstract class for state and transition attribute containers.
 *
 * \ingroup DataModel
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


    IAttributeContainer(QObject *parent, QString key);

    IAttributeContainer(const IAttributeContainer& c);

    IAttributeContainer();

    ~IAttributeContainer();


    virtual QString key() ;

    /**
      \fn addItem

      \brief add an attribute.

      If an attribute with the same key already exists in the container, do not add, but change
      the value of the existing attribute to be the value of the new attribute.

      \return if an attribute with the same key already exists, return false;
      */
    bool addItem( IAttribute* attr);


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

Q_DECLARE_METATYPE(IAttribute)
Q_DECLARE_METATYPE(IAttribute*)

Q_DECLARE_OPAQUE_POINTER(IAttribute*)


#endif // IATTRIBUTE_H

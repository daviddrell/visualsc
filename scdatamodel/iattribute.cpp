#include "iattribute.h"


// IAttribute - a base class for attributes


IAttribute::IAttribute() : QObject(), _key()
{
}

IAttribute::IAttribute(QObject *parent, QString initkey) : QObject(parent), _key(initkey)
{
}

IAttribute::IAttribute(const IAttribute& a) : QObject(a.parent()), _key(a.key())
{
}

QString IAttribute::key() const {return _key; }



// container - a Map of IAttribute-subclasses


IAttributeContainer::IAttributeContainer(QObject *parent, QString key) :
        QObject(parent), QMap<QString,IAttribute*>(),
        _containerName(key)
{
}

IAttributeContainer::IAttributeContainer(const IAttributeContainer& c):
        QObject(NULL), // can't copy QObjects
        QMap<QString,IAttribute*>(c),
        _containerName(c._containerName)
{
}

IAttributeContainer::IAttributeContainer():
        QObject(), // can't copy QObjects
        QMap<QString,IAttribute*>(),
        _containerName()
{
}

IAttributeContainer::~IAttributeContainer()
{
}

QString IAttributeContainer::key() {return _containerName; }

void IAttributeContainer::addItem( IAttribute* attr)
{
    QString k = attr->key();
    this->insert(k, attr);
    emit attributeAdded(attr);
}


void IAttributeContainer::setAttributes(const IAttributeContainer& sourceAttrList)
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
            addItem( sourceAttr);
            emit attributeAdded(sourceAttr);
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

/**
      * \fn operator=
      * \abstract sets the values of the abtribute list to the same values as the passed-in list, equivalent to setAttributes
      */
IAttributeContainer& IAttributeContainer::operator=( const IAttributeContainer& copyFrom )
                                                   {
    setAttributes(copyFrom);
    return *this;
}


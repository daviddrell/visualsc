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
    QMapIterator<QString,IAttribute*> i(*this);
    while (i.hasNext())
    {
        QString key  = i.next().key();
        IAttribute* attr = this->value(key)  ;
        delete attr;
    }
}

QString IAttributeContainer::key() {return _containerName; }

void IAttributeContainer::addItem( IAttribute* attr)
{
    QString k = attr->key();
    this->insert(k, attr);
    emit attributeAdded(attr);
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


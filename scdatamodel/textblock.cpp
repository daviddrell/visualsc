#include "textblock.h"
#include <QMapIterator>
#include "positionattribute.h"
#include "sizeattribute.h"
#include <QDebug>

SCTextBlock::SCTextBlock(): SCItem()
{
    FontFamilyAttribute * ff = new FontFamilyAttribute(NULL,"font-family", "courier");
    ff->setParent(this);
    attributes.addItem(ff);

    FontSizeAttribute * fs = new FontSizeAttribute(NULL,"font-size",8);
    fs->setParent(this);
    attributes.addItem(fs);

    FontColorAttribute * fc = new FontColorAttribute(NULL,"font-color", Qt::black);
    fc->setParent(this);
    attributes.addItem(fc);

    FontBoldAttribute * fb = new FontBoldAttribute(NULL,"font-bold",false);
    fb->setParent(this);
    attributes.addItem(fb);

    FontUnderlineAttribute * fu = new FontUnderlineAttribute(NULL,"font-underline", false);
    fu->setParent(this);
    attributes.addItem(fu);

    PositionAttribute* pa = new PositionAttribute(NULL, "position", QPointF(10,10));
    pa->setParent(this);
    attributes.addItem(pa);

    SizeAttribute * sa = new SizeAttribute(NULL, "size", QPointF(100,60) );
    sa->setParent(this);
    attributes.addItem(sa);


    connect(sa, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)));
}
/*
SCTextBlock::SCTextBlock(const SCState& st): SCItem(st.parent())
{
    FontFamilyAttribute * ff = new FontFamilyAttribute(NULL,"font-family", "courier");
    attributes.addItem(ff);

    FontSizeAttribute * fs = new FontSizeAttribute(NULL,"font-size",8);
    attributes.addItem(fs);

    FontColorAttribute * fc = new FontColorAttribute(NULL,"font-color", Qt::black);
    attributes.addItem(fc);

    FontBoldAttribute * fb = new FontBoldAttribute(NULL,"font-bold",false);
    attributes.addItem(fb);

    FontUnderlineAttribute * fu = new FontUnderlineAttribute(NULL,"font-underline", false);
    attributes.addItem(fu);

    PositionAttribute* pa = new PositionAttribute(NULL, "position", QPointF(10,10));
    attributes.addItem(pa);

    SizeAttribute * sa = new SizeAttribute(NULL, "size", QPointF(100,60) );
    attributes.addItem(sa);

    connect(sa, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)));
}
*/
IAttributeContainer * SCTextBlock::getAttributes()
{
    return & attributes;
}

QString SCTextBlock::getText()
{
    return  _text;
}

void  SCTextBlock::setText(QString text)
{
    if  ( text != _text)
    {
         _text = text;
        this->setObjectName(text);
        emit textChanged();
    }
}


void SCTextBlock::handleAttributeChanged(IAttribute *attr)
{
    qDebug()<<"SCTextBlock::handleAttributeChanged "<<attr->asString();
}


void SCTextBlock::writeSCVXML(QXmlStreamWriter & sw)
{
    sw.writeStartElement(QString("textblock"));

    QMapIterator<QString,IAttribute*> i(attributes);
    while (i.hasNext())
    {
        i.next();

        sw.writeAttribute(i.key(), i.value()->asString());
    }

    sw.writeEndElement();
}

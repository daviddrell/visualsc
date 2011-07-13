#include "textblock.h"
#include <QMapIterator>
#include "positionattribute.h"
#include "sizeattribute.h"


TextBlock::TextBlock()
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

}

IAttributeContainer * TextBlock::getAttributes()
{
    return & attributes;
}

QString TextBlock::getText()
{
    return  _text;
}

void  TextBlock::setText(QString text)
{
    _text = text;
}


void TextBlock::writeSCVXML(QXmlStreamWriter & sw)
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

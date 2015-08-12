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

    PositionAttribute* pa = new PositionAttribute(NULL, "position", QPointF(TEXTBLOCK_DEFAULT_X, TEXTBLOCK_DEFAULT_Y));
    pa->setParent(this);
    attributes.addItem(pa);

    SizeAttribute * sa = new SizeAttribute(NULL, "size", QPointF(TEXTBLOCK_DEFAULT_WIDTH, TEXTBLOCK_DEFAULT_HEIGHT));
    sa->setParent(this);
    attributes.addItem(sa);


    connect(sa, SIGNAL(changed(IAttribute*)), this, SLOT(handleAttributeChanged(IAttribute*)));
}

SizeAttribute* SCTextBlock::getSizeAttr()
{
    return dynamic_cast<SizeAttribute*>(attributes.value("size"));
}

PositionAttribute* SCTextBlock::getPosAttr()
{
    return dynamic_cast<PositionAttribute*>(attributes.value("position"));
}

IAttributeContainer * SCTextBlock::getAttributes()
{
    return & attributes;
}

QString SCTextBlock::getText()
{
    return  _text;
}

/**
 * @brief SCTextBlock::setText
 * @param text
 *
 * updates the datamodel for an item's textblock and then signals textChanged
 *
 * SIGNAL textChanged
 *
 *
 * connect (SCTextBlock, SIGNAL(textChanged()), SCState, SLOT(handleTextBlockChanged()));
 * connect (SCTextBlock, SIGNAL(textChanged()), SelectableTextBlock, SLOT(handleTextChanged()), Qt::QueuedConnection);
 *
 */
void  SCTextBlock::setText(QString text)
{
    if  ( text != _text)
    {
         _text = text;
        this->setObjectName(text);
        emit textChanged();
    }
}

void SCTextBlock::handleAttributeChanged(StateName * sn)
{
    this->setText(sn->asString());
}

void SCTextBlock::handleAttributeChanged(TransitionStringAttribute* tsa)
{
    this->setText(tsa->asString());
}


/**
 * @brief SCTextBlock::handleAttributeChanged
 * @param attr
 *
 * not currently used, function called is in this object's parent: SelectableTextBlock
 */
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

        qDebug () << "sctextblock write scxml: key: " << i.key() <<" value: " << i.value()->asString();
    }

    sw.writeEndElement();
}
